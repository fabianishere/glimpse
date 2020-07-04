#ifndef GLIMPSE_UNIFORM_H
#define GLIMPSE_UNIFORM_H

#include <glimpse/gl.hpp>
#include <glimpse/member.hpp>

#include <glm/glm.hpp>

#include <unordered_map>
#include <string>

namespace gl {
/**
 * A uniform is a global GLSL variable declared with the "uniform" storage
 * qualifier. These act as parameters that the user of a shader program can pass
 * to that program.
 */
class Uniform : public Member {
public:
    /**
     * Create an invalid uniform.
     */
    Uniform() noexcept;

    /**
     * Create a uniform variable.
     *
     * @param[in] handle The program this attribute belongs to.
     * @param[in] name The name of the attribute.
     * @param[in] type The type of the attribute.
     * @param[in] location The location of the attribute.
     * @param[in] count The number of array entries.
     */
    Uniform(gl::Handle handle, const std::string& name, gl::Type type, int location, int count) noexcept;

    /**
     * The type of the uniform.
     */
    gl::Type type() const noexcept;

    /**
     * The location of the uniform.
     */
    int location() const noexcept;

    /**
     * The array size of the uniform.
     */
    int count() const noexcept;

    /**
     * Write the specified value to uniform storage.
     */
    Uniform& operator=(bool value);
    Uniform& operator=(int value);
    Uniform& operator=(unsigned value);
    Uniform& operator=(float value);
    Uniform& operator=(double value);
    Uniform& operator=(const glm::bvec2& value);
    Uniform& operator=(const glm::ivec2& value);
    Uniform& operator=(const glm::uvec2& value);
    Uniform& operator=(const glm::vec2& value);
    Uniform& operator=(const glm::dvec2& value);
    Uniform& operator=(const glm::bvec3& value);
    Uniform& operator=(const glm::ivec3& value);
    Uniform& operator=(const glm::uvec3& value);
    Uniform& operator=(const glm::vec3& value);
    Uniform& operator=(const glm::dvec3& value);
    Uniform& operator=(const glm::bvec4& value);
    Uniform& operator=(const glm::ivec4& value);
    Uniform& operator=(const glm::uvec4& value);
    Uniform& operator=(const glm::vec4& value);
    Uniform& operator=(const glm::dvec4& value);
    Uniform& operator=(const glm::mat2& value);
    Uniform& operator=(const glm::dmat2& value);
    Uniform& operator=(const glm::mat2x3& value);
    Uniform& operator=(const glm::dmat2x3& value);
    Uniform& operator=(const glm::mat2x4& value);
    Uniform& operator=(const glm::dmat2x4& value);
    Uniform& operator=(const glm::mat3x2& value);
    Uniform& operator=(const glm::dmat3x2& value);
    Uniform& operator=(const glm::mat3& value);
    Uniform& operator=(const glm::dmat3& value);
    Uniform& operator=(const glm::mat3x4& value);
    Uniform& operator=(const glm::dmat3x4& value);
    Uniform& operator=(const glm::mat4x2& value);
    Uniform& operator=(const glm::dmat4x2& value);
    Uniform& operator=(const glm::mat4x3& value);
    Uniform& operator=(const glm::dmat4x3& value);
    Uniform& operator=(const glm::mat4& value);
    Uniform& operator=(const glm::dmat4& value);

private:
    gl::Type m_type{0xFFFFFF};
    int m_location{-1};
    int m_count{0};
};
}  // namespace gl
#endif /* GLIMPSE_UNIFORM_H */
