#ifndef GLIMPSE_MEMBER_H
#define GLIMPSE_MEMBER_H

#include <glimpse/gl.hpp>

#include <string>
#include <utility>

namespace gl {
/**
 * A member of a OpenGL shader program.
 */
class Member {
public:
    /**
     * Create an invalid member.
     */
    Member() noexcept;

    /**
     * Create a member.
     *
     * @param[in] handle The program this member belongs to.
     * @param[in] name The name of the member.
     */
    Member(gl::Handle handle, std::string  name) noexcept;

    /**
     * Determine whether the vertex array object is still valid.
     */
    explicit operator bool() const noexcept;

    /**
     * The name of the member.
     */
    const std::string& name() const noexcept;

    /**
     * The native handle of this member.
     */
    gl::Handle native_handle() const noexcept;

private:
    static constexpr gl::Handle INVALID = 0xFFFFFFFF;

    gl::Handle m_handle{INVALID};
    std::string m_name;
};
}  // namespace gl

#endif /* GLIMPSE_MEMBER_H */
