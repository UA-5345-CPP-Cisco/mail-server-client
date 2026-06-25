# Read the downloaded CMakeLists.txt
file(READ "CMakeLists.txt" content)

# Replace the outdated version requirement with a modern one
string(REPLACE "cmake_minimum_required(VERSION 3.0.2)" "cmake_minimum_required(VERSION 3.16)" content "${content}")

# Overwrite the file with the fixed content
file(WRITE "CMakeLists.txt" "${content}")