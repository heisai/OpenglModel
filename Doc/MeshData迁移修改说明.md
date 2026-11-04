# MeshData 迁移修改说明

## 1. 迁移目标

本次迁移的目标是让 `MeshData` 成为唯一 CPU 网格数据来源，避免旧流程中的往返转换：

- 旧流程：`float 顶点/索引 -> MeshData -> 再打平回 float -> GPU`
- 新流程：`MeshData(Vertex/indices) -> 直接上传 GPU`

这样后续无论接入开源 primitive 生成库，还是保留当前代码生成，都可以直接对齐到同一套数据接口。

---

## 2. 最终结构（当前实现）

### 2.1 数据层

文件：`src/ModelDatas/ModelDataInterFace.h`

- `ModelDataInfo` 已统一为 `MeshData`
- `MeshData`：
  - `PrimitiveTopology topology`
  - `std::vector<Vertex> vertices`
  - `std::vector<uint32_t> indices`
  - `std::string model_name_`
- `Vertex` 采用 AoS：
  - `position (vec3)`
  - `normal (vec3)`
  - `texCoord (vec2)`
  - `color (vec4)`

### 2.2 渲染层

文件：`src/Graphics/Graphics.h`

`Mesh` 结构已调整为：

```cpp
struct Mesh
{
	unsigned int vao_;
	unsigned int vbo_;
	unsigned int ebo_;
	MeshData data_;
};
```

说明：

- `vertices_datas / indices_datas` 已删除
- `model_type_` 从 `Mesh` 中移除，统一由 `GraphicsEngine::model_type_` 持有
- `model_name_` 放入 `MeshData::model_name_`

---

## 3. 代码改动清单

### 3.1 ModelDatas 相关

- `src/ModelDatas/ModelDataInterFace.h`
- `src/ModelDatas/CreatModelData.h`
- `src/ModelDatas/CreatModelData.cpp`
- `src/ModelDatas/CreatModelDataImpl.h`
- `src/ModelDatas/CreatModelDataImpl.cpp`
- `src/ModelDatas/CubeVerticesData.h`
- `src/ModelDatas/CubeVerticesData.cpp`
- `src/ModelDatas/CylinderVerticesData.h`
- `src/ModelDatas/CylinderVerticesData.cpp`
- `src/ModelDatas/ToursVerticesData.h`
- `src/ModelDatas/ToursVerticesData.cpp`
- `src/ModelDatas/GridVerticeData.h`
- `src/ModelDatas/GridVerticeData.cpp`
- `src/ModelDatas/AxisVerticeData.h`
- `src/ModelDatas/AxisVerticeData.cpp`

改动要点：

- 所有 `GetModelDatas()` 统一返回 `MeshData`
- 立方体、圆柱、圆环、网格和坐标轴生成器直接生成 `std::vector<Vertex>`，移除 `std::vector<float>` 顶点中间层
- 索引生成器直接返回 32 位索引并写入 `MeshData::indices`
- 立方体使用每面 0–1 UV；圆柱侧面使用环向/高度 UV、端盖使用平面圆盘 UV；圆环使用两个环向参数作为 UV；网格保留平铺 UV

### 3.2 Graphics/渲染上传相关

- `src/Graphics/Graphics.h`
- `src/Graphics/Graphics.cpp`
- `src/Graphics/ShaderModel/ShaderModel.cpp`
- `src/Graphics/GeneralModel/GeneralModel.cpp`
- `src/Graphics/GridModel/GridModel.cpp`
- `src/Graphics/GeneralModel/AxisModel.cpp`
- `src/Graphics/PlanteModel/PlanetModel.cpp`
- `src/Graphics/ManageEngine.cpp`
- `src/Graphics/GeneralModel/vertex_shader.vert`
- `src/Graphics/GeneralModel/geometry_shader.geom`
- `src/Graphics/GeneralModel/fragment_shader.frag`
- `src/Graphics/ShaderModel/vertex_shader.vert`
- `src/Graphics/ShaderModel/fragment_shader.frag`
- `src/Graphics/PlanteModel/vertex_shader.vert`
- `src/Graphics/PlanteModel/fragment_shader.frag`

改动要点：

- `GraphicsEngine::setModelInfo` 改为接收 `const MeshData&`
- 各模型 `InitBufferData()` 直接上传 `mesh_data_->data_.vertices / indices`
- `glVertexAttribPointer` 步长改为 `sizeof(Vertex)`，偏移改为 `offsetof(Vertex, ...)`
- `texCoord` 使用顶点属性 location 2，并从顶点着色器传递到后续着色阶段
- 行星实例矩阵改由 location 3–6 的四个属性列组成，避免与 UV 的 location 2 冲突
- `ManageEngine` 中旧字段访问改为 `mesh->data_.indices` 与 `getModeltype()`

---

## 4. 迁移收益（解决了什么）

- 去掉了 `MeshData -> float` 的重复转换
- 去掉了 primitive 生成器内部 `std::vector<float>` 顶点缓存及其转 `Vertex` 步骤
- `MeshData` 不再是“中转类型”，而是渲染前统一数据源
- 为后续接入开源库（只要能提供 position/normal/uv/indices）提供统一落地格式

---

## 5. 当前状态与已知说明

### 5.1 已验证

- 全量构建已通过：**生成成功**

### 5.2 现状说明

- 当前 primitive 生成器直接构造 `Vertex`；CPU 侧不再创建扁平 float 顶点数组
- `Vertex` 包含 `color`，但当前 shader 暂未使用该属性
- 采用 `Vertex` 后，VBO 步长统一为 `sizeof(Vertex)`（相较旧 6/8 float 布局会增大）
- 已生成并传递 UV 坐标，但本次没有为立方体/圆柱/圆环指定贴图图片或新增贴图采样/材质绑定接口；网格仍使用原有木纹贴图

### 5.3 同步处理

- 行星实例矩阵按完整 `mat4` 配置为 location 3–6，location 2 留给 UV
- `ScreenRenderModel` 的历史构造逻辑保持不变

---

## 6. 结论

本次迁移后，`MeshData` 已真正成为数据层与渲染层之间的统一网格契约，达到了“当前代码可用 + 后期可接开源库”的目标。
