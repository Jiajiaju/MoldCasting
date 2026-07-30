# MoldCasting 游戏设计与实现

本文档统一记录 MoldCasting 的游戏设计目标、核心框架、技术方案、实施路线、系统边界和验收要求。

## 1. 总体路线

MoldCasting 的核心游戏框架按以下九个领域进行组织，其中 Character、Controller、Camera 和 Combat 构成基础 4C：

1. Character
2. Controller
3. Camera
4. Combat
5. World
6. Narrative
7. Item
8. Input
9. UI

### 系统与技术方案总览

| 系统 | 主要方案 | 相关模块与说明 |
| --- | --- | --- |
| Character | Mover、GASP、Motion Matching | 使用 `Mover`、`NetworkPrediction`、`PoseSearch`、`Chooser` 和 `MotionWarping`。GASP 是参考工程和资源方案，不是 C++ 模块；Motion Matching 的核心运行时模块是 `PoseSearch`。 |
| Controller | StateTree | 使用 StateTree、Gameplay StateTree、AI Module 和 Navigation System 负责 AI 决策、状态组织与控制。PlayerController 和 AIController 消费来自 Input 或 AI 决策的控制意图；StateTree 不替代 PlayerController 或 AIController。 |
| Camera | JiajiaCamera | 以 `APlayerCameraManager`、Camera Modifier 和 Spring Arm 为基础，作为全项目唯一的镜头控制框架。 |
| Combat | Gameplay Ability System（GAS） | 使用 `GameplayAbilities`、`GameplayTags` 和 `GameplayTasks`，并可配合 `MotionWarping` 完成战斗动作对位。 |
| World | World Partition | 使用 World Partition、Data Layer、流式加载和 HLOD；PCG 作为后续可选方案。 |
| Narrative | Sequencer / Level Sequence | 使用 `LevelSequence`、`MovieScene` 和 `MovieSceneTracks`，按需接入 Control Rig 与 Contextual Animation。 |
| Item | Gameplay Interactions、Smart Objects、Chaos | 使用 Gameplay Interactions 和 Smart Objects 组织交互行为；使用 Chaos、Geometry Collection 和 Physics Core 支持物理与破坏，并衔接 Niagara 和声音表现。 |
| Input | Enhanced Input | 使用 `EnhancedInput` 组织 Input Action、Input Mapping Context、输入优先级、设备输入转换和按键重映射，并向其他系统输出统一的玩法意图。 |
| UI | CommonUI | 使用 CommonUI 组织界面层级、激活栈、输入路由、焦点和导航；底层按需使用 Common Input、UMG、Slate 与 SlateCore。 |

项目按以下六个阶段逐步实现。

### 第一阶段：角色移动与基本输入

- 完成移动、转向、视角和基本输入。
- 暂不接入动画，优先验证输入响应、加速度、移动惯性、制动和基础碰撞手感。
- 建立连续相机的基础控制权和生命周期。

### 第二阶段：接入 GASP

- 接入 Game Animation Sample Project（GASP）的 Motion Matching、动画数据库和运动逻辑。
- 完成项目角色的动画重定向。
- 动画表现不得破坏第一阶段已经确定的移动手感。

### 第三阶段：动作战斗框架

- 建立攻击、连段、闪避、防御、受击、硬直、打断和命中反馈。
- 根据战斗系统需要接入 Gameplay Ability System（GAS）。
- GASP 负责动画运动表现，GAS 负责能力、状态和战斗规则，两者保持清晰的职责边界。
- 提前预留统一的命中与 Item 交互接口。

### 第四阶段：World 世界地图框架

- 建立 World Partition、Data Layer、流式加载、区域状态和存档结构。
- 地图加载、区域切换和状态恢复必须服从一镜到底约束，不得通过切镜掩盖加载过程。

### 第五阶段：Narrative 剧情表演框架

- 统一负责剧情状态、事件触发、对话和剧情流程。
- 建立游戏与演出之间的无缝控制权交接。
- 支持角色走位、交互动画、对话、表情和 Sequencer 协作。
- Narrative 可以驱动 Camera，但最终镜头控制权始终归 Camera。
- 表演开始、进行和结束必须延续同一台相机，不得产生剪辑意义上的新镜头或切镜。

### 第六阶段：Item 环境物品框架

- Item 是项目对环境物品领域的统一命名，并不只表示背包或可拾取物。
- 建立统一的环境物品与交互接口。
- 支持可交互物、物理物体、机关、可破坏物，以及相关的碰撞、碎片、声音、特效和持续状态。
- 将 Item 与 Combat、World 及 Narrative 打通。

### 贯穿约束

- **一镜到底**是从第一阶段开始执行的底层架构约束，全流程绝不切镜，场景遮挡也不得用于隐藏切镜。
- Item 框架虽然在第六阶段集中实现，但相关接口必须在 Combat 阶段提前预留，避免后期推翻战斗系统。
- 每个阶段必须先定义可独立验证的完成标准，再进入下一阶段。

## 2. Character

### 2.1 目标

待填写。

### 2.2 职责边界

待填写。

### 2.3 C++ 基础层

待填写。

### 2.4 AngelScript/Blueprint 扩展层

待填写。

### 2.5 数据与配置

待填写。

### 2.6 网络与生命周期

待填写。

### 2.7 测试与验收

待填写。

### 2.8 待确认事项

待填写。

## 3. Controller

### 3.1 目标

待填写。

### 3.2 职责边界

待填写。

### 3.3 C++ 基础层

待填写。

### 3.4 AngelScript/Blueprint 扩展层

待填写。

### 3.5 数据与配置

待填写。

### 3.6 网络与生命周期

待填写。

### 3.7 测试与验收

待填写。

### 3.8 待确认事项

待填写。

## 4. Camera

### 4.1 目标

待填写。

### 4.2 职责边界

待填写。

### 4.3 C++ 基础层

待填写。

### 4.4 AngelScript/Blueprint 扩展层

待填写。

### 4.5 数据与配置

待填写。

### 4.6 网络与生命周期

待填写。

### 4.7 测试与验收

待填写。

### 4.8 待确认事项

待填写。

## 5. Combat

### 5.1 目标

待填写。

### 5.2 职责边界

待填写。

### 5.3 GAS 接入与战斗系统封装

待填写。

### 5.4 C++ 基础层

待填写。

### 5.5 AngelScript/Blueprint 扩展层

待填写。

### 5.6 数据与配置

待填写。

### 5.7 网络与生命周期

待填写。

### 5.8 测试与验收

待填写。

### 5.9 待确认事项

待填写。

## 6. World

### 6.1 目标

建立支持一镜到底体验的世界地图组织、加载、状态管理和持久化框架。

### 6.2 职责边界

- 负责 World Partition、Data Layer、区域组织和流式加载。
- 负责世界状态、对象生成、区域恢复和存档衔接。
- 不负责具体剧情流程、战斗规则或环境物品行为。

### 6.3 C++ 基础层

待填写。

### 6.4 AngelScript/Blueprint 扩展层

待填写。

### 6.5 数据与配置

待填写。

### 6.6 网络与生命周期

- 所有放入 World Partition、Runtime Data Layer 或 Streaming Level 的 Actor
  一律按“可反复入世”设计，不根据当前空间加载设置编写两套生命周期逻辑。
- Actor 向 GameInstance 级 Service 的注册与反注册必须分别归属
  `BeginPlay` 和 `EndPlay`；不得依赖 `Destroyed()` 完成注销。
- World Partition Cell 流出后，同一 Actor 实例可能存活并在重新激活时再次执行
  `PostInitializeComponents` 和 `BeginPlay`，因此初始化、注册和绑定必须幂等。
- `EndPlay(RemovedFromWorld)` 是有状态世界对象快照运行状态的权威时机；重新
  `BeginPlay` 时从 World 状态或存档数据恢复。
- Actor 流出后仍需继续执行的逻辑不得留在 Actor 本体，必须选择常驻、逻辑
  上移或惰性结算方案。
- 完整契约见“Actor 生命周期与 World Partition”章节。

### 6.7 测试与验收

- 在 PIE 中驱动 Streaming Source 离开并返回目标区域，确认 Actor 能够反复
  执行流出和流入流程。
- 将 Runtime Data Layer 在 `Activated` 和 `Loaded` 之间切换，确认 Service
  注册数量在重入后恢复且没有重复项。
- 分别覆盖 `RemovedFromWorld`、`Destroyed`、`LevelTransition` 和
  `EndPlayInEditor` 路径。
- 检查流出 Actor 不再持有 Timer、外部委托、异步请求或世界层资源占用。

### 6.8 待确认事项

待填写。

## 7. Narrative

### 7.1 目标

建立统一的剧情与表演框架，在不切镜的前提下完成剧情推进、角色表演和玩法衔接。

### 7.2 职责边界

- 负责剧情状态、事件触发、对话、剧情流程和表演编排。
- 负责角色走位、交互动画、表情、Sequencer 协作及玩法与表演之间的控制权交接。
- 可以向 Camera 提交镜头意图，但不得绕过 Camera 直接切换视点或制造隐藏切镜。
- 不负责 Camera 的最终镜头计算、Combat 的战斗规则或 World 的地图流送。

### 7.3 C++ 基础层

待填写。

### 7.4 AngelScript/Blueprint 扩展层

待填写。

### 7.5 数据与配置

待填写。

### 7.6 网络与生命周期

待填写。

### 7.7 测试与验收

待填写。

### 7.8 待确认事项

待填写。

## 8. Item

### 8.1 目标

建立统一的环境物品、环境交互和破坏框架。

### 8.2 职责边界

- `Item` 是本项目的环境物品领域名称，并不只表示背包、装备或可拾取物。
- 负责可交互物、物理物体、机关、可破坏物及其持续状态。
- 负责环境物品相关的碰撞、物理反馈、碎片、声音和特效衔接。
- 通过明确接口与 Combat、World 和 Narrative 协作，不直接承担战斗规则、地图流送或剧情编排。

### 8.3 C++ 基础层

待填写。

### 8.4 AngelScript/Blueprint 扩展层

待填写。

### 8.5 数据与配置

待填写。

### 8.6 网络与生命周期

待填写。

### 8.7 测试与验收

待填写。

### 8.8 待确认事项

待填写。

## 9. Input

### 9.1 目标

建立由 CommonUI 和 Enhanced Input 共同组成的统一输入框架，为玩家控制、战斗、交互、剧情表演和 UI 提供稳定、可仲裁的输入意图。

输入分为两个相互独立的实现层：

- **UI Input**：通过 CommonUI Action Router 响应输入，并决定当前物理输入是否继续传递。
- **Gameplay Input**：通过 Enhanced Input 生成带 GameplayTag 的语义输入，并在执行玩法行为前经过 InputTag 策略检查。

UI 不直接操作 Character、Combat 或其他 Gameplay 系统；Gameplay 也不查询 Widget 状态。双方只通过输入路由和策略层协作。

### 9.2 职责边界

- 负责 Input Action、Input Mapping Context、输入优先级、设备输入转换和按键重映射。
- 负责 UI Input 与 Gameplay Input 之间的输入流仲裁。
- 负责根据 GameplayTag 输入策略，逐项允许或阻止 Gameplay 语义输入。
- 负责根据玩法、剧情表演和 UI 状态切换或叠加输入上下文。
- 向 Controller、Character、Combat、Item 和 Narrative 输出经过仲裁的玩法意图。
- 不直接执行角色移动、战斗能力、环境交互、剧情流程或界面业务逻辑。

### 9.3 输入流与仲裁

```mermaid
flowchart TD
    Device["键鼠 / 手柄等输入设备"]
    Viewport["UMCGameViewportClient<br/>继承 UCommonGameViewportClient"]
    CommonRouter["CommonUI Action Router"]
    UIAction["当前活动 UI Action"]
    CommonMode{"CommonUI 是否允许继续传递"}
    EnhancedInput["Enhanced Input"]
    InputComponent["Gameplay InputComponent<br/>绑定 InputAction / 转换 InputTag"]
    GameplayRouter["Gameplay Input Router<br/>InputAction → InputTag"]
    PolicyGate{"InputTag Policy 是否允许"}
    Gameplay["Controller / Character / Combat / Item / Narrative"]
    Stop["输入流结束"]
    ActiveWidget["活动的 CommonActivatableWidget"]
    PolicyTag["Input.Policy.*"]
    PolicySubsystem["Input Policy Subsystem"]

    Device --> Viewport
    Viewport --> CommonRouter
    CommonRouter --> UIAction
    UIAction -->|"已消费"| Stop
    UIAction -->|"未消费"| CommonMode
    CommonMode -->|"Menu：全部截断"| Stop
    CommonMode -->|"Game / All：继续传递"| EnhancedInput
    EnhancedInput --> InputComponent
    InputComponent --> GameplayRouter
    GameplayRouter --> PolicyGate
    PolicyGate -->|"被策略阻止"| Stop
    PolicyGate -->|"允许"| Gameplay

    ActiveWidget -->|"声明"| PolicyTag
    PolicyTag -->|"激活 / 释放策略"| PolicySubsystem
    PolicySubsystem --> PolicyGate
```

输入存在两层相互补充的截断机制：

1. **物理输入消费**：CommonUI Action Binding 通过 `bConsumeInput` 决定当前按键或轴输入是否继续传入 Enhanced Input。
2. **Gameplay 语义输入阻止**：Gameplay Input Router 根据 InputTag 判断某项玩法意图是否允许执行。

例如，背包界面可以消费用于 UI 导航的十字键，允许未消费的鼠标移动继续控制镜头，同时通过 InputTag 策略阻止攻击和环境交互。

#### CommonUI 物理输入消费

`InputComponent` 绑定某个 Gameplay Input Action，只表示输入流到达 Enhanced Input 后由该组件接收，不代表它会先于 UI 获得物理输入。项目使用 `UMCGameViewportClient` 继承 `UCommonGameViewportClient` 后，物理输入首先由 Viewport 转交给本地玩家的 `UCommonUIActionRouterBase`：

```text
UCommonGameViewportClient::InputKey
→ UCommonUIActionRouterBase::ProcessInput
→ 当前活动 UI Action Binding
→ Handled / BlockGameInput / Unhandled
```

CommonUI 提供以下核心 API：

- `UCommonUserWidget::RegisterUIActionBinding`：注册 Widget 所拥有的 UI Action。
- `FBindUIActionArgs::bConsumeInput`：决定匹配的 UI Action 是否消费本次物理输入，默认值为 `true`。
- `UCommonUIActionRouterBase::ProcessInput`：按当前 LocalPlayer、活动 Widget 树、输入模式、按键和输入事件执行路由。
- `ERouteUIInputResult`：返回 `Handled`、`BlockGameInput` 或 `Unhandled`。

当 UI Action 匹配并且 `bConsumeInput == true` 时，Action Router 返回 `Handled`。`UCommonGameViewportClient` 将结果转换为 `FReply::Handled()`，并从 `InputKey` 返回 `true`，不再调用 `Super::InputKey`。因此该物理事件不会进入 PlayerController、Enhanced Input 或 Gameplay InputComponent。

```text
SpaceBar
→ 匹配 UI.Action.Confirm
→ 执行 Confirm
→ bConsumeInput == true
→ Viewport 截断本次 SpaceBar 事件
→ IA_Jump 不触发
```

UI 不根据 `SpaceBar` 倒查 Gameplay Mapping，也不把 `UI.Action.Confirm` 转换为 `Input.Gameplay.Jump`。两者只是可能共享同一个物理按键，不存在语义映射关系。消费本次物理事件会自然阻止该事件产生的所有下游 Gameplay Action；Gameplay InputTag 策略则独立负责禁止某一类玩法意图。

#### 条件性消费

UI 可以根据当前界面状态决定同一个输入是否被消费：

- Widget 不在活动 CommonUI 输入树中时，其非 Persistent Action Binding 不响应也不消费输入。
- Widget 激活且对应 Binding 的 `bConsumeInput == true` 时，匹配输入由 UI 消费。
- `ECommonInputMode::All` 下，未被 UI 消费的输入继续进入 Gameplay。
- `ECommonInputMode::Menu` 下，即使没有 UI Action 处理输入，CommonUI 也会阻止正常 Gameplay 输入；需要按项放行时不得使用该模式。

对于同一个活动 Widget 内部的动态条件，UE 5.8 的 `FUIActionBindingHandle` 没有公开的 `SetConsumeInput`。应当在条件变化时通过 `RegisterUIActionBinding` 和 `FUIActionBindingHandle::Unregister` 注册或注销消费型 Binding，或者将不同输入状态拆分为不同的 `UCommonActivatableWidget`。不得在 Tick 中反复注册，也不得直接修改 CommonUI 内部的 `FUIActionBinding::bConsumesInput`。

`FSimpleDelegate` 类型的 UI Action 回调没有“本次是否消费”的返回值，因此消费决定必须在事件到达前由活动 Widget 和 Binding 状态确定。只有确实无法提前确定的特殊输入规则，才考虑自定义 Action Router 或 Viewport 路由。

### 9.4 C++ 基础层

第一阶段已提供以下基础类型：

- `UMCGameViewportClient`：继承 `UCommonGameViewportClient`，确保 CommonUI Action Router 在 Gameplay Input 之前处理物理输入。
- `UMCInputPolicySubsystem`：基于 `ULocalPlayerSubsystem`，管理当前玩家的活动输入策略、策略优先级和阻止的 Gameplay InputTag。
- `UMCGameplayInputRouter`：作为所有 Gameplay Input Action 的统一入口，在分发玩法意图前执行 InputTag 策略检查。
- `FMCInputPolicyDefinition`：描述策略标签、CommonUI 输入模式、鼠标捕获方式和被阻止的 Gameplay InputTag。
- `FMCInputPolicyHandle`：表示一次策略申请；UI 关闭或对象销毁时通过句柄安全释放，避免重复恢复或状态泄漏。

Gameplay Action 不直接绑定最终玩法行为：

```text
Input Action
→ Gameplay Input Router
→ InputTag Policy
→ Gameplay 系统
```

`UMCInputPolicySubsystem` 只理解 InputTag 和策略定义，不引用具体 Widget、Character、Combat 或 Item 类型。

#### Gameplay InputComponent

角色使用的 Gameplay InputComponent 是 Pawn 层的输入适配器，不是全局输入管理器，也不负责判断当前 UI 状态。其职责为：

- 绑定角色所需的 Enhanced Input Action。
- 将 `InputAction + TriggerEvent + Value` 转换为项目统一的 Gameplay InputTag 语义。
- 将输入提交给 `UMCGameplayInputRouter`，在策略允许后更新角色输入状态或分发玩法意图。
- 缓存移动、跳跃、蹲伏等 Mover 所需输入，并通过 `IMoverInputProducerInterface` 提供预测输入。
- 在失去控制权、策略截断或输入上下文切换时清理持续输入状态。

Gameplay InputComponent 不得：

- 查询活动 Widget、保存 Widget 引用或自行判断 UI 是否打开。
- 定义当前 Input Policy。
- 绕过 Gameplay Input Router 直接执行最终移动、战斗、交互或剧情行为。
- 复制原始设备输入；网络层只处理经过仲裁的移动输入或玩法请求。

角色不在 `BeginPlay` 中监听玩家设备输入。角色获得本地玩家控制权并由 Unreal 调用 `SetupPlayerInputComponent` 后，才初始化 Gameplay InputComponent、绑定 Input Action 并连接 Mover Input Producer。初始化必须保证幂等，重新控制角色时先清理旧绑定。

只有同时满足以下条件的角色才监听玩家输入：

- 角色由 `APlayerController` 控制。
- 角色属于本地玩家。
- `IsLocallyControlled()` 为 `true`。

远端模拟角色、AI 角色以及服务端上的非本地角色不绑定本地设备输入。角色在 `UnPossessed`、InputComponent `OnUnregister`、Controller 更换或 `EndPlay` 时停止监听，移除角色 Mapping Context、清除 Action Binding、重置输入状态并解除 Mover Input Producer。

### 9.5 AngelScript/Blueprint 扩展层

- 项目的 Common Activatable Widget 基类提供 `InputPolicyTag` 属性。
- Widget 激活时使用 `InputPolicyTag` 申请策略，停用时释放策略句柄。
- Root Layout 或 UI Manager 注册打开系统菜单等全局 UI Action。
- Common Activatable Widget 基类注册返回、确认等公共 UI Action。
- 背包、地图、对话等具体 Widget 只注册自身页面使用的 UI Action。
- 只有始终有效的系统级 UI Action 才能注册为 Persistent Action。

UI Action 的路由入口统一为 CommonUI Action Router，但 Action 的注册位置由其实际所有者决定，不集中维护全部页面输入。

### 9.6 数据与配置

#### Input Policy Tag

Widget 使用 GameplayTag 声明输入策略，策略标签通过数据配置解析为具体行为：

| PolicyTag | CommonUI 模式 | Gameplay Input 策略 |
| --- | --- | --- |
| `Input.Policy.Gameplay` | `Game` | 允许 Gameplay Input |
| `Input.Policy.Mixed.Inventory` | `All` | 阻止移动、战斗和交互，按需允许镜头 |
| `Input.Policy.Mixed.Dialogue` | `All` | 阻止战斗和交互，按需允许移动及镜头 |
| `Input.Policy.UIOnly.PauseMenu` | `Menu` | 阻止全部 Gameplay Input |

Widget 只声明 PolicyTag，不直接设置或移除 Gameplay Mapping Context，也不调用 Gameplay Input Component。

#### UI Action Tag

CommonUI 使用 `FUIActionTag` 标识逻辑 UI Action。`FUIActionTag` 是 `FGameplayTag` 的强类型派生，统一使用 CommonUI 要求的 `UI.Action.*` 根标签：

```text
UI.Action.Confirm
UI.Action.Back
UI.Action.NextTab
UI.Action.PreviousTab
```

UI Action Tag 负责描述界面操作，由 CommonUI 根据当前输入设备和按键配置解析为实际 `FKey`。UI 代码不得硬编码空格、回车、手柄面键等物理按键，也不得根据物理按键倒查 Gameplay InputTag。

#### Gameplay Input Tag

每个 Gameplay Input Action 必须关联一个语义标签：

```text
Input.Gameplay.Move
Input.Gameplay.Look
Input.Gameplay.Jump
Input.Gameplay.Interact
Input.Gameplay.Combat.Attack
Input.Gameplay.Combat.Dodge
Input.Gameplay.Combat.Block
```

策略既可以阻止单项输入，也可以通过父标签批量阻止。例如，阻止 `Input.Gameplay.Combat` 时，Attack、Dodge 和 Block 均不可执行；只阻止 `Input.Gameplay.Combat.Attack` 时，Dodge 和 Block 不受影响。

Gameplay Input 配置最终采用 `InputTag + InputAction` 的数据映射形式，避免每增加一种输入就扩展一个固定成员字段。

#### Mapping Context 优先级

Mapping Context 优先级使用统一的具名常量，不允许各 Widget 自行填写任意数值：

| 类型 | 优先级 |
| --- | ---: |
| Gameplay | `0` |
| UI | `1000` |
| System | `2000` |

`IMC_UI_*` 由 Common Activatable Widget 基类以 UI 优先级激活；`IMC_System_*` 只容纳打开系统菜单等必须始终优先的输入。需要截断低优先级映射的 UI Action 必须启用 Consume Input。

### 9.7 网络与生命周期

- 原始设备输入、CommonUI 路由和 Input Policy 均属于本地玩家，不进行网络复制。
- 通过策略检查后的 Gameplay 意图再进入 Character、Combat、Item 等各自的网络预测或服务器权威流程。
- Input Policy 采用申请句柄管理生命周期，不允许通过单一布尔值直接开关 Gameplay Input。
- 多个 UI 同时激活时，被阻止的 Gameplay InputTag 取活动策略的并集；CommonUI 模式、鼠标捕获和焦点配置由最高优先级的活动策略决定。
- Widget 停用、LocalPlayer 退出或世界切换时必须释放对应策略句柄。
- 进入全 UI 模式时清理已按下按键；恢复 Gameplay 时避免菜单关闭按键继续触发攻击、跳跃等玩法行为。
- 当策略新增对持续型 InputTag 的阻止时，Gameplay Input Router 必须主动取消对应输入并通知 InputComponent 将状态归零，不能等待可能已被 UI 消费的 Released 事件。
- UI 关闭并恢复 Gameplay 时，当前仍处于按下状态的按键必须等待释放后才能重新触发，避免关闭菜单的 Confirm 或 Back 同时触发 Jump、Attack 等玩法行为。

### 9.8 测试与验收

- 验证 CommonUI Action 回调始终先于 Enhanced Input Gameplay 回调。
- 验证 UI Action 消费输入后，同一物理输入不会触发 Gameplay。
- 验证 UI 消费物理输入时不查询 Gameplay Mapping，也不需要解析对应 Gameplay InputTag。
- 验证 `All` 模式下，UI 未消费且策略允许的输入可以正常流入 Gameplay。
- 验证同一 Widget 状态变化时，消费型 Binding 的注册和注销能够分别截断和放行同一个输入。
- 验证非活动 Widget 的非 Persistent Action Binding 不会响应或消费输入。
- 验证混合界面能够允许 Look，同时独立阻止 Move、Attack 或 Interact。
- 验证父 InputTag 可以批量阻止其所有子输入，单项 InputTag 不影响同级输入。
- 验证 `Menu` 模式下，即使没有 Widget 处理某个按键，该输入也不会进入 Gameplay。
- 验证多个 UI 策略叠加及乱序关闭时，输入状态能够正确恢复。
- 验证 UI 打开、关闭和焦点切换时不会产生残留按键、幽灵攻击或持续移动。
- 验证角色仅在本地玩家控制且完成 `SetupPlayerInputComponent` 后监听输入，并在失去控制权时彻底清理。
- 验证键鼠、手柄及不同 LocalPlayer 的输入策略彼此独立。

### 9.9 当前决策

- 使用 `UMCGameViewportClient` 继承 `UCommonGameViewportClient`，由 CommonUI 提供 UI 优先的物理输入路由。
- UI 通过 GameplayTag 声明输入策略，不直接依赖 Gameplay 实现。
- CommonUI Action Router 负责物理输入消费，Gameplay Input Router 负责语义 InputTag 策略检查。
- UI Action 使用 `UI.Action.*`，Gameplay Input 使用 `Input.Gameplay.*`；两者不互相转换，也不通过物理按键倒查。
- 条件性拦截由活动 Widget 和消费型 Action Binding 的生命周期控制；需要逐项放行的混合界面使用 `All` 模式。
- Gameplay InputComponent 在本地玩家的 `SetupPlayerInputComponent` 阶段开始监听，并作为 Enhanced Input 到 Gameplay Input Router 之间的角色输入适配器。
- Gameplay、UI 和 System Mapping Context 分别使用 `0`、`1000` 和 `2000` 的固定优先级。
- 所有 Gameplay Input 必须先通过统一路由，不允许 Input Action 直接执行最终玩法行为。

### 9.10 实现状态

第一阶段 C++ 输入基础框架已完成，并通过 Unreal Engine 5.8 `MoldCastingEditor Win64 Development` 编译：

- 已启用 CommonUI，并将 `UMCGameViewportClient` 配置为项目 Game Viewport Client。
- 已提供 `UMCInputSettings`、`UMCInputPolicySubsystem`、`UMCGameplayInputRouter`、策略定义和策略句柄。
- 已提供 Gameplay、Input Policy 和基础 UI Action Native GameplayTag。
- `UMCRoleInputComponent` 已接入 Gameplay Input Router，并在策略变化、失去控制权和组件注销时清理持续输入。
- `UMCRoleInputConfig` 已支持 `InputAction + InputTag` 数据映射；现有固定 Action 字段暂时保留为兼容回退，避免破坏已有角色输入资源。
- 已统一 Gameplay、UI 和 System Mapping Context 的具名优先级常量。

尚未完成的后续工作：

- Common Activatable Widget 项目基类及其 `InputPolicyTag` 自动申请/释放。
- 具体 UI Action、UI Mapping Context 和页面级条件消费配置。
- Input Policy 的自动化测试，以及实际 UI 页面中的 PIE 输入顺序与残留按键验收。

## 10. UI

### 10.1 目标

建立基于 CommonUI 的统一界面框架。UI 的生命周期归属于 GameInstance 和
GameViewport，不依赖任何 Gameplay Level，并允许 AngelScript 通过稳定的
C++ 接口实现易变的业务逻辑。

### 10.2 职责边界

- UI 框架不引用 Character、Combat、World、Narrative、Item 或具体 Level 类型。
- Gameplay 不负责创建、持有或销毁 RootWidget。
- 稳定的对象生命周期和容器管理由 C++ 实现；易变的页面业务由 AngelScript
  通过 `UMCGameInstance::UIService` 调用。
- 第一阶段不设计页面 Tag、Layer、页面栈或具体业务页面。

### 10.3 C++ 基础层

- `UMCUIService`：继承项目已有的 `UMCBaseService`，由 `UMCGameInstance`
  获取并调用 `Init()`、`OnStart()`。
- `UMCUIRootWidget`：继承 `UCommonUserWidget`，是游戏所有 UI 的唯一根容器。
- `UMCUIService` 是 RootWidget 的唯一创建者和生命周期管理者。
- RootWidget 使用 GameInstance 作为 Outer，并通过
  `UGameViewportClient::AddViewportWidgetContent` 直接挂载到 GameViewport。
- `UMCUIService` 提供 `CreateAndAddWidget`、`AddWidget`、`RemoveWidget`、
  `ClearWidgets` 和 `GetRootWidget` 等稳定接口。
- `UMCUIRootWidget` 使用原生全屏 Overlay 承载子 Widget，并按 ZOrder 排序。

### 10.4 AngelScript 扩展层

- `UMCUIService` 和 `UMCUIRootWidget` 不设计 AngelScript 子类。
- AngelScript 通过 `UMCGameInstance::GetInstance()` 获取 GameInstance，再访问
  `UIService`。
- AngelScript 只负责具体页面的打开、关闭、交互和其他易变业务逻辑。
- AngelScript 不负责 Service、RootWidget、GameViewport 或跨地图生命周期。

### 10.5 数据与配置

- 第一阶段不引入 UI Tag、页面注册表或 Layer 配置。
- 具体页面通过 `TSubclassOf<UUserWidget>` 传给 `CreateAndAddWidget`。
- `ZOrder` 只用于最基础的显示顺序；统一 Layer 规划留到后续阶段。

### 10.6 网络与生命周期

- UIService 是 `UGameInstanceSubsystem`，随 GameInstance 创建和销毁。
- RootWidget 由 GameInstance 持有，不使用 World、Level、GameMode 或
  PlayerController 作为 Outer。
- UIService 监听 GameViewport 创建和地图加载事件；Viewport 暂时不可用时保留
  RootWidget，Viewport 可用后再挂载。
- RootWidget 直接挂到 GameViewport，不注册为随 World 清理的普通 Viewport
  Widget，因此地图切换期间保持同一实例。
- 当前按单本地玩家设计；多人分屏属于后续扩展。

### 10.7 测试与验收

- `UMCUIService::ValidateRootWidget` 验证 RootWidget、GameInstance Outer 和
  GameViewport 挂载状态。
- `UMCUIService::BeginLevelIndependenceTest` 记录 RootWidget，切换到引擎入口地图
  `/Engine/Maps/Entry`，地图加载后验证 RootWidget 仍为同一实例。
- 开发环境可通过控制台命令 `MC.UI.TestLevelIndependence` 执行完整测试。
- 测试成功日志为 `UI_LEVEL_INDEPENDENCE_TEST_PASSED`。
- 该测试不依赖项目 Gameplay Level；UE 显示任何 UI 的最低前提仍是
  GameViewport 已创建。

### 10.8 待确认事项

- 页面 Layer、页面栈和模态规则。
- 页面标识及 UI Tag 体系。
- 具体页面与此前 Input Policy 的自动接入方式。

## 11. 基础服务框架

### 11.1 目标

基础服务框架用于承载不属于具体 Gameplay Level、且需要贯穿整个游戏进程的
跨系统能力。所有服务继承 `UMCBaseService`，以 `UGameInstanceSubsystem`
作为生命周期基础，由 `UMCGameInstance` 统一持有并调用项目级初始化入口。

当前服务启动顺序为：

```text
UMCGameInstance
→ UMCLogService
→ UMCDebugService
→ UMCDataTableService
→ UMCCharacterService
→ UMCUIService
```

`UMCLogService` 最先启动，保证后续服务在初始化和启动阶段都可以使用统一日志。

### 11.2 LogService 职责

`UMCLogService` 负责提供统一的项目日志入口，隔离业务代码与零散的日志分类、
输出级别和 AngelScript 绑定实现。LogService 只负责诊断信息输出，不承载业务
状态、流程控制或错误恢复逻辑。

当前日志分类如下：

| 分类 | 用途 |
| --- | --- |
| `MCCommon` | 跨系统通用信息 |
| `MCUI` | UI 框架和页面业务 |
| `MCInScreen` | 屏幕调试信息及其日志副本 |
| `MCCombat` | 战斗系统 |
| `MCItem` | 环境物品与环境交互 |
| `MCWorld` | 世界地图、加载和世界状态 |
| `MCCharacter` | 角色、移动和动画相关逻辑 |

每个分类均提供 `Info`、`Warn` 和 `Error` 三种输出级别：

- `Info` 对应 Unreal `Display`。
- `Warn` 对应 Unreal `Warning`。
- `Error` 对应 Unreal `Error`。

### 11.3 C++ 接口

C++ 通过 `DECLARE_NATIVE_LOG_METHOD` 为每个分类生成带编译期格式检查的日志
方法，命名形式为：

```text
NativeLog<Category>Info
NativeLog<Category>Warn
NativeLog<Category>Error
```

调用示例：

```cpp
if (IsValid(LogService))
{
	LogService->NativeLogMCCharacterInfo(
		TEXT("Character [%s] initialized."),
		*GetNameSafe(Character)
	);
}
```

格式化完成后统一通过 `UE_LOG` 输出，业务代码不需要自行选择 Unreal 日志分类。

### 11.4 AngelScript 接口

`MoldCastingBasic` 模块依赖 `AngelscriptCode`，并通过
`FAngelscriptBinds::ExistingClass("UMCLogService")` 为现有 C++ Service 注册
AngelScript 方法。绑定对象使用项目命名 `Bind_MCLog`。

AngelScript 接口命名形式为：

```text
Log<Category>Info
Log<Category>Warn
Log<Category>Error
```

调用流程为：

```text
UMCGameInstance::GetInstance()
→ LogService
→ Log<Category><Level>(Message)
→ 对应 Unreal Log Category
```

AngelScript 只提交最终字符串，不直接依赖 `UE_LOG` 宏或 C++ 可变参数格式化。

### 11.5 屏幕调试输出

`PrintInScreen` 用于需要同时保留日志记录和屏幕提示的开发期诊断信息：

```text
PrintInScreen
├─ 写入 MCInScreen / Display
└─ GEngine::AddOnScreenDebugMessage
```

当前提供纯文本和 `float` 数值两种重载。屏幕信息默认显示 5 秒；未传入颜色时
使用随机颜色。即使当前没有有效 `GEngine`、无法显示屏幕信息，日志副本仍会
正常写入 `MCInScreen`。

### 11.6 生命周期与边界

- LogService 随 GameInstance 创建和销毁，不依赖 World、Level、GameMode 或
  PlayerController。
- `UMCGameInstance` 缓存 LogService，并在其他项目服务之前调用其 `Init()` 和
  `OnStart()`。
- C++ 和 AngelScript 共用相同日志分类，避免形成两套日志命名。
- 业务系统负责选择正确分类和日志级别，LogService 不判断业务成功或失败。
- 屏幕输出只用于开发诊断；需要持久保存或面向玩家显示的信息不得依赖
  `PrintInScreen`。
- GameInstance 级 Service 的 Actor 注册接口必须支持幂等注册、容忍重复反注册，
  并允许查询方在目标 Actor 尚未入世或已经流出时获得空结果。
- Service 不得假定已注册 Actor 的寿命与自身相同；Actor 必须在 `EndPlay`
  反注册，Service 也应在开发环境提供无效对象自检。

### 11.7 验证要求

- 使用 Unreal Engine 5.8 `MoldCastingEditor Win64 Development` 验证 C++、
  反射代码和 AngelScript 绑定能够完成编译与链接。
- 分别从 C++ 和 AngelScript 调用各日志级别，确认分类与级别正确。
- 验证 `PrintInScreen` 在可用 Viewport 中同时产生屏幕信息和日志副本。
- 验证无有效 `GEngine` 时不会访问无效对象，且日志输出仍然有效。

## 12. Actor 生命周期与 World Partition

> 状态：已拍板（2026-07-27）；2026-07-29 修订
> `PostInitializeComponents` 重入规则。
>
> 适用范围：所有放入关卡或运行时 Spawn 的游戏 Actor，包括角色、环境物品、
> 机关、Trigger、Maker 和其他世界对象。

### 12.1 目标

Actor 向 GameInstance 级 Service 的注册和反注册，在 World Partition 流送、
Runtime Data Layer 切换、切关及显式销毁路径下都必须满足：

- 不泄漏注册项。
- 不留下悬空引用。
- 同一实例可以反复流出和流入。
- 每类初始化和反初始化操作都有唯一明确的生命周期归属。

GameInstance 级 Service 的寿命跨 World。Actor 的注销如果只放在
`Destroyed()`，World Partition 流出和切关后就可能在 Service 中留下已经离开
世界的对象，因此本项目以 `EndPlay` 作为 Actor 运行期清理的统一出口。

### 12.2 UE 5.8 引擎事实

#### 12.2.1 Actor 消亡路径

| 路径 | `EndPlay` | `Destroyed()` |
| --- | --- | --- |
| 显式 `Destroy()` | `Destroyed` | 执行 |
| WP Cell 流出、Streaming Level 隐藏、Data Layer 停用 | `RemovedFromWorld` | 不执行 |
| 切关、PIE 结束、退出 | `LevelTransition`、`EndPlayInEditor` 或 `Quit` | 不执行 |

结论：所有运行期注销和清理只能依赖 `EndPlay`。`Destroyed()` 不承担注销职责。

源码依据：

- 显式销毁入口：`UWorld::DestroyActor`，`LevelActor.cpp`。
- 流送移除：`ULevel::RouteActorEndPlayForRemoveFromWorld`，`Level.cpp`。
- 世界结束：`UWorld::CleanupWorld` 相关流程，`World.cpp`。

#### 12.2.2 同一实例会反复入世

World Partition Runtime Cell 包含 `Unloaded`、`Loaded`、`Activated` 三种状态：

- `Activated → Loaded`：Level 从 World 移除，Actor 执行
  `EndPlay(RemovedFromWorld)`，但实例可以继续存活。
- 流送移除会重置 Actor 初始化状态。
- Cell 重新进入 `Activated` 后，同一实例可以重新执行
  `InitializeComponents`、`PostInitializeComponents` 和 `BeginPlay`。
- `Loaded → Unloaded` 后由 GC 清理；下次流入可能从包重新创建新实例。

因此必须遵守：

1. 成员变量在 `EndPlay` 到下一次 `BeginPlay` 之间不会自动恢复为构造默认值。
2. `PostInitializeComponents` 与 `BeginPlay` 都是“每次入世一次”，不是
   “同一实例一生一次”。
3. 引擎没有“同一实例只执行一次”的 Actor 入世钩子；真正的一次性操作必须
   使用对象自身持有的标志守门，或设计为天然幂等。
4. Runtime Data Layer 停用同样可以产生 `RemovedFromWorld`，因此
   `bIsSpatiallyLoaded == false` 不代表 Actor 永远不会流出。

相关源码流程包括：

- `AActor::RouteEndPlay`，`Actor.cpp`。
- `ULevel::RouteActorInitialize`，`Level.cpp`。
- `ResetLevelFlagsOnLevelAddedToWorld`，`World.cpp`。

#### 12.2.3 EndPlay 不会自动清理 Timer

`AActor::EndPlay` 不会替 Actor 调用 `ClearAllTimersForObject`。Actor 因
`RemovedFromWorld` 流出后实例仍可能存活，未清理的 Timer 仍会触发并回调一个
已经离开世界的 Actor。

结论：Actor 设置的 Timer 必须在自身 `EndPlay` 中显式清理。

#### 12.2.4 OnConstruction 不是运行时初始化钩子

- 编辑器中，属性修改、拖动和 Undo/Redo 都可能触发
  `RerunConstructionScripts`。
- 打包游戏加载关卡摆放 Actor 时，不重新执行其 Construction Script，而是
  直接使用保存关卡时烘焙的结果。
- 运行时 `SpawnActor` 创建的 Actor 会在 Spawn 流程中执行 Construction。

结论：`OnConstruction` 只负责根据作者属性生成可烘焙、确定性的程序化外观；
运行时注册、监听、Timer、状态恢复和业务逻辑禁止放入 `OnConstruction`。

### 12.3 生命周期职责表

| 钩子 | 引擎特性 | 项目职责 |
| --- | --- | --- |
| 构造函数 | CDO 和实例都会执行；不可依赖 World | 创建默认组件、设置成员默认值、Tick 开关、加载类别和碰撞 Profile 类默认 |
| `OnConstruction` | 编辑器可反复执行；打包游戏中的关卡摆放 Actor 不执行 | 仅可烘焙的程序化外观 |
| `PreRegisterAllComponents` | 组件注册前的特殊时机 | 仅处理必须早于组件 `OnRegister` 的数据灌入 |
| `PostInitializeComponents` | 每次入世执行；组件已注册；早于 `BeginPlay` | 对内且幂等的初始化、自身组件配置、自身组件委托绑定、组件间接线 |
| `BeginPlay` | 每次入世执行；Actor 之间顺序无保证 | Service 注册、外部委托监听、Timer、周期逻辑、解析其他 Actor 引用 |
| `EndPlay` | 全部消亡和流出路径的统一出口 | 反注册、反监听、清 Timer、取消异步、释放世界资源、回收伴生对象、状态快照 |
| `Destroyed()` | 仅显式 `Destroy()` 执行 | 不放项目清理逻辑，不保留空覆写 |
| `BeginDestroy` / `FinishDestroy` | GC 阶段，其他 UObject 可能已经无效 | 仅释放裸内存或操作系统句柄等非 UObject 资源 |

#### 自身组件与外部对象的绑定判据

- 绑定目标是自身组件：放在 `PostInitializeComponents`，使用
  `AddUniqueDynamic` 等幂等方式；绑定随实例共同存活，通常无需在
  `EndPlay` 解绑。
- 绑定目标是 Service、Message Router、其他 Actor 或其他外部对象：必须在
  `BeginPlay` 监听，在 `EndPlay` 反监听。

`PostInitializeComponents` 会在流送重入时再次执行，禁止对自身组件使用没有
幂等保护的裸 `AddDynamic`。

### 12.4 生命周期契约

#### 规则 1：注册与反注册成对

凡向 GameInstance 级 Service、Message Router 或其他长寿命对象注册的 Actor：

- `BeginPlay` 注册。
- `EndPlay` 反注册。
- 禁止把注销放在 `Destroyed()`、`BeginDestroy()` 或 `FinishDestroy()`。

#### 规则 2：EndPlay 最后调用 Super

`BeginPlay` 先调用 `Super`，再执行项目逻辑；`EndPlay` 先完成项目清理，最后
调用 `Super`：

```cpp
void AMCFoo::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	// 反注册、反监听、清 Timer、取消异步并释放运行期资源。

	Super::EndPlay(EndPlayReason);
}
```

#### 规则 3：所有 Actor 一律按可反复入世编写

不根据空间加载设置区分两套生命周期写法。所有 Actor 都必须能够反复执行：

```text
PostInitializeComponents
→ BeginPlay
→ EndPlay
→ PostInitializeComponents
→ BeginPlay
```

每次入世都要做的操作必须幂等；真正只允许一次的操作必须由对象自身标志守门。

#### 规则 4：跨对象注册使用句柄

跨对象注册接口应返回句柄并由 Actor 保存：

- `BeginPlay` 注册前确认旧句柄无效。
- 注册成功后保存句柄。
- `EndPlay` 使用句柄注销，然后重置句柄。

同一实例再次 `BeginPlay` 时，如果旧句柄仍然有效，应通过 `ensureMsgf` 在开发
阶段立即暴露漏注销问题。

#### 规则 5：加载类别在构造函数声明

- 数量少且必须始终存在的常驻 Actor，在构造函数中调用
  `SetIsSpatiallyLoaded(false)`。
- Item、门、Trigger、可破坏物等场景对象保持空间加载，接受流出、卸载和重建。
- 禁止在 `BeginPlay` 等运行期钩子中修改 `SetIsSpatiallyLoaded`。

加载类别属于类级设计决策，不能替代可重入生命周期要求。

#### 规则 6：运行时分支只认 EndPlayReason

需要区分流出和真正死亡时，使用 `EEndPlayReason`：

| Reason | 含义 | 典型处理 |
| --- | --- | --- |
| `RemovedFromWorld` | WP Cell 降级、Data Layer 停用或 Streaming Level 隐藏 | 反注册；按需快照运行状态 |
| `Destroyed` | 显式销毁形成的逻辑死亡 | 反注册；按需清理持久状态 |
| `LevelTransition`、`Quit`、`EndPlayInEditor` | 当前 World 收场 | 完成统一反注册和临时资源清理 |

禁止根据空间加载设置或其他间接状态猜测 Actor 为什么结束。

#### 规则 7：Service 侧必须防御

- 同实例或同标识重复注册：记录告警并忽略，不产生重复项。
- 反注册目标不存在：安全返回。
- 查询目标尚未注册或已经流出：返回空，不使用断言表示业务失败。
- 开发环境应能检查注册表中的 UObject 是否仍然 `IsValid`，并输出可定位日志。

#### 规则 8：明确边界和豁免

- 池化 Actor 的挂起与恢复不依赖 `BeginPlay`、`EndPlay`，必须使用单独的池化
  激活桥和记账流程，不与 World Partition 注册账混用。
- 必须早于 `BeginPlay` 的特殊数据灌入可以使用
  `PreRegisterAllComponents`，但不得借此提前进行 Service 注册。
- Possess 和输入绑定归属 `OnPossess`、`OnUnPossess` 及
  `SetupPlayerInputComponent`，不放入普通 Actor 的 `BeginPlay`。
- Actor Component 的 `BeginPlay`、`EndPlay` 遵循相同原则；
  `OnRegister`、`OnUnregister` 同样可能反复触发。

### 12.5 常见操作归属

| 操作 | 生命周期归属 |
| --- | --- |
| 创建默认组件、碰撞 Profile 类默认 | 构造函数 |
| 依赖组件实例的碰撞配置 | `PostInitializeComponents`，必须幂等 |
| 自身组件 Overlap 等委托 | `PostInitializeComponents` + `AddUniqueDynamic` |
| Service 注册与反注册 | `BeginPlay` / `EndPlay` |
| 外部委托监听与反监听 | `BeginPlay` / `EndPlay` |
| Timer | `BeginPlay` 设置，`EndPlay` 显式清理 |
| Smart Object 等世界层资源占用 | `EndPlay` 释放 |
| Streamable Manager、Latent Action 等异步请求 | `EndPlay` 取消 |
| Actor 创建的伴生 Spawn | `EndPlay` 回收或明确移交所有权 |
| 流出状态快照 | `EndPlay(RemovedFromWorld)` |
| Transform 等真正出厂数据 | 作者数据烘焙，或一次性标志保护的缓存 |

### 12.6 BeginPlay 警示清单

代码审查时，`BeginPlay` 出现以下内容必须说明理由，默认视为不能安全重入：

1. `static` 局部变量。
2. 只设置、不在 `EndPlay` 恢复的一次性 Bool。
3. `SetTimer` 后没有对应的 `ClearTimer`。
4. `AddDynamic` 既没有使用幂等绑定，也没有在 `EndPlay` 解绑。
5. 把 `BeginPlay` 或 `PostInitializeComponents` 时刻的 Transform 直接当作
   永久出厂值。
6. Spawn 出寿命可能超过自己的对象，但没有回收或移交所有权。
7. 向外部对象注册后没有保存可用于注销的句柄。

### 12.7 Actor 流出后的逻辑路线

流出后的 Actor 组件已经反注册，物理、渲染和导航能力均不可用。此时仍然触发
的 Timer 属于生命周期错误，不是离线运行机制。

需要在 Actor 流出后继续演化的系统，从以下路线中选择：

| 路线 | 实现 | 适用场景 | 代价 |
| --- | --- | --- | --- |
| A. 常驻 | 构造函数设置 `SetIsSpatiallyLoaded(false)` | 数量少且必须持续全保真模拟的对象 | 持续占用内存和运行成本 |
| B. 逻辑上移 | Actor 仅作为表现壳，权威逻辑放入 Service、World Subsystem 或其他常驻数据模型 | 流出后仍需移动、寻路、战斗或持续与世界交互 | 架构成本最高，但能力完整 |
| C. 惰性结算 | 流出时保存状态和时间戳，流入时按经过时间一次性结算 | 生长、燃烧、刷新等纯时间演化 | 成本最低，适合多数环境对象 |

选型原则：

- 需要持续与世界交互：路线 B。
- 只需随时间演化：路线 C。
- 前两者都不适用且对象数量很少：路线 A。

### 12.8 验证与验收

#### 手动验证

1. 在 PIE 中使用 Streaming Source 让测试区域流出后再流入。
2. 将 Runtime Data Layer 从 `Activated` 切换到 `Loaded`，再切回
   `Activated`。
3. 比较各 Service 在流出前、流出后和流入后的注册数量。
4. 确认同一实例重入时没有重复委托、重复 Timer 或重复注册。
5. 分别执行显式 `Destroy()`、切关和结束 PIE，确认都能完成清理。

#### 自动化验证

- 将 Cell 或 Data Layer 流出、流入流程纳入自动化测试。
- 对注册句柄、Service 注册数量和无效对象数量建立断言。
- 在 Actor 流出后推进时间，确认 Timer 和异步回调不会打到世外对象。
- 验证 `RemovedFromWorld` 状态快照能够在重新入世时正确恢复。
