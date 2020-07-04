#include <glimpse/attribute.hpp>
#include <glimpse/vertex_array.hpp>

#include <GL/glew.h>

#include <stdexcept>

gl::VertexArray::VertexArray(std::shared_ptr<gl::Program> program,
                             const std::unordered_map<std::string, gl::Data>& data,
                             std::optional<gl::Data> indices)
    : m_program(program), m_indices(indices), m_data(data), m_num_vertices(indices ? indices->size() : 0) {
    glCreateVertexArrays(1, &m_handle);

    if (indices) {
        glVertexArrayElementBuffer(m_handle, indices->buffer().native_handle());
    }

    for (auto& [name, view] : data) {
        // Unknown attributes are ignored
        if (program->attributes.count(name) == 0) {
            continue;
        }
        const gl::Attribute& attrib = program->attributes[name];
        std::slice slice = view.slice();
        gl::ElementDescriptor descriptor = view.descriptor();
        glVertexArrayVertexBuffer(m_handle, attrib.location(), view.buffer().native_handle(), 0,
                                  static_cast<GLsizei>(slice.stride()));
        glVertexArrayAttribFormat(m_handle, attrib.location(), descriptor.count(), descriptor.type(), GL_FALSE,
                                  static_cast<GLintptr>(slice.start()));
        glEnableVertexArrayAttrib(m_handle, attrib.location());
    }
}

void gl::VertexArray::reset() noexcept {
    if (this->operator bool()) {
        glDeleteVertexArrays(1, &m_handle);
        m_handle = INVALID;
    }
}

void gl::VertexArray::swap(gl::VertexArray& other) noexcept {
    std::swap(m_handle, other.m_handle);
    std::swap(m_program, other.m_program);
    std::swap(m_indices, other.m_indices);
    std::swap(m_data, other.m_data);
    std::swap(m_num_vertices, other.m_num_vertices);
}

gl::VertexArray::VertexArray(gl::VertexArray&& other) noexcept {
    swap(other);
}

gl::VertexArray& gl::VertexArray::operator=(gl::VertexArray&& other) noexcept {
    swap(other);
    return *this;
}

void gl::VertexArray::render(unsigned mode, int vertices) const {
    assert(this->operator bool());

    glUseProgram(m_program->native_handle());

    if (vertices < 0) {
        vertices = static_cast<int>(m_num_vertices);
    }

    glBindVertexArray(m_handle);

    if (m_indices) {
        glDrawElements(mode, vertices, GL_UNSIGNED_INT, nullptr);
    } else {
        glDrawArrays(mode, 0, vertices);
    }
}
