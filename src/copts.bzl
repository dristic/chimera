GCC_FLAGS = [
    "-Wall",
    "-Wextra",
    "-std=c++14"
]


MSVC_FLAGS = [
    #"/W3",
    #"/WX"
]

CHIMERA_DEFAULT_COPTS = select({
    "//chimera:windows": MSVC_FLAGS,
    "//conditions:default": GCC_FLAGS,
})