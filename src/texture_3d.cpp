#include <glimpse/gl.hpp>
#include <glimpse/texture.hpp>

#include <GL/glew.h>

#include <cassert>
#include <stdexcept>

gl::Texture3D::Texture3D(int width,
                         int height,
                         int depth,
                         int components,
                         const gl::PixelType& dtype,
                         const void* data,
                         int alignment)
    : m_width(width), m_height(height), m_depth(depth), m_components(components), m_dtype(dtype) {
    if (components < 1 || components > 4) {
        throw std::invalid_argument("Components must be 1, 2, 3 or 4");
    } else if (alignment != 1 && alignment != 2 && alignment != 4 && alignment != 8) {
        throw std::invalid_argument("The alignment must be 1, 2, 4 or 8");
    }

    size_t expected_size = static_cast<size_t>(width) * static_cast<size_t>(components) * dtype.size();
    expected_size = (expected_size + static_cast<size_t>(alignment) - 1) / static_cast<size_t>(alignment) *
                    static_cast<size_t>(alignment);
    expected_size = expected_size * static_cast<size_t>(height) * static_cast<size_t>(depth);

    int pixel_type = dtype.type();
    auto [base_format, internal_format] = dtype.format(components);
    GLenum texture_target = GL_TEXTURE_3D;

    glCreateTextures(texture_target, 1, &m_handle);

    glPixelStorei(GL_PACK_ALIGNMENT, alignment);
    glPixelStorei(GL_UNPACK_ALIGNMENT, alignment);

    glTextureStorage3D(m_handle, 1, internal_format, width, height, depth);
    glTextureParameteri(m_handle, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(m_handle, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    if (data) {
        glTextureSubImage3D(m_handle, 0, 0, 0, 0, width, height, depth, base_format, pixel_type, data);
    }
}

void gl::Texture3D::reset() noexcept {
    if (this->operator bool()) {
        glDeleteTextures(1, &m_handle);
        m_handle = INVALID;
    }
}

void gl::Texture3D::swap(gl::Texture3D& other) noexcept {
    std::swap(m_handle, other.m_handle);
    std::swap(m_width, other.m_width);
    std::swap(m_height, other.m_height);
    std::swap(m_depth, other.m_depth);
    std::swap(m_components, other.m_components);
    std::swap(m_dtype, other.m_dtype);
}

gl::Texture3D::Texture3D(gl::Texture3D&& other) noexcept {
    swap(other);
}

gl::Texture3D& gl::Texture3D::operator=(gl::Texture3D&& other) noexcept {
    swap(other);
    return *this;
}

void gl::Texture3D::use(unsigned slot) {
    assert(this->operator bool());

    glBindTextureUnit(slot, m_handle);
}
