cmake_minimum_required(VERSION 3.20)

project(EternalTerminal)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS
    Core
    Gui
    Quick
)


qt_standard_project_setup()


qt_add_executable(EternalTerminalApp
    src/main.cpp
    src/terminal/TerminalEngine.cpp
    src/terminal/TerminalEngine.h
    src/terminal/PtyProcess.cpp
    src/terminal/PtyProcess.h
    src/terminal/TerminalParser.cpp
    src/terminal/TerminalParser.h
    src/terminal/TerminalBuffer.cpp
    src/terminal/TerminalBuffer.h
    src/ui/TerminalRenderer.cpp
    src/ui/TerminalRenderer.h
    src/terminal/TerminalSelection.h
    src/terminal/TerminalSelection.cpp
)


qt_add_qml_module(EternalTerminalApp
    URI EternalTerminal
    VERSION 1.0
    QML_FILES
       src/Main.qml
       src/ui/TitleBar.qml
       src/ui/TerminalArea.qml
       src/ui/WindowButton.qml
)


target_link_libraries(EternalTerminalApp
    PRIVATE
        Qt6::Core
        Qt6::Gui
        Qt6::Quick
)
