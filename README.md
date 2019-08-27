
# Opengl Practice

## Opengl架构搭建

### 模型绘制

    - 坐标系
        - 模型坐标系
        - 投影坐标系
        - 视窗坐标系
    - 模型加载
        - 三角面绘制
        - 着色器
            - 基础着色器加载
            - 基于运行时可修改的着色器
            - 着色器解析器
        - 多种模型格式加载
            - *.obj
            - *.fbx
            - *.*
        - 纹理
            - 多种格式纹理的加载
            - 支持部分格式纹理的导出
            - 立方体纹理

### 场景元素

    - 摄像机
        - 透视相机
        - 正交相机

    - 光照
        - 基础光照
            - 灯光颜色
            - 灯光强度
            - 阴影属性
        - 光照类型
            - 平行光源
            - 点光源
            - 聚光灯
            - 区域光
    
    - 帧缓冲
        - 后期特效

    - 着色器系统
        - 透明度测试
            - 混合
        - 深度测试
        - 模板测试

    - 粒子系统

    - 雾系统

    - 阴影系统

### 界面系统

    - 文字
    - 精灵

### 交互系统

    - 鼠标输入
    - 键盘输入

## 实例编写

### 材质球编辑工具

### 游戏开发

# Fixed Pipelines

    - 未更新

# Programmable Pipelines

## Finished Feature

    - VBO
    - VAO
    - FBO
    - vertex shader
    - fragment shader
    - Instancing
    - Subroutine
    - ModelLoader(.obj/.fbx)
    - TextureLoader(.bmp/all farmat depandency third library)
    - RTT/MRT

## Next To Learn

    - SSBO
    - Particle System
    - Geometry Shader
    - Tessellation Shader

## Issues

    - MSAA(multi-sample Anti-aliasing):Unkown reason , it does not work on my win7 pc.
    - Compute Shader : calculate inverse color for texture2D has bug, all pixel are in black.

# Shading Lanuage 

    - 待完成
      - 变暗、变亮
      - 正片叠底、逆正片叠底
      - 颜色加深、颜色减淡
      - 柔光、强光
      - 相加、相减
      - 叠加、排除
      - 差值、反差值
      - 平滑、锐化、边缘监测

## Finished Features

    - attribute : vertex shader exclusively.(number limit OpenGL2.0 **8**)
    - uniform : use in all type shader.(number limit OpenGL2.0 **128**)
    - varying : property delivery (number limit OpenGL2.0 **8**)
    - fbo : frame buffer objcet.