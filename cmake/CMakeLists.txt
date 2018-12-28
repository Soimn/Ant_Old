cmake_minimum_required(VERSION 3.12)

set(PROJECT_MAJOR 0)
set(PROJECT_MINOR 1)

# PROJECT AND SETUP
project(ant_engine VERSION ${PROJECT_MAJOR}.${PROJECT_MINOR} LANGUAGES CXX)

set(TARGET ant_engine)
set(ROOT "${CMAKE_CURRENT_LIST_DIR}/../")
set(SRC "${ROOT}/src/")
set(DEPS "${ROOT}/dep")
set(PUBLIC_I "${ROOT}/include/")

set(CML "CMakeLists.txt")

set(PUBLIC_HEADS "${PUBLIC_I}/ant.h")
set(PUBLIC_CORE_HEADS "${SRC}/core/dll_definitions.h")

add_library(${TARGET} SHARED "${SRC}/core/common.h")

set_target_properties(${TARGET} PROPERTIES
	VERSION ${PROJECT_VERSION}
	CXX_STANDARD 17
	CXX_STANDARD_REQUIRED ON
	LINKER_LANGUAGE CXX
)

# COMPILE OPTIONS

target_compile_definitions(${TARGET} PRIVATE ANT_DLL_EXPORT)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
	message(STATUS "Compiler: Clang")
	if ("${CMAKE_CXX_COMPILER_VERSION}" VERSION_LESS 7.0.0)
		message(FATAL_ERROR "Unsuported version of Clang. Minimum required: 7.0.0")
	endif()
	
	target_compile_definitions(${TARGET} PUBLIC ANT_COMPILER_CLANG)
endif()

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
	message(STATUS "Compiler: GNU")
	if ("${CMAKE_CXX_COMPILER_VERSION}" VERSION_LESS 8.1.0)
		message(FATAL_ERROR "Unsuported version of gcc. Minimum required: 8.1.0")
	endif()
	
	target_compile_definitions(${TARGET} PUBLIC ANT_COMPILER_GNU)
endif()

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
	message(STATUS "Compiler: MSVC")
	if ("${CMAKE_CXX_COMPILER_VERSION}" VERSION_LESS 14)
		message(FATAL_ERROR "Unsuported version of MSVC. Minimum required: 14")
	endif()
	
	target_compile_definitions(${TARGET} PUBLIC ANT_COMPILER_MSVC)
endif()



if(WIN32)
	target_compile_definitions(${TARGET} PUBLIC ANT_PLATFORM_WINDOWS)
	target_compile_options(${TARGET} PRIVATE 
		"/permissive-" "/GR-" "/EHsc"
		$<$<CONFIG:Debug>: /Od /Zo /DANT_DEBUG>
		$<$<CONFIG:Release>: /O2 /DNDEBUG>
	)

elseif(UNIX AND NOT CYGWIN)
	target_compile_definitions(${TARGET} PUBLIC ANT_PLATFORM_UNIX)
	target_compile_options(${TARGET} PRIVATE 
		"-Wall" "-Wpedantic" "-fvisibility=hidden" "-fvisibility-inlines-hidden" "-Wextra" "-Wshadow"
		"-Wnon-virtual-dtor" "-Wold-style-cast" "-Wcast-align" "-Wunused" "-Woverloaded-virtual" "-Wconversion"
		"-Wsign-conversion" "-Wmisleading-indentation" "-Wduplicated-cond" "-Wlogical-op" "-Wnull-dereference"
		"-Wuseless-cast" "-Wdouble-promotion" "-Wformat=2"
		
		"-fno-exceptions" "-fno-rtti"

		$<$<CONFIG:Debug>: -O0 -g3 -DANT_DEBUG>
		$<$<CONFIG:Release>: -O2 -DNDEBUG>
	)

else()
	message(FATAL_ERROR "Platform or compiler not supported, currently supported platforms are: Windows, Linux, Mac OS, with compilers: GNU, CLANG, MSVC")
endif()

# EXTERNAL LIBS
target_link_libraries(${TARGET} LINK_PRIVATE EASTL)

find_package(GLEW REQUIRED)
if (GLEW_FOUND)
	target_include_directories(${TARGET} PRIVATE ${GLEW_INCLUDE_DIRS})
	target_link_libraries(${TARGET} LINK_PRIVATE ${GLEW_LIBRARIES})
else()
	message(FATAL_ERROR "Glew not found")
endif()

find_package(glfw3 3.2 REQUIRED)
if (glfw3_FOUND)
	target_include_directories(${TARGET} PRIVATE ${GLFW_INCLUDE_DIRS})
	target_link_libraries(${TARGET} LINK_PRIVATE ${GLFW_LIBRARIES})
else()
	message(FATAL_ERROR "GLFW not found")
endif()

# INCLUDE
target_include_directories(${TARGET} PRIVATE "${SRC}/")

include(GNUInstallDirs)

configure_file(${TARGET}.pc.in ${TARGET}.pc @ONLY)
configure_file(${TARGET}-config-version.cmake.in ${TARGET}-config-version.cmake @ONLY)

install(TARGETS ${TARGET} EXPORT ${TARGET} ARCHIVE DESTINATION lib LIBRARY DESTINATION lib)
install(FILES ${PUBLIC_HEADS} DESTINATION include)
install(FILES ${PUBLIC_CORE_HEADS} DESTINATION include/core/)
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${TARGET}.pc" DESTINATION pkgconfig)
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${TARGET}-config-version.cmake" DESTINATION lib)
install(FILES ${TARGET}-config.cmake DESTINATION lib)
install(EXPORT ${TARGET} DESTINATION lib)

include("${SRC}/CMakeLists.txt")
