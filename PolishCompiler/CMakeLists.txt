add_library(PolishCompiler_lib STATIC src/operations.cpp src/statement.cpp)
add_executable(PolishCompiler main.cpp)
target_link_libraries(PolishCompiler PolishCompiler_lib)
add_compile_definitions(TEST)