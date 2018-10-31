#version 330 core
out vec4 FragColor;

uniform vec3 lightPos;
uniform vec3 viewPos;

in vec3 Normal;
in vec3 FragPos;

struct Material {
    //环境光颜色几乎在所有情况下都等于漫反射颜色，所以不需要环境光，只用漫反射即可
//    vec3 ambient;//环境光，一般是物体颜色
//    vec3 diffuse;//漫反射光照下物体的颜色
    sampler2D diffuse;
//    vec3 specular;//镜面光照对物体的颜色影响
    sampler2D specular;
    sampler2D emission;//放射光的贴图
    float shininess;//反光度
};

//一个光源对它的ambient、diffuse和specular光照有着不同的强度
struct Light {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

//之前都是用灯光来设定环境光，漫反射光和镜面反射光，现在我们要分开设置这些材质
uniform Material material;

uniform Light light;

//上面用两个纹理，下面用一个接收？并不是，这个是纹理的坐标
//而纹理的对象，是设置给了Light结构体里的diffuse和specular
in vec2 TexCoords;

void main() {
       //环境光,现在设置为与漫反射同样的值
       vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));

       //漫反射,先对向量进行标准化
       vec3 norm = normalize(Normal);
       vec3 lightDir = normalize(lightPos - FragPos);//光的方向向量是光源位置与片段位置之间的向量差
       //依据法向量计算光源对当前片段的漫反射影响
       float diff = max(dot(norm, lightDir),0.0);//点积，算出来的是投影长度,夹角越小，越接近1，越亮
       vec3 diffuse = light.diffuse * (diff * vec3(texture(material.diffuse, TexCoords)));//混合纹理

       //镜面反射
       vec3 viewDir = normalize(viewPos - FragPos);//片段位置到摄像机位置的向量,也就是视线方向
       //反射向量，reflect函数要求第一个向量是从光源指向片段位置的向量
       vec3 reflectDir = reflect(-lightDir,norm);
       //点乘，避免负值，取32次幂，32是高光的反光度，这个点乘肯定小于1，32次方，那就更小了
       //一个物体的反光度越高，反射光的能力越强，散射得越少，高光点就会越小,可以理解
       //反光度material.shininess，必须是float类型，否则报错
       //镜面光贴图我们可以可以对物体设置大量的细节，比如物体的哪些部分需要有闪闪发光的属性，我们甚至可以设置它们对应的强度。
       //镜面光贴图能够在漫反射贴图之上给予我们更高一层的控制
       float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
       vec3 specular = light.specular * (spec * vec3(texture(material.specular,TexCoords)));

       //放射光贴图,这个.rgb跟前面的vec3()转是一个效果，都是把vec4转成vec3
       vec3 emission = texture(material.emission, TexCoords).rgb;

       //将环境光，漫反射光，镜面反射光作用于物体
       vec3 result = ambient + diffuse + specular + emission;
       //混合两个光照颜色
       FragColor = vec4(result, 1.0);

}
