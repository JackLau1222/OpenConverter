#!/bin/bash
TOP_DIR=$PWD

# Define the file extensions to process
EXTENSIONS=(".cpp" ".c" ".h" ".cc" ".hpp")

# Define the clang-format style
# Fix the -style parameter to use JSON format
STYLE="{\"BasedOnStyle\": \"llvm\", \"IndentWidth\": -2}"

# Check if clang-format is installed
if ! command -v clang-format &> /dev/null; then
    echo "Error: clang-format is not installed. Please install it and make sure it's in your PATH." >&2
    exit 1
fi

# Check if the clang-format version supports JSON style
# If not supported, fall back to YAML style definition
CLANG_VERSION=$(clang-format --version)
if ! printf "%s\n" "$CLANG_VERSION" | grep -q "based on LLVM"; then
    echo "Warning: clang-format version does not support JSON style. Falling back to YAML style."
    STYLE="BasedOnStyle: LLVM\nIndentWidth: 4"
fi

# Traverse the current directory and its subdirectories
echo "Scanning directory: $TOP_DIR"

# Build the find query for file extensions
FIND_QUERY=""
for ext in "${EXTENSIONS[@]}"; do
    FIND_QUERY+=" -o -name *$ext"
done
FIND_QUERY=${FIND_QUERY:4}  # Remove the first "-o"

# Find all files with matching extensions
FILES=$(find "$TOP_DIR" -type f \( $FIND_QUERY \))

if [ -z "$FILES" ]; then
    echo "No files found with supported extensions."
else
    echo "Files found:"
    echo "$FILES"
fi

# Perform formatting
echo "Formatting files..."
# Choose the appropriate style based on clang-format version
if printf "%s\n" "$CLANG_VERSION" | grep -q "based on LLVM"; then
    # Use JSON style
    eval "find \"$TOP_DIR\" -type f \( $FIND_QUERY \) -exec clang-format -style=\"$STYLE\" -i {} \;"
else
    # Use YAML style
    eval "find \"$TOP_DIR\" -type f \( $FIND_QUERY \) -exec clang-format -style=\"file\" -i {} \;"
fi

# Output the formatted file names
printf "Formatted files:\n%s\n" "$FILES"

echo "Formatting completed."