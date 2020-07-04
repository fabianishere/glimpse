#ifndef GLIMPSE_GL_H
#define GLIMPSE_GL_H

#include <glimpse/types.hpp>
#include <glimpse/error.hpp>

#include <array>
#include <exception>
#include <stdexcept>
#include <vector>

namespace gl {

/**
 * An {@link ElementDescriptor} describes an element in a buffer.
 */
class ElementDescriptor {
public:
    /**
     * Construct an {@link ElementDescriptor}.
     *
     * @param[in] type The type of this element.
     * @param[in] size The size of the element.
     * @param[in] count The number of components in case the element is a vector type.
     */
    ElementDescriptor(Type type, size_t size, size_t count) noexcept;

    /**
     * The OpenGL type identifier which represent the type of this element.
     */
    Type type() const noexcept;

    /**
     * The size of the element in bytes.
     */
    size_t size() const noexcept;

    /**
     * The number of components in case the element is a vector type.
     */
    size_t count() const noexcept;

    /**
     * Obtain an {@link ElementDescriptor} for the specified type <code>T</code>.
     */
    template <typename T>
    static ElementDescriptor get() {
        return converter<T>::get();
    }

private:
    template <typename T, typename = void>
    struct converter {
        static ElementDescriptor get() { return {gl::type<unsigned>(), 1, sizeof(T)}; }
    };

    // Specialization for OpenGL types
    template <typename T>
    struct converter<T, std::enable_if_t<std::is_arithmetic<T>::value>> {
        static ElementDescriptor get() { return {gl::type<T>(), sizeof(T), 1}; }
    };

    // Specialization for GLM vector types
    template <typename T>
    struct converter<T, std::void_t<typename T::value_type>> {
        static ElementDescriptor get() {
            return {gl::type<typename T::value_type>(), sizeof(typename T::value_type), T::length()};
        }
    };

    Type m_type;
    size_t m_size;
    size_t m_count;
};
}  // namespace gl

#endif /* GLIMPSE_GL_H */
