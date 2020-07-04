#include <glimpse/attribute.hpp>

#include <GL/gl.h>

#include <stdexcept>

gl::Attribute::Attribute() noexcept = default;

gl::Attribute::Attribute(gl::Handle handle,
                         const std::string& name,
                         gl::Type type,
                         unsigned location,
                         int count) noexcept
    : Member(handle, name), m_type(type), m_location(location), m_count(count) {}

gl::Type gl::Attribute::type() const noexcept {
    return m_type;
}

unsigned gl::Attribute::location() const noexcept {
    return m_location;
}

int gl::Attribute::count() const noexcept {
    return m_count;
}
