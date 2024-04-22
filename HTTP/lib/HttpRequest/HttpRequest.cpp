#include "HttpRequest.h"

/**
 * @brief Construct a new Http Request object
 * 
 * @param method HTTP method of the request (aka. GET, POST...)
 * @param path URI path of the request.
 * @param version HTTP version.
 * @param host Hostname of the server.
 */
HttpRequest::HttpRequest(char *method, char *path, char *version, char *host)
    : method(method), path(path), version(version), host(host) 
    { }

/**
 * @brief Print the request to the serial.
 * 
 * @param pc The BufferedSerial to print the request.
 */
void HttpRequest::print(BufferedSerial &buff_serial) {
    buff_serial.write((uint8_t*)this->toString(), strlen(this->toString()));
}

/**
 * @brief Convert the request to a string.
 * 
 * @return char* The request as a string.
 */
char* HttpRequest::toString() {
    char* buffer = new char[108];

    snprintf(buffer, 108, "Request: %s %s %s\r\nHost: %s\r\n\r\n", this->method, this->path, this->version, this->host);
    
    return buffer;
}
c