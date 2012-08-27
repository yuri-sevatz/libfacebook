cmake_minimum_required(VERSION 2.8)

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

set(Boost_USE_STATIC_LIBS OFF)
find_package(Boost COMPONENTS iostreams thread system REQUIRED)

set(QT_USE_NETWORK TRUE)
set(QT_USE_QTWEBKIT TRUE)
find_package(Qt4 REQUIRED)

find_package(QJSON REQUIRED)

# configuration

OPTION(INSTALL_HEADERS "Copy headers to target" OFF)
OPTION(MORE_VERBOSE "Enable more verbose output messages" OFF)

CONFIGURE_FILE(${CMAKE_SOURCE_DIR}/libfacebook/include/config.h.cmake ${CMAKE_SOURCE_DIR}/libfacebook/include/config.h)

# add your headers and source files here
# ---------------

set(${PROJECT_NAME}_pub_headers
    client.hpp
    app/settings.hpp
    auth/credentials.hpp
    auth/token.hpp
)

set(${PROJECT_NAME}_pub_sources

)

set(${PROJECT_NAME}_pri_headers
    clientprivate.hpp
)

set(${PROJECT_NAME}_pri_sources
    client.cpp
    clientprivate.cpp
)

set(${PROJECT_NAME}_headers
    ${${PROJECT_NAME}_pub_headers}
    ${${PROJECT_NAME}_pri_headers}
)

set(${PROJECT_NAME}_sources
    ${${PROJECT_NAME}_pub_sources}
    ${${PROJECT_NAME}_pri_sources}
)

QT4_WRAP_CPP(${PROJECT_NAME}_headers_moc ${${PROJECT_NAME}_headers})

# Includes
# ---------------
message("Boost_INCLUDE_DIR")
message(${Boost_INCLUDE_DIR})
include_directories(${Boost_INCLUDE_DIR})

# Pre-linkage Qt
# ---------------
INCLUDE(${QT_USE_FILE})
ADD_DEFINITIONS(${QT_DEFINITIONS})

message("QJSON_INCLUDE_DIR")
message(${QJSON_INCLUDE_DIR})
include_directories(${QJSON_INCLUDE_DIR})

# Linked Libraries
# ---------------
message("Boost_LIBRARY_DIRS")
message(${Boost_LIBRARY_DIRS})

message("QT_LIBRARIES")
message(${QT_LIBRARIES})

message("QJSON_LIBRARIES")
message(${QJSON_LIBRARIES})

## For MSVS2010 - has issues with target_link_libraries
## Still looking for a way to turn this on pragmatically
#link_directories(${Boost_LIBRARY_DIRS})

# Create the library
# ---------------
add_library(${PROJECT_NAME} SHARED ${${PROJECT_NAME}_headers} ${${PROJECT_NAME}_headers_moc} ${${PROJECT_NAME}_sources})

target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES} qjson ${QT_LIBRARIES})

# Install
# ---------------
install(TARGETS ${PROJECT_NAME}
                RUNTIME DESTINATION bin
                LIBRARY DESTINATION lib
                ARCHIVE DESTINATION lib/static)

IF(DEFINED INSTALL_HEADERS)
install(DIRECTORY . DESTINATION include/lib${PROJECT_NAME} FILES_MATCHING PATTERN "*.hpp")
ENDIF(DEFINED INSTALL_HEADERS)
