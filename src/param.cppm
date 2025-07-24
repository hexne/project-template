/*******************************************************************************
 * @Author : hexne
 * @Data   : 2024/11/19 21:15
*******************************************************************************/
export module args_parser;
import std;


template <typename T>
constexpr bool is_string_v = std::is_same_v<std::remove_cvref_t<T>, std::string>
                || std::is_same_v<char *, std::decay_t<T>>
                || std::is_same_v<char *, std::add_pointer_t<std::remove_const_t<std::remove_pointer_t<std::decay_t<T>>>>>;

template <typename T>
constexpr bool is_support_type_v = is_string_v<T> || std::is_invocable_v<T>;

// 现在仅支持参数选项，不能获取参数传递的值
// -cmake=4.0.0
// test_project ..
// cpp-template -eq --cmake-version=4.0.0 test_template .
// cpp-template -eq -cmake-version=4.0.0 test_template .
// 新的参数类型应该是tuple<std::string param, std::tuple<bool is_enable, std::string arg_val>>
//     可以用param[]获取内容，
//     返回tuple<bool, std::string>

struct Arg {
    std::vector<std::string> args;
    std::function<void()> callback;
    bool enable{};
    std::string value;
};

export class ArgsParser {
    std::vector<Arg> support_args_; // 支持的参数，通过add 添加
    std::vector<std::string> program_args_;


    Arg &find_arg(const std::string& find_arg) {
        for (auto &arg : support_args_) {
            auto &[args, _, _, _] = arg;
            // 查找是否是短指令 或 长指令
            if (std::ranges::find(args, find_arg) != std::ranges::end(args))
                return arg;

            // 查找是否是传递的键值对类型的值
            for (const auto& cur_args : args ) {
                if (cur_args.find(find_arg) != std::string::npos)
                    return arg;
            }
        }
        // 都没有找到
        throw std::invalid_argument("No such argument: " + find_arg);
    }

public:

    ArgsParser(int argc, char *argv[]) {

        // 对于短指令

        // 对于长指令

        // 对于键值对的值

        // 对于不是键值对的值

        // 先直接存储，不进行解析
        for (int i = 1;i < argc; ++i) {
            program_args_.emplace_back(argv[i]);
        }
    }

    // 如果最后一个参数是callback, 那么作为开关选项
    // 如果最后一个参数是字符串，那么作为值选项
    void add_args(auto&&... args) requires (is_support_type_v<decltype(args)> && ...) {
        auto tuple = std::forward_as_tuple(args...);
        auto back = std::get<sizeof...(args) - 1>(tuple);

        if constexpr (is_string_v<decltype(back)>) {

            // clangd 对该代码错误进行报错
            // (support_args_.emplace_back(Arg{.args = {args}}), ...);
            auto lam = [&](std::string arg) {
                support_args_.emplace_back(Arg{.args = {arg }});
            };
            (lam(args), ...);
        }
        else if constexpr (std::is_invocable_v<decltype(back)>) {
            // 最后一个参数是回调函数，存入 callback
            std::cout << "最后一个参数是lambda" << std::endl;
        }
    }
    // 通过参数获取值或是否开关
    std::string operator[](std::string arg) {
        return {};
    }

    // 获取单独参数的的接口


    // 解析
    void parser() {
        for (auto &arg : program_args_) {
            std::cout << arg << std::endl;
        }
    }

};
