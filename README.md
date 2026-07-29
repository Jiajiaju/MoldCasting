# MoldCasting

基于 Unreal Engine 5.8 的 C++ 项目。

## 项目核心目标

项目以动作手感、严格的一镜到底、动作表演精细度和完整环境交互为核心目标。详细说明见 [`Docs/PROJECT_SUMMARY.md`](Docs/PROJECT_SUMMARY.md)。

## 当前状态

项目目前处于基础工程与技术验证阶段：

- 主运行模块：`MoldCasting`
- 目标平台：Windows、DX12、Shader Model 6
- 渲染特性：Lumen、硬件光线追踪、Substrate
- 现有内容主要来自角色、动画、移动与 MetaHuman 示例资源
- C++ 层目前只有基础模块，尚未实现铸造业务系统

## 引擎要求

项目使用 Unreal Engine 5.8 的自定义/源码构建：

- `MoldCasting.uproject` 当前未设置 `EngineAssociation`
- Build Target 使用 `BuildSettingsVersion.V7`
- Include Order 使用 `EngineIncludeOrderVersion.Unreal5_8`

打开项目前，需要在本机准备兼容的 Unreal Engine 5.8 构建，并将 `.uproject`
关联到该引擎。

## 打开项目

1. 获取兼容的 Unreal Engine 5.8。
2. 右键 `MoldCasting.uproject`，生成项目文件。
3. 编译 `MoldCastingEditor` 的 Win64 Development 配置。
4. 使用 Unreal Editor 打开 `MoldCasting.uproject`。

也可以通过对应引擎目录下的 Unreal Editor 启动：

```powershell
<UE_ROOT>\Engine\Binaries\Win64\UnrealEditor.exe .\MoldCasting.uproject
```

## 仓库内容

会提交到 Git 的核心内容包括：

- `Source/`
- `Config/`
- `MoldCasting.uproject`
- 项目文档

以下目录属于生成文件或本机缓存，不应提交：

- `Binaries/`
- `DerivedDataCache/`
- `Intermediate/`
- `Saved/`

`Content/` 当前也被 `.gitignore` 排除，因此 Git 仓库本身不包含关卡、蓝图、
角色、动画等 Unreal 二进制资源。其他开发者仅克隆仓库时无法完整还原当前项目。
后续需要明确采用 Git LFS、Perforce，或单独的资源分发方案。

## 已知问题

- 默认启动地图仍指向引擎自带的 Open World 模板。
- `ABP_GenericRetarget` 在 PIE 中可能持续产生 IK Retarget Controller 空引用错误。
- 项目启用了较多动画和角色相关插件，启动及着色器编译成本较高。

## Agent 协作

项目级开发约束见 [`AGENTS.md`](AGENTS.md)。
