#ifndef GLIMPSE_TYPES_H
#define GLIMPSE_TYPES_H

#include <cstdint>

namespace gl {
/**
 * OpenGL native handle type.
 */
using Handle = unsigned;

/**
 * OpenGL native type.
 */
using Type = unsigned;

/**
 * Convert the specified host type to its OpenGL type identifier.
 */
template <typename T>
Type type() noexcept;

// Available specializations
template <>
gl::Type gl::type<bool>() noexcept;

template <>
gl::Type gl::type<std::uint8_t>() noexcept;

template <>
gl::Type gl::type<std::int8_t>() noexcept;

template <>
gl::Type gl::type<std::uint16_t>() noexcept;

template <>
gl::Type gl::type<std::int16_t>() noexcept;

template <>
gl::Type gl::type<std::uint32_t>() noexcept;

template <>
gl::Type gl::type<std::int32_t>() noexcept;

template <>
gl::Type gl::type<float>() noexcept;

template <>
gl::Type gl::type<double>() noexcept;


} // namespace gl

#endif /* GLIMPSE_TYPES_H */
