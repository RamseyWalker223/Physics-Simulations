#shader vertex
#version 410 core

//Location is the spot in our vertex buffer we want to access, first we pushed coordinates layout then we pushed texture position.
layout(location = 0) in vec4 position;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 texture_position;

//Making an output of that texture position for the fragment shader.
out vec2 v_texture_position;
out vec3 aPos;
out vec3 normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main(){
   //Multiply our screen position by matrix that normalizes it to the screen
   gl_Position = projection * view * model * position;
   v_texture_position = texture_position;
   aPos = vec3(model * position);
   normal = aNormal;
}

#shader fragment
#version 410 core

layout(location = 0) out vec4 color;

in vec2 v_texture_position;
in vec3 aPos;
in vec3 normal;

uniform vec4 u_Color;
uniform sampler2D u_Texture;
uniform bool u_istext;
uniform vec4 u_LightColor;
uniform vec3 lightPos;
uniform vec3 camPos;
uniform float shinyness;

void main(){
   //Ambient lighting term
   float ambientStrength = 0.1;

   //Diffuse lighting term
   vec3 lightRay = normalize(lightPos - aPos);
   float light_n = max(dot(normal, lightRay), 0.0);
   vec4 diffuse = light_n * u_LightColor;

   //Specular lighting term
   vec3 cam_ray = normalize(camPos - aPos);
   vec3 reflection_n = (light_n * normal);
   vec3 reflection = lightRay - reflection_n;
   reflection = reflection_n - reflection;
   vec4 specular = pow(max(dot(cam_ray, reflection), 0.0), shinyness) * u_LightColor;
   
   vec4 light = ambientStrength + diffuse + specular;
   if(u_istext){
      color = texture(u_Texture, v_texture_position) * u_Color * light;
   } else {
      color = u_Color * light;
   }
}