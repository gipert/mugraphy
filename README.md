# mugraphy

Cosmic rays propagation through large structures.

![CI](https://github.com/gipert/mugraphy/workflows/CI/badge.svg)

🚧 Work in progress...

<p></p>

### Prerequisites
* [CMake]() (≥ v3.8) — Build-only dependency
* [Geant4](https://geant4.web.cern.ch) (≥ v10.5) — with:
    * `GEANT4_INSTALL_DATA=ON`
    * `GEANT4_USE_HDF5=ON` [optional]
    * `GEANT4_USE_USOLIDS=ON` [optional], requires VecGeom library
    * `GEANT4_USE_GDML=ON` [optional]
    * `GEANT4_BUILD_MULTITHREADED=ON` [optional]

### Optional dependencies
* [VecGeom](https://gitlab.cern.ch/VecGeom/VecGeom), for vectorized geometry
  (Geant4 must be compiled with `GEANT4_USE_USOLIDS=ON` too)

### Installing the project
To build and install the project, first clone the repository:
```console
$ git clone git@github.com:gipert/mugraphy.git
```
Create a build directory, and run CMake from there:
```console
$ mkdir -p build && cd build
$ cmake <path-to-mugraphy-source> -DCMAKE_INSTALL_PREFIX=<your-custom-install-location>
```
The default build type is set to `RelWithDebInfo`, if you need something
different you can customize it by setting the
[`CMAKE_BUILD_TYPE`](https://cmake.org/cmake/help/latest/variable/CMAKE_BUILD_TYPE.html)
variable. Finally build and install the project (in `/usr/local` by default):
```console
$ cmake --build . --target install
```
and you're done!
