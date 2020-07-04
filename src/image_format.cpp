#include <glimpse/image_format.hpp>

#include <GL/gl.h>

const gl::ImageFormat gl::ImageFormat::f8{GL_UNSIGNED_BYTE,
                          1,
                          {
                              std::make_pair(GL_RED, GL_R8),
                              std::make_pair(GL_RG, GL_RG8),
                              std::make_pair(GL_RGB, GL_RGB8),
                              std::make_pair(GL_RGBA, GL_RGBA8),
                          }};

const gl::ImageFormat gl::ImageFormat::f16{GL_HALF_FLOAT,
                           2,
                           {
                               std::make_pair(GL_RED, GL_R16F),
                               std::make_pair(GL_RG, GL_RG16F),
                               std::make_pair(GL_RGB, GL_RGB16F),
                               std::make_pair(GL_RGBA, GL_RGBA16F),
                           }};

const gl::ImageFormat gl::ImageFormat::f32{GL_FLOAT,
                           4,
                           {
                               std::make_pair(GL_RED, GL_R32F),
                               std::make_pair(GL_RG, GL_RG32F),
                               std::make_pair(GL_RGB, GL_RGB32F),
                               std::make_pair(GL_RGBA, GL_RGBA32F),
                           }};

const gl::ImageFormat gl::ImageFormat::u8{GL_UNSIGNED_BYTE,
                          1,
                          {
                              std::make_pair(GL_RED_INTEGER, GL_R8UI),
                              std::make_pair(GL_RG_INTEGER, GL_RG8UI),
                              std::make_pair(GL_RGB_INTEGER, GL_RGB8UI),
                              std::make_pair(GL_RGBA_INTEGER, GL_RGBA8UI),
                          }};

const gl::ImageFormat gl::ImageFormat::u16{GL_UNSIGNED_SHORT,
                           2,
                           {
                               std::make_pair(GL_RED_INTEGER, GL_R16UI),
                               std::make_pair(GL_RG_INTEGER, GL_RG16UI),
                               std::make_pair(GL_RGB_INTEGER, GL_RGB16UI),
                               std::make_pair(GL_RGBA_INTEGER, GL_RGBA16UI),
                           }};

const gl::ImageFormat gl::ImageFormat::u32{GL_UNSIGNED_INT,
                           4,
                           {
                               std::make_pair(GL_RED_INTEGER, GL_R32UI),
                               std::make_pair(GL_RG_INTEGER, GL_RG32UI),
                               std::make_pair(GL_RGB_INTEGER, GL_RGB32UI),
                               std::make_pair(GL_RGBA_INTEGER, GL_RGBA32UI),
                           }};

const gl::ImageFormat gl::ImageFormat::i8{GL_BYTE,
                          1,
                          {
                              std::make_pair(GL_RED_INTEGER, GL_R8I),
                              std::make_pair(GL_RG_INTEGER, GL_RG8I),
                              std::make_pair(GL_RGB_INTEGER, GL_RGB8I),
                              std::make_pair(GL_RGBA_INTEGER, GL_RGBA8I),
                          }};

const gl::ImageFormat gl::ImageFormat::i16{GL_SHORT,
                           2,
                           {
                               std::make_pair(GL_RED_INTEGER, GL_R16I),
                               std::make_pair(GL_RG_INTEGER, GL_RG16I),
                               std::make_pair(GL_RGB_INTEGER, GL_RGB16I),
                               std::make_pair(GL_RGBA_INTEGER, GL_RGBA16I),
                           }};

const gl::ImageFormat gl::ImageFormat::i32{GL_INT,
                           4,
                           {
                               std::make_pair(GL_RED_INTEGER, GL_R32I),
                               std::make_pair(GL_RG_INTEGER, GL_RG32I),
                               std::make_pair(GL_RGB_INTEGER, GL_RGB32I),
                               std::make_pair(GL_RGBA_INTEGER, GL_RGBA32I),
                           }};
