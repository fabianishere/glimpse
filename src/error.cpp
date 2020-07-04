#include <glimpse/error.hpp>

#include <GL/glu.h>

gl::Error::Error(unsigned code) noexcept : m_code(code) {}

unsigned gl::Error::code() const noexcept {
    return m_code;
}

const char* gl::Error::what() const noexcept {
    return reinterpret_cast<const char*>(gluErrorString(m_code));
}
