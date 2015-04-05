# Automatically generated file, do not edit
SET(BII_IS_DEP False)


# LIBRARY yacoder_flatbuffers-with-spirit ##################################
# with interface yacoder_flatbuffers-with-spirit_interface

# Source code files of the library
SET(BII_LIB_SRC  )
# STATIC by default if empty, or SHARED
SET(BII_LIB_TYPE )
# Dependencies to other libraries (user2_block2, user3_blockX)
SET(BII_LIB_DEPS yacoder_flatbuffers-with-spirit_interface )
# System included headers
SET(BII_LIB_SYSTEM_HEADERS algorithm assert.h cmath cstddef cstdio cstring fstream iomanip iostream iterator limits map memory ostream set sstream stdbool.h stdexcept stdint.h stdlib.h streambuf string sys/time.h sys/types.h unistd.h vector windows.h)
# Required include paths
SET(BII_LIB_INCLUDE_PATHS )


# Executables to be created
SET(BII_BLOCK_EXES main)

SET(BII_BLOCK_TESTS )


# EXECUTABLE main ##################################

SET(BII_main_SRC catch.hpp
			main.cpp)
SET(BII_main_DEPS yacoder_flatbuffers-with-spirit_interface yacoder_flatbuffers-with-spirit)
# System included headers
SET(BII_main_SYSTEM_HEADERS map string utility vector)
# Required include paths
SET(BII_main_INCLUDE_PATHS )
