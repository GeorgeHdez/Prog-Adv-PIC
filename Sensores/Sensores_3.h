//@ Instucciones de inicio @//
#include <18F4550.H>
#device ADC = 10
#FUSES INTRC_IO,NOWDT,NOBROWNOUT,NOLVP,NOXINST
#use delay(internal = 8Mhz)
#use rs232(baud=115200,parity=N,xmit=PIN_C6,rcv=PIN_C7,bits=8)
#DEFINE l_1 PIN_D0
#DEFINE l_2 PIN_D1
#DEFINE limit_cort PIN_D2
#DEFINE limit_sensor PIN_D3
#DEFINE motor_pin_1 PIN_B0
#DEFINE motor_pin_2 PIN_B1
#DEFINE motor_pin_3 PIN_B2
#DEFINE motor_pin_4 PIN_B3

//@ Prototipos de Funciones @//

void ADC();
void sampling_ADC(void);
void calibracion(void);
void send_info_rs232(int16,int16,int16,int16,int16);
void r_datos(int*,int32*);
void corte_maquina(int32);
void motor_s(int,char,char);
char sensor_start(void);
void cut_start(int16);
void on_cortadora(void);
void off_cortadora(void);
