#ifndef AXEL_HTTP_GET_H
#define AXEL_HTTP_GET_H

#include <string>

namespace webutil {
/**
* @internal
* Function to run HTTP Get to the specified host, service and path
* This function is used to run a HTTPS GET request using Boost Beast.
*
*
* @param host - The hostname or IP address of the server
* @param service - The service name or port number
* @param path - The path on the server
*
* @return The body of the HTTP response from the server as a string
*/
    std::string
    https_get(const std::string& host, const std::string& service, const std::string& path);
}

#endif //AXEL_HTTP_GET_H
