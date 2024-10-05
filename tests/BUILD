cc_binary(
    name = "valfuzz_tests",
    srcs = [
        "asserts_test.cpp",
        "before_after_test.cpp",
        "benchmark_test.cpp",
        "generate_random_test.cpp",
        "simple_fuzz.cpp",
    ],
    includes = ["//include"],
    copts = ["-Iinclude -std=c++23"],
    visibility = ["//visibility:public"],
    deps = ["//src:libvalfuzz"]
)
