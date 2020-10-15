#version 450 core

layout (location = 0) in vec3 aPos;

uniform mat4 view;
uniform mat4 model;
uniform mat4 projection;

uniform int localMode;

out vec3 Color;

void main()
{
//    if(localMode){
        gl_Position = projection * view  * model * vec4(aPos, 1.0);
//    }else{
//        gl_Position = projection * view  * vec4(aPos, 1.0);
//    }
}