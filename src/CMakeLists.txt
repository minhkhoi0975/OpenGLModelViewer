# Add header files from third-party libraries.
include_directories( 
	"../Dependencies/GLFW/include"
	"../Dependencies/GLAD/include"
	"../Dependencies/glm"
	"../Dependencies/stb/include"
	"../Dependencies/Assimp/include"
	"../Dependencies/ImGui")

include_directories("pch")

set(PRECOMPILED_HEADER "pch/pch.h")

# Add source to this project's executable.
file(GLOB SOURCE_FILES CONFIGURE_DEPENDS 
	"core/*.h" "core/*.cpp"
	"resource_management/*.h" "resource_management/*.cpp"
	"renderer/*.h" "renderer/*.cpp"
	"gui/*.h" "gui/*.cpp")
add_executable (OpenGL ${SOURCE_FILES} ${PRECOMPILED_HEADER})

# Add pre-compiled header(s)
target_precompile_headers(OpenGL PRIVATE ${PRECOMPILED_HEADER})

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET OpenGL PROPERTY CXX_STANDARD 20)
endif()

# Link .lib files to executable.
target_link_libraries( OpenGL ${OPENGL_LIBRARIES} 
	glfw 
	glad 
	glm 
	assimp
	imgui)

# Copy shader files to the build folder.
file(GLOB SHADER_FILES "../shaders/*.vert" "../shaders/*.frag")
foreach(SHADER_FILE ${SHADER_FILES})
	file(COPY ${SHADER_FILE} DESTINATION "${CMAKE_BINARY_DIR}/src/shaders")
endforeach()

# Copy font files to the build folder.
file(GLOB FONT_FILES "../fonts/*.ttf")
foreach(FONT_FILE ${FONT_FILES})
	file(COPY ${FONT_FILE} DESTINATION "${CMAKE_BINARY_DIR}/src/fonts")
endforeach()

# Copy model files to the build folder.
file(GLOB MODEL_FILES 
	"../models/*.obj" 
	"../models/*.mtl"
	"../models/*.jpg"
	"../models/*.png"
)
foreach(FONT_FILE ${MODEL_FILES})
	file(COPY ${MODEL_FILES} DESTINATION "${CMAKE_BINARY_DIR}/src/models")
endforeach()