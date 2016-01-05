#include <Sensores_3.h>


Void main() {
  /* code */

    setup_adc_ports(AN0_TO_AN8);
    setup_adc(ADC_CLOCK_DIV_2);
    while (TRUE) {
      /* code */
      char opcion;
      opcion = getc();

      switch (opcion) {
        case 'x': //ENVIA LAS LECTURAS DE LOS SENSORES
        {
          //printf("Enviando datos...");
          sampling_ADC();

          break;
        }
        case 'c': // CALIBRA LA MAQUINA MANDANDOLA AL ORIGEN
        {
          //printf("Calibrando...");
          calibracion();
          break;
        }
        case 'r': // RECIVE LOS DATOS DE LA PC
        {
          int16 repeat, tiempo_motor;
          repeat = r_datos();
          for (int i = 0; i < repeat; i++) {

            tiempo_motor = corte_maquina(tiempo_motor);
          }
            break;
        }
        case 's':{ // PREPARA PARA EMPEZAR A SENSAR
          char regreso;
          regreso = sensor_start();

          printf("%c",regreso);
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
int16 r_datos(void){
  printf("ya");
  char bucle[2];
  for (int i = 0; i < 2; i++) {
    /* code */
    bucle[i] = getc();
  }
  int16 repeat;
  repeat = bucle[0] | bucle[1] >> 8;
  printf("%c",bucle[0]);
  printf("%c",bucle[1]);
  printf("%Lu",repeat);
  return repeat;
}
int16 corte_maquina(int16){

output_toggle(PIN_D2);
delay_ms(500);



}
void motor_s(int num_motor,char estado,char direccion){

  switch (num_motor) {
    case 1:{ // ELEGIR MOTOR 1
      switch (estado) {
        case 'I':{ // SELECCIONA ON
            switch (direccion) {
              case '>':{ // DIRECCION ADELANTE
                output_high(motor_pin_1);
                output_low(motor_pin_2);
                break;
              }
              case '<':{ // DIRECCION ATRAS
                output_high(motor_pin_2);
                output_low(motor_pin_1);
                break;
              }
            }
          break;
        }
        case 'O':{ // SELECCIONA OFF
          output_low(motor_pin_1);
          output_low(motor_pin_2);

          break;
        }
      }
      break;
    }
    case 2:{ // ELEGIR MOTOR 2
      switch (estado) {
        case 'I':{ // SELECCIONA ON
            switch (direccion) {
              case '>':{ // DIRECCION ADELANTE
                output_high(motor_pin_3);
                output_low(motor_pin_4);
                break;
              }
              case '<':{ // DIRECCION ATRAS
                output_high(motor_pin_4);
                output_low(motor_pin_3);
                break;
              }
            }
          break;
        }
        case 'O':{ // SELECCIONA OFF
          output_low(motor_pin_3);
          output_low(motor_pin_4);
          break;
        }
      }
      break;
    }
  }
}
char sensor_start(void){

  do {
    motor_s(1,'I','>');
  } while((input(limit_sensor)) != 1);
  motor_s(1,'O',' ');
  return 'n';
}
