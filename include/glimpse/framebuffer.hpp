#ifndef GLIMPSE_FRAMEBUFFER_H
#define GLIMPSE_FRAMEBUFFER_H

#include <glimpse/gl.hpp>
#include <glimpse/renderbuffer.hpp>
#include <glimpse/texture.hpp>

#include <glm/glm.hpp>

#include <memory>
#include <optional>
#include <variant>

namespace gl {
/**
 * A {@link Framebuffer} is a collection of buffers that can be used as the
 * destination for rendering.
 * The buffers for {@link Framebuffer} objects
 * reference images from {@link Texture}s.
 */
class Framebuffer {
public:
    using ColorAttachment = std::variant<std::shared_ptr<gl::Texture>, std::shared_ptr<gl::Renderbuffer>>;
    using DepthAttachment =
    std::variant<std::monostate, std::shared_ptr<gl::Texture>, std::shared_ptr<gl::Renderbuffer>>;

    /**
     * Construct a {@link Framebuffer} object.
     *
     * @param[in]
     * color_attachments The color attachments of the framebuffer.
     *
     * @param[in] depth_attachment The optional depth attachment of the
     * framebuffer.
     */
    Framebuffer(const std::vector<ColorAttachment>& color_attachments, DepthAttachment depth_attachment);

    ~Framebuffer() noexcept;

    // Disable copy constructors
    Framebuffer(const Framebuffer&) = delete;
    Framebuffer& operator=(const Framebuffer&) = delete;

    // Enable move constructors
    Framebuffer(Framebuffer&&) noexcept;
    Framebuffer& operator=(Framebuffer&&) noexcept;

    Framebuffer& operator=(std::nullptr_t);

    /**
     * Determine whether the texture object is still valid.
     */
    explicit operator bool() const noexcept;

    /**
     * The width of the frame buffer.
     */
    int width() const noexcept;

    /**
     * The height of the frame buffer.
     */
    int height() const noexcept;

    /**
     * The samples of the frame buffer.
     */
    int samples() const noexcept;

    /**
     * The color attachments of the frame buffer.
     */
    const std::vector<ColorAttachment>& color_attachments() const noexcept;

    /**
     * The depth attachment of the frame buffer.
     */
    const DepthAttachment& depth_attachment() const noexcept;

    /**
     * Determine if the frame buffer has a depth attachment.
     */
    bool has_depth_attachment() const noexcept;

    /**
     * The viewport of the framebuffer.
     */
    const glm::ivec4& viewport() const noexcept;

    /**
     * The viewport of the framebuffer.
     */
    glm::ivec4& viewport() noexcept;

    /**
     * The scissor box of the framebuffer.
     */
    const std::optional<glm::ivec4>& scissor() const noexcept;

    /**
     * The scissor box of the framebuffer.
     */
    std::optional<glm::ivec4>& scissor() noexcept;

    /**
     * The handle to the native OpenGL object.
     */
    gl::Handle native_handle() const noexcept;

    /**
     * Clear the framebuffer.
     *
     * @param[in] color The color to clear the framebuffer with.
     * @param[in] depth The depth component.
     */
    void clear(const glm::vec4& color, float depth) noexcept;

    /**
     * Clear the framebuffer.
     *
     * @param[in] color The color to clear the framebuffer with.
     * @param[in] depth The depth component.
     * @param[in] viewport The viewport of the framebuffer.
     */
    void clear(const glm::vec4& color, float depth, const glm::ivec4& viewport) noexcept;

    /**
     * Bind the framebuffer. Sets the target for rendering commands.
     */
    void use();

    /**
     * Construct a {@link Framebuffer} with a single color attachment
     * and depth buffer using {@link Renderbuffer} attachments.
     *
     * @param[in] width The width of the framebuffer.
     * @param[in] height The height of the framebuffer.
     * @param[in] components The number of components 1, 2, 3 or 4.
     * @param[in] dtype The data type of the buffer.
     * @param[in] samples The number of samples. Value 0 means no multisample format.
     */
    static Framebuffer simple(int width,
                              int height,
                              int components,
                              const gl::PixelType& dtype = gl::PixelType::f8,
                              int samples = 0);

private:
    /**
     * Reset the object state.
     */
    void reset() noexcept;

    /**
     * Swap object state.
     */
    void swap(Framebuffer& other) noexcept;

    void clear(const glm::vec4& color, float depth, const glm::ivec4* viewport) noexcept;

    static constexpr gl::Handle INVALID = 0xFFFFFFFF;

    gl::Handle m_handle{INVALID};
    std::vector<ColorAttachment> m_color_attachments;
    DepthAttachment m_depth_attachment;
    int m_width;
    int m_height;
    int m_samples;
    glm::ivec4 m_viewport;
    std::optional<glm::ivec4> m_scissor{std::nullopt};
    std::vector<unsigned> m_draw_buffers;
    std::vector<bool> m_color_mask;
};
}  // namespace gl

#endif /* GLIMPSE_FRAMEBUFFER_H */
