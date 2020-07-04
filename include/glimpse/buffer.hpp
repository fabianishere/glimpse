#ifndef GLIMPSE_BUFFER_H
#define GLIMPSE_BUFFER_H

#include <glimpse/gl.hpp>

#include <memory>
#include <vector>

namespace gl {
/**
 * An untyped OpenGL buffer that is allocated on the GPU.
 * These can be used to store vertex data, pixel data retrieved from images or
 * the framebuffer, and a variety of other things.
 *
 * Buffers have unique ownership and may not be copied (only moved). Use for
 * instance <code>std::shared_ptr</code> to share a buffer between instances.
 */
class Buffer {
public:
    /**
     * An enumeration of the different buffer usage types.
     */
    enum class Type { STREAM, DYNAMIC, STATIC };

    /**
     * Allocate a buffer with the specified reserved capacity.
     *
     * @param[in] reserve The capacity to reserve in bytes.
     * @param[in] type The type of buffer to allocate
     */
    explicit Buffer(size_t reserve, Buffer::Type type = Buffer::Type::STATIC)
        : Buffer(std::vector<char>(reserve), type) {}

    /**
     * Allocate a buffer with the specified data.
     *
     * @param[in] data The data to send to the GPU.
     * @param[in] type The type of buffer to allocate
     */
    template <typename T>
    explicit Buffer(const typename std::vector<T>& data,
                    Buffer::Type type = Buffer::Type::STATIC)
        : Buffer(data.data(), sizeof(T) * data.size(), type) {}

    /**
     * Allocate a buffer with the specified data.
     *
     * @param[in] data The data to send to the GPU.
     * @param[in] size The size in bytes of the data to send.
     * @param[in] type The type of buffer to allocate
     */
    explicit Buffer(const void* data,
                    size_t size,
                    Buffer::Type type = Buffer::Type::STATIC);

    ~Buffer() noexcept;

    // Disable copy constructors
    Buffer(const Buffer&) = delete;
    Buffer& operator=(const Buffer&) = delete;

    // Enable move constructors
    Buffer(Buffer&&) noexcept;
    Buffer& operator=(Buffer&&) noexcept;

    Buffer& operator=(std::nullptr_t);

    /**
     * Determine whether the vertex array object is still valid.
     */
    explicit operator bool() const noexcept;

    /**
     * The size of the buffer in bytes.
     */
    size_t size() const noexcept;

    /**
     * The usage type of the buffer.
     */
    Buffer::Type type() const noexcept;

    /**
     * The reference to the native OpenGL handle of the buffer.
     */
    gl::Handle native_handle() const noexcept;

    /**
     * Write the specified data to the buffer.
     *
     * @param[in] data The data to write to the buffer.
     * @param[in] offset The offset to write the data at.
     */
    template <typename T>
    void write(const typename std::vector<T>& data, int offset = 0) noexcept {
        size_t size = sizeof(T) * data.size();
        write(data.data(), size, offset);
    }

    /**
     * Write the specified data to the buffer.
     *
     * @param[in] data The data to write to the buffer.
     * @param[in] size The size of the data to write.
     * @param[in] offset The offset to write the data at.
     */
    void write(const void* data, size_t size, int offset = 0) noexcept;

    /**
     * Read the buffer from GPU memory.
     */
    std::vector<unsigned char> read() const;

    /**
     * Read the buffer from GPU memory.
     *
     * @param[in] size The size of the buffer to read.
     * @param[in] offset The offset in GPU memory at which to read.
     */
    std::vector<unsigned char> read(size_t size, int offset = 0) const;

private:
    /**
     * Reset the object state.
     */
    void reset() noexcept;

    /**
     * Swap object state.
     */
    void swap(Buffer& other) noexcept;

    static constexpr gl::Handle INVALID = 0xFFFFFFFF;

    gl::Handle m_handle{INVALID};
    size_t m_size{};
    Buffer::Type m_type;
};
}  // namespace gl

#endif /* GLIMPSE_BUFFER_H */
