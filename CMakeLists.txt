cmake_minimum_required(VERSION 2.8)
set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} /opt/local) 

IF (UNIX)
  SET (BOOST_ROOT "/opt/local")
  SET (Boost_NO_SYSTEM_PATHS "TRUE")
  SET (BOOST_INCLUDEDIR "/opt/local/include")
  SET (BOOST_LIBRARYDIR "/opt/local/lib") 
  message ("unix: " ${BOOST_ROOT})
ENDIF (UNIX)

message (${CMAKE_PREFIX_PATH})

project(BTree)

find_package(Boost REQUIRED COMPONENTS system thread)

SET(TARGET_SRC
  foo.cpp
)

SET(TARGET_HDRS
)

ADD_DEFINITIONS(
  ${Boost_DEFINES}
  )

add_executable (btree ${TARGET_SRC})
include_directories(
  ${Boost_INCLUDE_DIRS}
)

target_link_libraries (btree ${TARGET_EXTERNAL_LIBRARIES}
  ${Boost_LIBRARIES}
)
