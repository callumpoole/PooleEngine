# 3rd Party
add_subdirectory(3rd_party/glm)


# It is always easier to navigate in an IDE when projects are organized in folders.
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Collect Files for Library
set (APPLICATION_SRC ${APPLICATION_SRC} 
	"include/poole.h"
	"include/poole/core.h"
	"include/poole/core/core_logging.h"
	"include/poole/core/core_types.h"
	"src/core/core_logging.cpp"
	"include/poole/engine.h"
	"include/poole/entry_point.h"
	"src/engine.cpp"
	"include/poole/rendering/shader_loader.h"
	"src/rendering/shader_loader.cpp"
	"src/rendering/shaders/UniformColor.shader"
	"src/rendering/shaders/UniformColor2DTransform.shader"
	"src/rendering/shaders/VertexColor.shader"
	"src/rendering/shaders/VertexColor2DTransform.shader"
	"src/rendering/shaders/Circle2DTransform.shader"
	"src/rendering/shaders/Texture2DTransform.shader"
	"src/rendering/shaders/Experimental1.shader"
	"src/rendering/shaders/Experimental2.shader"
	"src/rendering/shaders/Experimental3.shader"
	"include/poole/rendering/camera/orthographic_camera.h"
	"src/rendering/camera/orthographic_camera.cpp"
	"include/poole/rendering/renderer2D.h"
	"include/poole/rendering/renderer.h"
	"src/rendering/renderer.cpp"
	"src/rendering/renderer2D.cpp"
	"include/poole/rendering/rendering_primitives.h"
	"src/rendering/rendering_primitives.cpp"
	"src/rendering/image/stb.cpp"
	"src/rendering/image/image.h"
	"src/rendering/image/image.cpp"	
	"src/rendering/graphics_api/vertex_array.h"
	"src/rendering/graphics_api/vertex_array.cpp"
	"src/rendering/graphics_api/buffer.h"
	"src/rendering/graphics_api/buffer.cpp"
	"src/rendering/graphics_api/renderer_api.h"
	"src/rendering/graphics_api/renderer_api.cpp"
	"src/rendering/graphics_api/texture.h"
	"src/rendering/graphics_api/texture.cpp"
	"src/platform/opengl/opengl_vertex_array.h"
	"src/platform/opengl/opengl_vertex_array.cpp"
	"src/platform/opengl/opengl_buffer.h"
	"src/platform/opengl/opengl_buffer.cpp"
	"src/platform/opengl/opengl_renderer_api.h"
	"src/platform/opengl/opengl_renderer_api.cpp"
	"src/platform/opengl/opengl_texture.h"
	"src/platform/opengl/opengl_texture.cpp"
	"include/poole/input/input.h"
	"include/poole/input/input_types.h"
	"src/input/input.cpp"
	"src/window/window.h"
	"src/window/window.cpp"
	"src/engine/command_args.h"
	"src/engine/command_args.cpp"
	"include/poole/engine/engine_time.h"
	"src/engine/engine_time.cpp"	
	)

# Setup Folder Filters for IDE
macro (organise_solution_folders)
	set (prefix ${CMAKE_CURRENT_SOURCE_DIR})
	foreach(source IN LISTS APPLICATION_SRC)
		get_filename_component(source_path "${source}" PATH)
		string(REPLACE ${prefix} "" source_path_no_prefix "${source_path}")
		
		file(TO_NATIVE_PATH "${source_path_no_prefix}" source_path_msvc)      #NEW WAY
		#string(REPLACE "/" "\\" source_path_msvc "${source_path_no_prefix}") #OLD WAY
		
		source_group("${source_path_msvc}" FILES "${source}")
	endforeach()
endmacro()
organise_solution_folders()



# PooleEngine
project(PooleEngineProject)

add_library(
	PooleEngine STATIC

	#3rd Party
	3rd_party/glad/include/glad/glad.h
	3rd_party/glad/include/KHR/khrplatform.h
	3rd_party/glad/src/glad.c
	3rd_party/stb/stb/stb_image.h
	3rd_party/entt/include/entt/entt.hpp
	

	#PooleEngine
	${APPLICATION_SRC}
	)

#3rd Party
find_package(OpenGL REQUIRED)
target_link_libraries(PooleEngine PUBLIC "${OPENGL_LIBRARIES}")
target_link_libraries(PooleEngine PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/3rd_party/glfw3.3/lib/glfw3.lib")


target_compile_features(PooleEngine PUBLIC cxx_std_23)
target_include_directories(PooleEngine PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/3rd_party/glfw3.3/include")
target_include_directories(PooleEngine PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/3rd_party/glad/include")
target_include_directories(PooleEngine PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/3rd_party/glm")
target_include_directories(PooleEngine PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/3rd_party/stb")
target_include_directories(PooleEngine PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/3rd_party/entt/include")


#PooleEngine
add_subdirectory(src)
target_include_directories(PooleEngine PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_include_directories(PooleEngine PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src")
