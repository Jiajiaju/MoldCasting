# AGENTS.md

## Project overview

- This repository contains an Unreal Engine 5.8 project named `MoldCasting`.
- The primary runtime module is `MoldCasting` under `Source/MoldCasting`.
- The scripting framework is [Unreal Engine Angelscript](https://angelscript.hazelight.se/).
- The project currently uses a custom/source engine build. `MoldCasting.uproject` has no `EngineAssociation`, so do not assume a globally installed engine path.
- The repository's primary branch is `main`.

## Repository layout

- `Source/`: C++ modules and Unreal Build Tool target files.
- `Config/`: project configuration that may be edited and committed.
- `Content/`: Unreal binary assets. This directory is currently ignored by Git.
- `MoldCasting.uproject`: project modules and plugin configuration.
- `Binaries/`, `DerivedDataCache/`, `Intermediate/`, `Saved/`, `.idea/`: generated or machine-local data.

## Working rules

- Before modifying any file, explain the intended changes and wait for explicit user confirmation.
- Never edit or commit generated files in `Binaries/`, `DerivedDataCache/`, `Intermediate/`, or `Saved/`.
- Preserve user-created Unreal assets. Do not delete, rename, or overwrite `.uasset` or `.umap` files unless the user explicitly requests it.
- Do not treat `.uasset` or `.umap` files as text. Use Unreal Editor or appropriate Unreal tooling for asset-level changes.
- Do not change the `Content/` ignore policy or introduce Git LFS without explicit user approval.
- Keep changes scoped to the request and preserve unrelated working-tree changes.
- Use `main` as the default branch. Do not force-push or rewrite shared history unless explicitly requested.
- When the user asks to commit changes, push the resulting commit to the tracked remote branch immediately after the commit succeeds, unless the user explicitly asks not to push.

## C++ conventions

- Follow Unreal Engine naming and reflection conventions (`A`, `U`, `F`, `E`, `I`, and `T` prefixes as applicable).
- Use `UCLASS`, `USTRUCT`, `UENUM`, `UFUNCTION`, and `UPROPERTY` where Unreal reflection, serialization, replication, or Blueprint exposure requires them.
- Keep public headers minimal and prefer forward declarations when practical.
- Add module dependencies to `MoldCasting.Build.cs` only when they are actually required.
- Match the existing Unreal style: tabs for indentation, braces on their own lines, and `PascalCase` identifiers.
- Replace magic numeric literals with clearly named constants.
- Use blank lines intentionally to communicate code structure.
- Blank lines must be completely empty and must not contain spaces, tabs, or other whitespace.
- Name function library classes `UXxxxFunctionLibrary`; the Angelscript binding automatically removes the `U` prefix and `FunctionLibrary` suffix.
- Never abbreviate `Component` as `Comp`; write `Component` in full.
- Avoid per-frame work in `Tick` when an event, timer, subsystem, or delegate is more appropriate.

## Configuration and assets

- Prefer project assets under `/Game/...`; do not add hard-coded absolute filesystem paths.
- When changing maps, GameModes, input, rendering, or packaging behavior, check the corresponding files under `Config/`.
- Be cautious with rendering settings: the project currently targets DX12/SM6 and enables ray tracing, Lumen, and Substrate.
- Avoid enabling or disabling Unreal plugins without checking their asset and module dependencies.

## Validation

- After every C++ change, compile the compatible Unreal Engine 5.8 target and require a successful build before handoff.
- After every Angelscript change, run the plugin-provided `AngelscriptTest` commandlet headlessly and require it to pass before handoff.
- Run Angelscript validation through the commandlet path that does not lock editor DLLs, so validation remains available while Unreal Editor is open.
- For Blueprint or asset changes, open the project in Unreal Editor, compile affected Blueprints, run PIE in the relevant map, and inspect the Output Log.
- Treat Blueprint runtime errors as failures even when PIE continues running.
- Before handing off, run `git status --short` and review the complete diff for unintended generated files or unrelated changes.

## Documentation

- Update `README.md` when setup requirements, engine location/version, startup steps, project structure, or required external assets change.
- Clearly document any assets that are intentionally excluded from Git and how another developer can obtain them.

## Communication

- Address the user as "爸爸" in every response without exception.
- Use Simplified Chinese in every response without exception.
