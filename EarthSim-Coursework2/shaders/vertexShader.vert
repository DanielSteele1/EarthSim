#version 460

//object position with values retrieved from main.cpp
layout (location = 0) in vec3 position;

//Texture coordinates from last stage

layout (location = 2) in vec2 textureVertex;

//Model-View-Projection Matrix
uniform mat4 mvpIn;

//Texture to send
out vec2 textureFrag;

void main()
{
    //Transformation applied to vertices
    gl_Position = mvpIn * vec4(position.x, position.y, position.z, 1.0);
    //Sending texture coordinates to next stage
    textureFrag = textureVertex;
}