# the name of the target operating system
set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR aarch64)

set(TARGET_HOST aarch64-alpine-linux-musl-gcc)

# which compilers to use for C and C++
set(CMAKE_C_COMPILER   ${TARGET_HOST}-alpine-linux-musl-gcc)
set(CMAKE_CXX_COMPILER ${TARGET_HOST}-alpine-linux-musl-gcc)

# where is the target environment located
set(CMAKE_FIND_ROOT_PATH  /usr/bin/aarch64-alpine-linux-musl-gcc)

# adjust the default behavior of the FIND_XXX() commands:
# search programs in the host environment
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)

# search headers and libraries in the target environment
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
