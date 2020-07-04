#ifndef GLIMPSE_IMAGE_FORMAT_H
#define GLIMPSE_IMAGE_FORMAT_H

#include <glimpse/types.hpp>

#include <array>
#include <vector>

namespace gl {
/**
 * Data type for pixel buffers.
 */
class ImageFormat {
public:
    /**
     * Construct a data type.
     */
    ImageFormat(gl::Type type, size_t size, std::array<std::pair<unsigned, unsigned>, 4>&& formats);

    /**
     * The OpenGL type corresponding to this datatype.
     */
    Type type() const noexcept;

    /**
     * The size of a pixel.
     */
    size_t size() const noexcept;

    /**
     * The storage formats for this datatype.
     */
    const std::array<std::pair<unsigned, unsigned>, 4>& formats() const noexcept;

    /**
     * Return the storage format for the specified number of components.
     */
    std::pair<unsigned, unsigned> format(int components) const;

    /**
     * Compare the specified data type for equality.
     */
    bool operator==(const gl::ImageFormat& other) const noexcept;

    /**
     * Compare the specified data type for inequality.
     */
    bool operator!=(const gl::ImageFormat& other) const noexcept;

    /**
     * An 8-bit floating point.
     */
    static const ImageFormat f8;

    /**
     * A 16-bit floating point.
     */
    static const ImageFormat f16;

    /**
     * A 32-bit floating point.
     */
    static const ImageFormat f32;

    /**
     * An 8-bit unsigned integer.
     */
    static const ImageFormat u8;

    /**
     * A 16-bit unsigned integer.
     */
    static const ImageFormat u16;

    /**
     * A 32-bit unsigned integer.
     */
    static const ImageFormat u32;

    /**
     * A 8-bit signed integer.
     */
    static const ImageFormat i8;

    /**
     * A 16-bit signed integer.
     */
    static const ImageFormat i16;

    /**
     * A 32-bit signed integer.
     */
    static const ImageFormat i32;

private:
    Type m_type;
    size_t m_size;
    std::array<std::pair<unsigned, unsigned>, 4> m_formats;
};
}  // namespace gl

#endif /* GLIMPSE_IMAGE_FORMAT_H */
