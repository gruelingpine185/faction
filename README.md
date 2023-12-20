# Faction

Faction is an FPS game inspired by my appreciation for low poly graphics and voxel art. This is my dream game. It is being build from the ground up, using Vulkan for rendering. 

## Cloning

```sh
git clone --recurse-submodules https://github.com/gruelingpine185/faction 
cd faction
```

## Building

Faction provides 2 options for building: using pure Makefiles or Cmake.

### Using Makefiles

For the first time the project is built, GLFW will also have to be built. Run:

```sh
make glfw
make
```

and the resulting executable will be located in `./bin/`. For any subsequent build, assumming that GLFW source hasn't been modified,
run:

```sh
make
```

and the resulting executable will be located in `./bin/`.

### Using Cmake

Cmake using Ninja:

```cmake
cmake .. -DCMAKE_C_COMPILER=clang -DCMAKE_BUILD_TYPE="Release" -G "Ninja"
Ninja
```

or Cmake using Makefiles:

```cmake
cmake .. -DCMAKE_C_COMPILER=clang -DCMAKE_BUILD_TYPE="Release" -G "Unix Makefiles"
Make
```

## Contributing

Contributions are welcomed and appreciated.
