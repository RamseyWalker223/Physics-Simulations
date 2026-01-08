#shader vertex
#version 330 core

//Location is the spot in our vertex buffer we want to access, first we pushed coordinates layout then we pushed texture position.
layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texture_position;

//Making an output of that texture position for the fragment shader.
out vec2 v_texture_position;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main(){
   //Multiply our screen position by matrix that kinda normalizes it to the screen
   gl_Position = projection * view * model * position;
   v_texture_position = texture_position;
}

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec2 v_texture_position;

uniform vec4 u_Color;
uniform sampler2D u_Texture;

void main(){
   color = texture(u_Texture, v_texture_position);
}