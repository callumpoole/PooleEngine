#3rd Party
add_subdirectory(3rdParty/glm)
add_subdirectory(3rdParty/glfw3)

#PooleEngine
project(PooleEngineProject)
FILE(GLOB_RECURSE PooleEngine_include_Poole_h 	include/Poole/*.h)
FILE(GLOB_RECURSE PooleEngine_src_h   		src/*.h)
FILE(GLOB_RECURSE PooleEngine_src_cpp 		src/*.cpp)

add_library(
	PooleEngine SHARED

	#3rd Party
	3rdParty/glad/include/glad/glad.h
	3rdParty/glad/include/KHR/khrplatform.h
	3rdParty/glad/src/glad.c
	3rdParty/stb/stb/stb_image.h
	

	#PooleEngine
	include/Poole.h
	${PooleEngine_include_Poole_h}
	${PooleEngine_src_h}
	${PooleEngine_src_cpp}
	)


#3rd Party
find_package(OpenGL REQUIRED)
target_link_libraries(PooleEngine PUBLIC "${OPENGL_LIBRARIES}")
target_link_libraries(PooleEngine PRIVATE glfw ${GLFW_LIBRARIES})

target_compile_features(PooleEngine PUBLIC cxx_std_17)

target_include_directories(PooleEngine PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/glfw3/include")
target_include_directories(PooleEngine PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/glad/include")
target_include_directories(PooleEngine PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/glm")
target_include_directories(PooleEngine PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/stb")


#PooleEngine
add_subdirectory(src)
target_include_directories(PooleEngine PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include") #Allow main.cpp to do #include "PooleEngine/......h"