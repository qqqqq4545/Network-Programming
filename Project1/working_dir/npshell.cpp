#include <iostream>
#include <string>
#include <cstring>
#include <unistd.h>
#include <vector>
#include <sys/wait.h>
#include <stdlib.h>

#define MAX_LINE_LENGTH 15000
#define MAX_CMD_LENGTH 256

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
        
        if (input == "exit") {
            break;
        }
        
        std::cout << "Unkwon Command：" << input << std::endl;
    }
    
    return 0;
}
