#include <glimpse/gl.hpp>
#include <glimpse/texture.hpp>

#include <GL/glew.h>

#include <cassert>
#include <stdexcept>

gl::TextureArray::TextureArray(int width,
                               int height,
                               int layers,
                               int components,
                               const gl::PixelType& dtype,
                               const std::vector<const void*>& data,
                               int alignment)
    : m_width(width), m_height(height), m_layers(layers), m_components(components), m_dtype(dtype) {
    if (components < 1 || components > 4) {
        throw std::invalid_argument("Components must be 1, 2, 3 or 4");
    } else if (alignment != 1 && alignment != 2 && alignment != 4 && alignment != 8) {
        throw std::invalid_argument("The alignment must be 1, 2, 4 or 8");
    } else if (data.size() != 1 && data.size() != layers) {
        throw std::invalid_argument("Data not given for all layers");
    }

    size_t expected_size = static_cast<size_t>(width) * static_cast<size_t>(components) * dtype.size();
    expected_size = (expected_size + static_cast<size_t>(alignment) - 1) / static_cast<size_t>(alignment) *
                    static_cast<size_t>(alignment);
    expected_size = expected_size * static_cast<size_t>(height);

    int pixel_type = dtype.type();
    auto [base_format, internal_format] = dtype.format(components);

    GLenum texture_target = GL_TEXTURE_2D_ARRAY;

    glCreateTextures(texture_target, 1, &m_handle);

    glPixelStorei(GL_PACK_ALIGNMENT, alignment);
    glPixelStorei(GL_UNPACK_ALIGNMENT, alignment);

    glTextureStorage3D(m_handle, 1, internal_format, width, height, layers);
    glTextureParameteri(m_handle, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(m_handle, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    if (data.size() == 1) {
        // Data is a single pointer containing all data
        if (data[0]) {
            glTextureSubImage3D(m_handle, 0, 0, 0, 0, width, height, layers, base_format, pixel_type, data[0]);
        }
    } else {
        // Upload texture per layer
        for (int layer = 0; layer < layers; layer++) {
            if (data[layer]) {
                glTextureSubImage3D(m_handle, 0, 0, 0, layer, width, height, 1, base_format, pixel_type, data[layer]);
            }
        }
    }
}

void gl::TextureArray::reset() noexcept {
    if (this->operator bool()) {
        glDeleteTextures(1, &m_handle);
        m_handle = INVALID;
    }
}

void gl::TextureArray::swap(gl::TextureArray& other) noexcept {
    std::swap(m_handle, other.m_handle);
    std::swap(m_width, other.m_width);
    std::swap(m_height, other.m_height);
    std::swap(m_layers, other.m_layers);
    std::swap(m_components, other.m_components);
    std::swap(m_dtype, other.m_dtype);
}

gl::TextureArray::TextureArray(gl::TextureArray&& other) noexcept {
    swap(other);
}

gl::TextureArray& gl::TextureArray::operator=(gl::TextureArray&& other) noexcept {
    swap(other);
    return *this;
}

void gl::TextureArray::write_layer(const void* data, int layer, const glm::ivec4 viewport, int alignment) {
    assert(this->operator bool());

    auto pixel_type = m_dtype.get().type();
    auto [base_format, internal_format] = m_dtype.get().format(m_components);

    glTextureSubImage3D(m_handle, 0, viewport[0], viewport[1], layer, viewport[2], viewport[3], 1, base_format,
                        pixel_type, data);
}

void gl::TextureArray::use(unsigned slot) {
    assert(this->operator bool());

    glActiveTexture(slot);
    glBindTexture(GL_TEXTURE_2D_ARRAY, m_handle);
}
