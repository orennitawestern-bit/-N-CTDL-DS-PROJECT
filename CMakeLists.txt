cmake_minimum_required(VERSION 3.16)
project(PRO_CTDL_5 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(airline_core
    src/MayBay.cpp
    src/KhachHang.cpp
    src/Ve.cpp
    src/ChuyenBay.cpp
    src/AirlineManager.cpp
    src/Utils.cpp
)
target_include_directories(airline_core PUBLIC include)

add_executable(PRO_CTDL_5 src/main.cpp)
target_link_libraries(PRO_CTDL_5 PRIVATE airline_core)

if(MSVC)
    target_compile_options(airline_core PRIVATE /W4)
    target_compile_options(PRO_CTDL_5 PRIVATE /W4)
else()
    target_compile_options(airline_core PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(PRO_CTDL_5 PRIVATE -Wall -Wextra -Wpedantic)
endif()

# Co the chay chuong trinh khi working directory la thu muc build.
file(COPY "${CMAKE_SOURCE_DIR}/data" DESTINATION "${CMAKE_BINARY_DIR}")

enable_testing()
add_executable(PRO_CTDL_5_tests tests/test_airline.cpp)
target_link_libraries(PRO_CTDL_5_tests PRIVATE airline_core)
add_test(NAME CoreFunctions COMMAND PRO_CTDL_5_tests)
