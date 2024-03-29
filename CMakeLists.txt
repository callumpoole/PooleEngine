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
	"src/core/core_types.cpp"
	"include/poole/engine.h"
	"include/poole/entry_point.h"
	"src/engine.cpp"
	"src/debug/debug_overlay.h"
	"src/debug/debug_overlay.cpp"
	"include/poole/rendering/shader_loader.h"
	"src/rendering/shader_loader.cpp"
	"src/rendering/shaders/UniformColor.shader"
	"src/rendering/shaders/UniformColor2DTransform.shader"
	"src/rendering/shaders/VertexColor.shader"
	"src/rendering/shaders/VertexColor2DTransform.shader"
	"src/rendering/shaders/Circle2DTransform.shader"
	"src/rendering/shaders/Circle2DBatchedTransform.shader"
	"src/rendering/shaders/Line2DBatchedTransform.shader"
	"src/rendering/shaders/Texture2DTransform.shader"
	"src/rendering/shaders/Texture2DBatchedTransform.shader"
	"src/rendering/shaders/Experimental1.shader"
	"src/rendering/shaders/Experimental2.shader"
	"src/rendering/shaders/Experimental3.shader"
	"include/poole/rendering/camera/orthographic_camera.h"
	"src/rendering/camera/orthographic_camera.cpp"
	"include/poole/rendering/renderer2D.h"
	"include/poole/rendering/unbatched_renderer2D.h"
	"include/poole/rendering/batched_renderer2D.h"
	"include/poole/rendering/renderer.h"
	"src/rendering/renderer.cpp"
	"src/rendering/unbatched_renderer2D.cpp"
	"src/rendering/batched_renderer2D.cpp"
	"src/rendering/image/stb.cpp"
	"include/poole/rendering/image/svg.h"
	"src/rendering/image/svg.cpp"
	"include/poole/rendering/image/image.h"
	"src/rendering/image/image.cpp"	
	"include/poole/rendering/image/sub_image.h"
	"src/rendering/image/sub_image.cpp"	
	"include/poole/rendering/image/image_utils.h"
	"src/rendering/image/image_utils.cpp"	
	"src/rendering/graphics_api/vertex_array.h"
	"src/rendering/graphics_api/vertex_array.cpp"
	"src/rendering/graphics_api/buffer.h"
	"src/rendering/graphics_api/buffer.cpp"
	"src/rendering/graphics_api/renderer_api.h"
	"src/rendering/graphics_api/renderer_api.cpp"
	"src/rendering/graphics_api/texture.h"
	"src/rendering/graphics_api/texture.cpp"
	"include/poole/rendering/text/text_renderer_factory.h"
	"include/poole/rendering/text/text_renderer.h"
	"include/poole/rendering/text/font_renderer.h"
	"include/poole/rendering/text/svg_font_renderer.h"
	"src/rendering/text/text_renderer_factory.cpp"
	"src/rendering/text/text_renderer.cpp"
	"src/rendering/text/font_renderer.cpp"
	"src/rendering/text/svg_font_renderer.cpp"
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
	"include/poole/physics/collision_2D/collision_manager_2D.h"
	"include/poole/physics/collision_2D/collider_2D.h"
	"include/poole/physics/collision_2D/box_collider_2D.h"
	"include/poole/physics/collision_2D/circle_collider_2D.h"
	"src/physics/collision_2D/collision_manager_2D.cpp"
	"src/physics/collision_2D/collider_2D.cpp"
	"src/physics/collision_2D/box_collider_2D.cpp"
	"src/physics/collision_2D/circle_collider_2D.cpp"
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
	3rd_party/stb/stb/stb_truetype.h
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
