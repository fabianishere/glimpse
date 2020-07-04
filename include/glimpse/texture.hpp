#ifndef GLIMPSE_TEXTURE_H
#define GLIMPSE_TEXTURE_H

#include <glimpse/gl.hpp>

#include <glm/glm.hpp>

#include <exception>
#include <functional>
#include <vector>

namespace gl {
/**
 * A Texture is an OpenGL object that contains one or more images that all have
 * the same image format. A texture can be used in two ways. It can be the
 * source of a texture access from a shader, or it can be used as a render
 * target.
 */
class Texture {
public:
    /**
     * Create an empty OpenGL texture.
     *
     * @param[in] width The width of the texture.
     * @param[in] height The height of the texture.
     * @param[in] components The number of components per pixel.
     * @param[in] dtype The data type of the texture format.
     * @param[in] alignment The byte alignment 1, 2, 4 or 8.
     */
    Texture(int width, int height, int components, const gl::PixelType& dtype, int alignment = 1)
        : Texture(width, height, components, false, dtype, nullptr, 0, alignment) {}

    /**
     * Create an OpenGL texture and load data into it.
     *
     * @param[in] width The width of the texture.
     * @param[in] height The height of the texture.
     * @param[in] components The number of components per pixel.
     * @param[in] dtype The data type of the texture format.
     * @param[in] data The data to load into the texture.
     * @param[in] samples The number of samples. Value 0 means no multisample
     * format.
     * @param[in] alignment The byte alignment 1, 2, 4 or 8.
     */
    template <typename T>
    Texture(int width,
            int height,
            int components,
            const gl::PixelType& dtype,
            const typename std::vector<T>& data,
            int samples = 0,
            int alignment = 1)
        : Texture(width, height, components, false, dtype, data.data(), samples, alignment) {}

    /**
     * Create an OpenGL texture and load data into it.
     *
     * @param[in] width The width of the texture.
     * @param[in] height The height of the texture.
     * @param[in] components The number of components per pixel.
     * @param[in] dtype The data type of the texture
     * format.
     * @param[in] data The data to load into the texture.
     * @param[in] samples The number of samples. Value 0 means no multisample
     * format.
     * @param[in] alignment The byte alignment 1, 2, 4 or 8.
     */
    Texture(int width,
            int height,
            int components,
            const gl::PixelType& dtype,
            const void* data,
            int samples = 0,
            int alignment = 1)
        : Texture(width, height, components, false, dtype, data, samples) {}

    /**
     * Construct a depth {@link Texture}.
     *
     * @param[in] width The width of the texture.
     * @param[in] height The height of the texture.
     * @param[in] samples The number of samples. Value 0 means
     * no multisample format.
     * @param[in] alignment The byte alignment 1, 2, 4 or 8.
     */
    static Texture depth(int width, int height, int samples = 0, int alignment = 1) {
        return Texture(width, height, 1, true, gl::PixelType::f32, nullptr, samples, alignment);
    }

    ~Texture() noexcept { reset(); }

    // Disable copy constructors
    Texture(const Texture&) = delete;
    Texture& operator=(const Texture&) = delete;

    // Enable move constructors
    Texture(Texture&&) noexcept;
    Texture& operator=(Texture&&) noexcept;

    Texture& operator=(std::nullptr_t) {
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
     * The number of components per pixel.
     */
    int components() const noexcept { return m_components; }

    /**
     * The number of samples set for the texture used in multisampling.
     */
    int samples() const noexcept { return m_samples; }

    /**
     * Determine whether this texture is a depth texture.
     */
    bool is_depth_texture() const noexcept { return m_depth; }

    /**
     * The data type of the buffer.
     */
    const gl::PixelType& dtype() const noexcept { return m_dtype; }

    /**
     * The handle to the native OpenGL object.
     */
    gl::Handle native_handle() const noexcept { return m_handle; }

    /**
     * Update the content of the texture.
     *
     * @param[in] data The data to write to the texture.
     * @param[in] level The mipmap level.
     * @param[in] alignment The alignment of the pixels.
     */
    template <typename T>
    void write(const typename std::vector<T>& data, int level = 0, int alignment = 1) {
        write(data.data(), sizeof(T) * data.size(), level, alignment);
    }

    /**
     * Update the content of the texture.
     *
     * @param[in] data The data to write to the texture.
     * @param[in] size The number of bytes to write to the texture.
     * @param[in] level The mipmap level.
     * @param[in] alignment The alignment of the pixels.
     */
    void write(const void* data, size_t size, int level = 0, int alignment = 1);

    /**
     * Bind the texture to a texture unit.
     *
     * The location is the texture unit we want to bind the texture. This should
     * correspond with the value of the sampler2D uniform in the shader because
     * samplers read from the texture unit we assign to them.
     */
    void use(unsigned location);

private:
    /**
     * Create an OpenGL texture and load data into it.
     *
     * @param[in] width The width of the texture.
     * @param[in] height The height of the texture.
     * @param[in] components The number of components per pixel.
     * @param[in] depth A flag to indicate to generate
     * a depth buffer.
     * @param[in] dtype The data type of the texture format.
     * @param[in] data The data to load into the texture.
     * @param[in] samples The number of samples. Value 0 means no multisample
     * format.
     * @param[in] alignment The byte alignment 1, 2, 4 or 8.
     */
    Texture(int width,
            int height,
            int components,
            bool depth,
            const gl::PixelType& dtype,
            const void* data,
            int samples = 0,
            int alignment = 1);

    /**
     * Reset the object state.
     */
    void reset() noexcept;

    /**
     * Swap object state.
     */
    void swap(Texture& other) noexcept;

    static constexpr gl::Handle INVALID = 0xFFFFFFFF;

    gl::Handle m_handle{INVALID};
    int m_width;
    int m_height;
    int m_components;
    int m_samples;
    bool m_depth;
    int m_max_level;
    std::reference_wrapper<const gl::PixelType> m_dtype{gl::PixelType::i8};
};

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

    ~TextureCube() noexcept { reset(); }

    // Disable copy constructors
    TextureCube(const TextureCube&) = delete;
    TextureCube& operator=(const TextureCube&) = delete;

    // Enable move constructors
    TextureCube(TextureCube&&) noexcept;
    TextureCube& operator=(TextureCube&&) noexcept;

    TextureCube& operator=(std::nullptr_t) {
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
     * The number of components per pixel.
     */
    int components() const noexcept { return m_components; }

    /**
     * The handle to the native OpenGL object.
     */
    unsigned native_handle() const noexcept { return m_handle; }

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

    ~TextureArray() noexcept { reset(); }

    // Disable copy constructors
    TextureArray(const TextureArray&) = delete;
    TextureArray& operator=(const TextureArray&) = delete;

    // Enable move constructors
    TextureArray(TextureArray&&) noexcept;
    TextureArray& operator=(TextureArray&&) noexcept;

    TextureArray& operator=(std::nullptr_t) {
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
     * The number of layers in the texture
     */
    int layers() const noexcept { return m_layers; }

    /**
     * The number of components per pixel.
     */
    int components() const noexcept { return m_components; }

    /**
     * The handle to the native OpenGL object.
     */
    gl::Handle native_handle() const noexcept { return m_handle; }

    /**
     * Update the content of the texture.
     *
     * @param[in] data The data to write to the texture.
     * @param[in] layer The layer of the texture to update.
     * @param[in] viewport The viewport of the texture to overwrite.
     * @param[in] alignment The alignment of the pixels.
     */
    void write_layer(const void* data, int layer, int alignment = 1) {
        write_layer(data, layer, glm::ivec4(0, 0, m_width, m_height), alignment);
    }

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

#endif /* GLIMPSE_TEXTURE_H */
