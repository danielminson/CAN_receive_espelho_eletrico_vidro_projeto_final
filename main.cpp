#include "mbed.h"

DigitalOut IN_1(D5);
DigitalOut IN_2(D4);
DigitalOut EN_12(D2);
DigitalOut IN_3(A0);
DigitalOut IN_4(A1);
DigitalOut EN_34(A4);
DigitalOut B(D7);
InterruptIn botao(PC_13); // definição do botão de usuário

Serial pc(USBTX, USBRX);

PwmOut saida1(D9);

CAN can1(PB_8, PB_9);

static char info = 0x55;
static char left = 0x01;
static char right = 0x02;
static char up = 0x03;
static char down = 0x04;
static char stop = 0x05;
static char vidro_up = 0x06;
static char vidro_down = 0x07;
static char vidro_stop = 0x08;
static char marcha_lenta_1 = 0x09;
static char marcha_lenta_2 = 0x10;
static char marcha_lenta_3 = 0x11;

int tempo = 6000;
unsigned int estado;
int ig = 0;
float leitura;

int main() {
  pc.baud(9600); // definição da taxa de transferencia de dados do conector USB
  // 3 braços começam desacoplados
  IN_1 = 0;
  IN_2 = 0;
  IN_3 = 0;
  IN_4 = 0;
  EN_12 = 0;
  EN_34 = 0;
  estado = -1;
  CANMessage msg(5, &info, 8, CANData, CANStandard);
  CANMessage msg_marcha_lenta1(6, &marcha_lenta_1, 8, CANData, CANStandard);
  CANMessage msg_marcha_lenta2(6, &marcha_lenta_2, 8, CANData, CANStandard);
  CANMessage msg_marcha_lenta3(6, &marcha_lenta_3, 8, CANData, CANStandard);

  while (1) {
   
    if (estado == 0) {
    //    pc.printf("\n\rtamo lendo aqui po");

      if (can1.read(msg)) {
        // if (msg.data[0] == marcha_lenta_1) {
        if (msg.data[0] == marcha_lenta_1) {
            pc.printf("\n\rmudando pra estado 1");

          // move_up();
          estado = 1;
        }
        // if (msg.data[0] == marcha_lenta_2) {
        if (msg.data[0] == marcha_lenta_2) {
            pc.printf("\n\rmudando pra estado 2");
          estado = 2;
        }
        if (msg.data[0] == marcha_lenta_3) {
            pc.printf("\n\rmudando pra estado 3");
          // move_up();
          estado = 3;
        }
      }
      msg.data[0] = 0x00; // limpa a variavel
    }
    if (estado == -1) {
      pc.printf("\n\rcarro desligado! RPM = 0. Deslocamento linear: 0 mm");

      for (int i = 0; i < 60; ++i) {
        // sequencia de acionamento de cada braço com diferentes polaridades
        // seguidas de desacoplamento, fazendo uma sequencia que permite que o
        // motor funcione atráves de 6 passos step1
        pc.printf("\n\rvoltando");

        EN_12 = 1;
        EN_34 = 0;
        IN_1 = 1;
        IN_2 = 0;

        wait_us(tempo);

        IN_1 = 0;
        IN_2 = 1;

        wait_us(tempo);
        EN_12 = 0;
        EN_34 = 1;
        IN_3 = 1;
        IN_4 = 0;

        wait_us(tempo);
        IN_3 = 0;
        IN_4 = 1;
        wait_us(tempo);
      }
      pc.printf("\n\rvoltou");

      IN_1 = 0;
      IN_2 = 0;
      IN_3 = 0;
      IN_4 = 0;
      EN_12 = 0;
      EN_34 = 0;
      estado = 0;
    }

    if (estado == 1) { //
      pc.printf("\n\rtemperatura: GELADO. RPM = 1200. Pressão: 0.6 bar. "
                "Deslocamento linear: 10 mm");
      saida1.period(0.0125f); // 4 second period
      saida1.write(0.50f);

      for (int i = 0; i < 60; ++i) {
        // sequencia de acionamento de cada braço com diferentes polaridades
        // seguidas de desacoplamento, fazendo uma sequencia que permite que o
        // motor funcione atráves de 6 passos step1
        pc.printf("\n\rvoltando");

        EN_12 = 1;
        EN_34 = 0;
        IN_1 = 1;
        IN_2 = 0;

        wait_us(tempo);

        IN_1 = 0;
        IN_2 = 1;

        wait_us(tempo);
        EN_12 = 0;
        EN_34 = 1;
        IN_3 = 1;
        IN_4 = 0;

        wait_us(tempo);
        IN_3 = 0;
        IN_4 = 1;
        wait_us(tempo);
      }
      pc.printf("\n\rvoltou");

      IN_1 = 0;
      IN_2 = 0;
      IN_3 = 0;
      IN_4 = 0;
      EN_12 = 0;
      EN_34 = 0;
      estado = 0;
    }

    if (estado == 2) { //
      ////saida1.period(0.015f);      // 4 second period
      // saida1.write(0.50f);
      pc.printf("\n\rtemperatura: QUENTE. RPM = 1000. Pressão: 0.2 bar. "
                "Deslocamento linear: 0 mm");

      for (int i = 0; i < 60; ++i) { // dentro
        // sequencia de acionamento de cada braço com diferentes polaridades
        // seguidas de desacoplamento, fazendo uma sequencia que permite que o
        // motor funcione atráves de 6 passos step1
        pc.printf("\n\rvoltando");

        EN_12 = 1;
        EN_34 = 0;
        IN_1 = 1;
        IN_2 = 0;

        wait_us(tempo);

        IN_1 = 0;
        IN_2 = 1;

        wait_us(tempo);
        EN_12 = 0;
        EN_34 = 1;
        IN_3 = 1;
        IN_4 = 0;

        wait_us(tempo);
        IN_3 = 0;
        IN_4 = 1;
        wait_us(tempo);
      }
      pc.printf("\n\rvoltou");

      for (int i = 0; i < 45; ++i) { // fora
        pc.printf("\n\rfora");

        // sequencia de acionamento de cada braço com diferentes polaridades
        // seguidas de desacoplamento, fazendo uma sequencia que permite que o
        // motor funcione atráves de 6 passos step1
        EN_12 = 0;
        EN_34 = 1;
        IN_3 = 0;
        IN_4 = 1;

        wait_us(tempo);

        IN_3 = 1;
        IN_4 = 0;

        wait_us(tempo);
        EN_34 = 0;
        EN_12 = 1;

        IN_1 = 0;
        IN_2 = 1;

        wait_us(tempo);

        IN_1 = 1;
        IN_2 = 0;

        wait_us(tempo);
      }
      IN_1 = 0;
      IN_2 = 0;
      IN_3 = 0;
      IN_4 = 0;
      EN_12 = 0;
      EN_34 = 0;
      estado = 0;
    }

    if (estado == 3) {
      // saida1.period(0.0136f);      // 4 second period
      // saida1.write(0.50f);
      pc.printf("\n\rtemperatura: LIGARAM O AR. RPM = 1100. Pressão: 0.4 bar. "
                "Deslocamento linear: 4 mm");
      for (int i = 0; i < 60; ++i) { // dentro total
        pc.printf("\n\rvoltando");

        EN_12 = 1;
        EN_34 = 0;
        IN_1 = 1;
        IN_2 = 0;

        wait_us(tempo);

        IN_1 = 0;
        IN_2 = 1;

        wait_us(tempo);
        EN_12 = 0;
        EN_34 = 1;
        IN_3 = 1;
        IN_4 = 0;

        wait_us(tempo);
        IN_3 = 0;
        IN_4 = 1;
        wait_us(tempo);
      }
      for (int i = 0; i < 24; ++i) {
        pc.printf("\n\rfora");
        // sequencia de acionamento de cada braço com diferentes polaridades
        // seguidas de desacoplamento, fazendo uma sequencia que permite que o
        // motor funcione atráves de 6 passos step1
        EN_12 = 0;
        EN_34 = 1;
        IN_3 = 0;
        IN_4 = 1;

        wait_us(tempo);

        IN_3 = 1;
        IN_4 = 0;

        wait_us(tempo);
        EN_34 = 0;
        EN_12 = 1;

        IN_1 = 0;
        IN_2 = 1;

        wait_us(tempo);

        IN_1 = 1;
        IN_2 = 0;

        wait_us(tempo);
      }
      pc.printf("\n\racabou");
      IN_1 = 0;
      IN_2 = 0;
      IN_3 = 0;
      IN_4 = 0;
      EN_12 = 0;
      EN_34 = 0;
      estado = 0;
    }
    wait_us(20000); // controla o envio/recebimento de mensagens
    msg.data[0] = 0x00; // limpa a variavel
  }
}