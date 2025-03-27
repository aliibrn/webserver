#include "webserver.hpp"

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>



int main() {
    ServerConfig server;
    set_serve(server);
    int server_fd, new_socket;
    struct sockaddr_in address;
    socklen_t addrlen = sizeof(address);
    char buffer[1024];
    
    // 1. Create a socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == 0) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }
    
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(server.getPort()); 

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 3) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }
    
    std::cout << CYAN << "Server listening on port " << COLOR_RESET << RED << server.getPort() << "...\n" << COLOR_RESET;
    
    while (true) {
        std::cout << YELLOW << "Waiting for a new connection...\n" << COLOR_RESET;
        
        new_socket = accept(server_fd, (struct sockaddr *)&address, &addrlen);
        if (new_socket < 0) {
            perror("Accept failed");
            continue;
        }
        
        std::cout << GREEN << "Client connected!\n" << COLOR_RESET;
        read(new_socket, buffer, sizeof(buffer));
        HttpRequest HttpRequest;
        HttpRequest.parseRequest(buffer);
        const char *http_response = 
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/html\r\n"
            "Content-Length: 19\r\n"  // Length of the message below
            "\r\n"
            "waslatna chokrane";  // Body content
            send(new_socket, http_response, strlen(http_response), 0);
            
        close(new_socket);
    }

    return 0;
}