/********************************************************************************
* @Author : hexne
* @Date   : 2025/07/24 00:52:22
********************************************************************************/
module;
#include <cstdio>
#include <utility>
export module command;

import std;

export class Command {
    std::string command_;
    std::string command_out_;
    std::shared_ptr<FILE> pfile_{};
public:
    explicit Command(std::string command) : command_(std::move(command)) {  }

    std::string run() {
        pfile_.reset(popen(command_.data(),"r"));
        if (!pfile_)
            throw std::runtime_error("popen() failed!");
        char buffer[512] = "";
        while(fgets(buffer,sizeof(buffer),pfile_.get()))
            command_out_ += buffer;
        return command_out_;
    }
    static std::string run(std::string command) {
        Command cmd(std::move(command));
        return cmd.run();
    }

};