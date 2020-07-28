#3rd Party
add_subdirectory(3rd_party/glm)

#PooleEngine
project(PooleEngineProject)

add_library(
	PooleEngine SHARED

	#3rd Party
	3rd_party/glad/include/glad/glad.h
	3rd_party/glad/include/KHR/khrplatform.h
	3rd_party/glad/src/glad.c
	3rd_party/stb/stb/stb_image.h
	

	#PooleEngine
	include/poole.h
	include/poole/core.h
	include/poole/engine.h
	include/poole/entry_point.h
	src/engine.cpp
	src/rendering/shader_loader.h
	src/rendering/shader_loader.cpp
	src/rendering/shader_literals/shader_literals.h
	)

#3rd Party
find_package(OpenGL REQUIRED)
target_link_libraries(PooleEngine PUBLIC "${OPENGL_LIBRARIES}")
target_link_libraries(PooleEngine PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/3rd_party/glfw3.3/lib/glfw3.lib")

target_compile_features(PooleEngine PUBLIC cxx_std_17)

target_include_directories(PooleEngine PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/3rd_party/glfw3.3/include")
target_include_directories(PooleEngine PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/3rd_party/glad/include")
target_include_directories(PooleEngine PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/3rd_party/glm")
target_include_directories(PooleEngine PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/3rd_party/stb")


#PooleEngine
add_subdirectory(src)
target_include_directories(PooleEngine PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")

# It is always easier to navigate in an IDE when projects are organized in folders.
set_property(GLOBAL PROPERTY USE_FOLDERS ON)