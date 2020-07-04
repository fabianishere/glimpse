#ifndef GLIMPSE_DATA_H
#define GLIMPSE_DATA_H

#include <glimpse/gl.hpp>
#include <glimpse/buffer.hpp>

#include <memory>
#include <utility>
#include <valarray>
#include <vector>

namespace gl {
template <typename T, typename = void>
class TypedData;

/**
 * A view on an OpenGL buffer resource allocated on the GPU.
 * This class manages ownership of native OpenGL buffers and shares it with
 * slices created from the instance.
 */
class Data {
public:
    /**
     * Allocate an untyped view from the specified buffer.
     *
     * @param[in] buffer The buffer to take shared ownership of.
     * @param[in] slice The slice of the buffer to represent.
     * @param[in] descriptor The descriptor describing the contents of the slice.
     */
    Data(std::shared_ptr<gl::Buffer> buffer,
         std::slice slice,
         gl::ElementDescriptor descriptor = gl::ElementDescriptor::get<unsigned>())
        : m_buffer(std::move(buffer)), m_slice(slice), m_descriptor(descriptor) {}

    /**
     * Allocate an untyped view for the specified buffer.
     *
     * @param[in] buffer The buffer to take ownership of.
     * @param[in] stride The size of the elements in the buffer.
     * @param[in] descriptor The descriptor describing the contents of the
     * slice.
     */
    Data(gl::Buffer&& buffer,
         std::slice slice,
         gl::ElementDescriptor descriptor = gl::ElementDescriptor::get<unsigned>())
        : Data(std::make_shared<gl::Buffer>(std::move(buffer)), slice, descriptor) {}

    /**
     * Allocate an untyped buffer with the specified reserved capacity.
     *
     * @param[in] reserve The capacity to reserve as the number of items of type
     * <code>T</code>.
     * @param[in] element_size The size of the elements in the buffer.
     * @param[in] type The type of buffer to allocate
     * @param[in] descriptor The descriptor describing the contents of the
     * slice.
     */
    explicit Data(size_t reserve,
                  size_t element_size,
                  gl::Buffer::Type type = gl::Buffer::Type::STATIC,
                  gl::ElementDescriptor descriptor = gl::ElementDescriptor::get<unsigned>())
        : Data(std::make_shared<gl::Buffer>(reserve * element_size, type),
               std::slice(0, reserve * element_size, element_size),
               descriptor) {}

    /**
     * Allocate an untyped buffer from the specified buffer.
     *
     * @param[in] buffer The buffer to take ownership of.
     * @param[in] stride The size of the elements in the buffer.
     */
    template <typename T>
    explicit Data(gl::Buffer&& buffer, gl::ElementDescriptor descriptor = gl::ElementDescriptor::get<T>())
        : Data(buffer, std::slice(0, buffer.size(), sizeof(T)), descriptor) {}

    /**
     * Allocate a buffer with the specified reserved capacity.
     *
     * @param[in] reserve The capacity to reserve in bytes.
     * @param[in] type The type of buffer to allocate.
     * @param[in] descriptor The descriptor describing the contents of the
     * slice.
     */
    template <typename T>
    explicit Data(size_t reserve,
                  gl::Buffer::Type type = gl::Buffer::Type::STATIC,
                  gl::ElementDescriptor descriptor = gl::ElementDescriptor::get<T>())
        : Data(reserve, sizeof(T), type, descriptor) {}

    /**
     * Allocate a buffer with the specified data.
     *
     * @param[in] data The data to send to the GPU.
     * @param[in] type The type of buffer to allocate
     * @param[in] descriptor The descriptor describing the contents of the slice.
     */
    template <typename T>
    explicit Data(const typename std::vector<T>& data,
                  gl::Buffer::Type type = gl::Buffer::Type::STATIC,
                  gl::ElementDescriptor descriptor = gl::ElementDescriptor::get<T>())
        : Data(std::move(gl::Buffer(data, type)), std::slice(0, data.size() * sizeof(T), sizeof(T)), descriptor) {}

    /**
     * The size of the buffer as the number of items of type <code>T</code>.
     */
    size_t size() const noexcept { return m_buffer->size() / m_slice.stride(); }

    /**
     * The underlying buffer that holds the data.
     */
    Buffer& buffer() noexcept { return *m_buffer; }

    /**
     * The underlying buffer that holds the data.
     */
    const Buffer& buffer() const noexcept { return *m_buffer; }

    /**
     * The slice that represents the view of the data.
     */
    std::slice slice() const noexcept { return m_slice; }

    /**
     * The {@link ElementDescriptor} describing the elements.
     */
    const ElementDescriptor& descriptor() const noexcept { return m_descriptor;  }

    /**
     * Select subsets of the elements in memory.
     *
     * @param[in] offset The offset in memory from the base of the element.
     * @param[in] stride The stride between the elements.
     */
    Data select(size_t offset, size_t stride) const noexcept {
        std::slice slice(offset, size(), stride);
        return Data(m_buffer, slice);
    }

    /**
     * Reinterpret the data inside the buffer.
     *
     * @param[in] descriptor The descriptor that describes the format of the elements.
     */
    Data reshape(gl::ElementDescriptor descriptor) const noexcept {
        return Data(m_buffer, m_slice, descriptor);
    }

protected:
    std::shared_ptr<gl::Buffer> m_buffer;
    std::slice m_slice;
    gl::ElementDescriptor m_descriptor;
};

/**
 * An instantiation of {@link TypedData} for classes which allows the user to
 * select class members.
 */
template <typename T>
class TypedData<T, typename std::enable_if_t<std::is_class<T>::value>> final : public Data {
    using Data::Data;

private:
    // XXX Hack to extract type of class member (requires C++17)
    template <auto value>
    struct member_type_extractor {};

    template <typename R, R T::*value>
    struct member_type_extractor<value> {
        using type = R;
    };

public:
    template <auto member>
    TypedData<typename member_type_extractor<member>::type> select() const noexcept {
        using M = typename member_type_extractor<member>::type;
        // XXX Hack to compute the offset from T's base to the selected member
        // type See
        // http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2018/p0908r0.html
        alignas(T) char container_space[sizeof(T)] = {};
        T* fake_container = reinterpret_cast<T*>(container_space);
        size_t offset =
            reinterpret_cast<uintptr_t>(&(fake_container->*member)) - reinterpret_cast<uintptr_t>(fake_container);
        std::slice slice(offset, size(), sizeof(T));
        gl::ElementDescriptor descriptor = gl::ElementDescriptor::get<M>();
        return TypedData<M>(m_buffer, slice, descriptor);
    }
};

/**
 * An instantiation of {@link TypedData} for non-class types.
 */
template <typename T>
class TypedData<T, typename std::enable_if_t<!std::is_class<T>::value>> final : public Data {
    using Data::Data;
};
}  // namespace gl

#endif /* GLIMPSE_DATA_H */
