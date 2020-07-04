#include <glimpse/types.hpp>

#include <GL/gl.h>

template<>
gl::Type gl::type<bool>() noexcept {
    return GL_BOOL;
}

template <>
gl::Type gl::type<std::uint8_t>() noexcept {
    return GL_UNSIGNED_BYTE;
}

template <>
gl::Type gl::type<std::int8_t>() noexcept {
    return GL_BYTE;
}

template <>
gl::Type gl::type<std::uint16_t>() noexcept {
    return GL_UNSIGNED_SHORT;
}

template <>
gl::Type gl::type<std::int16_t>() noexcept {
    return GL_SHORT;
}

template <>
gl::Type gl::type<std::uint32_t>() noexcept {
    return GL_UNSIGNED_INT;
}

template <>
gl::Type gl::type<std::int32_t>() noexcept {
    return GL_INT;
}

template <>
gl::Type gl::type<float>() noexcept {
    return GL_FLOAT;
}

template <>
gl::Type gl::type<double>() noexcept {
    return GL_DOUBLE;
}
