# MiniMathLib

一个为计算机图形学而生的轻量级 C++ 数学库，提供 `Vector3f` 与 `Matrix4f` 的完整实现。无第三方依赖，适合作为软渲染器、光线追踪及 GPU 项目的数学基础。

## 项目简介

在计算机图形学中，所有几何变换（平移、旋转、缩放、投影）都依赖于向量与矩阵运算。C++ 标准库并未提供矩阵类，因此本项目从零实现了一套轻量、高效、易扩展的数学库。

本项目诞生于作者大一上学期学习图形学的过程中，目标是：
- 为 GAMES101 软渲染器提供底层数学支持
- 理解图形学变换背后的线性代数本质
- 为后续路径追踪、CUDA 图形学项目打下坚实基础

## 设计目标

- **无依赖**：仅使用 C++ 标准库，不引入任何第三方库
- **高性能**：所有运算均在栈上完成，避免动态内存分配
- **易用性**：接口命名符合图形学惯例，支持自然数学表达式
- **可扩展**：清晰的类结构，便于后续添加矩阵求逆、投影矩阵等功能

## 功能特性

### Vector3f（三维向量）

- 默认构造：`Vector3f()`，初始化为 (0, 0, 0)
- 带参构造：`Vector3f(float x, float y, float z)`
- 加法：`operator+` / `operator+=`
- 减法：`operator-` / `operator-=`
- 标量乘法：`operator*(float)`
- 点乘：`dot(const Vector3f&)`，返回 `float`
- 叉乘：`cross(const Vector3f&)`，返回 `Vector3f`
- 长度：`length()`
- 归一化：`normalized()`，安全处理零向量
- 索引访问：`operator()(int)`，支持 `v(0)`、`v(1)`、`v(2)`
- 调试输出：`show()`

### Matrix4f（4x4 矩阵）

- 默认构造：`Matrix4f()`，初始化为单位矩阵
- 数组构造：`Matrix4f(const float (&)[4][4])`
- 加法：`operator+` / `operator+=`
- 减法：`operator-` / `operator-=`
- 矩阵乘法：`operator*(const Matrix4f&)`
- 矩阵乘向量：`operator*(const Vector3f&)`，含齐次坐标与透视除法
- 索引访问：`operator()(int i, int j)`
- 调试输出：`show()`
- 平移矩阵：`static translation(tx, ty, tz)`
- 缩放矩阵：`static scale(sx, sy, sz)`
- 旋转矩阵：`static rotationX/Y/Z(angleRad)`，参数为弧度

## 项目结构

```
MiniMathLib/
├── include/
│   └── matrix_and_vector.h    # 数学库唯一头文件
├── src/
│   └── main.cpp               # 使用示例与测试入口（可选）
├── README.md
└── LICENSE
```

所有实现均为 `inline`，只需包含 `matrix_and_vector.h` 即可使用。

## 快速开始

### 环境要求

- C++11 或更高版本
- 支持 C++ 的任意编译器（g++、clang++、MSVC）
- 无需安装任何第三方库

### 编译与运行

```bash
g++ -std=c++11 -O2 -Iinclude src/main.cpp -o demo
./demo
```

若使用 CMake，可自行添加 `CMakeLists.txt`。

## 使用示例

### 向量基本运算

```cpp
#include "matrix_and_vector.h"

int main() {
    Vector3f a(1, 0, 0);
    Vector3f b(0, 1, 0);

    float d = a.dot(b);           // 0.0f
    Vector3f n = a.cross(b);      // (0, 0, 1)

    Vector3f dir(3, 0, 4);
    Vector3f unit = dir.normalized();  // (0.6, 0, 0.8)
    float len = dir.length();          // 5.0f

    return 0;
}
```

### 矩阵变换组合

```cpp
#include "matrix_and_vector.h"

int main() {
    Matrix4f S = Matrix4f::scale(2, 2, 2);
    Matrix4f R = Matrix4f::rotationZ(3.14159f / 2.0f); // 旋转 90°
    Matrix4f T = Matrix4f::translation(1, 2, 3);

    Matrix4f M = T * R * S;   // 注意乘法顺序

    Vector3f p(1, 0, 0);
    Vector3f p_transformed = M * p;
    p_transformed.show();

    return 0;
}
```

## 注意事项

1. 旋转角度使用弧度：所有旋转工厂函数参数均为弧度制。
2. 矩阵乘法顺序：`A * B` 表示先应用 B，再应用 A。
3. 浮点数比较：透视除法中使用了 `w != 0` 判断，实际工程中建议改为 `std::abs(w) > 1e-6f`。
4. 矩阵乘向量的语义：当前 `operator*(const Vector3f&)` 将向量视为点（w = 1），适用于顶点变换。如需变换法线（w = 0），需扩展新接口。
5. 索引越界：`Vector3f::operator()` 对越界索引会输出错误信息并返回第一个分量，建议仅在调试阶段使用。

## 设计细节

- 内存布局：`Matrix4f` 使用一维数组 `float m[16]` 按行优先存储，与 OpenGL/Vulkan 等图形 API 的列优先习惯不同，但在 CPU 端计算时更为直观。
- 运算符重载惯用法：`operator+` 采用“拷贝左操作数 + `operator+=`”的模式，代码简洁且异常安全。
- 单位矩阵初始化：利用 `i % 5 == 0` 巧妙设置对角线为 1，避免双重循环。
- 内联实现：所有函数均声明为 `inline`，避免多重定义，同时方便头文件直接使用。

## 性能考量

- 所有运算均在栈上完成，无堆分配。
- 矩阵乘法为 O(4^3) 固定循环，编译器可自动展开。
- 传参统一使用 `const` 引用，避免拷贝。
- 对于 4x4 矩阵，当前性能已足够软渲染器使用；若需极致优化，可后续引入 SIMD 指令或循环展开。

## 路线图

- [x] Vector3f 基础运算（加减、点乘、叉乘、归一化）
- [x] Matrix4f 基础运算（构造、加法、乘法）
- [x] 静态工厂函数（平移、缩放、旋转）
- [ ] 转置（transpose）
- [ ] 求逆（inverse）
- [ ] 行列式（determinant）
- [ ] 透视投影矩阵、视图矩阵工厂函数
- [ ] 法线变换接口（w = 0）
- [ ] 泛型模板版本（Matrix3f、Matrix2f）

## 参考资料

- 《C++ Primer（第5版）》—— Stanley B. Lippman 等

## License

本项目基于 MIT License 开源，可自由使用、修改、分发。

## 作者
Siina

NJU计算机大一学生

- 方向：AI + 图形学 / GPU 开发

---

本项目为个人学习实践产物，欢迎交流指正。
