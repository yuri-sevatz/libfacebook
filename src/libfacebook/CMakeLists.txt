cmake_minimum_required(VERSION 2.8)

# project details
# ---------------
project(facebook)

set(VERSION_MAJOR "0")
set(VERSION_MINOR "1")
set(VERSION_PATCH "0")
set(VERSION "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}")

# required dependencies
set(Boost_USE_STATIC_LIBS OFF)
find_package(Boost COMPONENTS iostreams thread system REQUIRED)
set(QT_USE_QTWEBKIT TRUE)
find_package(Qt4 REQUIRED)

# add your headers and source files here
# ---------------

set(${PROJECT_NAME}_app_headers
    app/settings.hpp
)

set(${PROJECT_NAME}_app_sources
    app/settings.cpp
)

set(${PROJECT_NAME}_auth_headers
    auth/credentials.hpp
    auth/session.hpp
    auth/sessionprivate.hpp
    auth/token.hpp
)

set(${PROJECT_NAME}_auth_sources
    auth/credentials.cpp
    auth/session.cpp
    auth/sessionprivate.cpp
    auth/token.cpp
)

set(${PROJECT_NAME}_headers
    ${${PROJECT_NAME}_app_headers}
    ${${PROJECT_NAME}_auth_headers}
)
set(${PROJECT_NAME}_sources
    ${${PROJECT_NAME}_app_sources}
    ${${PROJECT_NAME}_auth_sources}
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

# Linked Libraries
# ---------------
message("Boost_LIBRARY_DIRS")
message(${Boost_LIBRARY_DIRS})
## For MSVS2010 - has issues with target_link_libraries
## Still looking for a way to turn this on pragmatically
#link_directories(${Boost_LIBRARY_DIRS})

# Create the library
# ---------------
add_library(${PROJECT_NAME} SHARED ${${PROJECT_NAME}_headers} ${${PROJECT_NAME}_headers_moc} ${${PROJECT_NAME}_sources})

target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES} ${QT_LIBRARIES})

# Install
# ---------------
install(TARGETS ${PROJECT_NAME}
                RUNTIME DESTINATION bin
                LIBRARY DESTINATION lib
                ARCHIVE DESTINATION lib/static)

