#include "mbed.h"
#include "nRF24L01P.h"

// #include <algorithm>
// #include <list>

// #include <Package.h>

using namespace std;

// Hardware
Serial pc(USBTX, USBRX); // tx, rx
nRF24L01P my_nrf24l01p(PA_7, PA_6, PA_5, PA_9, PC_7, PA_8);    // mosi, miso, sck, csn, ce, irq


// TODO: Protocolo stop-and-wait

// mandar pkg --> esperar pra ver se tem resposta
// se receber akg --> esperar ele enviar um pacote
// se não receber ack --> reenviar o pacote --> até receber akg

#define TRANSFER_SIZE 3 // 28 bits


void retransmit_ack() {
    
}

int main() {

    // Create new package to send
    OutgoingPackages.push_back(new Package({}, 0, {3, 21, 32}, ));


    // See if there is any new data incoming



    // Num de bytes tx = num de bytes rx
    char rxData[TRANSFER_SIZE];
    int rxDataCnt = 0;   //Bytes recebidos

    char txData[TRANSFER_SIZE]=;
    int txDataCnt = TRANSFER_SIZE;
    unsigned int contador=0;
    
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
    
    
    while(1){
        // Read incoming data for starting
        if (pc.getc() == 't') {

            // Envia o conteúdo de txData via nRF24L01+
            my_nrf24l01p.write( NRF24L01P_PIPE_P0, txData, txDataCnt);
            contador++;
                
            pc.printf("Enviei %d, %d, %d pela %d vez \r\n", txData[0],txData[1],txData[2],contador);
                
            wait(0.8);  //Espera 0.8s para transmitir   
        }

        bool valid_response = false;
        bool ack_recived = false;

        ackger.attach_us(&retransmit_ack, ACK_TIMEOUT);

        while (!valid_response) {

            // Pooling: Verifica se a NRF24L01+ recebeu algo
            while ( !my_nrf24l01p.readable() ) { }

            // Leia o que está na fila de recepção
            rxDataCnt = my_nrf24l01p.read( NRF24L01P_PIPE_P0, rxData, sizeof( rxData ) );

            if (rxData[0] == 3) {
                valid_response = true;
             
                // Mostre os 3 bytes recebidos através da terminal conectado na porta serial
                pc.printf("Valores recebidos: %d, %d, %d\r\n", rxData[0],rxData[1],rxData[2]);
            }
        }
        
        ackger.detach();   
    }
}
