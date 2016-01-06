//@ Instucciones de inicio @//
#include <18F4550.H>
#device ADC = 10
#FUSES NOWDT,NOBROWNOUT,NOLVP,NOXINST
#use delay(crystal = 20Mhz)
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
int r_datos(void);
void corte_maquina(void);
void motor_s(int,char,char);
char sensor_start(void);
void cut_start(void);
