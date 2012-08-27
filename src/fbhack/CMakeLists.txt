cmake_minimum_required(VERSION 2.8)

# project details
# ---------------
project(fbhack)

set(VERSION_MAJOR "0")
set(VERSION_MINOR "1")
set(VERSION_PATCH "0")
set(VERSION "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}")

# required dependencies
find_package(Qt4 REQUIRED)

# add your headers and source files here
# ---------------

set (${PROJECT_NAME}_headers
    bootstrap.hpp
    fbhack.hpp
)
set (${PROJECT_NAME}_sources
    bootstrap.cpp
    fbhack.cpp
    main.cpp
)

QT4_WRAP_CPP(${PROJECT_NAME}_headers_moc ${${PROJECT_NAME}_headers})

# Includes
# ---------------

# Pre-linkage Qt
# ---------------
INCLUDE(${QT_USE_FILE})
ADD_DEFINITIONS(${QT_DEFINITIONS})

# Create the binary
# ---------------
add_executable(${PROJECT_NAME} ${${PROJECT_NAME}_headers} ${${PROJECT_NAME}_headers_moc} ${${PROJECT_NAME}_sources})

target_link_libraries(${PROJECT_NAME} ${QT_LIBRARIES} facebook)

# Install
# ---------------
install(TARGETS ${PROJECT_NAME}
                RUNTIME DESTINATION bin
                LIBRARY DESTINATION lib
                ARCHIVE DESTINATION lib/static)

