#include "HttpRequest.hpp"

HttpRequest::HttpRequest()
{
}

HttpRequest::~HttpRequest()
{
}

void    HttpRequest::print_vector(std::vector<std::string>& vec){
    std::cout << YELLOW;
    for(int i = 0; i < vec.size(); i++){
        if(vec[i] == "\r")
            std::cout << "CNRL\n";
        else if(vec[i] == "\n")
            std::cout << "NL\n";
        else
            std::cout << "$" << vec[i] << "$" << std::endl;
    }
    std::cout << "size: "<< vec.size() << COLOR_RESET << std::endl;
}

void    HttpRequest::split_buffer(const std::string &buffer, std::vector<std::string>& lines){
    int start = 0;

    for (int i = 0; i < buffer.size(); i++) {
        if (buffer[i] == '\n') {
            lines.push_back(buffer.substr(start, i - start + 1));
            start = i + 1;
        }
    }
    if (start < buffer.size()) {
        lines.push_back(buffer.substr(start));
    }
}

void    HttpRequest::storethebuffer(const std::string &buffer)
{
    split_buffer(buffer, lines);
}

void HttpRequest::split_line(const std::string &buffer, std::vector<std::string> &words)
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

bool HttpRequest::validstartline(std::vector<std::string> &vstart_line){
    if(vstart_line.size() != 7)
        return false;
    if(vstart_line[0] != "GET" && vstart_line[0] != "POST" && vstart_line[0] != "DELETE")
        return false;
    if(vstart_line[1] != " ")
        return false;
    if(vstart_line[2][0] != '/')
        return false;
    if(vstart_line[3] != " ")
        return false;
    if(vstart_line[4] != "HTTP/1.1" && vstart_line[4] != "HTTP/1.0")
        return false;
    if(vstart_line[5] != "\r")
        return false;
    if(vstart_line[6] != "\n")
        return false;
    return true;
}

void HttpRequest::start_line(){
    // METHODE SP /url SP version /CRNL/n
    std::cout << CYAN << "START LINE: $" << lines[0] << COLOR_RESET;
    std::vector<std::string>vstart_line;
    split_line(lines[0], vstart_line); // mura raytsplita line , rantchekiw forma wach hiya hadik
    
    // print_vector(vstart_line);    
    
    if(!validstartline(vstart_line))
    {
           std::cout << RED << "INVALID START LINE" << COLOR_RESET << std::endl;
           throw std::exception();
    }
    tstart_line.method = vstart_line[0];
    tstart_line.url = vstart_line[2];
    tstart_line.version = vstart_line[4];
    std::cout << GREEN << "VALID START LINE" << COLOR_RESET << std::endl;
    std::cout << "METHOD: " << tstart_line.method << std::endl;
    std::cout << "URL: " << tstart_line.url << std::endl;
    std::cout << "VERSION: " << tstart_line.version << std::endl;
}

bool validheadername(const std::string &name) {
    if (name.empty())  
        return false; // Header name cannot be empty

    for (size_t i = 0; i < name.size(); i++) {
        if (!std::isalnum(name[i]) && name[i] != '-') 
            return false; // Only allow letters, numbers, and `-`
    }

    return true;
}


bool HttpRequest::validheader(const std::vector<std::string> &vheader) {
    if (vheader.size() != 2)  
        return false; // Must be exactly "Header-Name: value" format

    if (!validheadername(vheader[0])) 
        return false; // Invalid header name

    mheaders[vheader[0]] = vheader[1]; // Store header
    return true;
}

void HttpRequest::split_header(const std::string &buffer, std::vector<std::string> &words) {
    size_t pos = buffer.find(":");

    if (pos == std::string::npos || pos == 0) {
        words.clear();  // ❌ No `:` or header name missing → Invalid header
        return;
    }

    // Extract header name (must not have leading spaces)
    std::string header_name = buffer.substr(0, pos);

    // Check for spaces before `:` (invalid)
    if (header_name.find(" ") != std::string::npos) {
        words.clear();  // ❌ Reject headers with spaces before `:`
        return;
    }

    // Extract header value and remove leading space if present
    std::string header_value = buffer.substr(pos + 1);
    if (!header_value.empty() && header_value[0] == ' ')  
        header_value.erase(0, 1);  // ✅ Trim leading space from value

    words.clear();
    words.push_back(header_name);
    words.push_back(header_value);
}

void HttpRequest::print_map(const std::map<std::string, std::string> &m) {
    std::cout << YELLOW;
    for (std::map<std::string, std::string>::const_iterator it = m.begin(); it != m.end(); ++it) {
        std::cout << it->first << " => " << it->second;
    }
    std::cout << COLOR_RESET;
}


void    HttpRequest::headers(){
    std::cout << CYAN << "HEADERS: $" << lines[1] << COLOR_RESET;
    std::vector<std::string> vheaders;
    for(int i = 1; i < lines.size(); i++){
        if(lines[i] == "\r\n")
            break;
        split_header(lines[i], vheaders);
        validheader(vheaders);
        vheaders.clear();
    }
    std::cout << GREEN << "VALID HEADERS" << COLOR_RESET << std::endl;
    print_map(mheaders);
}

void HttpRequest::parseRequest(const std::string &buffer) {
    try{
        storethebuffer(buffer);
        start_line();
        headers();
    }
    catch(std::exception &e)
    {

    }
}
