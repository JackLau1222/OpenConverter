#!/bin/bash
TOP_DIR=$PWD

# Define file extensions to process
EXTENSIONS=(".cpp" ".c" ".h" ".cc" ".hpp")

# Define clang-format default style (using JSON format)
STYLE="{\"BasedOnStyle\": \"llvm\", \"IndentWidth\": -2}"

# Check if clang-format is installed
if ! command -v clang-format &> /dev/null; then
    echo "Error: clang-format is not installed. Please install it and make sure it's in your PATH." >&2
    exit 1
fi

# Check clang-format version for JSON style support, fallback to YAML if unsupported
CLANG_VERSION=$(clang-format --version)
if ! printf "%s\n" "$CLANG_VERSION" | grep -q "based on LLVM"; then
    echo "Warning: clang-format version does not support JSON style. Falling back to YAML style."
    STYLE="BasedOnStyle: LLVM\nIndentWidth: 4"
fi

echo "Scanning directory: $TOP_DIR"

# Build find command query for file extensions
FIND_QUERY=""
for ext in "${EXTENSIONS[@]}"; do
    FIND_QUERY+=" -o -name *$ext"
done
FIND_QUERY=${FIND_QUERY:4}  # Remove leading "-o"

# Find all matching files (use null delimiter to handle spaces in filenames)
FILES=$(find "$TOP_DIR" -type f \( $FIND_QUERY \) -print0)

# Check if any files were found
if [ -z "$FILES" ]; then
    echo "No files found with supported extensions."
    exit 0
fi

echo "Formatting files..."

FORMATTED_FILES=""

# Process files using while loop with null delimiter
while IFS= read -r -d '' file; do
    # Get file directory
    DIR=$(dirname "$file")
    # Use -style=file if .clang-format exists in directory
    if [ -f "$DIR/.clang-format" ]; then
        clang-format -style=file -i "$file"
    else
        clang-format -style="$STYLE" -i "$file"
    fi
    FORMATTED_FILES+="$file"$'\n'
done < <(find "$TOP_DIR" -type f \( $FIND_QUERY \) -print0)

printf "Formatted files:\n%s\n" "$FORMATTED_FILES"

echo "Formatting completed."