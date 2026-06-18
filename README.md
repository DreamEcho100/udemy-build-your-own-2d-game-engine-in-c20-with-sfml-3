# Build Your Own 2D Game Engine with C++20 and SFML 3

This repository is a workspace containing multiple independent CMake projects.
Every project lives under `projects/` and can be configured, built, run,
debugged, and cleaned without affecting the other projects.

```text
.
├── projects/
│   └── 0-starter/
│       ├── CMakeLists.txt
│       ├── CMakePresets.json
│       ├── cmake/
│       └── src/
├── tools/
├── .nvim/
└── .vscode/
```

## Requirements

- CMake 3.22 or newer
- Ninja
- Git
- A C++20 compiler
- Python 3 for workspace management
- clangd for editor language support
- GDB and/or CodeLLDB for debugging

SFML also requires platform development libraries. On Ubuntu:

```bash
sudo apt update
sudo apt install \
  libxrandr-dev libxcursor-dev libxi-dev libudev-dev \
  libfreetype-dev libflac-dev libvorbis-dev \
  libgl1-mesa-dev libegl1-mesa-dev libharfbuzz-dev
```

## CMake concepts used here

CMake does not compile the C++ files itself. It reads `CMakeLists.txt` and
generates files for a build tool. This workspace uses Ninja as that build tool.

- **Source directory:** The project directory containing `CMakeLists.txt`.
- **Build directory:** Generated files, dependencies, object files, and
  executables. Each project uses `build/debug` and `build/release`.
- **Configure:** Read CMake files, download/configure SFML, and generate the
  Ninja build files.
- **Build:** Ask Ninja to compile and link the configured project.
- **Target:** A buildable item such as the `HelloSFML` executable.
- **Preset:** A named group of CMake settings. Each project defines `debug` and
  `release` presets in `CMakePresets.json`.
- **Cache:** Saved configuration values in `CMakeCache.txt`. Reconfiguring
  normally reuses them; `--fresh` discards them.

The configure and build steps are separate:

```text
CMakeLists.txt --configure--> build/debug/build.ninja
source files  --build-------> build/debug/bin/<executable>
```

## Build and run one project

The simplest workflow is to enter the project directory first. For
`0-starter`:

```bash
cd projects/0-starter
```

### Configure a Debug build

Run this after cloning, after deleting the build directory, or after changing
important CMake settings:

```bash
cmake --preset debug
```

This creates `build/debug`, downloads SFML 3.1.0 when necessary, and generates
`compile_commands.json` for clangd.

### Build

```bash
cmake --build --preset debug
```

Build in parallel:

```bash
cmake --build --preset debug --parallel
```

Build only one target:

```bash
cmake --build --preset debug --target HelloSFML
```

### Run

CMake builds the executable but does not automatically run it.

Linux and macOS:

```bash
./build/debug/bin/HelloSFML
```

Windows PowerShell:

```powershell
.\build\debug\bin\HelloSFML.exe
```

### Build and run

Linux and macOS:

```bash
cmake --build --preset debug --parallel && ./build/debug/bin/HelloSFML
```

Windows PowerShell:

```powershell
cmake --build --preset debug --parallel
if ($LASTEXITCODE -eq 0) { .\build\debug\bin\HelloSFML.exe }
```

The same commands work for every directory under `projects/`. Change into that
project and replace `HelloSFML` with its executable target name.

## Build from the repository root

You do not have to change directories. Configure by providing the project
source directory:

```bash
cmake -S projects/0-starter --preset debug
```

Build by providing the generated build directory:

```bash
cmake --build projects/0-starter/build/debug --parallel
```

Run:

```bash
./projects/0-starter/build/debug/bin/HelloSFML
```

Build and run:

```bash
cmake --build projects/0-starter/build/debug --parallel \
  && ./projects/0-starter/build/debug/bin/HelloSFML
```

## Release builds

A Release build enables compiler optimization and normally omits most debugging
information:

```bash
cd projects/0-starter
cmake --preset release
cmake --build --preset release --parallel
./build/release/bin/HelloSFML
```

Debug and Release use separate directories, so they can exist at the same time.

## Useful CMake commands

Run these from an individual project directory unless stated otherwise.

### Show available presets

```bash
cmake --list-presets=all
```

### Reconfigure from a clean cache

This retains downloaded/build files where CMake can reuse them, but recreates
the configuration cache:

```bash
cmake --preset debug --fresh
```

### Show build commands

Use this when diagnosing compiler flags, include directories, or linker
commands:

```bash
cmake --build --preset debug --verbose
```

### Show available targets

```bash
cmake --build --preset debug --target help
```

### Clean compiled outputs

This runs Ninja's clean target but retains the CMake cache and fetched
dependencies:

```bash
cmake --build --preset debug --target clean
```

### Inspect cached values

```bash
cmake -LAH -N build/debug
```

### Set a CMake cache variable

Variables passed with `-D` override or add configuration values:

```bash
cmake --preset debug -DMY_OPTION=ON
```

The variable must be supported by the project or one of its dependencies.

### Remove the complete generated build

Use the workspace cleanup command when you want to recover disk space,
including fetched SFML source and compiled libraries:

```bash
python3 tools/workspace.py clean 0-starter
```

Clean every project:

```bash
python3 tools/workspace.py clean --all
```

Without a project argument, cleanup displays an interactive project picker:

```bash
python3 tools/workspace.py clean
```

## Manage independent projects

### Recommended: generate a project

Create a self-contained project under `projects/`:

```bash
python3 tools/workspace.py new
```

The command asks for:

- **Project directory:** The folder under `projects/`, such as `1-window`.
- **CMake project name:** A C/C++ identifier used by `project(...)`, such as
  `WindowExample`.
- **Executable target:** The name passed to `add_executable`, often the same as
  the CMake project name.

You can also provide all names non-interactively:

```bash
python3 tools/workspace.py new 1-window \
  --name WindowExample \
  --target WindowExample
```

If you manually add or remove a project, synchronize VS Code:

```bash
python3 tools/workspace.py sync
```

A directory is recognized as a project when it is an immediate child of
`projects/` and contains both `CMakeLists.txt` and `CMakePresets.json`.

The generator also runs `sync`, so a generated project is registered with VS
Code immediately.

### Manually copy the starter

If you want to study every setup step instead of using the generator:

```bash
cp -R .templates/sfml-project projects/1-window
```

Then edit `projects/1-window/CMakeLists.txt`:

1. Replace `__PROJECT_NAME__` with a CMake identifier such as
   `WindowExample`.
2. Replace `__TARGET_NAME__` with the executable target, such as
   `WindowExample`.
3. Update the window title and code under `src/`.
4. Run `python3 tools/workspace.py sync`.
5. Configure the new project with `cmake --preset debug`.

The template contains no build output. Do not copy another project's `build/`
directory or `compile_commands.json`; CMake stores absolute paths in those
generated files.

### Recommended project layout

Small tutorial projects can remain minimal:

```text
projects/1-window/
├── CMakeLists.txt
├── CMakePresets.json
├── cmake/
│   └── DevTools.cmake
├── src/
│   └── main.cpp
└── assets/                 # optional
```

As a project grows:

```text
projects/5-ecs/
├── include/                # public project headers
├── src/
│   ├── main.cpp
│   ├── engine/
│   └── game/
├── assets/
├── tests/                  # add when there is testable non-window logic
├── CMakeLists.txt
└── CMakePresets.json
```

Prefer keeping reusable logic outside `main.cpp`. A thin `main` function makes
the code easier to test and debug.

### Adding source files

The starter uses `GLOB_RECURSE ... CONFIGURE_DEPENDS`, so new `.cpp`, `.cc`,
and `.cxx` files under `src/` are discovered when CMake checks the build.

```bash
touch src/player.cpp
cmake --build --preset debug
```

Header files do not need to be listed for compilation. If you add an
`include/` directory, expose it to the target:

```cmake
target_include_directories(HelloSFML PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include")
```

For larger production projects, explicit source lists are more predictable than
globbing. The glob is retained here because it keeps small learning projects
simple.

### Adding SFML modules

The starter links Graphics, Window, and System:

```cmake
target_link_libraries(
  HelloSFML
  PRIVATE
    SFML::Graphics
    SFML::Window
    SFML::System
)
```

Audio and Network are disabled to reduce build time and disk usage. To use
them, enable the module before `FetchContent_MakeAvailable(SFML)`:

```cmake
set(SFML_BUILD_AUDIO ON CACHE BOOL "Build SFML Audio" FORCE)
set(SFML_BUILD_NETWORK ON CACHE BOOL "Build SFML Network" FORCE)
```

Then link what the target uses:

```cmake
target_link_libraries(HelloSFML PRIVATE SFML::Audio SFML::Network)
```

Reconfigure after changing dependency options:

```bash
cmake --preset debug --fresh
cmake --build --preset debug --parallel
```

### Assets and the working directory

VS Code and Neovim launch each target with the individual project directory as
the working directory. Running the executable after `cd projects/<project>`
uses the same directory. Relative paths such as `assets/player.png` therefore
resolve to the project's `assets/` directory in all three workflows.

When packaging the game, or if you deliberately run it from
`build/debug/bin`, copy assets beside the executable:

```cmake
add_custom_command(
  TARGET HelloSFML
  POST_BUILD
  COMMAND
    "${CMAKE_COMMAND}" -E copy_directory
    "${CMAKE_CURRENT_SOURCE_DIR}/assets"
    "$<TARGET_FILE_DIR:HelloSFML>/assets"
  COMMENT "Copying game assets"
)
```

The post-build copy is optional during normal workspace development because the
shared debugger configurations already use the project directory.

### Renaming a project directory

CMake caches absolute paths. After renaming, for example from `0-basics` to
`0-starter`, remove the generated build state and resynchronize:

```bash
python3 tools/workspace.py clean 0-starter --yes
python3 tools/workspace.py sync

cd projects/0-starter
cmake --preset debug --fresh
cmake --build --preset debug --parallel
```

If the cleanup tool cannot recognize the renamed directory, delete only that
project's generated `build/` directory and `compile_commands.json`, then run
`sync`.

## How CMakeLists.txt is structured

Each project is self-contained and follows the same basic sequence:

1. Declare the project and C++ language.
2. Enable local development helpers.
3. Fetch the pinned SFML version.
4. Discover C++ source files under `src/`.
5. Create one or more executable targets with `add_executable`.
6. Link the required SFML targets with `target_link_libraries`.
7. Require C++20 and enable compiler warnings.

The starter project places every `src/*.cpp` file in its first executable. If a
project later needs multiple executables, replace the broad source glob and
`add_executable` call with target-specific source lists:

```cmake
file(
  GLOB_RECURSE HELLO_SFML_SOURCES
  CONFIGURE_DEPENDS
  "${CMAKE_CURRENT_SOURCE_DIR}/src/hello-sfml/*.cpp"
)

file(
  GLOB_RECURSE SECOND_EXAMPLE_SOURCES
  CONFIGURE_DEPENDS
  "${CMAKE_CURRENT_SOURCE_DIR}/src/second-example/*.cpp"
)

add_executable(HelloSFML ${HELLO_SFML_SOURCES})
add_executable(SecondExample ${SECOND_EXAMPLE_SOURCES})
target_link_libraries(SecondExample PRIVATE SFML::Graphics)
target_compile_features(SecondExample PRIVATE cxx_std_20)
enable_project_warnings(SecondExample)
```

Each target directory should contain only one `main` function.

You can then build it specifically:

```bash
cmake --build --preset debug --target SecondExample
```

## clangd and `compile_commands.json`

The Debug configure exports the exact compiler command for every source file to
`build/debug/compile_commands.json`. It also creates
`compile_commands.json` in the project directory, normally as a symbolic link.

clangd uses this database to discover:

- SFML headers
- C++20 mode
- preprocessor definitions
- warning flags
- project include directories

If clangd still shows stale errors after configuring, restart the language
server in VS Code or run `:LspRestart` in Neovim.

## How debugging is organized

There are three separate concepts:

- **Project:** A directory such as `projects/0-starter`.
- **CMake target:** An executable such as `HelloSFML`.
- **Debugger configuration:** How that executable is launched: debugger,
  arguments, environment variables, and working directory.

You normally do not add a debugger configuration for every new project. The
shared configurations in `.vscode/launch.json` ask CMake for the active
executable path, so they work with all projects and all executable targets.

Use a Debug build when setting breakpoints:

```bash
cmake --preset debug
cmake --build --preset debug --parallel
```

The Debug preset includes source-level debug information. Release builds are
optimized, so lines may be reordered or variables may be unavailable.

### Custom debugger arguments or environment

For a target that needs command-line arguments or environment variables,
duplicate the CodeLLDB entry in `.vscode/launch.json` and keep the CMake target
inputs:

```json
{
  "name": "1-window: CodeLLDB with diagnostics",
  "type": "lldb",
  "request": "launch",
  "program": "${input:cmakeTargetPath}",
  "cwd": "${input:cmakeProjectDirectory}",
  "args": ["--show-fps"],
  "env": {
    "GAME_LOG_LEVEL": "debug"
  },
  "preLaunchTask": "CMake: Prepare active Debug"
}
```

The Neovim bridge reads `lldb` entries from the same file, changes the adapter
to its installed `codelldb`, resolves the nearest CMake target, and preserves
fields such as `args` and `env`. Name custom configurations with the intended
project so they are easy to select.

## VS Code

Always open the repository root rather than an individual project:

```bash
code .
```

Install the recommended extensions when VS Code prompts:

- clangd
- CMake Tools
- Microsoft C/C++ Debugger
- CodeLLDB

### Add or discover a project

When a project is generated, `workspace.py new` updates
`.vscode/settings.json`. For a manually copied or renamed project:

```bash
python3 tools/workspace.py sync
```

If VS Code was already open, reload the window if the CMake project list does
not update.

### Configure and build

Use the CMake Tools status bar or Command Palette:

1. Run **CMake: Select Active Folder** and choose the project.
2. Run **CMake: Select Configure Preset** and choose `debug`.
3. Run **CMake: Configure**.
4. Run **CMake: Set Build Target** if the project has multiple targets.
5. Build with `Ctrl+Shift+B` or **CMake: Build**.

The root tasks provide:

- Debug and Release configuration
- Debug and Release builds
- creating projects
- synchronizing project discovery
- cleaning generated files

Run them through **Tasks: Run Task**.

### Debug

Before debugging:

1. Select the active CMake project.
2. Configure it with the `debug` preset.
3. Run **CMake: Set Debug Target** and choose the executable.
4. Add a breakpoint in project source code.

The Run and Debug panel provides:

- `CMake: Debug active target (GDB)`
- `CMake: Debug active target (CodeLLDB)`

Both configurations prepare a Debug build before launching the active CMake
target.

If VS Code launches the wrong executable, the selected CMake folder or debug
target is stale. Select both again; do not hardcode an executable path into the
shared debugger configurations.

## Neovim 0.12

Open Neovim at the repository root:

```bash
nvim .
```

The existing nvim-dap setup loads `.nvim/dap.lua`, which reads CodeLLDB
configurations from `.vscode/launch.json`.

Open a source file inside the project you want to debug, add a breakpoint with
your nvim-dap mapping, and press `F5`. The workspace bridge:

1. Finds the nearest CMake project.
2. Configures its Debug preset.
3. Reads executable targets from the CMake File API.
4. Prompts for a target.
5. Builds and launches it with codelldb.

Unlike VS Code, Neovim infers the active project from the current source file.
If the wrong project is selected, first open a source file from the intended
project.

For projects with multiple executable targets, Neovim asks which one to build
on every new debug session. No hardcoded target path is required.

Useful checks:

```vim
:LspInfo
:LspRestart
:DapNew
```

If `F5` only shows the global “Launch file” prompt, confirm that Neovim was
opened at the repository root and that the repository-local `.nvim/dap.lua`
was loaded by your dotfiles.

The repository-local bridge does not modify the external Neovim dotfiles.

## Suggested learning workflow

For each lesson or engine subsystem:

1. Generate a new independent project or copy the previous completed project.
2. Rename its CMake project and target before writing new code.
3. Configure a Debug build immediately so clangd has a compilation database.
4. Make one small change at a time.
5. Build frequently and treat compiler warnings as defects.
6. Run from the terminal before debugging editor integration.
7. Add a breakpoint and inspect call stacks, locals, and object state.
8. Keep reusable logic outside `main.cpp`.
9. Commit a working checkpoint before beginning the next concept.
10. Clean old project builds when disk usage grows.

Good chapter boundaries are observable capabilities: opening a window, input,
textures, animation, collision, scene management, entities/components, and
rendering. Keeping each project runnable makes it easier to compare approaches
and diagnose regressions.

## Common problems

### `SFML/Graphics.hpp` not found

Configure the Debug preset, verify the project-level `compile_commands.json`
exists, then restart clangd:

```bash
cd projects/<project>
cmake --preset debug
```

### CMake reports that the source directory changed

The project was moved or renamed while a build cache existed. Remove that
project's generated files and configure again:

```bash
python3 tools/workspace.py clean <project> --yes
python3 tools/workspace.py sync
```

### The new project is missing in VS Code

```bash
python3 tools/workspace.py sync
```

Then reload VS Code and select the new active CMake folder.

### The debugger starts the wrong program

- VS Code: select the active folder and debug target again.
- Neovim: open a source file in the intended project before pressing `F5`.

### Linker error about multiple definitions of `main`

More than one source file with a `main` function was added to one executable
target. Split those source files into separate CMake targets.

### Assets cannot be found

Confirm that `assets/` exists in the active project and that the project is the
selected VS Code folder or nearest Neovim project. If running directly from the
binary output directory, add the post-build asset copy command shown above.

### First configuration fails to download SFML

The first configuration needs Git and internet access. Retry after checking the
network:

```bash
cmake --preset debug --fresh
```

### Disk usage is growing

Every independent project owns an SFML source and build tree. Clean projects
that you are not currently studying:

```bash
python3 tools/workspace.py clean
```
