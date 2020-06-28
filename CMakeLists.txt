#3rd Party
#add_subdirectory(3rdParty/glfw-3.3.2)
add_subdirectory(3rdParty/glm-0.9.9.8)
#add_subdirectory(3rdParty/assimp-master)

#PooleEngine
project(PooleEngineProject)
FILE(GLOB_RECURSE PooleEngine_include_LibraryABC_h 	include/PooleEngine/*.h)
FILE(GLOB_RECURSE PooleEngine_src_h   			src/*.h)
FILE(GLOB_RECURSE PooleEngine_src_cpp 			src/*.cpp)

add_library(
	PooleEngine STATIC

	#3rd Party
	3rdParty/glad/include/glad/glad.h
	3rdParty/glad/include/KHR/khrplatform.h
	3rdParty/glad/src/glad.c
	3rdParty/stb/stb/stb_image.h
	

	#PooleEngine
	${PooleEngine_include_LibraryABC_h}
	${PooleEngine_src_h}
	${PooleEngine_src_cpp}
	)


#3rd Party
find_package(OpenGL REQUIRED)
target_link_libraries(PooleEngine PUBLIC "${OPENGL_LIBRARIES}")

target_include_directories(PooleEngine PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/glfw-3.3.2/include")
target_include_directories(PooleEngine PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/glad/include")
target_include_directories(PooleEngine PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/glm-0.9.9.8")
target_include_directories(PooleEngine PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/stb")
target_link_libraries(PooleEngine PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/glfw-3.3.2/glfw3.lib")

#PooleEngine
add_subdirectory(src)
target_include_directories(PooleEngine PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include") #Allow main.cpp to do #include "PooleEngine/foo_calls_bar.h"