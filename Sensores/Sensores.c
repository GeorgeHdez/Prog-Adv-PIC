#include <18f4550.h>
#device ADC=10
#fuses INTRC,XT,NOPROTECT,NOWDT,NOLVP
#use delay(CLOCK=8M)
#use RS232(rcv=pin_c7,xmit=pin_c6,baud=115200,bits=8,parity=N)

const float tconv=(100.0*5.0)/1024.0;

void sampling_ADC();

void main()
{
  int16 data = 0;

  //Habilitar Oscilador interno
  setup_oscillator(OSC_8MHZ,OSC_INTRC);

  //Configuración de ADC
  setup_adc_ports(AN0_TO_AN8);
  setup_adc(ADC_CLOCK_INTERNAL);

  while(TRUE)
  {
     sampling_ADC();
     printf("\n\n");
  }
}

void sampling_ADC()
{
  float data[50][6];

  for(int i = 0; i <= 49; i++)
  {
    for(int j = 0; j <= 8; j++)
    {
      if (j==4) //Cambiar a AN8
      {
         j=8;
         set_adc_channel(j);
         delay_us(20);
         data[i][j-3] = read_adc();
   
         printf("%g\t",tconv*data[i][j-3]);
         delay_us(10);
      }
      
      else
      {
         set_adc_channel(j);
         delay_us(20);
         data[i][j] = read_adc();
   
         printf("%g\t",tconv*data[i][j]);
         delay_us(10);
         }
    }
    
    delay_us(10);
    printf("\r\n");
    
  }
}
