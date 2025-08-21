/********************************************************************************
* @Author : hexne
* @Date   : 2025/07/24 00:31:05
********************************************************************************/

import std;
import args_parser;
import command;
import config;
import create_cmake_file;

ArgsParser args_parser;

std::string get_project_name() {
    auto name_arg = args_parser["project_name"];
    if (name_arg.enable)
        return name_arg.value;
    if (args_parser.other_arg().empty())
        return "untitled";
    return args_parser.other_arg().front();
}
std::filesystem::path get_project_path() {
    auto path_arg = args_parser["project_path"];
    if (path_arg.enable)
        return path_arg.value;
    if (args_parser.other_arg().size() < 2)
        return ".";
    return args_parser.other_arg()[1];
}
int get_cxx_version() {
    if (args_parser["cxx-version"].enable)
        return std::stoi(args_parser["cxx-version"].value);
    return std::stoi("23");
}

std::string AddComment(const std::string & content) {
    std::string ret, line;
    while (std::getline(std::cin, line))
        ret += "# " + line;
    return ret;
}

/**
 * @note 只能使用cmake 4.0.0 及以上版本, 且是按照系统cmake版本进行获取的
**/
std::string enable_import_std() {
    if (args_parser["--multiple_cmake"].enable) {
        return  "if(CMAKE_VERSION VERSION_LESS 4.0.3)\n"
                "    set(CMAKE_EXPERIMENTAL_CXX_IMPORT_STD \"a9e1cf81-9932-4810-974b-6eccaf14e457\")\n"
                "elseif(CMAKE_VERSION VERSION_EQUAL 4.0.3)\n"
                "    set(CMAKE_EXPERIMENTAL_CXX_IMPORT_STD \"d0edc3af-4c50-42ea-a356-e2862fe7a444\")\n"
                "else()\n"
                "    message(WARNING \"Unsupported CMake version: ${CMAKE_VERSION}\")\n"
                "endif()\n"
                "\n";
    }
    // auto cmake_version = get_cmake_version();
    std::string cmake_version;
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

void create_module_file(const std::string &file) {
    std::ofstream out(file);
    out << std::string {
        #embed "module-file-template.txt"
    };
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

    if (project_config.is_module)
        create_module_file(project_dir / "src/module.cpp");
}

// project-name
//     src/
//     include/(非module模式下创建)
void create_project_struct() {
    auto project_dir = project_config.project_path / project_config.project_name;
    if (std::filesystem::exists(project_dir))
        throw std::runtime_error(project_dir.string() + " already exists");

    std::filesystem::create_directory(project_dir);
    std::filesystem::create_directory(project_dir / "src");
    if (!project_config.is_module)
        std::filesystem::create_directory(project_dir / "include");
}

void create_project() {
    create_project_struct();
    create_project_files();
}

int main(int argc, char *argv[]) {
    args_parser = ArgsParser(argc, argv);

    args_parser.add_args("-m", "--module", [] {
        project_config.is_module = true;
    });
    args_parser.add_args("-e", "--exe", [] {
        project_config.is_exe = true;
    });
    args_parser.add_args("--sl", "--static",[] {
        project_config.is_static_lib = true;
    });
    args_parser.add_args("--dl", "--dynamic",[] {
        project_config.is_shared_lib = true;
    });
    args_parser.add_args("-g", "--github",[] {
        project_config.enable_github = true;
    });

    args_parser.add_args("-h", "--help",[] {
        std::cout <<"-m --module, this project enable c++ module\n"
                    "-e --exe, this project is exe\n"
                    "-g --github, this project is a github repo\n"
                    "--sl --static, this project is static lib\n"
                    "--dl --dynamic, this project is dynamic lib\n"
                    "-project_name=xxx, project name\n"
                    "-project_path=xxx, project path\n"
                    "-cmake_version=xxx, cmake version\n"
                    "-cxx_version=xxx, cxx version(default is 23)\n"
                    "-depend=xx,xx,xxx, depends is xxx";
        std::exit(0);
    });
    args_parser.add_args("project_name");
    args_parser.add_args("project_path");
    args_parser.add_args("cxx_version");
    args_parser.add_args("cmake_version");
    args_parser.add_args("depend");

    args_parser.parser();
    project_config.project_name = get_project_name();
    project_config.project_path = get_project_path();
    project_config.cxx_version = get_cxx_version();
    if (args_parser["cmake_version"].enable)
        project_config.cmake_version = args_parser["cmake_version"].value;

    create_project();

    return 0;
}

