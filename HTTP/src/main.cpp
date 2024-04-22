#include "mbed.h"
#include <HttpRequest.h>

using namespace std;

/* Mbed v6 - STM Nucleo_F072RB */
BufferedSerial buff_serial(USBTX, USBRX);

int main()
{
    HttpRequest request("GET", "/update?api_key=XRYSRVZBER1146KC&field1=77&field2=120", "HTTP/1.1", "184.106.153.149");
    
    while (1) {
        request.print(buff_serial);
        
        ThisThread::sleep_for(1s);
    }

    return 0;
}
