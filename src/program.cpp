#include <glimpse/gl.hpp>
#include <glimpse/program.hpp>

#include <GL/glew.h>

#include <cassert>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

static bool checkShaderErrors(GLuint shader);
static bool checkProgramErrors(GLuint program);
static std::string readFile(std::filesystem::path filePath);

gl::Program::Program(unsigned handle) : m_handle(handle) {
    {
        GLint num_uniforms = 0;
        glGetProgramiv(m_handle, GL_ACTIVE_UNIFORMS, &num_uniforms);

        if (num_uniforms != 0) {
            GLint max_name_len = 0;
            GLsizei length = 0;
            GLsizei count = 0;
            GLenum type = GL_NONE;
            glGetProgramiv(m_handle, GL_ACTIVE_UNIFORM_MAX_LENGTH, &max_name_len);
            auto uniform_name = std::make_unique<char[]>(static_cast<size_t>(max_name_len));

            for (GLuint i = 0; i < static_cast<GLuint>(num_uniforms); i++) {
                glGetActiveUniform(m_handle, i, max_name_len, &length, &count, &type, uniform_name.get());

                int location = glGetUniformLocation(m_handle, uniform_name.get());

                if (location < 0) {
                    continue;
                }

                std::string str_name(uniform_name.get(), static_cast<size_t>(length));
                uniforms[str_name] = gl::Uniform(m_handle, str_name, type, location, count);
            }
        }
    }

    {
        GLint num_attributes = 0;
        glGetProgramiv(m_handle, GL_ACTIVE_ATTRIBUTES, &num_attributes);

        if (num_attributes != 0) {
            GLint max_name_len = 0;
            GLsizei length = 0;
            GLsizei count = 0;
            GLenum type = GL_NONE;
            glGetProgramiv(m_handle, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &max_name_len);
            auto attrib_name = std::make_unique<char[]>(static_cast<size_t>(max_name_len));

            for (GLuint i = 0; i < static_cast<GLuint>(num_attributes); i++) {
                glGetActiveAttrib(m_handle, i, max_name_len, &length, &count, &type, attrib_name.get());

                int location = glGetAttribLocation(m_handle, attrib_name.get());

                if (location < 0) {
                    continue;
                }

                std::string str_name(attrib_name.get(), (static_cast<size_t>(length)));
                attributes[str_name] = gl::Attribute(m_handle, str_name, type, location, count);
            }
        }
    }
}

void gl::Program::reset() noexcept {
    if (this->operator bool()) {
        glDeleteProgram(m_handle);
        m_handle = INVALID;
    }
}

void gl::Program::swap(gl::Program& other) noexcept {
    std::swap(m_handle, other.m_handle);
    std::swap(uniforms, other.uniforms);
    std::swap(attributes, other.attributes);
}

gl::Program::Program(gl::Program&& other) noexcept {
    swap(other);
}

gl::Program& gl::Program::operator=(gl::Program&& other) noexcept {
    swap(other);
    return *this;
}

void gl::Program::use() const noexcept {
    glUseProgram(native_handle());
}

gl::ProgramBuilder::~ProgramBuilder() {
    freeStages();
}

gl::ProgramBuilder& gl::ProgramBuilder::add_stage(unsigned stage, std::filesystem::path path) {
    if (!std::filesystem::exists(path)) {
        throw ProgramLoadingException("File does not exist");
    }

    const std::string shaderSource = readFile(path);
    const GLuint shader = glCreateShader(stage);
    const char* shaderSourcePtr = shaderSource.c_str();
    glShaderSource(shader, 1, &shaderSourcePtr, nullptr);
    glCompileShader(shader);
    if (!checkShaderErrors(shader)) {
        glDeleteShader(shader);
        throw ProgramLoadingException("Failed to compile shader {}");
    }

    m_stages.push_back(shader);
    return *this;
}

gl::Program gl::ProgramBuilder::build() {
    // Combine vertex and fragment shaders into a single shader program.
    unsigned handle = glCreateProgram();

    for (GLuint shader : m_stages) {
        glAttachShader(handle, shader);
    }
    glLinkProgram(handle);
    freeStages();

    if (!checkProgramErrors(handle)) {
        throw ProgramLoadingException("Shader program failed to link");
    }

    return Program(handle);
}

void gl::ProgramBuilder::freeStages() {
    for (GLuint shader : m_stages) {
        glDeleteShader(shader);
    }
}

static std::string readFile(std::filesystem::path filePath) {
    std::ifstream file(filePath, std::ios::binary);

    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

static bool checkShaderErrors(GLuint shader) {
    // Check if the shader compiled successfully.
    GLint compileSuccessful;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compileSuccessful);

    // If it didn't, then read and print the compile log.
    if (!compileSuccessful) {
        GLint logLength;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);

        std::string logBuffer;
        logBuffer.resize(static_cast<size_t>(logLength));
        glGetShaderInfoLog(shader, logLength, nullptr, logBuffer.data());

        std::cerr << logBuffer << std::endl;
        return false;
    } else {
        return true;
    }
}

static bool checkProgramErrors(GLuint program) {
    // Check if the program linked successfully
    GLint linkSuccessful;
    glGetProgramiv(program, GL_LINK_STATUS, &linkSuccessful);

    // If it didn't, then read and print the link log
    if (!linkSuccessful) {
        GLint logLength;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);

        std::string logBuffer;
        logBuffer.resize(static_cast<size_t>(logLength));
        glGetProgramInfoLog(program, logLength, nullptr, logBuffer.data());

        std::cerr << logBuffer << std::endl;
        return false;
    } else {
        return true;
    }
}
