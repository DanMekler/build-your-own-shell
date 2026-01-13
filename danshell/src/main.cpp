#include <iostream>
#include <string>
#include <unistd.h>
#include <vector>
#include <sys/wait.h>
#include <sstream>

std::vector<std::string> tokenize(const std::string& str) {
    std::stringstream ss(str);
    std::string token;
    std::vector<std::string> tokens;

    while (ss >> token) {
        tokens.push_back(token);
    }

    return tokens;
}

int main() {
    std::string input;

    while (true) {
        std::cout << "$ ";

        if (!std::getline(std::cin, input)) {
            std::cout << std::endl;
            break;
        }
        std::vector<std::string> args = tokenize(input);

        //builtins:
        if (input.empty())
            continue;
        if (input == "exit")
            break;
        if (args[0] == "cd") {
            if (args.size() != 2) {
                std::cout << "cd: missing argument" << std::endl;
            } else {
                if (chdir(args[1].c_str()) != 0) {
                    std::cout << "cd failed" << std::endl;
                }
            }
            continue;
        }

        pid_t pid = fork();
        if (pid == -1) {
            std::cout << "fork failed\n";
            return 1;
        }
        if (pid == 0) {
            std::vector<char*> c_args;
            for (auto& arg : args) {
                c_args.push_back(&arg[0]);
            }
            c_args.push_back(nullptr);

            execvp(c_args[0], c_args.data());
            return 1;
        }
        wait(nullptr);
    }
    return 0;
}