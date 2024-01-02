cmake_minimum_required(VERSION 3.0)
project(CustomGame)
find_package(OpenGL REQUIRED)

# set(UNICODE ON)
# set(_UNICODE ON)
set(CMAKE_CXX_STANDARD 17) 
set(CMAKE_CXX_FLAGS "-finput-charset=utf-8")  
set(CMAKE_C_FLAGS "-finput-charset=utf-8")  

# 如果想要生成的可执行文件拥有符号表，可以gdb调试，就直接加上这句
add_definitions("-Wall -g")

#是否使用fmod studio，加载bank文件的形式
add_definitions(-D USE_FMOD_STUDIO) 

# 生成可执行文件
include_directories(include)
include_directories(${PROJECT_SOURCE_DIR}/External/imgui)
include_directories(${PROJECT_SOURCE_DIR}/External/soil/inc/SOIL)
include_directories(${PROJECT_SOURCE_DIR}/External/glew-2.2.0-win32/glew-2.2.0/include/GL)
include_directories(${PROJECT_SOURCE_DIR}/External/freetype-windows-binaries/include)
include_directories(${PROJECT_SOURCE_DIR}/External/FMOD/FMOD_Studio_API_Windows_1.09.09/api/studio/inc)
include_directories(${PROJECT_SOURCE_DIR}/External/FMOD/FMOD_Studio_API_Windows_1.09.09/api/fsbank/inc)
include_directories(${PROJECT_SOURCE_DIR}/External/FMOD/FMOD_Studio_API_Windows_1.09.09/api/lowlevel/inc)
include_directories(${PROJECT_SOURCE_DIR}/External/glfw-3.3.8.bin.WIN32/glfw-3.3.8.bin.WIN32/include/GLFW)
include_directories(${PROJECT_SOURCE_DIR}/External/RapidJson/rapidjson-1.1.0/rapidjson-1.1.0/include/rapidjson)
include_directories(${PROJECT_SOURCE_DIR}/External/SDL2/SDL2-devel-2.0.22-mingw/SDL2-2.0.22/i686-w64-mingw32/include/SDL2)
include_directories(${PROJECT_SOURCE_DIR}/External/SDL2/SDL2_ttf-devel-2.20.2-mingw/SDL2_ttf-2.20.2/i686-w64-mingw32/include/SDL2)
include_directories(${PROJECT_SOURCE_DIR}/External/SDL2/SDL2_image-devel-2.6.3-mingw/SDL2_image-2.6.3/i686-w64-mingw32/include/SDL2)

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
    ${PROJECT_SOURCE_DIR}/External/glfw-3.3.8.bin.WIN32/glfw-3.3.8.bin.WIN32/lib-mingw/glfw3.dll
    ${PROJECT_SOURCE_DIR}/External/glew-2.2.0-win32/glew-2.2.0/lib/Release/Win32/glew32.lib
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
    ./src/General/Cmath.cpp
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
    ${PROJECT_SOURCE_DIR}/External/SDL2/SDL2-devel-2.0.22-mingw/SDL2-2.0.22/i686-w64-mingw32/bin/SDL2.dll
    ${PROJECT_SOURCE_DIR}/External/SDL2/SDL2_image-devel-2.6.3-mingw/SDL2_image-2.6.3/i686-w64-mingw32/bin/SDL2_image.dll
    ${PROJECT_SOURCE_DIR}/External/SDL2/SDL2_ttf-devel-2.20.2-mingw/SDL2_ttf-2.20.2/i686-w64-mingw32/bin/SDL2_ttf.dll
    ${PROJECT_SOURCE_DIR}/External/soil/build/libsoil.dll
    
    ${PROJECT_SOURCE_DIR}/External/FMOD/FMOD_Studio_API_Windows_1.09.09/api/lowlevel/lib/fmod.dll
    ${PROJECT_SOURCE_DIR}/External/FMOD/FMOD_Studio_API_Windows_1.09.09/api/fsbank/lib/fsbank.dll
    ${PROJECT_SOURCE_DIR}/External/FMOD/FMOD_Studio_API_Windows_1.09.09/api/studio/lib/fmodstudio.dll
    ${PROJECT_SOURCE_DIR}/External/freetype-windows-binaries/release_dll/win32/freetype.dll
)

link_libraries(${PROJECT_NAME}
    ${PROJECT_SOURCE_DIR}/External/FMOD/FMOD_Studio_API_Windows_1.09.09/api/lowlevel/lib/fmod_vc.lib 
    ${PROJECT_SOURCE_DIR}/External/FMOD/FMOD_Studio_API_Windows_1.09.09/api/studio/lib/fmodstudio_vc.lib
)