/********************************************************************************
* @Author : hexne
* @Date   : 2025/08/21 13:46:15
********************************************************************************/

export module create_cmake_file;
import std;
import config;



void create_root_cmake_file(const std::filesystem::path &root_path) {
    std::ofstream out(root_path / "CMakeLists.txt");
    out << "cmake_minimum_required(VERSION 3.30.0)\n"
        << "set(CMAKE_CXX_STANDARD " + std::to_string(project_config.cxx_version) + ")";

    if (project_config.is_module) {
        out <<  "if(CMAKE_VERSION VERSION_GREATER_EQUAL 3.30.0 AND CMAKE_VERSION VERSION_LESS_EQUAL 3.31.7)\n"
                "    set(CMAKE_EXPERIMENTAL_CXX_IMPORT_STD \"0e5b6991-d74f-4b3d-a41c-cf096e0b2508\")\n"
                "elseif(CMAKE_VERSION EQUAL 3.31.8)\n"
                "    set(CMAKE_EXPERIMENTAL_CXX_IMPORT_STD \"d0edc3af-4c50-42ea-a356-e2862fe7a444\")\n"
                "elseif(CMAKE_VERSION VERSION_GREATER_EQUAL 4.0.0 AND CMAKE_VERSION VERSION_LESS_EQUAL 4.0.2)\n"
                "    set(CMAKE_EXPERIMENTAL_CXX_IMPORT_STD \"a9e1cf81-9932-4810-974b-6eccaf14e457\")\n"
                "elseif(CMAKE_VERSION VERSION_GREATER_EQUAL 4.0.3 AND CMAKE_VERSION VERSION_LESS_EQUAL 4.1.0)\n"
                "    set(CMAKE_EXPERIMENTAL_CXX_IMPORT_STD \"d0edc3af-4c50-42ea-a356-e2862fe7a444\")\n"
                "endif()\n"
                "set(CMAKE_CXX_MODULE_STD 1)\n";
    }
    out <<  "project(" + project_config.project_name + ")"
            "add_subdirectory(src)";
}
void create_src_cmake_file(const std::filesystem::path &src_path) {
    std::ofstream out(src_path / "CMakeLists.txt");
    std::string lib_type, module_name;
    if (project_config.is_exe) {
        module_name = "module";
    }
    else if (project_config.is_shared_lib) {
        lib_type = "SHARED";
        module_name = "${PROJECT_NAME}";
    }
    else if (project_config.is_static_lib) {
        lib_type = "STATIC";
        module_name = "${PROJECT_NAME}";
    }

    if (project_config.is_module) {
        out <<  "file(GLOB module_files \"*.cppm\")\n"
                "add_library(" + module_name + " " + lib_type +")\n"
                "target_sources(${PROJECT_NAME}\n"
                "    PUBLIC\n"
                "    FILE_SET cxx_modules TYPE CXX_MODULES FILES\n"
                "    ${module_files}\n"
                ")\n";
        if (project_config.is_exe) {
            out <<  "add_executable(${PROJECT_NAME}\n"
                    "    src/main.cpp\n"
                    ")\n"
                    "target_link_libraries(${PROJECT_NAME}\n"
                    "    module\n"
                    ")\n";
        }
    }
    else {

    }

}
export void create_cmake_file(const std::filesystem::path &root_path) {
    create_root_cmake_file(root_path);
    create_src_cmake_file(root_path / "src");
}
