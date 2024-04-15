#include "mbed.h"
#include "nRF24L01P.h"
#include <algorithm>

using namespace std;

#define TRANSFER_SIZE   3  // 3 bytes na forma de 3 caracteres (3 bytes = 28 bits)
#define ACK_TIMEOUT 500000 //us

// Hardware
Serial pc(USBTX, USBRX); // tx, rx
nRF24L01P my_nrf24l01p(PA_7, PA_6, PA_5, PA_9, PC_7, PA_8);    // mosi, miso, sck, csn, ce, irq

// Incoming Package information
char rxData[TRANSFER_SIZE];
int rxDataCnt = 0;   //Bytes recebidos

// Outgoing Package information
int countAckg = 0;
char txData[TRANSFER_SIZE]={3, countAckg, 1};

// LOGIC
Timeout ackger;
bool timedOut = false; // Control TRUE when timeout
bool ackged = false;
int contador = 1;

void sendData(){
    // Update Count
    txData[1] = countAckg % 2;

    my_nrf24l01p.write(NRF24L01P_PIPE_P0, txData, TRANSFER_SIZE);
    pc.printf("Enviei %d, %d, %d pela %d vez \r\n", txData[0],txData[1],txData[2],contador);
    contador++;
}

void callbackTimeout() {
    pc.printf("Timed out!\r\n");
    timedOut = true;

    if (!ackged)
        sendData();
}

int main() {
    my_nrf24l01p.powerUp();   //Aciona a NRF24L01+

    my_nrf24l01p.setTransferSize( TRANSFER_SIZE );  //Determina tamanho do payload


    // Mostre as configurações 
    pc.printf( "nRF24L01+ Frequencia    : %d MHz\r\n",  my_nrf24l01p.getRfFrequency() );
    pc.printf( "nRF24L01+ Pot. de saida : %d dBm\r\n",  my_nrf24l01p.getRfOutputPower() );
    pc.printf( "nRF24L01+ Taxa          : %d kbps\r\n", my_nrf24l01p.getAirDataRate() );
    pc.printf( "nRF24L01+ End. TX       : 0x%010llX\r\n", my_nrf24l01p.getTxAddress() );
    pc.printf( "nRF24L01+ End. RX       : 0x%010llX\r\n", my_nrf24l01p.getRxAddress() );
    pc.printf( "nRF24L01+ CRC           : %d bits\r\n", my_nrf24l01p.getCrcWidth() );
    pc.printf( "nRF24L01+ Tam. Payload  : %d bytes\r\n", my_nrf24l01p.getTransferSize() );


    my_nrf24l01p.setReceiveMode();   //O TX mode é ativado quando se usar a função de escrita
    
    my_nrf24l01p.enable();
    
    
    while(true){

        // Read incoming data for starting
        if (pc.getc() == 't') { // pc.getc --> STOPS EXECUTION OF THREAD!
            sendData();
            ackged = false;
            contador = 1;
            wait(0.8);  // Espera 0.8s para transmitir   
        }

        while(!ackged) {
            timedOut = false;
            ackger.attach_us(&callbackTimeout, ACK_TIMEOUT);

            while (!ackged && !timedOut ) {
                while (!my_nrf24l01p.readable() ) { } // .readable() --> STOPS EXECUTION OF THREAD!

                // Leia o que está na fila de recepção
                rxDataCnt = my_nrf24l01p.read( NRF24L01P_PIPE_P0, rxData, sizeof( rxData ) );

                // Pacote para nosso endereço recebido
                if (rxData[0] == 3) {
                    pc.printf("Pacote recebido!!\r\n");
                    pc.printf("TRANSMITIDO: %d, %d, %d\r\n", txData[0], txData[1], txData[2]);
                    pc.printf("RECEBIDO: %d, %d, %d\r\n", rxData[0], rxData[1], rxData[2]);

                    // ACK recebido
                    ackged = (txData[0] == rxData[0] && txData[1] == rxData[1] && txData[2] == rxData[2]);                    
                }
            }

            ackger.detach();
        }

        pc.printf("ACK recebido!\r\n");
        countAckg++;
    }
};
