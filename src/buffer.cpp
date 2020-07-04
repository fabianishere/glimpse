#include <glimpse/buffer.hpp>
#include <glimpse/gl.hpp>

#include <GL/glew.h>

#include <cassert>
#include <stdexcept>

gl::Buffer::Buffer(const void* data, size_t size, gl::Buffer::Type type)
    : m_size(size), m_type(type) {
    glCreateBuffers(1, &m_handle);

    GLenum buffer_type = 0;
    switch (type) {
        case gl::Buffer::Type::STATIC:
            buffer_type = GL_STATIC_DRAW;
            break;
        case gl::Buffer::Type::DYNAMIC:
            buffer_type = GL_DYNAMIC_DRAW;
            break;
        case gl::Buffer::Type::STREAM:
            buffer_type = GL_STREAM_DRAW;
            break;
    }

    glNamedBufferData(m_handle, size, data, buffer_type);
}

gl::Buffer::~Buffer() noexcept {
    reset();
}

void gl::Buffer::reset() noexcept {
    if (this->operator bool()) {
        glDeleteBuffers(1, &m_handle);
        m_handle = INVALID;
    }
}

void gl::Buffer::swap(gl::Buffer& other) noexcept {
    std::swap(m_handle, other.m_handle);
    std::swap(m_size, other.m_size);
    std::swap(m_type, other.m_type);
}

gl::Buffer::Buffer(gl::Buffer&& other) noexcept : m_size(0), m_type(gl::Buffer::Type::STATIC) {
    swap(other);
}

gl::Buffer& gl::Buffer::operator=(gl::Buffer&& other) noexcept {
    swap(other);
    return *this;
}

void gl::Buffer::write(const void* data, size_t size, int offset) noexcept {
    assert(this->operator bool());
    glNamedBufferSubData(m_handle, offset, size, data);
}

std::vector<unsigned char> gl::Buffer::read() const {
    return read(m_size);
}

std::vector<unsigned char> gl::Buffer::read(size_t size, int offset) const {
    assert(this->operator bool());

    if (offset < 0 || offset + size > m_size) {
        throw std::invalid_argument("Size or offset out of bounds");
    }

    glBindBuffer(GL_ARRAY_BUFFER, m_handle);
    auto* map = static_cast<unsigned char*>(
        glMapBufferRange(GL_ARRAY_BUFFER, 0, m_size, GL_MAP_READ_BIT));

    if (!map) {
        GLenum err = glGetError();
        throw gl::Error(err);
    }

    std::vector<unsigned char> res(map, map + m_size);
    glUnmapBuffer(GL_ARRAY_BUFFER);
    return res;
}
