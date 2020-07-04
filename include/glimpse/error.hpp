#ifndef GLIMPSE_ERROR_H
#define GLIMPSE_ERROR_H

#include <exception>

namespace gl {
/**
 * An exception that is thrown by OpenGL API calls.
 */
class Error : public std::exception {
public:
    /**
     * Construct an OpenGL exception based on the error code.
     */
    explicit Error(unsigned code) noexcept;

    /**
     * Obtain the underlying error code of the exception.
     */
    unsigned code() const noexcept;

    /**
     * The error message of the exception.
     */
    const char* what() const noexcept override;

private:
    /**
     * The error code specified by the runtime.
     */
    unsigned m_code;
};
} // namespace gl

#endif /* GLIMPSE_ERROR_H */
