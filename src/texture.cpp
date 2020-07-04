#include <glimpse/gl.hpp>
#include <glimpse/texture.hpp>

#include <GL/glew.h>

#include <cassert>
#include <stdexcept>

gl::Texture::Texture(int width,
                     int height,
                     int components,
                     bool depth,
                     const gl::PixelType& dtype,
                     const void* data,
                     int samples,
                     int alignment)
    : m_width(width),
      m_height(height),
      m_components(components),
      m_samples(samples),
      m_depth(depth),
      m_dtype(dtype),
      m_max_level(0) {
    if (components < 1 || components > 4) {
        throw std::invalid_argument("Components must be 1, 2, 3 or 4");
    } else if (samples & (samples - 1)) {
        throw std::invalid_argument("The number of samples is invalid");
    } else if (data != nullptr && samples) {
        throw std::invalid_argument("Multisample textures are not writable directly");
    } else if (alignment != 1 && alignment != 2 && alignment != 4 && alignment != 8) {
        throw std::invalid_argument("Alignment must be 1, 2, 4 or 8");
    } else if (depth && dtype != gl::PixelType::f32) {
        throw std::invalid_argument("Depth buffer only supports dtype gl::f32");
    }

    size_t expected_size = static_cast<size_t>(width) * static_cast<size_t>(components) * dtype.size();
    expected_size = (expected_size + static_cast<size_t>(alignment) - 1) / static_cast<size_t>(alignment) *
                    static_cast<size_t>(alignment);
    expected_size = expected_size * static_cast<size_t>(height);

    int pixel_type = dtype.type();
    auto [base_format, internal_format] = dtype.format(components);
    GLenum texture_target = m_samples ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;

    glCreateTextures(texture_target, 1, &m_handle);

    if (samples) {
        glTextureStorage2DMultisample(m_handle, samples, depth ? GL_DEPTH_COMPONENT24 : internal_format, width, height,
                                      true);
    } else {
        glPixelStorei(GL_PACK_ALIGNMENT, alignment);
        glPixelStorei(GL_UNPACK_ALIGNMENT, alignment);
        glTextureStorage2D(m_handle, 1, depth ? GL_DEPTH_COMPONENT24 : internal_format, width, height);
        glTextureParameteri(m_handle, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTextureParameteri(m_handle, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        if (depth) {
            glTextureParameteri(m_handle, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
            glTextureParameteri(m_handle, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
        }

        if (data) {
            glTextureSubImage2D(m_handle, 0, 0, 0, width, height, base_format, pixel_type, data);
        }
    }
}

void gl::Texture::reset() noexcept {
    if (this->operator bool()) {
        glDeleteTextures(1, &m_handle);
        m_handle = INVALID;
    }
}

void gl::Texture::swap(gl::Texture& other) noexcept {
    std::swap(m_handle, other.m_handle);
    std::swap(m_width, other.m_width);
    std::swap(m_height, other.m_height);
    std::swap(m_components, other.m_components);
    std::swap(m_samples, other.m_samples);
    std::swap(m_depth, other.m_depth);
    std::swap(m_dtype, other.m_dtype);
    std::swap(m_max_level, other.m_max_level);
}

gl::Texture::Texture(gl::Texture&& other) noexcept {
    swap(other);
}

gl::Texture& gl::Texture::operator=(gl::Texture&& other) noexcept {
    swap(other);
    return *this;
}

void gl::Texture::write(const void* data, size_t size, int level, int alignment) {
    assert(this->operator bool());

    if (alignment != 1 && alignment != 2 && alignment != 4 && alignment != 8) {
        throw std::invalid_argument("Alignment must be 1, 2, 4 or 8");
    } else if (level > m_max_level) {
        throw std::invalid_argument("Invalid level");
    } else if (m_samples) {
        throw std::logic_error("Multisample textures are not writable directly");
    }

    int width = m_width / (1 << level);
    int height = m_height / (1 << level);

    width = width > 1 ? width : 1;
    height = height > 1 ? height : 1;

    size_t expected_size = static_cast<size_t>(width) * static_cast<size_t>(m_components) * m_dtype.get().size();
    expected_size = (expected_size + static_cast<size_t>(alignment) - 1) / static_cast<size_t>(alignment) *
                    static_cast<size_t>(alignment);
    expected_size = expected_size * static_cast<size_t>(height);

    auto pixel_type = m_dtype.get().type();
    auto [base_format, internal_format] = m_dtype.get().format(m_components);

    glPixelStorei(GL_PACK_ALIGNMENT, alignment);
    glPixelStorei(GL_UNPACK_ALIGNMENT, alignment);

    glTextureSubImage2D(m_handle, 0, 0, 0, width, height, base_format, pixel_type, data);
    glTextureParameteri(m_handle, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(m_handle, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void gl::Texture::use(unsigned slot) {
    assert(this->operator bool());

    glBindTextureUnit(slot, m_handle);
}
