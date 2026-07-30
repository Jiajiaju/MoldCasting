---
name: style-review
description: 按 MoldCasting 项目 AGENTS.md 检查当前 Git 工作区中的代码改动。提交前，或用户要求“检查风格”“review 一下”“代码规范检查”时使用。
---

# 项目风格 Review

## 1. 获取本次改动

```powershell
git status --short
git diff -- <file>
git diff --cached -- <file>
git ls-files --others --exclude-standard
git diff --check
```

- 对已跟踪文件检查暂存和未暂存 Diff。
- 对未跟踪的新文件直接读取全文。
- 忽略 `Binaries/`、`DerivedDataCache/`、`Intermediate/`、`Saved/` 和 `.idea/`。
- 只检查本次任务改动涉及的行及必要上下文。存量问题单独说明，不混入本次结论。
- 保留工作区中的无关改动，不通过还原文件来缩小检查范围。

## 2. 对照项目规则

完整读取仓库根目录 `AGENTS.md`，以其中“代码风格”和相关工作规则为单一来源。不要在 Skill 中复制另一份风格清单。

至少检查：

- C++、AngelScript 和 Unreal 命名、格式、反射及生命周期规则。
- 公共头文件依赖、前向声明和 `Build.cs` 依赖是否必要。
- 指针判空、成员默认值、代码块、函数签名、宏注释和类成员顺序。
- 是否引入 Magic Number、空白行、无必要 Tick 或不存在的 Debug 命令。
- 是否误改生成目录、二进制资源或任务范围外文件。

## 3. 输出检查结果

- 每条问题使用“`文件:行号` + 违反的 AGENTS.md 小节 + 一句修改建议”。
- 区分本次改动问题和存量问题。
- 全部通过时明确说明“风格检查通过”，不要为了输出问题而硬凑。
- 修复后重新检查修改位置，并再次运行 `git diff --check`。

## 4. 验证

修改 C++ 后：

1. 检查 `UnrealEditor.exe` 是否正在打开当前 `MoldCasting.uproject`；其他项目的编辑器不构成本项目 DLL 锁定。
2. 编译 Unreal Engine 5.8 `MoldCastingEditor Win64 Development` Target：

```powershell
& 'D:\JiaXYTrunk\Engine\Build\BatchFiles\Build.bat' MoldCastingEditor Win64 Development '-Project=D:\JiaXYTrunk\Projects\MoldCasting\MoldCasting.uproject' -WaitMutex -NoHotReloadFromIDE
```

修改 AngelScript 后，运行：

```powershell
& 'D:\JiaXYTrunk\Engine\Binaries\Win64\UnrealEditor-Cmd.exe' 'D:\JiaXYTrunk\Projects\MoldCasting\MoldCasting.uproject' -run=AngelscriptTest -unattended -nullrhi -nosplash -stdout
```

交付前再次运行 `git status --short` 并检查完整 Diff。文档或 Skill 单独修改时不要求编译 C++ 或运行 AngelScript commandlet。
