#include <glimpse/gl.hpp>
#include <glimpse/texture.hpp>

#include <GL/glew.h>

#include <cassert>
#include <stdexcept>

gl::TextureCube::TextureCube(int width,
                             int height,
                             int components,
                             const gl::PixelType& dtype,
                             const std::array<const void*, 6>& faces,
                             int alignment)
    : m_width(width), m_height(height), m_components(components), m_dtype(dtype) {
    if (components < 1 || components > 4) {
        throw std::invalid_argument("Components must be 1, 2, 3 or 4");
    } else if (alignment != 1 && alignment != 2 && alignment != 4 && alignment != 8) {
        throw std::invalid_argument("The alignment must be 1, 2, 4 or 8");
    }

    size_t expected_size = static_cast<size_t>(width) * static_cast<size_t>(components) * dtype.size();
    expected_size = (expected_size + static_cast<size_t>(alignment) - 1) / static_cast<size_t>(alignment) *
                    static_cast<size_t>(alignment);
    expected_size = expected_size * static_cast<size_t>(height);

    int pixel_type = dtype.type();
    auto [base_format, internal_format] = dtype.format(components);
    GLenum texture_target = GL_TEXTURE_CUBE_MAP;

    glCreateTextures(texture_target, 1, &m_handle);

    glPixelStorei(GL_PACK_ALIGNMENT, alignment);
    glPixelStorei(GL_UNPACK_ALIGNMENT, alignment);

    glTextureStorage2D(m_handle, 1, internal_format, width, height);
    glTextureParameteri(m_handle, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(m_handle, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTextureParameteri(m_handle, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTextureParameteri(m_handle, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTextureParameteri(m_handle, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    for (int face = 0; face < static_cast<int>(faces.size()); face++) {
        const void* data = faces[face];
        if (data) {
            glTextureSubImage3D(m_handle, 0, 0, 0, face, width, height, 1, base_format, pixel_type, data);
        }
    }
}

void gl::TextureCube::reset() noexcept {
    if (this->operator bool()) {
        glDeleteTextures(1, &m_handle);
        m_handle = INVALID;
    }
}

void gl::TextureCube::swap(gl::TextureCube& other) noexcept {
    std::swap(m_handle, other.m_handle);
    std::swap(m_width, other.m_width);
    std::swap(m_height, other.m_height);
    std::swap(m_components, other.m_components);
    std::swap(m_dtype, other.m_dtype);
}

gl::TextureCube::TextureCube(gl::TextureCube&& other) noexcept {
    swap(other);
}

gl::TextureCube& gl::TextureCube::operator=(gl::TextureCube&& other) noexcept {
    swap(other);
    return *this;
}

void gl::TextureCube::use(unsigned slot) {
    assert(this->operator bool());

    glBindTextureUnit(slot, m_handle);
}
