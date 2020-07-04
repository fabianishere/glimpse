#ifndef GLIMPSE_PROGRAM_H
#define GLIMPSE_PROGRAM_H

#include <glimpse/gl.hpp>
#include <glimpse/attribute.hpp>
#include <glimpse/uniform.hpp>

#include <exception>
#include <filesystem>
#include <unordered_map>
#include <vector>

namespace gl {
/**
 * An exception that is thrown when a program fails to load.
 */
struct ProgramLoadingException : public std::runtime_error {
    using std::runtime_error::runtime_error;
};

/**
 * A Program object represents fully processed executable code in the OpenGL
 * Shading Language, for one or more Shader stages.
 */
class Program {
public:
    ~Program() noexcept;

    // Disable copy constructors
    Program(const Program&) = delete;
    Program& operator=(const Program&) = delete;

    // Enable move constructors
    Program(Program&&) noexcept;
    Program& operator=(Program&&) noexcept;

    Program& operator=(std::nullptr_t);

    /**
     * Determine whether the vertex array object is still valid.
     */
    explicit operator bool() const noexcept;

    /**
     * The native OpenGL handle.
     */
    gl::Handle native_handle() const noexcept;

    /**
     * The attributes of the program.
     */
    std::unordered_map<std::string, gl::Attribute> attributes;

    /**
     * The uniforms of the program.
     */
    std::unordered_map<std::string, gl::Uniform> uniforms;

    /**
     * Use the program.
     */
    void use() const noexcept;

private:
    friend class ProgramBuilder;
    Program(gl::Handle handle);

private:
    /**
     * Reset the object state.
     */
    void reset() noexcept;

    /**
     * Swap object state.
     */
    void swap(Program& other) noexcept;

    static constexpr gl::Handle INVALID = 0xFFFFFFFF;

    gl::Handle m_handle{INVALID};
};

/**
 * Helper class for constructing OpenGL programs.
 */
class ProgramBuilder {
public:
    ProgramBuilder() = default;
    ProgramBuilder(const ProgramBuilder&) = delete;
    ProgramBuilder(ProgramBuilder&&) = default;
    ~ProgramBuilder();

    /**
     * Add a stage to the program to construct.
     *
     * @param[in] stage The stage of the shader.
     * @param[in] path The path to the shader.
     */
    ProgramBuilder& add_stage(unsigned stage, std::filesystem::path path);

    /**
     * Build a program from the loaded stages.
     */
    Program build();

private:
    void freeStages();

private:
    std::vector<unsigned> m_stages;
};
}  // namespace gl

#endif /* GLIMPSE_PROGRAM_H */
