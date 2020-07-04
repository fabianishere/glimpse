# Glimpse
Small OpenGL wrapper for modern C++.

```cpp
#include <glimpse/gl.hpp>
#include <glm/glm.hpp>

gl::Program program;
gl::Data<glm::vec3> vertices{
    {-0.5, -0.5, 0.0},
    { 0.5, -0.5, 0.0},
    { 0.0,  0.5, 0.0}
};

gl::VertexArray vao(program, {{"position", vertices}});
vao.render(gl::RenderMode::TRIANGLES, 3);
```

## License
Glimpse is available under the [MIT license](LICENSE.txt).
