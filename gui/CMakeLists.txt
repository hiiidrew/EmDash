cmake_minimum_required(VERSION 3.20)
project(emdash_gui LANGUAGES CXX)

add_library(emdash_gui STATIC
    src/ai_client.cpp
    src/app_window.cpp
    src/dashboard_model.cpp
    src/serial_interface.cpp
    src/settings_manager.cpp
    src/telemetry_plot.cpp
    main.cpp
)

target_include_directories(emdash_gui PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_features(emdash_gui PUBLIC cxx_std_17)
