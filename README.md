Cosmonaut
=========

The Cosmonaut UI is a high performance user interface library for C++ projects. It is targeting projects that utilize the GPU such as simulations and games. The aim is to provide a framework that is easy to use and integrate yet delivers the high quality and performance that these applications expect.

Project Goals:
- Fast execution speed and rendering performance
- Straightforward C/C++ API that can be used with/without bindings
- Ability to create highly polished UIs
- Minimum amount of third-party library dependencies
- Designed to embed in an existing application

Non Goals:
- No support for high-level UI features like pages or routes
- Not designed to be a standalone application

Specifically, the API mimics the one provided from web browsers such as Chromium. The goal is to provide an easy way to create a highly polished user interface that targets a much smaller memory footprint than the one that Chromium provides. Also, by backing on C++, it makes it easier to create a well memory managed UI that performs for high fidelity interfaces.

Getting Started
===============

To get started you have to build the project using CMake so make sure that is available on your system. The project requires Freetype and Harfbuzz to be compiled with the project. The following steps will be for building the example but if you just want to build the library you can skip to the next section.

You will also have to make and install the following dependencies using `brew install freetype harfbuzz` or other means:
- Freetype 2
- Harfbuzz

After cloning the project you will have to initialize the submodules in order to build GLFW with the example:
```
git clone <project-url>
git submodule init
git submodule update
```

After the submodules have been initialized you can do an out-of-source build:

```
mkdir build
cd build
cmake .. -DNOVA_BUILD_EXAMPLES=true
make
```

This should install the dependencies using conan, build GLFW and dependencies with the project, and output the example binary. You can then run the output using `./OpenGL3`.

Building the Library
====================

Building the library without the example is straightforward. Simply do an out of source build:

```
mkdir build
cd build
cmake ..
make
```

This should output the library file for your system.
