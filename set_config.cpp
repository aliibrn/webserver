#include "webserver.hpp"

void    set_serve(ServerConfig& server){
    server.setHost("127.0.0.1");
    server.setPort(8080);
    server.setServerName("localhost");
    server.setRoot("/var/www/html");
    server.setIndex("index.html");
    server.setErrorPage(404, "/errors/404.html");
    server.setErrorPage(500, "/errors/500.html");
    server.setClientMaxBodySize(10 * 1024 * 1024); // 10MB

    // Route 1: "/"
    RouteConfig route1;
    route1.setPath("/");
    route1.addMethod("GET");
    route1.addMethod("POST");
    route1.setRoot("/var/www/html");
    route1.setDirectoryListing(false);
    server.addRoute(route1);

    // Route 2: "/upload"
    RouteConfig route2;
    route2.setPath("/upload");
    route2.addMethod("POST");
    route2.setRoot("/var/www/uploads");
    route2.setUploadEnabled(true);
    server.addRoute(route2);

    // Route 3: "/redirect"
    RouteConfig route3;
    route3.setPath("/redirect");
    route3.addMethod("GET");
    route3.setRedirect("https://example.com");
    server.addRoute(route3);

    // Print the stored configuration
    //server.printServerConfig();
}