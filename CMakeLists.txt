cmake_minimum_required(VERSION 3.27)
project(final_aed)

# Establecer el estándar de C++
set(CMAKE_CXX_STANDARD 17)

# Incluir las rutas de las bibliotecas
include_directories(/opt/homebrew/opt/glew/include)
include_directories(/opt/homebrew/opt/glfw/include)
link_directories(/opt/homebrew/opt/glew/lib)
link_directories(/opt/homebrew/opt/glfw/lib)
include_directories(/opt/homebrew/opt/glm/include)
include_directories(/opt/homebrew/Cellar/freeglut/3.6.0/include)
link_directories(/opt/homebrew/Cellar/freeglut/3.6.0/lib)

# Buscar OpenGL (nativo en macOS)
find_package(OpenGL REQUIRED)

# Agregar el ejecutable principal

#add_executable(final_aed main.cpp)
#add_executable(octree Octree.cpp)
#add_executable(open OpenGL.cpp)x
#add_executable(helper OpenGLHelpers.cpp)

add_executable(final_aed main.cpp Octree.cpp OpenGLHelpers.cpp)


# Vincular las bibliotecas necesarias
target_link_libraries(final_aed OpenGL::GL glfw GLEW)
