cmake_minimum_required(VERSION 3.10)

project(DataStructure_Cxx)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories("ADTs")
include_directories("Common")
include_directories("Interactor")

file(GLOB_RECURSE DataStructureCxxIncludeFiles
    "Common/*.h"
    "Interactor/*.h"
)

file(GLOB_RECURSE DataStructureCxxAdtsSourceFiles "ADTs/*.hpp")

if(BuildTest)
    add_definitions(-D BuildTest)
    add_executable(DSCxx_InteractorTest
        Main.cpp
        "Interactor/Interactor.cpp"
        ${DataStructureCxxIncludeFiles}
        "Test/Test.hpp"
    )
else()
    add_executable(DSCxx_Interactor
        Main.cpp
        "Interactor/Interactor.cpp"
        ${DataStructureCxxIncludeFiles}
        ${DataStructureCxxAdtsSourceFiles}
    )
endif()