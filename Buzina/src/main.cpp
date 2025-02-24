#include "mbed.h"

const float lim = 0.55;

// Filtro #1
const float b01 = 0.0233738160 ;
const float b21 = -0.0233738160 ;
const float a21 = 0.9532523680 ;
const float a11 = -1.5397923347 ;
float f1(float x, float x2, float y1, float y2) {
    return b01 * x + b21 * x2 - a11 * y1 - a21 * y2;
}

// Filtro #2
const float b02 = 0.0616280836 ;
const float b22 = -0.0616280836 ;
const float a12 = -1.4794789527 ;
const float a22 = 0.8767438328 ;
float f2(float x, float x2, float y1, float y2) {
    return b02 * x + b22 * x2 - a12 * y1 - a22 * y2;
}

Ticker timer_amostra, timer_led;

DigitalOut led1(LED1);
AnalogIn analog_value(A0);
static UnbufferedSerial serial_port(USBTX, USBRX);

float x[3] = { 0 };

float y1[3] = { 0 };
float p1[2] = { 0 };

float y2[3] = { 0 };
float p2[2] = { 0 };

bool det = false;

void ProcessaAmostra() {

    // Memória
    p1[1] = p1[0];
    p2[1] = p2[0];
    for (int i = 2; i >= 1; i--) {
        x[i] = x[i - 1];
        y1[i] = y1[i - 1];
        y2[i] = y2[i - 1];
    }

    // Leitura X
    x[0] = analog_value.read(); // Converts and read the analog input value (value from 0.0 to 1.0)
    x[0] = x[0] - 0.5; // Elimina DC
    
    // Filtragem
    y1[0] = f1(x[0], x[2], y1[1], y1[2]);
    y2[0] = f2(x[0], x[2], y2[1], y2[2]);

    // Cálculo potência
    p1[0] = 0.999 * p1[1] + 0.001 * y1[0] * y1[0];
    p2[0] = 0.999 * p2[1] + 0.001 * y2[0] * y2[0];

    // Comparacao com limiar
    det = (p1[0] > lim*p2[0]) ? true : det;
}

void ChaveiaLed(){
    led1=!led1;
    printf("%1.2e, %1.2e \r",p1[0], lim*p2[0]);
    if (det){
        printf("Buzina detectada\r\n");
        det=false;
    }
}

int main() {
    printf("Detecao de buzina\n\r");
    printf("------------------\n\r");
    printf("\n\r");

    timer_amostra.attach(&ProcessaAmostra, 0.000125);
    timer_led.attach(&ChaveiaLed, 0.1);

    while(1){} // Forever loop
}
