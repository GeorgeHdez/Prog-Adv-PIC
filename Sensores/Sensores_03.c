#include <Sensores_3.h>
const int stepper_motor[4] = {0x08,0x04,0x02,0x01};

Void main() {
  /* code */

    setup_adc_ports(AN0_TO_AN8);
    setup_adc(ADC_CLOCK_DIV_2);
    while (TRUE) {
      /* code */
      char opcion;
      opcion = getc();

      switch (opcion) {
        case 'x':
        {
          //printf("Enviando datos...");
          sampling_ADC();

          break;
        }
        case 'c':
        {
          //printf("Calibrando...");
          calibracion();
          break;
        }
        case 'b':
        {
          bucle_stepper_motor();

          break;
        }
        case 'r':
        {
          r_datos();
          break;
        }
        default:
        {
          printf("Letra No Especificada");
          delay_ms(1000);
          break;
        }
      }
    }

}
void send_info_rs232(int16 a, int16 b, int16 c, int16 d, int16 e){ // @ Envia los datos a la computadora en forma de caracteres @//
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
void sampling_ADC(){ //@ Realiza las mediciones que existan en el puerto ADC @//

  int16 medicion[5];

  for(int j = 0; j <= 4; j++)
    {
      /*if (j==4) //Cambiar a AN8
      {
         j=8;
         set_adc_channel(j);
         delay_us(20);
         medicion[j-4] = read_adc();
      }
      */
      //else
      //{
         set_adc_channel(j);
         delay_us(20);
         medicion[j] = read_adc();
        // }
    }

    send_info_rs232(medicion[0],medicion[1],medicion[2],medicion[3],medicion[4]);
}
void calibracion(){

    int1 limit_1 = 1,limit_2 = 1;
    do {

       if(limit_1 == 1){
         if(input(PIN_D0)){
           output_low(PIN_D2);
           limit_1 = 0;
         }
         else{
           output_high(PIN_D2);
           bucle_stepper_motor();
         }
       }
       if(limit_2 == 1){
         if(input(PIN_D1)){
           output_low(PIN_D4);
           limit_2 = 0;
         }
         else{
           output_high(PIN_D4);
         }
        }
    } while((limit_1 != 0)||(limit_2 != 0));

    char end = 'c';

    printf("%c",end);
}
void bucle_stepper_motor(void){

    for(int i = 0 ; i<4 ; i++){
      output_b(stepper_motor[i]);
      delay_ms(30);
    }

}
void r_datos(void){
  output_high(PIN_D2);
  char bucle[6];
  for (int i = 0; i < 6; i++) {
    /* code */
    scanf("%c\n",bucle[i]);
  }

}
