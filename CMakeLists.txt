cmake_minimum_required(VERSION 3.16)
project(RPG_Game)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)

# ----- 编译器 -----
set(CMAKE_CXX_COMPILER "C:/Qt/Tools/mingw1120_64/bin/g++.exe")
set(CMAKE_C_COMPILER   "C:/Qt/Tools/mingw1120_64/bin/gcc.exe")

# ----- Qt -----
set(CMAKE_PREFIX_PATH "C:/Qt/6.7.3/mingw_64")

# ----- SFML -----
set(SFML_ROOT "C:/SFML/SFML-2.6.1")
set(SFML_INCLUDE_DIR "${SFML_ROOT}/include")
set(SFML_LIBRARY_DIR "${SFML_ROOT}/lib")

# ----- tmxlite -----
set(TMXLITE_INCLUDE_DIR "C:/tmxlit/tmxlite-master/tmxlite/include")

list(APPEND CMAKE_PREFIX_PATH "${SFML_ROOT}")
find_package(SFML 2.6 COMPONENTS graphics window system REQUIRED)

# 收集 tmxlite 的所有 .cpp 和 .c 源文件
file(GLOB_RECURSE TMXLITE_SOURCES "C:/tmxlit/tmxlite-master/tmxlite/src/*.cpp" "C:/tmxlit/tmxlite-master/tmxlite/src/*.c")

# ----- Qt6 -----
find_package(Qt6 REQUIRED COMPONENTS Widgets)

add_definitions(-DTMXLITE_STATIC)
add_definitions(-DTMXLITE_USE_MINIZ)

# ----- 源文件 -----
add_executable(RPG_Game
    main.cpp
    MainWindow.cpp
    MainWindow.h
    Character.cpp
    Character.h
    Enemy.cpp
    Enemy.h
    GameManager.cpp
    GameManager.h
    Item.cpp
    Item.h
    Shop.cpp
    Shop.h
    Task.cpp
    Task.h
    Utils.cpp
    Utils.h
    Map.cpp
    Map.h
    SFMLMapWindow.cpp
    SFMLMapWindow.h
    EditCharacterDialog.cpp
    EditCharacterDialog.h
    BattleDialog.cpp
    BattleDialog.h
    TaskDetailDialog.cpp
    TaskDetailDialog.h
    ShopDialog.cpp
    ShopDialog.h
    InventoryDialog.cpp
    InventoryDialog.h
    ${TMXLITE_SOURCES}
)

# ----- 链接库和包含目录 -----
target_link_libraries(RPG_Game
    Qt6::Widgets
    sfml-graphics
    sfml-window
    sfml-system
    z
)

target_include_directories(RPG_Game PRIVATE
    ${SFML_INCLUDE_DIR}
    ${TMXLITE_INCLUDE_DIR}
)

# 添加库目录（帮助链接器找到 .a 或 .dll.a）
target_link_directories(RPG_Game PRIVATE ${SFML_LIBRARY_DIR})

# ----- GUI 模式 -----
# set_target_properties(RPG_Game PROPERTIES LINK_FLAGS "-mwindows")