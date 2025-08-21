/********************************************************************************
* @Author : hexne
* @Date   : 2025/08/21 14:14:23
********************************************************************************/
export module config;
import std;
import args_parser;

export struct Config {
    bool is_module; // -m --module
    bool is_exe;    // -e --exe
    bool is_static_lib; // --sl --static
    bool is_shared_lib; // --dl --dynamic_lib
    bool enable_github; // -g --gihub
    int cxx_version; // -cxx_version=
    std::string project_name; // -project_name=
    std::filesystem::path project_path; // -project_path=
    std::string cmake_version; // -cmake_version
    std::vector<std::string> depend; // -depend=qt,opencv
} config;

// export Config project_config;