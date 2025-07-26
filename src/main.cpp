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
    bool github_project{};
} flags;

ArgsParser args_parser;

std::string get_cmake_version() {
    if (args_parser["cmake-version"].enable)
        return args_parser["cmake-version"].value;

    auto cmake_version_out = Command::run("cmake --version");
    std::vector<int> version_num(3);
    if (std::sscanf(cmake_version_out.data(), "cmake version %d.%d.%d",
               &version_num[0], &version_num[1], &version_num[2]) != 3)
        throw std::runtime_error("get cmake version error");

    std::string ret;
    for (int i = 0;i < version_num.size() - 1; ++i)
        ret += std::to_string(version_num[i]) + '.';
    ret += std::to_string(version_num.back());
    return ret;
}

std::string get_project_name() {
    if (args_parser.other_arg().empty())
        return "untitled";
    return args_parser.other_arg().front();
}
std::filesystem::path get_project_path() {
    if (args_parser.other_arg().size() < 2)
        return ".";
    return args_parser.other_arg()[1];
}
std::string get_cxx_version() {
    if (args_parser["cxx-version"].enable)
        return args_parser["cxx-version"].value;
    return "23";
}

/**
 * @note 只能使用cmake 4.0.0 及以上版本, 且是按照系统cmake版本进行获取的
**/
std::string enable_import_std() {
    auto cmake_version = get_cmake_version();
    std::string ret;
    if (cmake_version == "4.0.0" || cmake_version == "4.0.1" || cmake_version == "4.0.2") {
        ret = R"(set(CMAKE_EXPERIMENTAL_CXX_IMPORT_STD "a9e1cf81-9932-4810-974b-6eccaf14e457"))";
        ret += '\n';
    }
    else if (cmake_version == "4.0.3") {
        ret = R"(set(CMAKE_EXPERIMENTAL_CXX_IMPORT_STD "d0edc3af-4c50-42ea-a356-e2862fe7a444"))";
        ret += '\n';
    }
    else {
        throw std::runtime_error("cmake version is unsupported");
    }
    ret += "set(CMAKE_CXX_MODULE_STD 1)\n";
    return ret;
}

void create_main_file(const std::string &file) {
    std::ofstream out(file);
    out << std::string {
        #embed "main-template.txt"
    };
}

void create_cmake_file(const std::string &file) {
    std::ofstream out(file);


    std::string cmake_file_contents;
    cmake_file_contents +=  "cmake_minimum_required(VERSION 4.0.0)\n"
                            "set(CMAKE_CXX_STANDARD " + get_cxx_version() + ")\n" +
                            enable_import_std() + "\n"
                            "project(" + get_project_name() + ")\n\n";

    cmake_file_contents += std::string {
         #embed "module-template.txt"
    };

    if (flags.exe) {
        cmake_file_contents +=  "add_executable(${PROJECT_NAME}\n"
                                "    src/main.cpp\n"
                                ")\n\n";
    }


    cmake_file_contents +=  "link_directories(${PROJECT_SOURCE_DIR}/lib)\n"
                            "# target_link_libraries(${PROJECT_NAME}\n"
                            "#     module\n"
                            "# )\n";

    out << cmake_file_contents;
}

// project-name/src/main.cpp
// project-name/CMakeLists.txt
// project-name/.gitignore(github 选项)
// project-name/LICENSE(github 选项)
// project-name/README.md（github 选项）
void create_project_files() {
    auto project_dir = get_project_path() / get_project_name();

    create_main_file(project_dir / "src/main.cpp");
    create_cmake_file(project_dir / "CMakeLists.txt");

}

// project-name
//     src/
void create_project_struct() {

    auto project_dir = get_project_path() / get_project_name();
    // @TODO 测试文件是否生成正确
    // if (std::filesystem::exists(project_dir))
        // throw std::runtime_error(project_dir.string() + " already exists");

    std::filesystem::create_directory(project_dir);
    std::filesystem::create_directory(project_dir / "src");
}


void create_project() {
    create_project_struct();
    create_project_files();
}

int main(int argc, char *argv[]) {
    args_parser = ArgsParser(argc, argv);

    args_parser.add_args("-e", "--exe", [&] {
        flags.exe = true;
    });
    args_parser.add_args("-m", "--module", [&] {
        flags.module = true;
    });
    args_parser.add_args("-s", "--static_lib", [&] {
        flags.static_lib = true;
    });
    args_parser.add_args("-d", "--dynamic_lib", [&] {
        flags.dynamic_lib = true;
    });
    args_parser.add_args("-q", "--qt", [&] {
        flags.qt = true;
    });
    args_parser.add_args("-g", "--github", [&] {
        flags.github_project = true;

    });
    args_parser.add_args("-h", "--help", [&] {
        std::cout << "args_parser : \n"
                     "";
    });
    args_parser.add_args("cmake-version", "cxx-version");
    
    args_parser.parser();

    create_project();

    return 0;
}


