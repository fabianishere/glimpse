#ifndef GLIMPSE_ATTRIBUTE_H
#define GLIMPSE_ATTRIBUTE_H

#include <glimpse/gl.hpp>
#include <glimpse/member.hpp>

#include <string>
#include <unordered_map>

namespace gl {
/**
 * An attribute of a shader program.
 */
class Attribute : public Member {
public:
    /**
     * Create an invalid uniform.
     */
    Attribute() noexcept;

    /**
     * Construct a new attribute.
     *
     * @param[in] handle The program this attribute belongs to.
     * @param[in] name The name of the attribute.
     * @param[in] type The type of the attribute.
     * @param[in] location The location of the attribute.
     * @param[in] count The number of array entries.
     */
    Attribute(gl::Handle handle,
              const std::string& name,
              gl::Type type,
              unsigned location,
              int count) noexcept;

    /**
     * The type of attribute.
     */
    gl::Type type() const noexcept;

    /**
     * The location of the attribute.
     */
    unsigned location() const noexcept;

    /**
     * The array size of the attribute.
     */
    int count() const noexcept;

private:
    gl::Type m_type{0xFFFFFFF};
    unsigned m_location{0xFFFFFFF};
    int m_count{0};
};
}  // namespace gl

#endif /* GLIMPSE_ATTRIBUTE_H */
