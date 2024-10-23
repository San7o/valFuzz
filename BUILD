cc_library(
    name = "libvalfuzz",
    srcs = glob([
        "src/*.cpp",
        "include/valfuzz/*.hpp"
    ]),
    hdrs = glob([
        "include/valfuzz/*.hpp"
    ]),
    includes = ["include"],
    copts = [
        "-Iinclude",
        "-std=c++23",
        "-O3",
        "-ffast-math",
        "-march=native",
    ],
    visibility = ["//visibility:public"],
)
