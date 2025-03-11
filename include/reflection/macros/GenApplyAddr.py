import sys

N = int(sys.argv[1]) if len(sys.argv) > 1 else 64

with open("ApplyAddr.h", "w", encoding="utf-8") as f:
    f.write("#pragma once\n\n")
    f.write("#define ADDR_OF(x) &(x)\n\n")

    for i in range(1, N + 1):
        params = ", ".join(f"x{i}" for i in range(1, i + 1))
        expands = ", ".join(f"ADDR_OF(x{i})" for i in range(1, i + 1))
        f.write(f"#define APPLY_ADDR_{i}({params}) {expands}\n")

    param_list = ", ".join(f"_{i}" for i in range(1, N + 1))
    macro_list = ", ".join(f"APPLY_ADDR_{i}" for i in range(N, 0, -1))
    f.write(f"#define GET_MACRO({param_list}, NAME, ...) NAME\n")

    f.write(
        "#define APPLY_ADDR(...) GET_MACRO(__VA_ARGS__, "
        + macro_list
        + ")(__VA_ARGS__)\n\n"
    )

print(f"生成 ApplyAddr.h 成功，支持 1 ~ {N} 个参数！")
