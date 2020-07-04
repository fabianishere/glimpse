#include <glimpse/member.hpp>

#include <utility>

gl::Member::Member() noexcept = default;

gl::Member::Member(gl::Handle handle, std::string name) noexcept : m_handle(handle), m_name(std::move(name)) {}

gl::Member::operator bool() const noexcept {
    return m_handle != INVALID;
}

const std::string& gl::Member::name() const noexcept {
    return m_name;
}

gl::Handle gl::Member::native_handle() const noexcept {
    return m_handle;
}
