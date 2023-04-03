cmake_minimum_required(VERSION 3.5)

project(binary_loader LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${PROJECT_SOURCE_DIR}/include)

set(SOURCES
    main.cpp loader.cpp disasm.cpp disasembler.cpp)

set(HEADERS include/symbol.h include/section.h include/binary.h include/loader.h include/bfd_interface.h
            include/common.h include/disasm.h include/ins.h include/disasembler.h)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

install(TARGETS binary_loader
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

target_link_libraries(binary_loader -lbfd -lcapstone)


