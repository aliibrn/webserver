#include <iostream>
#include <vector>
#include <cctype>

#define BLACK "\033[30m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN "\033[36m"
#define WHITE "\033[37m"

#define COLOR_RESET "\033[0m"

void split_line(const std::string &buffer, std::vector<std::string> &words)
{
    std::string word_to_push;
    int i = 0;

    while (i < buffer.size())
    {
        if (std::isspace(buffer[i]) || buffer[i] == '\r' || buffer[i] == '\n')
        {
            words.push_back(std::string(1, buffer[i])); 
            i++;
        }
        else
        {
            word_to_push.clear();
            while (buffer[i] && buffer[i] != ' ' && buffer[i] != '\r' && buffer[i] != '\n')
            {
                word_to_push += buffer[i];
                i++;
            }
            words.push_back(word_to_push);
        }
    }
}

int main()
{
    std::string buffer = "  GET / HTTP/1.1\r\n";
    std::vector<std::string> vstart_line;

    split_line(buffer, vstart_line);
    std::cout << YELLOW;
    for(int i = 0; i < vstart_line.size(); i++){
        if(vstart_line[i] == "\r")
            std::cout << "CNRL\n";
        else if(vstart_line[i] == "\n")
            std::cout << "NL\n";
        else
            std::cout << "$" << vstart_line[i] << "$" << std::endl;
    }
    std::cout << "size: "<< vstart_line.size() << COLOR_RESET << std::endl;
}