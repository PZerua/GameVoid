#version 330 core
// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec2 vertex;
layout(location = 1) in vec2 uv;

out vec2 uvs;

void main()
{
    uvs = uv;

    // Output position of the vertex
    gl_Position = vec4(vertex, 0.0f, 1.0f);
}
