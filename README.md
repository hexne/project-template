# project-template

| 短指令 | 长指令              | 含义                                          |
|-----|------------------|---------------------------------------------|
| -e  | --exe            | 项目生成exe                                     |
| -s  | --static         | 项目生成静态库                                     |
| -d  | --dy             | 项目生成动态库                                     |
| -m  | --module         | 项目内使用module                                 |
| -q  | --qt             | 生成qt exe项目                                  |
| -g  | --github         | 生成更完整的项目结构，包括.gitignore, LICENSE, README.md |
| /   | --multiple_cmake | 项目是否需要支持多个cmake版本                           |


| 值             | 含义                                            |
|---------------|-----------------------------------------------|
| cmake_version | 使用的cmake版本，影响CMakeLists.txt中开启import std的uuid |
| cxx_version   | 项目使用的c++版本                                    |
