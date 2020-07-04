#ifndef GLIMPSE_TEXTURE_H
#define GLIMPSE_TEXTURE_H

#include <glimpse/gl.hpp>

#include <glm/glm.hpp>

#include <exception>
#include <functional>
#include <vector>

namespace gl {
/**
 * A 3-dimensional texture.
 */
class Texture3D {
public:
    /**
     * Create an OpenGL texture and load data into it.
     *
     * @param[in] width The width of the texture.
     * @param[in] height The height of the texture.
     * @param[in] depth The
     * depth of the texture.
     * @param[in] components The number of components per pixel.
     * @param[in] dtype The data type of the texture format.
     * @param[in] data The data to load into the texture.
     * @param[in] alignment The byte alignment 1, 2, 4 or 8.
     */
    Texture3D(int width,
              int height,
              int depth,
              int components,
              const gl::PixelType& dtype,
              const void* data,
              int alignment = 1);

    ~Texture3D() noexcept { reset(); }

    // Disable copy constructors
    Texture3D(const Texture3D&) = delete;
    Texture3D& operator=(const Texture3D&) = delete;

    // Enable move constructors
    Texture3D(Texture3D&&) noexcept;
    Texture3D& operator=(Texture3D&&) noexcept;

    Texture3D& operator=(std::nullptr_t) {
        reset();
        return *this;
    }

    /**
     * Determine whether the texture object is still valid.
     */
    operator bool() const noexcept { return m_handle != INVALID; }

    /**
     * The width of the texture.
     */
    int width() const noexcept { return m_width; }

    /**
     * The height of the texture.
     */
    int height() const noexcept { return m_height; }

    /**
     * The depth of the texture.
     */
    int depth() const noexcept { return m_depth; }

    /**
     * The number of components per pixel.
     */
    int components() const noexcept { return m_components; }

    /**
     * The handle to the native OpenGL object.
     */
    gl::Handle native_handle() const noexcept { return m_handle; }

    /**
     * Bind the texture to a texture unit.
     *
     * The location is the texture unit we want to bind the texture. This should
     * correspond with the value of the sampler3D uniform in the shader because
     * samplers read from the texture unit we assign to them.
     */
    void use(unsigned location);

private:
    /**
     * Reset the object state.
     */
    void reset() noexcept;

    /**
     * Swap object state.
     */
    void swap(Texture3D& other) noexcept;

    static constexpr gl::Handle INVALID = 0xFFFFFFFF;

    gl::Handle m_handle{INVALID};
    int m_width;
    int m_height;
    int m_depth;
    int m_components;
    std::reference_wrapper<const gl::PixelType> m_dtype{gl::PixelType::i8};
};
}  // namespace gl

#endif /* GLIMPSE_TEXTURE_3D_H */
