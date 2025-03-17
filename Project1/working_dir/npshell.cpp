#include <iostream>
#include <string>
#include <cstring>
#include <unistd.h>
#include <vector>
#include <sys/wait.h>
#include <stdlib.h>
#include <sstream>

#define MAX_LINE_LENGTH 15000
#define MAX_CMD_LENGTH 256

// 將字串分割成tokens
std::vector<std::string> split_string(const std::string& str, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream token_stream(str);
    
    while (std::getline(token_stream, token, delimiter)) {
        if (!token.empty()) {
            tokens.push_back(token);
        }
    }
    
    return tokens;
}

bool handle_builtin_command(const std::string& command, const std::vector<std::string>& args) {
    if (command == "exit") {
        exit(0);
        return true;
    }
    
    if (command == "printenv") {
        if (args.size() == 1) {
            return true;
        }
        
        char* env_value = getenv(args[1].c_str());
        if (env_value != nullptr) {
            std::cout << env_value << std::endl;
        }
        return true;
    }
    
    if (command == "setenv") {
        if (args.size() < 3) {
            return true;
        }
        
        setenv(args[1].c_str(), args[2].c_str(), 1);
        return true;
    }
    
    return false;
}

void print_unknown_command(const std::string& command) {
    std::cerr << "Unknown command: [" << command << "]." << std::endl;
}

bool execute_command(const std::string& command, const std::vector<std::string>& args) {
    // 先檢查命令是否存在
    // 之後再做外部命令的東西
    
    char* path_env = getenv("PATH");
    if (path_env == nullptr) {
        return false;
    }
    
    std::vector<std::string> paths = split_string(std::string(path_env), ':');
    
    for (const auto& path : paths) {
        std::string full_path = path + "/" + command;
        if (access(full_path.c_str(), X_OK) == 0) {
            return true;
        }
    }
    
    print_unknown_command(command);
    return false;
}

void show_prompt() {
    std::cout << "% ";
    std::cout.flush();
}

void initialize_environment() {
    setenv("PATH", "bin:.", 1);
}

int main() {
    std::string input;
    char line[MAX_LINE_LENGTH];
    
    initialize_environment();
    
    while (true) {
        show_prompt();
        
        if (!fgets(line, MAX_LINE_LENGTH, stdin)) {
            std::cout << std::endl;
            break;
        }
        
        size_t len = strlen(line);
        if (len > 0 && line[len-1] == '\n') {
            line[len-1] = '\0';
        }
        
        input = std::string(line);
        
        if (input.empty()) {
            continue;
        }
        
        std::vector<std::string> args = split_string(input, ' ');
        
        if (args.empty()) {
            continue;
        }
        
        std::string command = args[0];
        
        if (handle_builtin_command(command, args)) {
            continue;
        }
        
        // 嘗試執行外部命令
        execute_command(command, args);
    }
    
    return 0;
}