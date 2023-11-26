cmake_minimum_required(VERSION 2.6)

SET(INCLUDE_INSTALL_DIR "${PROJECT_SOURCE_DIR}/include")

install(DIRECTORY include/rapidjson
    DESTINATION "${INCLUDE_INSTALL_DIR}"
    COMPONENT dev)
include_directories(${INCLUDE_INSTALL_DIR})
include_directories(${PROJECT_SOURCE_DIR}/api)
link_directories(${PROJECT_SOURCE_DIR}/api)


file(GLOB APP_HEAD *.h)
file(GLOB APP_SRC *.cpp)
set(XTP_BASE_LIBS 
	  debug xtpquoteapi
	   optimized xtpquoteapi
	)
IF(WIN32)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
ENDIF()	

# set output path
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
add_executable(XTPApiDemo ${APP_SRC} ${APP_HEAD})
target_link_libraries(XTPApiDemo ${XTP_BASE_LIBS})
SET_TARGET_PROPERTIES(XTPApiDemo PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG ${PROJECT_SOURCE_DIR}/api)  
SET_TARGET_PROPERTIES(XTPApiDemo PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE ${PROJECT_SOURCE_DIR}/api) 