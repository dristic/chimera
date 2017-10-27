GCC_FLAGS = [
    "-Wall",
    "-Wextra",
    "-std=c++14"
]

MSVC_FLAGS = [
    #"/W3",
    #"/WX"
]

WINDOWS_DEFINES = [
    "WIN32"
]

CHIMERA_DEFAULT_COPTS = select({
    "//chimera:windows": MSVC_FLAGS,
    "//conditions:default": GCC_FLAGS,
})

CHIMERA_DEFAULT_DEFINES = select({
    "//chimera:windows": WINDOWS_DEFINES,
    "//conditions:default": []
})