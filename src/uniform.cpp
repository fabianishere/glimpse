#include <glimpse/gl.hpp>
#include <glimpse/uniform.hpp>

#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>

#include <stdexcept>

gl::Uniform::Uniform() noexcept = default;

gl::Uniform::Uniform(gl::Handle handle, const std::string& name, gl::Type type, int location, int count) noexcept
    : Member(handle, name), m_type(type), m_location(location), m_count(count) {}

gl::Type gl::Uniform::type() const noexcept {
    return m_type;
}

int gl::Uniform::location() const noexcept {
    return m_location;
}

int gl::Uniform::count() const noexcept {
    return m_count;
}

gl::Uniform& gl::Uniform::operator=(bool value) {
    if (this->operator bool()) {
        glProgramUniform1i(native_handle(), location(), value);
    }
    return *this;
}

gl::Uniform& gl::Uniform::operator=(int value) {
    if (this->operator bool()) {
        glProgramUniform1i(native_handle(), location(), value);
    }
    return *this;
}

gl::Uniform& gl::Uniform::operator=(unsigned value) {
    if (this->operator bool()) {
        glProgramUniform1ui(native_handle(), location(), value);
    }
    return *this;
}

gl::Uniform& gl::Uniform::operator=(float value) {
    if (this->operator bool()) {
        glProgramUniform1f(native_handle(), location(), value);
    }
    return *this;
}

gl::Uniform& gl::Uniform::operator=(double value) {
    if (this->operator bool()) {
        glProgramUniform1d(native_handle(), location(), value);
    }
    return *this;
}

gl::Uniform& gl::Uniform::operator=(const glm::bvec2& value) {
    if (this->operator bool()) {
        glProgramUniform2iv(native_handle(), location(), 1, glm::value_ptr(glm::ivec2(value)));
    }
    return *this;
}

gl::Uniform& gl::Uniform::operator=(const glm::ivec2& value) {
    if (this->operator bool()) {
        glProgramUniform2iv(native_handle(), location(), 1, glm::value_ptr(value));
    }
    return *this;
}

gl::Uniform& gl::Uniform::operator=(const glm::uvec2& value) {
    if (this->operator bool()) {
        glProgramUniform2uiv(native_handle(), location(), 1, glm::value_ptr(value));
    }
    return *this;
}

gl::Uniform& gl::Uniform::operator=(const glm::vec2& value) {
    if (this->operator bool()) {
        glProgramUniform2fv(native_handle(), location(), 1, glm::value_ptr(value));
    }
    return *this;
}

gl::Uniform& gl::Uniform::operator=(const glm::dvec2& value) {
    if (this->operator bool()) {
        glProgramUniform2dv(native_handle(), location(), 1, glm::value_ptr(value));
    }
    return *this;
}

gl::Uniform& gl::Uniform::operator=(const glm::bvec3& value) {
    if (this->operator bool()) {
        glProgramUniform3iv(native_handle(), location(), 1, glm::value_ptr(glm::ivec3(value)));
    }
    return *this;
}

gl::Uniform& gl::Uniform::operator=(const glm::ivec3& value) {
    if (this->operator bool()) {
        glProgramUniform3iv(native_handle(), location(), 1, glm::value_ptr(value));
    }
    return *this;
}

gl::Uniform& gl::Uniform::operator=(const glm::uvec3& value) {
    if (this->operator bool()) {
        glProgramUniform3uiv(native_handle(), location(), 1, glm::value_ptr(value));
    }
    return *this;
}

gl::Uniform& gl::Uniform::operator=(const glm::vec3& value) {
    if (this->operator bool()) {
        glProgramUniform3fv(native_handle(), location(), 1, glm::value_ptr(value));
    }
    return *this;
}

gl::Uniform& gl::Uniform::operator=(const glm::dvec3& value) {
    if (this->operator bool()) {
        glProgramUniform3dv(native_handle(), location(), 1, glm::value_ptr(value));
    }
    return *this;
}

gl::Uniform& gl::Uniform::operator=(const glm::bvec4& value) {
    if (this->operator bool()) {
        glProgramUniform4iv(native_handle(), location(), 1, glm::value_ptr(glm::ivec4(value)));
    }
    return *this;
}

gl::Uniform& gl::Uniform::operator=(const glm::ivec4& value) {
    if (this->operator bool()) {
        glProgramUniform4iv(native_handle(), location(), 1, glm::value_ptr(value));
    }
    return *this;
}

gl::Uniform& gl::Uniform::operator=(const glm::uvec4& value) {
    if (this->operator bool()) {
        glProgramUniform4uiv(native_handle(), location(), 1, glm::value_ptr(value));
    }
    return *this;
}

gl::Uniform& gl::Uniform::operator=(const glm::vec4& value) {
    if (this->operator bool()) {
        glProgramUniform4fv(native_handle(), location(), 1, glm::value_ptr(value));
    }
    return *this;
}

gl::Uniform& gl::Uniform::operator=(const glm::dvec4& value) {
    if (this->operator bool()) {
        glProgramUniform4dv(native_handle(), location(), 1, glm::value_ptr(value));
    }
    return *this;
}

gl::Uniform& gl::Uniform::operator=(const glm::mat2& value) {
    if (this->operator bool()) {
        glProgramUniformMatrix2fv(native_handle(), location(), 1, false, glm::value_ptr(value));
    }
    return *this;
}

gl::Uniform& gl::Uniform::operator=(const glm::dmat2& value) {
    if (this->operator bool()) {
        glProgramUniformMatrix2dv(native_handle(), location(), 1, false, glm::value_ptr(value));
    }
    return *this;
}

gl::Uniform& gl::Uniform::operator=(const glm::mat2x3& value) {
    if (this->operator bool()) {
        glProgramUniformMatrix2x3fv(native_handle(), location(), 1, false, glm::value_ptr(value));
    }
    return *this;
}

gl::Uniform& gl::Uniform::operator=(const glm::dmat2x3& value) {
    if (this->operator bool()) {
        glProgramUniformMatrix2x3dv(native_handle(), location(), 1, false, glm::value_ptr(value));
    }
    return *this;
}

gl::Uniform& gl::Uniform::operator=(const glm::mat2x4& value) {
    if (this->operator bool()) {
        glProgramUniformMatrix2x3fv(native_handle(), location(), 1, false, glm::value_ptr(value));
    }
    return *this;
}

gl::Uniform& gl::Uniform::operator=(const glm::dmat2x4& value) {
    if (this->operator bool()) {
        glProgramUniformMatrix2x4dv(native_handle(), location(), 1, false, glm::value_ptr(value));
    }
    return *this;
}

gl::Uniform& gl::Uniform::operator=(const glm::mat3x2& value) {
    if (this->operator bool()) {
        glProgramUniformMatrix3x2fv(native_handle(), location(), 1, false, glm::value_ptr(value));
    }
    return *this;
}

gl::Uniform& gl::Uniform::operator=(const glm::dmat3x2& value) {
    if (this->operator bool()) {
        glProgramUniformMatrix3x2dv(native_handle(), location(), 1, false, glm::value_ptr(value));
    }
    return *this;
}

gl::Uniform& gl::Uniform::operator=(const glm::mat3& value) {
    if (this->operator bool()) {
        glProgramUniformMatrix3fv(native_handle(), location(), 1, false, glm::value_ptr(value));
    }
    return *this;
}

gl::Uniform& gl::Uniform::operator=(const glm::dmat3& value) {
    if (this->operator bool()) {
        glProgramUniformMatrix3dv(native_handle(), location(), 1, false, glm::value_ptr(value));
    }
    return *this;
}

gl::Uniform& gl::Uniform::operator=(const glm::mat3x4& value) {
    if (this->operator bool()) {
        glProgramUniformMatrix3x4fv(native_handle(), location(), 1, false, glm::value_ptr(value));
    }
    return *this;
}

gl::Uniform& gl::Uniform::operator=(const glm::dmat3x4& value) {
    if (this->operator bool()) {
        glProgramUniformMatrix3x4dv(native_handle(), location(), 1, false, glm::value_ptr(value));
    }
    return *this;
}

gl::Uniform& gl::Uniform::operator=(const glm::mat4x2& value) {
    if (this->operator bool()) {
        glProgramUniformMatrix4x2fv(native_handle(), location(), 1, false, glm::value_ptr(value));
    }
    return *this;
}

gl::Uniform& gl::Uniform::operator=(const glm::dmat4x2& value) {
    if (this->operator bool()) {
        glProgramUniformMatrix4x2dv(native_handle(), location(), 1, false, glm::value_ptr(value));
    }
    return *this;
}

gl::Uniform& gl::Uniform::operator=(const glm::mat4x3& value) {
    if (this->operator bool()) {
        glProgramUniformMatrix4x3fv(native_handle(), location(), 1, false, glm::value_ptr(value));
    }
    return *this;
}

gl::Uniform& gl::Uniform::operator=(const glm::dmat4x3& value) {
    if (this->operator bool()) {
        glProgramUniformMatrix4x3dv(native_handle(), location(), 1, false, glm::value_ptr(value));
    }
    return *this;
}

gl::Uniform& gl::Uniform::operator=(const glm::mat4& value) {
    if (this->operator bool()) {
        glProgramUniformMatrix4fv(native_handle(), location(), 1, false, glm::value_ptr(value));
    }
    return *this;
}

gl::Uniform& gl::Uniform::operator=(const glm::dmat4& value) {
    if (this->operator bool()) {
        glProgramUniformMatrix4dv(native_handle(), location(), 1, false, glm::value_ptr(value));
    }
    return *this;
}
