#ifndef GLIMPSE_TEXTURE_ARRAY_H
#define GLIMPSE_TEXTURE_ARRAY_H

#include <glimpse/gl.hpp>

#include <glm/glm.hpp>

#include <exception>
#include <functional>
#include <vector>

namespace gl {
/**
 * A texture with multiple layers.
 */
class TextureArray {
public:
    /**
     * Create an OpenGL texture and load data into it.
     *
     * @param[in] width The width of the texture.
     * @param[in] height The height of the texture.
     * @param[in] layers The
     * number of layers in the texture.
     * @param[in] components The number of components per pixel.
     * @param[in] dtype The data type of the texture format.
     * @param[in] layers The layers to load into the texture.
     * @param[in] alignment The byte alignment 1, 2, 4 or 8.
     */
    TextureArray(int width,
                 int height,
                 int layers,
                 int components,
                 const gl::PixelType& dtype,
                 const std::vector<const void*>& data,
                 int alignment = 1);

    /**
     * Create an OpenGL texture and load data into it.
     *
     * @param[in] width The width of the texture.
     * @param[in] height The height of the texture.
     * @param[in] layers The
     * number of layers in the texture.
     * @param[in] components The number of components per pixel.
     * @param[in] dtype The data type of the texture format.
     * @param[in] layers The layers to load into the texture.
     * @param[in] alignment The byte alignment 1, 2, 4 or 8.
     */
    TextureArray(int width,
                 int height,
                 int layers,
                 int components,
                 const gl::PixelType& dtype,
                 const void* data,
                 int alignment = 1)
        : TextureArray(width, height, layers, components, dtype, std::vector<const void*>{data}, alignment) {}

    ~TextureArray() noexcept;

    // Disable copy constructors
    TextureArray(const TextureArray&) = delete;
    TextureArray& operator=(const TextureArray&) = delete;

    // Enable move constructors
    TextureArray(TextureArray&&) noexcept;
    TextureArray& operator=(TextureArray&&) noexcept;

    TextureArray& operator=(std::nullptr_t);

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
     * The number of layers in the texture
     */
    int layers() const noexcept;

    /**
     * The number of components per pixel.
     */
    int components() const noexcept;

    /**
     * The handle to the native OpenGL object.
     */
    gl::Handle native_handle() const noexcept;

    /**
     * Update the content of the texture.
     *
     * @param[in] data The data to write to the texture.
     * @param[in] layer The layer of the texture to update.
     * @param[in] viewport The viewport of the texture to overwrite.
     * @param[in] alignment The alignment of the pixels.
     */
    void write_layer(const void* data, int layer, int alignment = 1);

    /**
     * Update the content of the texture.
     *
     * @param[in] data The data to write to the texture.
     * @param[in] layer The layer of the texture to update.
     * @param[in] viewport The viewport of the texture to overwrite.
     * @param[in] alignment The alignment of the pixels.
     */
    void write_layer(const void* data, int layer, const glm::ivec4 viewport, int alignment = 1);

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
    void swap(TextureArray& other) noexcept;

    static constexpr gl::Handle INVALID = 0xFFFFFFFF;

    gl::Handle m_handle{INVALID};
    int m_width;
    int m_height;
    int m_layers;
    int m_components;
    std::reference_wrapper<const gl::PixelType> m_dtype{gl::PixelType::i8};
};
}  // namespace gl

#endif /* GL_TEXTURE_ARRAY_H */
