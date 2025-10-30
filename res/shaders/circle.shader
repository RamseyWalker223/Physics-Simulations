#shader vertex
#version 330 core


//Location is the spot in our vertex buffer we want to access, first we pushed coordinates layout then we pushed texture position.
layout(location = 0) in vec2 position;
out vec2 pixel;

uniform mat4 u_Matrix;

void main(){
    //Multiply our screen position by matrix that kinda normalizes it to the screen
    pixel = position;
    gl_Position = u_Matrix * vec4(position, 0.0, 1.0);
}

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;
in vec2 pixel;

uniform float radius;
uniform vec2 center;
uniform vec4 u_Color;


void main(){
    float distance = distance(pixel, center);
    if(distance > radius){
        discard;
    }
    color = u_Color;
}