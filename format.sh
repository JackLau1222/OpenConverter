#!/bin/bash
TOP_DIR=$PWD

# 定义需要处理的文件扩展名
EXTENSIONS=(".cpp" ".c" ".h" ".cc" ".hpp")

# 定义 clang-format 的格式化风格
# 修复 -style 参数，使用 JSON 格式
STYLE="{\"BasedOnStyle\": \"llvm\", \"IndentWidth\": -2}"

# 检查 clang-format 是否存在
if ! command -v clang-format &> /dev/null; then
    echo "Error: clang-format is not installed. Please install it and make sure it's in your PATH." >&2
    exit 1
fi

# 检查 clang-format 版本是否支持 JSON 风格
# 如果不支持，尝试使用基于 YAML 的风格定义
CLANG_VERSION=$(clang-format --version)
if ! printf "%s\n" "$CLANG_VERSION" | grep -q "based on LLVM"; then
    echo "Warning: clang-format version does not support JSON style. Falling back to YAML style."
    STYLE="BasedOnStyle: LLVM\nIndentWidth: 4"
fi

# 遍历同级目录及其子目录
echo "Scanning directory: $TOP_DIR"

# 构建文件扩展名的查找条件
FIND_QUERY=""
for ext in "${EXTENSIONS[@]}"; do
    FIND_QUERY+=" -o -name *$ext"
done
FIND_QUERY=${FIND_QUERY:4}  # 移除第一个 "-o"

# 查找所有符合扩展名的文件
FILES=$(find "$TOP_DIR" -type f \( $FIND_QUERY \))

if [ -z "$FILES" ]; then
    echo "No files found with supported extensions."
else
    echo "Files found:"
    echo "$FILES"
fi

# 执行格式化
echo "Formatting files..."
# 根据 clang-format 版本选择合适的风格
if printf "%s\n" "$CLANG_VERSION" | grep -q "based on LLVM"; then
    # 使用 JSON 风格
    eval "find \"$TOP_DIR\" -type f \( $FIND_QUERY \) -exec clang-format -style=\"$STYLE\" -i {} \;"
else
    # 使用 YAML 风格
    eval "find \"$TOP_DIR\" -type f \( $FIND_QUERY \) -exec clang-format -style=\"file\" -i {} \;"
fi

# 输出处理的文件名
printf "Formatted files:\n%s\n" "$FILES"

echo "Formatting completed."
