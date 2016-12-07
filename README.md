Nova UI
=======

The Nova UI is a high performance user interface library for C++ projects. It is targeting projects that utilize the GPU such as simulations and games. The aim is to provide a framework that is easy to integrate yet delivers the high quality and performance that these applications expect.

Project Goals:
- Fast execution speed and rendering performance
- Straightforward C/C++ API that can be used with/without bindings
- Minimum amount of third-party library dependencies
- Ability to create highly polished UIs
- Designed to embed in an existing application

Non Goals:
- No support for high-level UI features like pages or routes
- Not designed to be a standalone application

Specifically, the API mimics the one provided from web browsers such as Chromium. The goal is to provide an easy way to create a highly polished user interface that targets a much smaller memory footprint than the one that Chromium provides. Also, by backing on C++, it makes it easier to create a well memory managed UI.

Getting Started
===============
To start off you will have to include this project as a dependency to your C++ application. This can be done easily through CMake using `add_subdirectory` or manually through the build system of your choice.


