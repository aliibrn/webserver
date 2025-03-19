#ifndef WEBSERVER_HPP
#define WEBSERVER_HPP

#include <sys/socket.h>
#include <iostream>
#include <string>
#include <fstream>
#include <cstring>
#include <vector>
#include <map>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>

class RouteConfig
{
private:
    std::string path;
    std::vector<std::string> methods;
    std::string root;
    bool directory_listing;
    bool upload_enabled;
    std::string redirect;

public:
    RouteConfig() : directory_listing(false), upload_enabled(false) {}

    void setPath(const std::string &p) { path = p; }
    void addMethod(const std::string &method) { methods.push_back(method); }
    void setRoot(const std::string &r) { root = r; }
    void setDirectoryListing(bool enable) { directory_listing = enable; }
    void setUploadEnabled(bool enable) { upload_enabled = enable; }
    void setRedirect(const std::string &r) { redirect = r; }

    std::string getPath() const { return path; }
    std::vector<std::string> getMethods() const { return methods; }
    std::string getRoot() const { return root; }
    bool isDirectoryListing() const { return directory_listing; }
    bool isUploadEnabled() const { return upload_enabled; }
    std::string getRedirect() const { return redirect; }

    void printRouteConfig() const
    {
        std::cout << "Path: " << path << "\n";
        std::cout << "Methods: ";
        for (size_t i = 0; i < methods.size(); ++i)
        {
            std::cout << methods[i] << (i + 1 < methods.size() ? ", " : "\n");
        }
        std::cout << "Root: " << root << "\n";
        std::cout << "Directory Listing: " << (directory_listing ? "Enabled" : "Disabled") << "\n";
        std::cout << "Upload Enabled: " << (upload_enabled ? "Yes" : "No") << "\n";
        if (!redirect.empty())
        {
            std::cout << "Redirect: " << redirect << "\n";
        }
        std::cout << "----------------------\n";
    }
};

class ServerConfig
{
private:
    std::string host;
    int port;
    std::string server_name;
    std::string root;
    std::string index;
    std::map<int, std::string> error_pages;
    int client_max_body_size;
    std::vector<RouteConfig> routes;

public:
    ServerConfig() : port(8080), client_max_body_size(10 * 1024 * 1024) {}

    void setHost(const std::string &h) { host = h; }
    void setPort(int p) { port = p; }
    void setServerName(const std::string &name) { server_name = name; }
    void setRoot(const std::string &r) { root = r; }
    void setIndex(const std::string &i) { index = i; }
    void setErrorPage(int code, const std::string &page) { error_pages[code] = page; }
    void setClientMaxBodySize(int size) { client_max_body_size = size; }
    void addRoute(const RouteConfig &route) { routes.push_back(route); }

    std::string getHost() const { return host; }
    int getPort() const { return port; }
    std::string getServerName() const { return server_name; }
    std::string getRoot() const { return root; }
    std::string getIndex() const { return index; }
    int getClientMaxBodySize() const { return client_max_body_size; }
    std::vector<RouteConfig> getRoutes() const { return routes; }

    void printServerConfig() const
    {
        std::cout << "Host: " << host << "\n";
        std::cout << "Port: " << port << "\n";
        std::cout << "Server Name: " << server_name << "\n";
        std::cout << "Root: " << root << "\n";
        std::cout << "Index File: " << index << "\n";
        std::cout << "Max Body Size: " << client_max_body_size << " bytes\n";

        std::cout << "Error Pages:\n";
        for (std::map<int, std::string>::const_iterator it = error_pages.begin(); it != error_pages.end(); ++it)
        {
            std::cout << "  " << it->first << " -> " << it->second << "\n";
        }

        std::cout << "Routes:\n";
        for (size_t i = 0; i < routes.size(); ++i)
        {
            routes[i].printRouteConfig();
        }
    }
};

void    set_serve(ServerConfig& server);

#endif