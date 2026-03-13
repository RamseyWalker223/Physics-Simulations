#shader vertex
#version 410 core

//Location is the spot in our vertex buffer we want to access, first we pushed coordinates layout then we pushed texture position.
layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texture_position;

//Making an output of that texture position for the fragment shader.
out vec2 v_texture_position;
out vec3 v_local_position;
out vec3 v_world_position;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main(){
   //Multiply our screen position by matrix that normalizes it to the screen
   gl_Position = projection * view * model * position;
   v_texture_position = texture_position;
   //Local position is for calculating normal vectors, and world position is multiplied by the matrix to get the fragment position
   v_local_position = position.xyz;
   v_world_position = vec3(position * model);
}

#shader geometry
#version 410 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in vec2 v_texture_position[];
in vec3 v_local_position[];
in vec3 v_world_position[];

out vec2 texture_position;
out vec3 normal;
out vec3 frag_position;

void main(){
   vec3 vectorA = v_local_position[1] - v_local_position[0];
   vec3 vectorB = v_local_position[2] - v_local_position[0];
   normal = normalize(cross(vectorA, vectorB));

   texture_position = v_texture_position[0];
   frag_position = v_world_position[0];
   gl_Position = gl_in[0].gl_Position;
   EmitVertex();

   texture_position = v_texture_position[1];
   frag_position = v_world_position[1];
   gl_Position = gl_in[1].gl_Position;
   EmitVertex();

   texture_position = v_texture_position[2];
   frag_position = v_world_position[2];
   gl_Position = gl_in[2].gl_Position;
   EmitVertex();

   EndPrimitive();
}

#shader fragment
#version 410 core

layout(location = 0) out vec4 color;

in vec2 texture_position;
in vec3 frag_position;
in vec3 normal;

uniform vec4 u_Color;
uniform sampler2D u_Texture;
uniform bool u_istext;
uniform vec4 u_LightColor;
uniform vec3 lightPos;

void main(){
   vec3 lightRay = normalize(lightPos - frag_position);
   vec4 diffuse = max(dot(normal, lightRay), 0.0) * u_LightColor;

   float ambientStrength = 0.1;
   vec4 light = ambientStrength + diffuse;
   if(u_istext){
      color = texture(u_Texture, texture_position) * u_Color * light;
   } else {
      color = u_Color * light;
   }
}