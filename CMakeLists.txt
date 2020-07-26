#3rd Party
add_subdirectory(3rd_party/glm)

#PooleEngine
project(PooleEngineProject)
FILE(GLOB_RECURSE PooleEngine_include_Poole_h 	include/Poole/*.h)
FILE(GLOB_RECURSE PooleEngine_src_h   		src/*.h)
FILE(GLOB_RECURSE PooleEngine_src_cpp 		src/*.cpp)

add_library(
	PooleEngine SHARED

	#3rd Party
	3rd_party/glad/include/glad/glad.h
	3rd_party/glad/include/KHR/khrplatform.h
	3rd_party/glad/src/glad.c
	3rd_party/stb/stb/stb_image.h
	

	#PooleEngine
	include/Poole.h
	${PooleEngine_include_Poole_h}
	${PooleEngine_src_h}
	${PooleEngine_src_cpp}
	)


#3rd Party
find_package(OpenGL REQUIRED)
target_link_libraries(PooleEngine PUBLIC "${OPENGL_LIBRARIES}")
target_link_libraries(PooleEngine PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/3rd_party/glfw3/glfw3.lib")

target_compile_features(PooleEngine PUBLIC cxx_std_17)

target_include_directories(PooleEngine PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/3rd_party/glfw3.3-stable/include")
target_include_directories(PooleEngine PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/3rd_party/glad/include")
target_include_directories(PooleEngine PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/3rd_party/glm")
target_include_directories(PooleEngine PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/3rd_party/stb")


#PooleEngine
add_subdirectory(src)
target_include_directories(PooleEngine PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include") #Allow main.cpp to do #include "PooleEngine/......h"