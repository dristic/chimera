cc_library(
    name = "gtest",
    srcs = glob(
        ["googletest/src/*.cc"],
        exclude = ["googletest/src/gtest-all.cc"]
    ),
    hdrs = glob([
        "googletest/include/**/*.h",
        "googletest/src/*.h"
    ]),
    includes = [
        "googletest/include",
        "googletest"
    ],
    linkopts = ["-pthread"],
    visibility = ["//visibility:public"],
)