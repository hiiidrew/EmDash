cmake_minimum_required(VERSION 3.20)
project(emdash_embedded LANGUAGES C CXX)

add_executable(emdash_embedded
    src/main.cpp
    src/system.cpp
    utils/crc.cpp
    protocols/dis_encoder.cpp
    protocols/flatbuffer_serializer.cpp
    rtos/tasks/telemetry_task.cpp
    rtos/tasks/scheduler.cpp
)

target_include_directories(emdash_embedded PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/protocols
    ${CMAKE_CURRENT_SOURCE_DIR}/rtos
    ${CMAKE_CURRENT_SOURCE_DIR}/rtos/tasks
    ${CMAKE_CURRENT_SOURCE_DIR}/config
    ${CMAKE_CURRENT_SOURCE_DIR}/freertos
)

target_compile_features(emdash_embedded PRIVATE cxx_std_17)

find_package(Threads REQUIRED)
target_link_libraries(emdash_embedded PRIVATE Threads::Threads)
