import sys

N = int(sys.argv[1]) if len(sys.argv) > 1 else 128

with open("./GEN_REFLECT_GET_FIELD_NAMES.inl", "w", encoding="utf-8") as f:
    for i in range(1, N + 1):
        params = ", ".join(f"i{i}" for i in range(1, i + 1))
        f.write(f"GEN_REFLECT_GET_FIELD_NAMES({i}, {params})\n")

print(f"生成 GEN_REFLECT_GET_FIELD_NAMES.inl 成功，支持 1 ~ {N} 个参数！")
