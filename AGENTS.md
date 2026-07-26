# AGENTS.md

## 项目概览

- 此仓库包含名为 `MoldCasting` 的 Unreal Engine 5.8 项目。
- 主要运行时模块是 `Source/MoldCasting` 下的 `MoldCasting`。
- 脚本框架是 [Unreal Engine AngelScript](https://angelscript.hazelight.se/)。
- 项目当前使用自定义/源码引擎构建。`MoldCasting.uproject` 未设置 `EngineAssociation`，因此不要假定存在全局安装的引擎路径。
- 仓库的主分支是 `main`。

## 仓库结构

- `Source/`：C++ 模块和 Unreal Build Tool Target 文件。
- `Config/`：可以编辑并提交的项目配置。
- `Content/`：Unreal 二进制资源。此目录当前被 Git 忽略。
- `MoldCasting.uproject`：项目模块和插件配置。
- `Binaries/`、`DerivedDataCache/`、`Intermediate/`、`Saved/`、`.idea/`：生成数据或本机数据。

## 工作规则

- 修改任何文件前，先说明打算如何修改，并等待用户明确确认。
- 不得编辑或提交 `Binaries/`、`DerivedDataCache/`、`Intermediate/` 或 `Saved/` 中的生成文件。
- 保护用户创建的 Unreal 资源。除非用户明确要求，否则不得删除、重命名或覆盖 `.uasset` 或 `.umap` 文件。
- 不得把 `.uasset` 或 `.umap` 文件当作文本处理。资源级修改应使用 Unreal Editor 或适当的 Unreal 工具。
- 未经用户明确批准，不得修改 `Content/` 的忽略策略或引入 Git LFS。
- 修改范围必须限定在用户请求内，并保留工作区中不相关的改动。
- 默认使用 `main` 分支。除非用户明确要求，否则不得强制推送或改写共享历史。
- 用户要求提交修改时，提交成功后立即推送到当前跟踪的远程分支；仅当用户明确要求不推送时例外。

## C++ 规范

- 遵循 Unreal Engine 命名和反射规范，按适用情况使用 `A`、`U`、`F`、`E`、`I` 和 `T` 前缀。
- 需要 Unreal 反射、序列化、复制或蓝图公开时，使用 `UCLASS`、`USTRUCT`、`UENUM`、`UFUNCTION` 和 `UPROPERTY`。
- 尽量精简公共头文件，并优先使用前向声明。
- 仅在确有需要时向 `MoldCasting.Build.cs` 添加模块依赖。
- 遵循现有 Unreal 风格：使用 Tab 缩进、大括号独占一行、标识符使用 `PascalCase`。
- 用含义明确的具名常量代替 Magic Number。
- 有意识地使用空行表达代码结构。
- 空行必须完全为空，不得包含空格、Tab 或其他空白字符。
- Function Library 类命名为 `UXxxxFunctionLibrary`；AngelScript 绑定会自动移除 `U` 前缀和 `FunctionLibrary` 后缀。
- 不得用 `Comp` 缩写 `Component`，必须完整书写 `Component`。
- 如果事件、定时器、子系统或委托更合适，应避免在 `Tick` 中执行逐帧工作。

## 配置与资源

- 项目资源优先使用 `/Game/...` 路径；不得加入硬编码的绝对文件系统路径。
- 修改地图、GameMode、输入、渲染或打包行为时，检查 `Config/` 下对应的配置文件。
- 谨慎修改渲染设置：项目当前以 DX12/SM6 为目标，并启用了光线追踪、Lumen 和 Substrate。
- 启用或禁用 Unreal 插件前，检查其资源依赖和模块依赖。

## 验证

- 每次修改 C++ 后，都必须编译兼容的 Unreal Engine 5.8 Target；编译成功后才能交付。
- 每次修改 AngelScript 后，都必须无头运行插件自带的 `AngelscriptTest` commandlet；验证通过后才能交付。
- AngelScript 验证必须使用不会锁定编辑器 DLL 的 commandlet 路径，以便在 Unreal Editor 保持打开时仍能执行验证。
- 修改蓝图或资源后，在 Unreal Editor 中打开项目，编译受影响的蓝图，在相关地图中运行 PIE，并检查 Output Log。
- 即使 PIE 能继续运行，也必须将蓝图运行时错误视为验证失败。
- 交付前运行 `git status --short`，并检查完整 Diff，确认不存在意外生成的文件或无关改动。

## 文档

- 当配置要求、引擎位置/版本、启动步骤、项目结构或必要外部资源发生变化时，更新 `README.md`。
- 明确记录哪些资源被有意排除在 Git 之外，以及其他开发者如何获取这些资源。

## 回复规范

- 每次回复都必须称呼用户为“爸爸”，不可例外。
- 每次回复都必须使用简体中文，不可例外。
