#shader vertex
#version 330 core


//Location is the spot in our vertex buffer we want to access, first we pushed coordinates layout then we pushed texture position.
layout(location = 0) in vec4 position;

//uniform mat4 u_MVP;

void main(){
   //Multiply our screen position by matrix that kinda normalizes it to the screen
   //gl_Position = u_MVP * position;
   gl_Position = position;
   //v_texture_position = texture_position;
}

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

uniform vec4 u_Color;

void main(){
   color = u_Color;
}