#ifndef HTTPREQUEST_H
#define HTTPREQUEST_H

#include "mbed.h"
#include <string>

class HttpRequest {
public:
    char *method;
    char *path;
    char *version;
    char *host;

    HttpRequest(char *method, char *path, char *version, char *host);
    void print(BufferedSerial &buff_serial);
    char* toString();

};

#endif // HTTPREQUEST_H