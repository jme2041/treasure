# Treasure's Demonstration Library

Treasure's Demonstration Library (`treasure.h`) is a small header-only library
used to demonstrate building and testing a header-only library using Visual
Studio and CMake.

This repository demonstrates a header-only library that depends on an external
header-only library that is managed via `vcpkg`. For this example, the
dependency is the
[Windows Implementation Library](https://github.com/microsoft/wil) (WIL). This
was motivated by the situation in which a header-only library is being
developed that depends on software obtained via `vcpkg`, which is in turn used
by clients that also depend on software obtained via `vcpkg`.

In this case, it is generally recommended to distribute the library via
`vcpkg`, so that `vcpkg` can manage both the library and client dependencies
when building the client. In this example, Treasure's Demonstration Library
is distributed via a custom `vcpkg` repository.

A companion project,
[Maggie's Demonstration Library](https://github.com/jme2041/maggie),
demonstrates distributing a compiled library via `vcpkg`.

# Getting Started

Requirements:

 - [Microsoft Windows](https://www.microsoft.com/en-us/windows): The library
   is developed and tested on x86-64 versions of Windows 11.
 - [Visual Studio](https://visualstudio.microsoft.com/): The "Desktop
   development with C++" workload and "C++ CMake tools for Windows" optional
   package are required. If building with Clang, the "C++ Clang tools for
   Windows" optional package is also required. The library is developed and
   tested using Visual Studio 2026.
 - [CMake](https://cmake.org/): Version 3.31 or newer. If building in Visual
   Studio, the CMake that is bundled with Visual Studio is sufficient. If
   building via the command prompt, a standalone CMake installation is also
   needed. The library is developed and tested using CMake 3.31, 4.1, and 4.2.
 - [vcpkg](https://github.com/microsoft/vcpkg): The library is developed and
   tested on the 2025.12.12 and newer releases.

# Developers

To obtain the source code, use:

```cmd
git clone https://github.com/jme2041/treasure
```

The use of CMake presets for configuration, building, and testing is
recommended. The following CMake presets are available in
[CMakePresets.json](CMakePresets.json). All presets target the x86-64
architecture and enable tests. The presets differ by build type and compiler.

| Preset              | Build Type | Compiler       |
| ------------------- | ---------- | -------------- |
| `x64-Debug-MSVC`    | Debug      | `cl.exe`       |
| `x64-Debug-Clang`   | Debug      | `clang-cl.exe` |
| `x64-Release-MSVC`  | Release    | `cl.exe`       |
| `x64-Release-Clang` | Release    | `clang-cl.exe` |

Here is an example of how to configure, build, and run the tests that include
the header-only library. Run the following from an architecture-specific (i.e.,
x64 for these examples) Visual Studio developer command prompt from the
project's root directory (i.e., the directory that contains the top-level
`CMakeLists.txt`). This example uses the `x64-Debug-MSVC` CMake preset.

```
cmake --preset=x64-Debug-MSVC
cmake --build --preset=x64-Debug-MSVC
ctest --preset=x64-Debug-MSVC
```

To build, develop, and test using the Visual Studio IDE, use the option to
open a local folder and select the directory that contains the top-level
`CMakeLists.txt`.

# Consumers

Treasure's Demonstration Library is available through a custom vcpkg registry
at https://github.com/jme2041/vcpkg-registry. `treasure` is distributed via
commit hash `d7f56ffb9601421324da5a00683594c07c3a32b6` or newer.

To build a CMake C or C++ project that depends on `treasure`, run
`vcpkg new --application` in the directory containing the project's top-level
`CMakeLists.txt` file. This will create two files, `vcpkg-configuration.json`
and `vcpkg.json`. Add the custom `vcpkg` registry to `vcpkg-configuration.json`
so that it looks like this:

```json
{
  "default-registry": {
    "kind": "git",
    "baseline": "000...",
    "repository": "https://github.com/microsoft/vcpkg",
    "reference": "2025.12.12"
  },
  "registries": [
    {
      "kind": "git",
      "repository": "https://github.com/jme2041/vcpkg-registry",
      "reference": "main",
      "baseline": "000...",
      "packages": [
        "treasure"
      ]
    }
  ]
}
```

The `default-registry` entry is always needed (this is where `treasure` obtains
`wil` and the client obtains other dependencies). The `"000..."` in the
baseline field will be filled in automatically by `vcpkg new --application`,
but it can be adjusted manually by editing the file. The `reference` can also
be adjusted as needed.

The `registries` array contains entries for custom registries used by the
project. Add the entry for `jme2041/vcpkg-registry`, replacing `"000..."` in
the baseline field with the earliest commit hash of `jme2041/vcpkg-registry` to
be used by the project. To use `treasure`, this must be the `d7f56...` hash
listed above or newer. List the packages for which to use the custom registry
in the `packages` array.

Next, run `vcpkg add port treasure`. This will add an entry for `treasure` to
`vcpkg.json`:

```
{
  "dependencies": [
    "treasure"
  ]
}
```

If a specific minimum version of `treasure` is needed, the entry for `treasure`
in `vcpkg.json` can be edited to contain a JSON object with the version
information:

```json
{
  "dependencies": [
    {
      "name": "treasure",
      "version>=": "0.1.0.0"
    }
  ]
}
```

Finally, add `find_package(treasure CONFIG REQUIRED)` to `CMakeLists.txt` and
define `CMAKE_TOOLCHAIN_FILE` to point to the `vcpkg` toolchain file.
From the command line, this is typically
`-DCMAKE_TOOLCHAIN_FILE="%VCPKG_ROOT%/scripts/buildsystems/vcpkg.cmake"`. To
specify whether to link to `treasure` as a shared or static library, set the
CMake configuration variable `VCPKG_TARGET_TRIPLET`. Common triplets include
`x64-windows`, `x64-windows-static`, and `x64-windows-static-md`.

Then, in `CMakeLists.txt`, use `find_package(treasure CONFIG REQUIRED)` and
link to `treasure` using
`target_link_libraries(exe_target PRIVATE treasure::treasure)`.

[Amber's Demonstration Client](https://github.com/jme2041/amber) is a small
executable that uses `treasure`. It has CMake presets that configure CMake
appropriately and a `CMakeLists.txt` that links `treasure` to the executable.

# License

Treasure's Demonstration Library is released under the MIT License. For
details, see [LICENSE.txt](LICENSE.txt).

`WIL` is released under the MIT License and is used in accordance with that
license.
