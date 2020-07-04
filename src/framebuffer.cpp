#include <glimpse/framebuffer.hpp>

#include <GL/glew.h>

#include <stdexcept>

gl::Framebuffer::Framebuffer(const std::vector<ColorAttachment>& color_attachments,
                             const DepthAttachment depth_attachment)
    : m_color_attachments(color_attachments), m_depth_attachment(depth_attachment) {
    if (color_attachments.empty() && std::holds_alternative<std::monostate>(depth_attachment)) {
        throw std::invalid_argument("The framebuffer cannot be empty");
    }

    // If the attachment sizes are not all identical, rendering will be limited
    // to the largest area that can fit in all of the attachments (an
    // intersection of rectangles having a lower left of (0; 0) and an upper
    // right of (width; height) for each attachment).

    int width = 0;
    int height = 0;
    int samples = 0;

    for (size_t i = 0; i < color_attachments.size(); i++) {
        auto color_attachment = color_attachments[i];
        if (std::holds_alternative<std::shared_ptr<gl::Texture>>(color_attachment)) {
            auto texture = std::get<std::shared_ptr<gl::Texture>>(color_attachment);

            if (i == 0) {
                width = texture->width();
                height = texture->height();
                samples = texture->samples();
            } else if (texture->width() != width || texture->height() != height || texture->samples() != samples) {
                throw std::invalid_argument("The color_attachments have different sizes or samples");
            }
        } else {
            auto renderbuffer = std::get<std::shared_ptr<gl::Renderbuffer>>(color_attachment);

            if (i == 0) {
                width = renderbuffer->width();
                height = renderbuffer->height();
                samples = renderbuffer->samples();
            } else if (renderbuffer->width() != width || renderbuffer->height() != height ||
                       renderbuffer->samples() != samples) {
                throw std::invalid_argument("The color_attachments have different sizes or samples");
            }
        }
    }

    if (std::holds_alternative<std::shared_ptr<gl::Texture>>(depth_attachment)) {
        auto texture = std::get<std::shared_ptr<gl::Texture>>(depth_attachment);

        if (color_attachments.empty()) {
            width = texture->width();
            height = texture->height();
            samples = texture->samples();
        } else if (texture->width() != width || texture->height() != height || texture->samples() != samples) {
            throw std::invalid_argument("The depth_attachments have different sizes or samples");
        }
    } else if (std::holds_alternative<std::shared_ptr<gl::Renderbuffer>>(depth_attachment)) {
        auto renderbuffer = std::get<std::shared_ptr<gl::Renderbuffer>>(depth_attachment);

        if (color_attachments.empty()) {
            width = renderbuffer->width();
            height = renderbuffer->height();
            samples = renderbuffer->samples();
        } else if (renderbuffer->width() != width || renderbuffer->height() != height ||
                   renderbuffer->samples() != samples) {
            throw std::invalid_argument("The depth_attachments have different sizes or samples");
        }
    }

    glCreateFramebuffers(1, &m_handle);

    if (color_attachments.empty()) {
        glNamedFramebufferDrawBuffer(m_handle, GL_NONE);
    }

    for (size_t i = 0; i < color_attachments.size(); i++) {
        auto color_attachment = color_attachments[i];
        if (std::holds_alternative<std::shared_ptr<gl::Texture>>(color_attachment)) {
            auto texture = std::get<std::shared_ptr<gl::Texture>>(color_attachment);
            glNamedFramebufferTexture2DEXT(m_handle, GL_COLOR_ATTACHMENT0 + static_cast<GLenum>(i),
                                           texture->samples() ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D,
                                           texture->native_handle(), 0);

        } else {
            auto renderbuffer = std::get<std::shared_ptr<gl::Renderbuffer>>(color_attachment);
            glNamedFramebufferRenderbuffer(m_handle, GL_COLOR_ATTACHMENT0 + static_cast<GLenum>(i), GL_RENDERBUFFER,
                                           renderbuffer->native_handle());
        }
    }

    if (std::holds_alternative<std::shared_ptr<gl::Texture>>(depth_attachment)) {
        auto texture = std::get<std::shared_ptr<gl::Texture>>(depth_attachment);
        glNamedFramebufferTexture2DEXT(m_handle, GL_DEPTH_ATTACHMENT,
                                       texture->samples() ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D,
                                       texture->native_handle(), 0);

    } else if (std::holds_alternative<std::shared_ptr<gl::Renderbuffer>>(depth_attachment)) {
        auto renderbuffer = std::get<std::shared_ptr<gl::Renderbuffer>>(depth_attachment);
        glNamedFramebufferRenderbuffer(m_handle, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, renderbuffer->native_handle());
    }

    int status = glCheckNamedFramebufferStatus(m_handle, GL_FRAMEBUFFER);
    if (status != GL_FRAMEBUFFER_COMPLETE) {
        const char* message = "The framebuffer is not complete";

        switch (status) {
            case GL_FRAMEBUFFER_UNDEFINED:
                message = "The framebuffer is not complete (UNDEFINED)";
                break;

            case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
                message = "The framebuffer is not complete (INCOMPLETE_ATTACHMENT)";
                break;

            case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
                message = "The framebuffer is not complete (INCOMPLETE_MISSING_ATTACHMENT)";
                break;

            case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
                message = "The framebuffer is not complete (INCOMPLETE_DRAW_BUFFER)";
                break;

            case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
                message = "The framebuffer is not complete (INCOMPLETE_READ_BUFFER)";
                break;

            case GL_FRAMEBUFFER_UNSUPPORTED:
                message = "The framebuffer is not complete (UNSUPPORTED)";
                break;

            case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:
                message = "The framebuffer is not complete (INCOMPLETE_MULTISAMPLE)";
                break;

            case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS:
                message = "The framebuffer is not complete (INCOMPLETE_LAYER_TARGETS)";
                break;
        }

        throw std::runtime_error(message);
    }

    m_width = width;
    m_height = height;
    m_samples = samples;

    m_viewport[0] = 0;
    m_viewport[1] = 0;
    m_viewport[2] = width;
    m_viewport[3] = height;

    m_draw_buffers.resize(color_attachments.size());
    for (size_t i = 0; i < color_attachments.size(); i++) {
        m_draw_buffers[i] = GL_COLOR_ATTACHMENT0 + static_cast<GLenum>(i);
    }

    m_color_mask.resize(color_attachments.size() * 4 + 1);

    for (size_t i = 0; i < color_attachments.size(); i++) {
        auto color_attachment = color_attachments[i];
        if (std::holds_alternative<std::shared_ptr<gl::Texture>>(color_attachment)) {
            auto texture = std::get<std::shared_ptr<gl::Texture>>(color_attachment);
            m_color_mask[i * 4 + 0] = texture->components() >= 1;
            m_color_mask[i * 4 + 1] = texture->components() >= 2;
            m_color_mask[i * 4 + 2] = texture->components() >= 3;
            m_color_mask[i * 4 + 3] = texture->components() >= 4;
        } else {
            auto renderbuffer = std::get<std::shared_ptr<gl::Renderbuffer>>(color_attachment);
            m_color_mask[i * 4 + 0] = renderbuffer->components() >= 1;
            m_color_mask[i * 4 + 1] = renderbuffer->components() >= 2;
            m_color_mask[i * 4 + 2] = renderbuffer->components() >= 3;
            m_color_mask[i * 4 + 3] = renderbuffer->components() >= 4;
        }
    }
}

void gl::Framebuffer::reset() noexcept {
    if (this->operator bool()) {
        glDeleteFramebuffers(1, &m_handle);
        m_handle = INVALID;
    }
}

void gl::Framebuffer::swap(gl::Framebuffer& other) noexcept {
    std::swap(m_handle, other.m_handle);
    std::swap(m_width, other.m_width);
    std::swap(m_height, other.m_height);
    std::swap(m_samples, other.m_samples);
    std::swap(m_viewport, other.m_viewport);
    std::swap(m_scissor, other.m_scissor);
    std::swap(m_color_attachments, other.m_color_attachments);
    std::swap(m_depth_attachment, other.m_depth_attachment);
    std::swap(m_draw_buffers, other.m_draw_buffers);
    std::swap(m_color_mask, other.m_color_mask);
}

gl::Framebuffer::Framebuffer(gl::Framebuffer&& other) noexcept {
    swap(other);
}

gl::Framebuffer& gl::Framebuffer::operator=(gl::Framebuffer&& other) noexcept {
    swap(other);
    return *this;
}

void gl::Framebuffer::clear(const glm::vec4& color, float depth, const glm::ivec4* viewport) noexcept {
    assert(this->operator bool());

    glBindFramebuffer(GL_FRAMEBUFFER, m_handle);
    glDrawBuffers(m_draw_buffers.size(), m_draw_buffers.data());

    glClearColor(color[0], color[1], color[2], color[3]);
    glClearDepth(depth);

    for (size_t i = 0; i < m_draw_buffers.size(); i++) {
        glColorMaski(i, m_color_mask[i * 4 + 0], m_color_mask[i * 4 + 1], m_color_mask[i * 4 + 2],
                     m_color_mask[i * 4 + 3]);
    }

    glDepthMask(has_depth_attachment());

    // Respect the passed in viewport even with scissor enabled
    if (viewport) {
        glEnable(GL_SCISSOR_TEST);
        glScissor((*viewport)[0], (*viewport)[1], (*viewport)[2], (*viewport)[3]);
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

        // restore scissor if enabled
        if (m_scissor) {
            glScissor((*m_scissor)[0], (*m_scissor)[1], (*m_scissor)[2], (*m_scissor)[3]);
        } else {
            glDisable(GL_SCISSOR_TEST);
        }
    } else {
        // clear with scissor if enabled
        if (m_scissor) {
            glEnable(GL_SCISSOR_TEST);
            glScissor((*m_scissor)[0], (*m_scissor)[1], (*m_scissor)[2], (*m_scissor)[3]);
        }
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void gl::Framebuffer::use() {
    assert(this->operator bool());

    glBindFramebuffer(GL_FRAMEBUFFER, m_handle);
    glDrawBuffers(m_draw_buffers.size(), m_draw_buffers.data());

    if (m_viewport[2] && m_viewport[3]) {
        glViewport(m_viewport[0], m_viewport[1], m_viewport[2], m_viewport[3]);
    }

    if (m_scissor) {
        glEnable(GL_SCISSOR_TEST);
        glScissor((*m_scissor)[0], (*m_scissor)[1], (*m_scissor)[2], (*m_scissor)[3]);
    } else {
        glDisable(GL_SCISSOR_TEST);
    }

    for (size_t i = 0; i < m_draw_buffers.size(); i++) {
        glColorMaski(i, m_color_mask[i * 4 + 0], m_color_mask[i * 4 + 1], m_color_mask[i * 4 + 2],
                     m_color_mask[i * 4 + 3]);
    }

    glDepthMask(has_depth_attachment());
}
