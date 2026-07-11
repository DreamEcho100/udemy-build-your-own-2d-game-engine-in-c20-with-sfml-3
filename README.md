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

For a project that loads textures, fonts, sounds, or other runtime files, add
the optional assets directory and CMake copy step during generation:

```bash
python3 tools/workspace.py new 5-textures \
  --name Textures \
  --target Textures \
  --with-assets-dir
```

This creates `assets/.gitkeep` and configures every successful target link to
copy `assets/` into the executable directory.

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

`:CMakeRun` and direct binary execution use `build/debug/bin` as the working
directory. Generate asset-using projects with `--with-assets-dir`, or add the
equivalent post-build copy command manually:

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

The shared debugger configuration uses the project directory, while CMakeTools
uses the executable directory. Keeping the source `assets/` directory and its
post-build copy supports both workflows.

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

Each independent project owns two portable editor files:

- `.vscode/tasks.json` defines configure, build, and prepare-Debug tasks.
- `.vscode/launch.json` defines build-and-debug and debug-existing launches.

VS Code reads these files directly. Neovim reads the same files through
nvim-dap and Overseer. There are no project-local `.nvim` files.

The Debug preset includes source-level debug information. Release builds are
optimized, so lines may be reordered and variables may be unavailable.

### Build, run, and debug actions

| Action | Neovim | Result |
| --- | --- | --- |
| Build only | `<leader>mcmb` | Runs the active CMakeTools build; no program or debugger starts. |
| Build and debug | `<F5>` → `CMake: Build and Debug` | Runs the generated prepare task, then launches CodeLLDB. |
| Native CMake debug | `:CMakeDebug` | Builds and debugs the selected CMakeTools launch target. |
| Debug existing | `<F5>` → `CMake: Debug Existing` | Launches the existing Debug binary without building. |
| Run normally | `<leader>mcmr` | Runs through CMakeTools without DAP. |

The portable launch files use VS Code's `type = "lldb"`. Global Neovim
configuration aliases that type to its Mason-installed `codelldb` adapter.
nvim-dap natively expands `${workspaceFolder}` and reads `.vscode/launch.json`
from Neovim's current working directory.

Before using F5 or native CMakeTools commands, run `:cd projects/<project>`
or start Neovim from that project directory. Then select/configure its CMake
target as usual.

### Breakpoints, exceptions, and crashes

Useful nvim-dap mappings are:

- `<leader>daptb`: toggle a normal source-line breakpoint.
- `<leader>dapb`: create a conditional breakpoint.
- `<F5>`: start, continue, or resume execution.
- `<F1>`, `<F2>`, `<F3>`: step into, over, and out.
- `<F7>`: toggle the debugger UI.
- `<leader>dapr`: open the debugger REPL.

CodeLLDB advertises C++ break-on-throw as a default exception filter. To
disable exception breakpoints for the current session, run:

```vim
:lua require("dap").set_exception_breakpoints({})
```

CodeLLDB is configured not to create synthetic disassembly buffers. When an
exception physically stops in `__cxa_throw`, `std::terminate`, or a signal
handler, nvim-dap therefore opens the first stack frame that has source code.
Runtime frames remain listed in the DAP stack panel, but source-less assembly
does not replace your source buffer.

An SFML window can be reported as unresponsive while the debugger is paused at
a breakpoint or exception. That is expected: its event loop is stopped. Return
to Neovim, inspect the stack and variables, then press `<F5>` to resume.

### Custom debugger arguments or environment

Edit that project's `.vscode/launch.json`. Keep the generated program and
working-directory values, then add `args` or `env`:

```json
{
  "name": "CMake: Build and Debug with diagnostics",
  "type": "lldb",
  "request": "launch",
  "program": "${workspaceFolder}/build/debug/bin/WindowExample",
  "cwd": "${workspaceFolder}",
  "args": ["--show-fps"],
  "env": {
    "GAME_LOG_LEVEL": "debug"
  },
  "preLaunchTask": "CMake: Prepare Debug"
}
```

## VS Code

You can open the repository root with `code .` or open one independent project.
The root workspace keeps the multi-project CMake Tools list; each project also
contains its own portable build and debug configurations.

Install clangd, CMake Tools, and CodeLLDB. In an individual project, use:

- **Tasks: Run Build Task** for the generated Debug build.
- **Run and Debug → CMake: Build and Debug** to build before debugging.
- **Run and Debug → CMake: Debug Existing** to skip the build.

When a project is generated, `workspace.py new` creates its editor metadata and
updates `.vscode/settings.json`. For manually copied or renamed projects, run:

```bash
python3 tools/workspace.py sync
```

The sync command creates missing project editor files but does not overwrite
existing customized `tasks.json` or `launch.json` files.

## Neovim 0.12

Start Neovim from the independent project you want to debug:

```bash
cd projects/4-shapes
nvim .
```

If Neovim is already open, use `:cd projects/4-shapes` before pressing F5.

A typical shared-launch session is:

1. Toggle a breakpoint with `<leader>daptb`.
2. Press `<F5>` and select `CMake: Build and Debug`.
3. Inspect locals, stack frames, and watches when execution pauses.
4. Press `<F5>` again to continue.

For the native CMakeTools path, use `:CMakeDebug` after selecting the project
and launch target.

Use `:OverseerToggle` or `<leader>tt` to inspect task output. Useful diagnostic
commands are `:LspInfo`, `:LspRestart`, `:DapNew`, and `:messages`.

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
- Neovim: run `:cd projects/<project>` before pressing F5.

### Linker error about multiple definitions of `main`

More than one source file with a `main` function was added to one executable
target. Split those source files into separate CMake targets.

### Assets cannot be found

Confirm that `assets/` exists in the active project and that the project is the
selected VS Code folder or current Neovim project. If running directly from the
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
