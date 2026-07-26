# MoldCasting 当前框架

## 1. 技术栈与运行环境

| 类别 | 当前配置 |
| --- | --- |
| 游戏引擎 | Unreal Engine 5.8 自定义/源码构建 |
| 原生代码 | C++ |
| 脚本框架 | [Unreal Engine AngelScript](https://angelscript.hazelight.se/) |
| 操作系统与目标平台 | Windows、Win64 |
| 默认开发配置 | Development Editor |
| 编译工具链 | Visual Studio 2022、MSVC 14.44、Windows SDK 10.0.22621 |
| 图形接口 | DirectX 12、Shader Model 6 |
| 全局光照与反射 | Lumen |
| 光线追踪 | 已启用 |
| 材质框架 | Substrate |
| 输入系统 | Enhanced Input |

当前源码引擎根目录：

```text
D:\JiaXYTrunk
```

`MoldCasting.uproject` 当前未设置 `EngineAssociation`，项目通过本机源码引擎进行构建和运行。

## 2. 仓库目录结构

```text
MoldCasting/
├─ Config/                       Unreal 项目配置
├─ Content/                      关卡、蓝图、角色、动画、音频和界面资源
├─ Docs/                         项目文档
├─ Plugins/
│  └─ Developer/RiderLink/       Rider 开发辅助插件
├─ Script/                       AngelScript 脚本目录
├─ Source/
│  ├─ MoldCasting/               主 Runtime 模块
│  ├─ MoldCastingBasic/          基础 Runtime 模块
│  ├─ MoldCastingEditor/         Editor 模块
│  ├─ MoldCasting.Target.cs      Game Target
│  └─ MoldCastingEditor.Target.cs
├─ AGENTS.md                     项目开发与 Agent 协作规则
├─ MoldCasting.uproject          项目、模块和插件声明
└─ README.md                     仓库入口说明
```

生成目录包括：

```text
Binaries/
DerivedDataCache/
Intermediate/
Saved/
```

## 3. 三个 C++ 模块的当前职责

### MoldCasting

类型：Runtime 主模块。

当前职责：

- 注册项目 Primary Game Module。
- 提供项目级 `UMCGameInstance`。
- 提供基于 `UGameInstanceSubsystem` 的 `UMCBaseService`。
- 引入 Core、CoreUObject、Engine、InputCore 和 EnhancedInput。

### MoldCastingBasic

类型：Runtime 基础模块。

当前职责：

- 提供基础 Pawn 类型 `AMCBaseCharacter`。
- 提供基础 GameMode 类型 `AMCBaseGameMode`。
- 提供独立的模块启动与关闭入口。
- 引入 Core、CoreUObject、Engine、Slate 和 SlateCore。

### MoldCastingEditor

类型：Editor 模块。

当前职责：

- 提供独立的编辑器模块启动与关闭入口。
- 为后续项目专用编辑器扩展提供模块位置。
- 引入 Core、CoreUObject、Engine、Slate 和 SlateCore。

## 4. Game Target 与 Editor Target 的当前组成

| Target | Target 类型 | 当前注册模块 |
| --- | --- | --- |
| `MoldCastingTarget` | `TargetType.Game` | MoldCasting、MoldCastingBasic、MoldCastingEditor |
| `MoldCastingEditorTarget` | `TargetType.Editor` | MoldCasting、MoldCastingBasic、MoldCastingEditor |

两个 Target 当前均使用：

```text
BuildSettingsVersion.V7
EngineIncludeOrderVersion.Unreal5_8
```

三个模块同时在 `MoldCasting.uproject` 中声明：

| 模块 | UProject 类型 | Loading Phase |
| --- | --- | --- |
| MoldCasting | Runtime | Default |
| MoldCastingBasic | Runtime | Default |
| MoldCastingEditor | Editor | Default |

## 5. 当前核心类型

### UMCGameInstance

```text
UGameInstance
└─ UMCGameInstance
```

- 所属模块：MoldCasting。
- 项目级 GameInstance 类型。
- 已通过 `DefaultEngine.ini` 配置为当前 `GameInstanceClass`。
- 当前没有额外成员或生命周期实现。

### UMCBaseService

```text
UGameInstanceSubsystem
└─ UMCBaseService
```

- 所属模块：MoldCasting。
- 声明为抽象 `UCLASS`。
- 提供 `Init()` 虚函数。
- 提供 `OnStart()` 虚函数。
- 两个函数当前为空实现。

### AMCBaseCharacter

```text
APawn
└─ AMCBaseCharacter
```

- 所属模块：MoldCastingBasic。
- 构造时启用 Actor Tick。
- 重写 `BeginPlay()`。
- 重写 `Tick(float DeltaTime)`。
- 重写 `SetupPlayerInputComponent(UInputComponent*)`。
- 当前三个重写函数只调用父类实现。

### AMCBaseGameMode

```text
AGameMode
└─ AMCBaseGameMode
```

- 所属模块：MoldCastingBasic。
- 作为项目基础 GameMode 类型。
- 当前没有额外成员或生命周期实现。

## 6. 当前配置入口

### MoldCasting.uproject

负责声明：

- MoldCasting Runtime 模块。
- MoldCastingBasic Runtime 模块。
- MoldCastingEditor Editor 模块。
- Modeling Tools、动画、LiveLink、Mover、SmartObjects 等项目插件。

### Config/DefaultEngine.ini

当前关键配置：

```ini
[/Script/EngineSettings.GameMapsSettings]
GameDefaultMap=/Engine/Maps/Templates/OpenWorld
GameInstanceClass=/Script/MoldCasting.MCGameInstance
```

同时包含：

- DX12 与 Shader Model 6。
- Lumen 动态全局光照与反射。
- 硬件光线追踪。
- Substrate。
- Virtual Shadow Maps。
- Windows、Linux、Mac 目标平台配置。

### Config/DefaultInput.ini

当前输入类：

```ini
DefaultPlayerInputClass=/Script/EnhancedInput.EnhancedPlayerInput
DefaultInputComponentClass=/Script/EnhancedInput.EnhancedInputComponent
```

### Config/DefaultGame.ini

当前包含：

- CommonUI 基础设置。
- 项目 `ProjectID`。

### Config/DefaultEditor.ini

保存 Unreal Editor 相关的项目级编辑器配置。
