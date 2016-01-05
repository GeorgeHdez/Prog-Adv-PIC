#include <18f4550.h>
#device ADC=10
#fuses HS,NOPROTECT,NOWDT,NOLVP
#use delay(clock=20M)
#use RS232(rcv=pin_c7,xmit=pin_c6,baud=115200,bits=8,parity=N)

/*#int_rda
void pc_command()
{
   if( kbhit())
      button = getc();
}
*/

void sampling_ADC();
void send_info_rs232(int16 , int16 , int16 , int16 , int16 );

void main()
{

 enable_interrupts(int_rda);
 enable_interrupts(global);

  //Configuracion de ADC
  setup_adc_ports(AN0_TO_AN8);
  setup_adc(ADC_CLOCK_DIV_64);
  
  char medicion;

  while(TRUE)
  {
         medicion = getc();
         
         
        if(medicion == 'x'){
        sampling_ADC();
        medicion  = 'z';
        }
        
         
     
  }
}
void send_info_rs232(int16 a, int16 b, int16 c, int16 d, int16 e){

    char data[10];
    int x;

    data[0] = a;
    data[1] = a>>8;
    data[2] = b;
    data[3] = b>>8;
    data[4] = c;
    data[5] = c>>8;
    data[6] = d;
    data[7] = d>>8;
    data[8] = e;
    data[9] = e>>8;

    for(x = 0 ; x<10 ; x++){

      printf("%c",data[x]);

    }
}
void sampling_ADC()
{
  int16 medicion[5];

  for(int j = 0; j <= 8; j++)
    {
      if (j==4) //Cambiar a AN8
      {
         j=8;
         set_adc_channel(j);
         delay_us(20);
         medicion[j-4] = read_adc();
      }

      else
      {
         set_adc_channel(j);
         delay_us(20);
         medicion[j] = read_adc();
         }
    }

    send_info_rs232(medicion[0],medicion[1],medicion[2],medicion[3],medicion[4]);
}
