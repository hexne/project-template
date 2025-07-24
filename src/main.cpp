// /********************************************************************************
// * @Author : hexne
// * @Date   : 2025/07/24 00:31:05
// ********************************************************************************/
import std;
import args_parser;
import command;

struct Flags {
    bool qt{};
    bool exe{};
    bool module = true;
    bool static_lib{};
    bool dynamic_lib{};
} flags;

std::string get_cmake_version() {
    auto cmake_version_out = Command::run("cmake --version");
    std::vector<int> version_num(3);
    if (std::sscanf(cmake_version_out.data(), "cmake version %d.%d.%d",
               &version_num[0], &version_num[1], &version_num[2]) != 3)
        throw std::runtime_error("get cmake version error");

    std::string ret;
    for (int i = 0;i < version_num.size() - 1; ++i)
        ret += std::to_string(version_num[i]) + ' ';
    ret += std::to_string(version_num.back());
    return ret;
}

/**
 * @note 只能使用cmake 4.0.0 及以上版本, 且是按照系统cmake版本进行获取的
**/
std::string enable_import_std() {
    auto cmake_version = get_cmake_version();
    std::string ret;
    if (cmake_version == "4.0.0" || cmake_version == "4.0.1" || cmake_version == "4.0.2") {
        ret = R"(set(CMAKE_EXPERIMENTAL_CXX_IMPORT_STD "a9e1cf81-9932-4810-974b-6eccaf14e457"))";
    }
    else if (cmake_version == "4.0.3") {
        ret = R"(set(CMAKE_EXPERIMENTAL_CXX_IMPORT_STD "d0edc3af-4c50-42ea-a356-e2862fe7a444"))";
    }
    else {
        throw std::runtime_error("cmake version is unsupported");
    }
    ret += "set(CMAKE_CXX_MODULE_STD 1)\n";
    return ret;
}

void create_cmake_file() {
    std::string cmake_file_contents;
    cmake_file_contents += "cmake_minimum_required(VERSION 4.0.0)\n";
    cmake_file_contents += "set(CMAKE_CXX_STANDARD 23)\n";
    cmake_file_contents += enable_import_std();
    cmake_file_contents += "project(untitled)\n";

    if (flags.exe) {
        cmake_file_contents += "add_executable(${PROJECT_NAME}\n";
        cmake_file_contents += "    src/main.cpp\n";
        cmake_file_contents += ")";
    }
    else if (flags.static_lib) {
        cmake_file_contents += "add_library(${PROJECT_NAME} STATIC)\n";
        cmake_file_contents += "target_sources(${PROJECT_NAME} PRIVATE\n";
        cmake_file_contents += "    src/lib.cpp\n";
        cmake_file_contents += ")\n";
    }
    else if (flags.dynamic_lib) {
        cmake_file_contents += "add_library(${PROJECT_NAME} SHARED)\n";
        cmake_file_contents += "target_sources(${PROJECT_NAME} PRIVATE\n";
        cmake_file_contents += "    src/lib.cpp\n";
        cmake_file_contents += ")\n";
    }
}

void create_main_file() {


}

int main(int argc, char *argv[]) {

    ArgsParser args_parser(argc, argv);
    args_parser.parser();
    args_parser.add_args("123", "123");
    args_parser.add_args("123", "123", [] {

    });
    // param.add_support_arg("-a", "--all", [] {
    //     std::cout << "all" << std::endl;
    // });
    //
    // param.add_support_arg("-e", "--exe", [&] {
    //     flags.exe = true;
    // });
    // param.add_support_arg("-m", "--module", [&] {
    //     flags.module = true;
    // });
    // param.add_support_arg("-s", "--static_lib", [&] {
    //     flags.static_lib = true;
    // });
    // param.add_support_arg("-d", "--dynamic_lib", [&] {
    //     flags.dynamic_lib = true;
    // });
    // param.add_support_arg("-q", "--qt", [&] {
    //     flags.qt = true;
    // });
    // param.add_support_arg("-h", "--help", [&] {
    //     std::cout << "param : \n"
    //               << "";
    // });
    // param.add_support_val("cmake-version");
    //
    // param.analyze();




    return 0;
}


