#ifndef GLIMPSE_VERTEX_ARRAY_H
#define GLIMPSE_VERTEX_ARRAY_H

#include <glimpse/gl.hpp>
#include <glimpse/data.hpp>
#include <glimpse/program.hpp>

#include <optional>
#include <unordered_map>

namespace gl {
/**
 * A VertexArray object is an OpenGL object that stores all of the state needed
 * to supply vertex data. It stores the format of the vertex data as well as the
 * Buffer objects providing the vertex data arrays.
 */
class VertexArray {
public:
    /**
     * Create a vertex array object.
     *
     * @param[in] program The shader program to attach to this vertex array.
     * @param[in] data The data to assign to the shader attributes.
     * @param[in] indices The index buffer to use.
     */
    VertexArray(std::shared_ptr<gl::Program> program,
                const std::unordered_map<std::string, gl::Data>& data,
                const gl::Data& indices)
        : VertexArray(program, data, std::optional(indices)) {}

    /**
     * Create a vertex array object.
     *
     * @param[in] program The shader program to attach to this vertex array.
     * @param[in] data The data to assign to the shader attributes.
     * @param[in] indices The index buffer to use.
     */
    VertexArray(std::shared_ptr<gl::Program> program,
                const std::unordered_map<std::string, gl::Data>& data,
                std::optional<gl::Data> indices = std::nullopt);

    ~VertexArray() noexcept { reset(); }

    // Disable copy constructors
    VertexArray(const VertexArray&) = delete;
    VertexArray& operator=(const VertexArray&) = delete;

    // Enable move constructors
    VertexArray(VertexArray&&) noexcept;
    VertexArray& operator=(VertexArray&&) noexcept;

    VertexArray& operator=(std::nullptr_t) {
        reset();
        return *this;
    }

    /**
     * Determine whether the vertex array object is still valid.
     */
    operator bool() const noexcept { return m_handle != INVALID; }

    /**
     * Obtain the shader program attached to this vertex array.
     */
    gl::Program& program() const noexcept { return *m_program; }

    /**
     * The index buffer of this vertex array.
     */
    std::optional<gl::Data> indices() const noexcept { return m_indices; }

    /**
     * The reference to the native OpenGL handle of the buffer.
     */
    gl::Handle native_handle() const noexcept { return m_handle; }

    /**
     * Render the vertex array to the framebuffer.
     *
     * @param[in] mode The rendering mode to use.
     * @param[in] vertices The number of vertices to render.
     */
    void render(unsigned mode, int vertices = -1) const;

private:
    /**
     * Reset the object state.
     */
    void reset() noexcept;

    /**
     * Swap object state.
     */
    void swap(VertexArray& other) noexcept;

    static constexpr gl::Handle INVALID = 0xFFFFFFFF;

    gl::Handle m_handle{INVALID};
    std::shared_ptr<gl::Program> m_program;
    std::optional<gl::Data> m_indices;
    std::unordered_map<std::string, gl::Data> m_data;
    size_t m_num_vertices{};
};
}  // namespace gl

#endif /* GLIMPSE_VERTEX_ARRAY_H */
