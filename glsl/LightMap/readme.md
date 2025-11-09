## 光照贴图：

    光照贴图是一种特殊的纹理贴图，用于存储场景中各个点的光照信息。通常，光照贴图包含了环境光、漫反射光和镜面反射光等信息。在渲染场景时，通过采样光照贴图，可以动态地计算出物体表面的光照效果，从而实现更加真实的光照模拟。


## 理解
    之前的光照模型 通过材质中的环境光，漫反射，镜面反射，以及反光指数组成， 现在用贴图的形式呈现，那就也会存在对应的漫反射贴图，镜面贴图，以及反光指数（因为使用贴图所以环境光颜色在几乎所有情况下都等于漫反射颜色，我们不需要将它们分开储存）

    在渲染场景时，通过采样光照贴图，可以动态地计算出物体表面的光照效果 获取到对应点的光照信息，从而计算出最终的颜色值 实现更加真实的光照模拟。

## 漫反射贴图

    在光照场景中，它通常叫做一个漫反射贴图(Diffuse Map)（3D艺术家通常都这么叫它），它是一个表现了物体所有的漫反射颜色的纹理图像。 和之前的纹理操作流程基本一致。
```cpp
    struct Material {
    sampler2D diffuse;
    vec3      specular;
    float     shininess;
}; 

in vec2 TexCoords;

注意我们将在片段着色器中再次需要纹理坐标，所以我们声明一个额外的输入变量。接下来我们只需要从纹理中采样片段的漫反射颜色值即可：

vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
不要忘记将环境光的材质颜色设置为漫反射材质颜色同样的值。

vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));

// 第一张 漫反射贴图
lightingShader.setInt("material.diffuse", 0);
...
glActiveTexture(GL_TEXTURE0);
glBindTexture(GL_TEXTURE_2D, diffuseMap);

```

## 镜面光贴图

    镜面光贴图和其它的纹理非常类似，所以代码也和漫反射贴图的代码很类似。记得要保证正确地加载图像并生成一个纹理对象。由于我们正在同一个片段着色器中使用另一个纹理采样器，我们必须要对镜面光贴图使用一个不同的纹理单元（见纹理），所以我们在渲染之前先把它绑定到合适的纹理单元上：
```cpp

    // 第二章 镜面反射贴图
    lightingShader.setInt("material.specular", 1);
    ...
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, specularMap);
    接下来更新片段着色器的材质属性，让其接受一个sampler2D而不是vec3作为镜面光分量：

    struct Material {
        sampler2D diffuse;
        sampler2D specular;
        float     shininess;
    };
    最后我们希望采样镜面光贴图，来获取片段所对应的镜面光强度：

    vec3 ambient  = light.ambient  * vec3(texture(material.diffuse, TexCoords));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.diffuse, TexCoords));  
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
    FragColor = vec4(ambient + diffuse + specular, 1.0);

```
