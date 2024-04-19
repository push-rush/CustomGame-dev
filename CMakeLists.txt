cmake_minimum_required(VERSION 3.0)
project(CustomGame)

if (POLICY CMP0072)
    set(OpenGL_GL_PREFERENCE LEGACY)
endif()

# set(UNICODE ON)
# set(_UNICODE ON)
set(CMAKE_CXX_STANDARD 17) 
set(CMAKE_CXX_FLAGS "-finput-charset=utf-8")  
set(CMAKE_C_FLAGS "-finput-charset=utf-8")  

# 如果想要生成的可执行文件拥有符号表，可以gdb调试，就直接加上这句
add_definitions("-Wall -g")

#是否使用fmod studio，加载bank文件的形式
add_definitions(-D USE_FMOD_STUDIO) 

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(freetype REQUIRED)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

find_package(SDL2_ttf REQUIRED)
include_directories(${SDL2_TTF_INCLUDE_DIRS})

find_package(GLEW REQUIRED)
include_directories(${GLEW_INCLUDE_DIRS})

# 生成可执行文件
include_directories(include)
include_directories(${PROJECT_SOURCE_DIR}/External/imgui)
include_directories(${PROJECT_SOURCE_DIR}/External/soil/include/SOIL)
include_directories(${PROJECT_SOURCE_DIR}/External/freetype/include)

include_directories(${PROJECT_SOURCE_DIR}/External/RapidJson/rapidjson-1.1.0/rapidjson-1.1.0/include/rapidjson)
include_directories(${PROJECT_SOURCE_DIR}/External/fmodstudioapi20300linux/api/core/inc)
include_directories(${PROJECT_SOURCE_DIR}/External/fmodstudioapi20300linux/api/fsbank/inc)
include_directories(${PROJECT_SOURCE_DIR}/External/fmodstudioapi20300linux/api/studio/inc)

add_library(IMGUI SHARED 
    ${PROJECT_SOURCE_DIR}/External/imgui/imgui.cpp 
    ${PROJECT_SOURCE_DIR}/External/imgui/backends/imgui_impl_glfw.cpp 
    ${PROJECT_SOURCE_DIR}/External/imgui/backends/imgui_impl_opengl3.cpp
    ${PROJECT_SOURCE_DIR}/External/imgui/imgui_draw.cpp 
    ${PROJECT_SOURCE_DIR}/External/imgui/imgui_tables.cpp 
    ${PROJECT_SOURCE_DIR}/External/imgui/imgui_widgets.cpp
    ${PROJECT_SOURCE_DIR}/External/imgui/imgui_demo.cpp
)

target_link_libraries(IMGUI 
    OpenGL::GL
    glfw
)

add_executable(${PROJECT_NAME} 
    ./main.cpp
    ./src/Game.cpp
    
    # Renderers
    ./src/Renderers/VertexArray.cpp
    ./src/Renderers/Shader.cpp
    ./src/Renderers/Texture.cpp 
    ./src/Renderers/Mesh.cpp
    ./src/Renderers/Renderer.cpp
    ./src/Renderers/Collision.cpp
    ./src/Renderers/PhysWorld.cpp
    ./src/Renderers/UIScreen.cpp 
    ./src/Renderers/PauseMenu.cpp
    ./src/Renderers/Button.cpp 
    ./src/Renderers/DialogBox.cpp 
    ./src/Renderers/HUD.cpp
    ./src/Renderers/Skeleton.cpp
    ./src/Renderers/Animation.cpp 
    ./src/Renderers/GBuffer.cpp
    ./src/Renderers/Font.cpp
    ./src/Renderers/FreeTypeFont.cpp
    ./src/Renderers/Setting.cpp
    ./src/Renderers/EmptySprite.cpp
    ./src/Renderers/ResourceMenu.cpp
    ./src/Renderers/Graphics2d.cpp
    ./src/Renderers/Graphics3d.cpp
    ./src/Renderers/TextButton.cpp
    ./src/Renderers/Console.cpp
    ./src/Renderers/PropertyMenu.cpp
    
    # Actors
    ./src/Actors/Actor.cpp
    ./src/Actors/Ship.cpp 
    ./src/Actors/Asteroid.cpp 
    ./src/Actors/Laser.cpp
    ./src/Actors/CameraActor.cpp 
    ./src/Actors/PlaneActor.cpp
    ./src/Actors/FPSActor.cpp
    ./src/Actors/FollowActor.cpp
    ./src/Actors/OrbitActor.cpp
    ./src/Actors/SplineActor.cpp
    ./src/Actors/BallActor.cpp
    ./src/Actors/TargetActor.cpp
    ./src/Actors/SphereActor.cpp
    
    # Components
    ./src/Components/Component.cpp 
    ./src/Components/MoveComponent.cpp
    ./src/Components/InputComponent.cpp 
    ./src/Components/SpriteComponent.cpp 
    ./src/Components/CircleComponent.cpp 
    ./src/Components/MeshComponent.cpp
    ./src/Components/AudioComponent.cpp
    ./src/Components/CameraComponent.cpp
    ./src/Components/FPSCamera.cpp 
    ./src/Components/FollowCamera.cpp 
    ./src/Components/OrbitCamera.cpp 
    ./src/Components/SplineCamera.cpp 
    ./src/Components/BoxComponent.cpp
    ./src/Components/BallMove.cpp 
    ./src/Components/TargetComponent.cpp
    ./src/Components/SkeletalMeshComponent.cpp  
    ./src/Components/PointLightComponent.cpp
    ./src/Components/MirrorCamera.cpp
    
    # General
    ./src/General/CMath.cpp
    ./src/General/Random.cpp 
    ./src/General/LevelLoader.cpp
    ./src/General/DebugManager.cpp
    ./src/General/TreeStruct.cpp
    ./src/General/ResourceManager.cpp
    
    # Audios
    ./src/Audios/AudioSystem.cpp
    ./src/Audios/SoundEvent.cpp
)
    
# 链接操作
target_link_libraries(${PROJECT_NAME}
    PUBLIC

    IMGUI
    SDL2
    SDL2_ttf
    freetype
    GLEW::GLEW
   
    ${PROJECT_SOURCE_DIR}/External/fmodstudioapi20300linux/api/core/lib/x86_64/libfmod.so
    ${PROJECT_SOURCE_DIR}/External/fmodstudioapi20300linux/api/fsbank/lib/x86_64/libfsbank.so
    ${PROJECT_SOURCE_DIR}/External/fmodstudioapi20300linux/api/studio/lib/x86_64/libfmodstudio.so

    ${PROJECT_SOURCE_DIR}/External/soil/build/lib/libSOIL.a
)