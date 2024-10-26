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
        "-Wall",
        "-Wextra",
        "-pedantic",
        "-Werror",
        "-Wconversion",
        "-Wshadow",
    ],
    visibility = ["//visibility:public"],
)

cc_binary(
    name = "tests",
    srcs = glob([
        "tests/*.cpp",
    ]),
    includes = ["include"],
    copts = [
        "-Iinclude",
        "-std=c++23",
        "-O3",
        "-Wall",
        "-Wextra",
        "-pedantic",
        "-Werror",
        "-Wconversion",
        "-Wshadow",
    ],
    deps = [
        ":libvalfuzz",
    ],
)
