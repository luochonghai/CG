#version 330 core

layout (location = 0) in vec3 aPos;//位置变量的属性位置值为0
layout (location = 1) in vec3 aNormal;//法向量
layout (location = 2) in vec2 aTexCoords;//纹理

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;



out vec3 Normal;
out vec3 FragPos;
out vec2 TexCoords;

void main() {
    gl_Position = projection*view*model*vec4(aPos,1.0);
    FragPos = vec3(model * vec4(aPos,1.0));
    //把法向量也转换为世界空间坐标,所以需要一个法线矩阵
    //由于我们只在世界空间中进行操作（不是在观察空间），我们只使用模型矩阵
    //使用inverse和transpose函数修改model生成这个法线矩阵,必须是3x3的
    mat3 normalMatrix =  mat3(transpose(inverse(model)));
    Normal = normalMatrix * aNormal;
    TexCoords = aTexCoords;
}
