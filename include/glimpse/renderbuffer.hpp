#ifndef GLIMPSE_RENDERBUFFER_H
#define GLIMPSE_RENDERBUFFER_H

#include <glimpse/buffer.hpp>
#include <glimpse/gl.hpp>

#include <functional>

namespace gl {

/**
 * {@link Renderbuffer} objects are OpenGL objects that contain images.
 *
 * They are created and used specifically with {@link Framebuffer} objects. They
 * are optimized for use as render targets, while {@link Texture} objects may
 * not be, and are the logical choice when you do not need to sample from the
 * produced image.
 */
class Renderbuffer {
public:
    /**
     * Construct a regular {@link Renderbuffer} instance.
     *
     * @param[in] width The width of the buffer.
     * @param[in] height The height of the buffer.
     * @param[in] components The number of components per pixel.
     * @param[in] dtype The data type of the render buffer.
     * @param[in] samples The number of samples. Value 0
     * means no multisample format.
     */
    Renderbuffer(int width,
                 int height,
                 int components,
                 const gl::PixelType& dtype,
                 int samples = 0)
        : Renderbuffer(width, height, components, false, dtype, samples) {}

    /**
     * Construct a depth {@link Renderbuffer} instance.
     *
     * @param[in] width The width of the buffer.
     * @param[in] height The height of the buffer.
     * @param[in] components The number of components per pixel.
     * @param[in] samples The number of samples. Value 0 means no multisample format.
     */
    static Renderbuffer depth(int width, int height, int components, int samples = 0) {
        return Renderbuffer(width, height, components, true, gl::PixelType::f32, samples);
    }

    ~Renderbuffer() noexcept;

    // Disable copy constructors
    Renderbuffer(const Renderbuffer&) = delete;
    Renderbuffer& operator=(const Renderbuffer&) = delete;

    // Enable move constructors
    Renderbuffer(Renderbuffer&&) noexcept;
    Renderbuffer& operator=(Renderbuffer&&) noexcept;

    Renderbuffer& operator=(std::nullptr_t);

    /**
     * Determine whether the render buffer object is still valid.
     */
    explicit operator bool() const noexcept;

    /**
     * The width of the render buffer.
     */
    int width() const noexcept;

    /**
     * The height of the render buffer.
     */
    int height() const noexcept;

    /**
     * The number of components per pixel.
     */
    int components() const noexcept;

    /**
     * Determine whether this render buffer is a depth buffer.
     */
    bool is_depth_buffer() const noexcept;

    /**
     * The samples of the render buffer.
     */
    int samples() const noexcept;

    /**
     * The data type of the buffer.
     */
    const gl::PixelType& dtype() const noexcept;

    /**
     * The handle to the native OpenGL object.
     */
    gl::Handle native_handle() const noexcept;

private:
    /**
     * Construct a {@link Renderbuffer} instance.
     *
     * @param[in] width The width of the buffer.
     * @param[in] height The height of the buffer.
     * @param[in] components The number of components per pixel.
     * @param[in] depth A flag to indicate that it should be a depth buffer.
     * @param[in] dtype The data type of the render buffer.
     * @param[in] samples The number of samples. Value 0 means no multisample
     * format.
     */
    Renderbuffer(int width,
                 int height,
                 int components,
                 bool depth,
                 const gl::PixelType& dtype = gl::PixelType::f32,
                 int samples = 0);

    /**
     * Reset the object state.
     */
    void reset() noexcept;

    /**
     * Swap object state.
     */
    void swap(Renderbuffer& other) noexcept;

    static constexpr gl::Handle INVALID = 0xFFFFFFFF;

    gl::Handle m_handle{INVALID};
    int m_width;
    int m_height;
    int m_components;
    bool m_depth;
    std::reference_wrapper<const gl::PixelType> m_dtype{gl::PixelType::i8};
    int m_samples;
};
}  // namespace gl

#endif /* GLIMPSE_RENDERBUFFER_H */
