#ifndef GLIMPSE_TEXTURE_CUBE_H
#define GLIMPSE_TEXTURE_CUBE_H

#include <glimpse/gl.hpp>

#include <glm/glm.hpp>

#include <exception>
#include <functional>
#include <vector>

namespace gl {
/**
 * A cubemap texture.
 */
class TextureCube {
public:
    /**
     * Create an OpenGL texture and load data into it.
     *
     * @param[in] width The width of the texture.
     * @param[in] height The height of the texture.
     * @param[in] components The number of components per pixel.
     * @param[in] dtype The data type of the texture format.
     * @param[in] faces The faces to load into the texture.
     * @param[in] alignment The byte alignment 1, 2, 4 or 8.
     */
    TextureCube(int width,
                int height,
                int components,
                const gl::PixelType& dtype,
                const std::array<const void*, 6>& faces,
                int alignment = 1);

    ~TextureCube() noexcept;

    // Disable copy constructors
    TextureCube(const TextureCube&) = delete;
    TextureCube& operator=(const TextureCube&) = delete;

    // Enable move constructors
    TextureCube(TextureCube&&) noexcept;
    TextureCube& operator=(TextureCube&&) noexcept;

    TextureCube& operator=(std::nullptr_t);

    /**
     * Determine whether the texture object is still valid.
     */
    explicit operator bool() const noexcept;

    /**
     * The width of the texture.
     */
    int width() const noexcept;

    /**
     * The height of the texture.
     */
    int height() const noexcept;

    /**
     * The number of components per pixel.
     */
    int components() const noexcept;

    /**
     * The handle to the native OpenGL object.
     */
    gl::Handle native_handle() const noexcept;

    /**
     * Bind the texture to a texture unit.
     *
     * The location is the texture unit we want to bind the texture. This should
     * correspond with the value of the samplerCube uniform in the shader
     * because samplers read from the texture unit we assign to them.
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
    void swap(TextureCube& other) noexcept;

    static constexpr gl::Handle INVALID = 0xFFFFFFFF;

    gl::Handle m_handle{INVALID};
    int m_width;
    int m_height;
    int m_components;
    std::reference_wrapper<const gl::PixelType> m_dtype{gl::PixelType::i8};
};

}  // namespace gl

#endif /* GLIMPSE_TEXTURE_CUBE_H */
