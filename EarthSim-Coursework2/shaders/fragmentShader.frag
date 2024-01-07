#version 460
//Colour value to send to next stage
out vec4 FragColor;

//Texture coordinates from last stage
in vec2 textureFrag;

uniform sampler2D texture_diffuse;

void main()
{
   FragColor = texture(texture_diffuse, textureFrag);

}