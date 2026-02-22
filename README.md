# SMP Wind

A Skyrim mod that adds wind physics to HDT-SMP.

## Build Instructions And Requirements

- [CMake](https://cmake.org/)
  - Add this to your `PATH`
- [PowerShell](https://github.com/PowerShell/PowerShell/releases/latest)
- [Vcpkg](https://github.com/microsoft/vcpkg)
  - Add the environment variable `VCPKG_ROOT` with the value as the path to the folder containing vcpkg
- [Visual Studio Community 2019](https://visualstudio.microsoft.com/)
  - Desktop development with C++
- [CommonLibSSENG](https://github.com/alandtse/CommonLibVR/tree/ng)
  - Add this as as an environment variable `CommonLibSSEPath`

## User Requirements

- [Address Library for SKSE](https://www.nexusmods.com/skyrimspecialedition/mods/32444)
  - Needed for SSE/AE
- [VR Address Library for SKSEVR](https://www.nexusmods.com/skyrimspecialedition/mods/58101)
  - Needed for VR

## Register Visual Studio as a Generator

- Open `x64 Native Tools Command Prompt`
- Run `cmake`
- Close the cmd window

## Building
```
# clone latest git
git clone -b test https://github.com/Backporter/smp-wind.git

# go into the directory
cd smp-wind

# init/update submodules 
git submodule update --init --recursive

# build target
cmake --preset <Target>

# build project
cmake --build build --config <Mode>
```

## Build Targets
vs2022-windows-nocuda<br>
vs2022-windows-nocuda-avx<br>
vs2022-windows-nocuda-avx2<br>
vs2022-windows-nocuda-avx51<br>

## Build Modes
Debug<br>
Release<br>
