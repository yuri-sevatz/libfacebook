cmake_minimum_required(VERSION 2.8.8)

# project details
# ---------------
project(facebook)

set(VERSION_MAJOR "0")
set(VERSION_MINOR "1")
set(VERSION_PATCH "0")
set(VERSION "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}")

# required dependencies

set(CMAKE_MODULE_PATH
  ${CMAKE_SOURCE_DIR}/cmake/Modules
)

find_package(Qt5 COMPONENTS Core Network WebKit WebKitWidgets REQUIRED)

# configuration

OPTION(INSTALL_HEADERS "Copy headers to target" OFF)
OPTION(MORE_VERBOSE "Enable more verbose output messages" OFF)

CONFIGURE_FILE(${CMAKE_SOURCE_DIR}/libfacebook/include/config.h.cmake ${CMAKE_SOURCE_DIR}/libfacebook/include/config.h)

# add your headers and source files here
# ---------------

set(${PROJECT_NAME}_pub_headers
    app.hpp
    client.hpp
    token.hpp
)

set(${PROJECT_NAME}_pub_sources

)

set(${PROJECT_NAME}_pri_headers
    appdata.hpp
    clientprivate.hpp
    facebookpage.hpp
    tokendata.hpp
)

set(${PROJECT_NAME}_pri_sources
    app.cpp
    client.cpp
    clientprivate.cpp
    facebookpage.cpp
    token.cpp
)

set(${PROJECT_NAME}_headers
    ${${PROJECT_NAME}_pub_headers}
    ${${PROJECT_NAME}_pri_headers}
)

set(${PROJECT_NAME}_sources
    ${${PROJECT_NAME}_pub_sources}
    ${${PROJECT_NAME}_pri_sources}
)

QT5_WRAP_CPP(${PROJECT_NAME}_headers_moc ${${PROJECT_NAME}_headers})

# Create the library
# ---------------
add_library(${PROJECT_NAME} SHARED ${${PROJECT_NAME}_headers} ${${PROJECT_NAME}_headers_moc} ${${PROJECT_NAME}_sources})

target_link_libraries(${PROJECT_NAME} twirl)
qt5_use_modules(${PROJECT_NAME} Core Network WebKit WebKitWidgets)

# Install
# ---------------
install(TARGETS ${PROJECT_NAME}
                RUNTIME DESTINATION bin
                LIBRARY DESTINATION lib
                ARCHIVE DESTINATION lib/static)

IF(DEFINED INSTALL_HEADERS)
install(DIRECTORY . DESTINATION include/lib${PROJECT_NAME} FILES_MATCHING PATTERN "*.hpp")
ENDIF(DEFINED INSTALL_HEADERS)
