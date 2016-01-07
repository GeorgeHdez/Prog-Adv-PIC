#include <Sensores_3.h>


Void main() {
  /* code */
    setup_oscillator(OSC_8MHZ|OSC_INTRC);
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
          motor_s(1,'I','>');
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
          int repeat ;
          int32 time_motor;
          int32 milimetro = 2000;
          r_datos(&repeat,&time_motor);
          cut_start(time_motor);
          output_b(0x00);
          on_cortadora();
          for (int i = 0; i < repeat; i++) {
           corte_maquina(milimetro);
          }
          motor_s(1,'O',' ');
          motor_s(2,'O',' ');
          off_cortadora();
          break;
        }
        case 's':{ // PREPARA PARA EMPEZAR A SENSAR
          char regreso;
          regreso = sensor_start();
          printf("%c",regreso);
          break;
        }
        case 'o':{
          motor_s(1,'O',' ');
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
      unsigned char data[10];
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
   {/*
      if (j==4) //Cambiar a AN8
      {
         j=8;
         set_adc_channel(j);
         delay_us(20);
         medicion[j-4] = read_adc();
      }

      else
      {*/
         set_adc_channel(j);
         delay_us(20);
         medicion[j] = read_adc();
        }
  //  }

    send_info_rs232(medicion[0],medicion[1],medicion[2],medicion[3],medicion[4]);
}
void calibracion(){

    int1 limit_1 = 1,limit_2 = 1;
    do {

       if(limit_1 == 1){
         if(input(PIN_D0)){
           motor_s(1,'O','>');
           limit_1 = 0;
         }
         else{
           motor_s(1,'I','<');

         }
       }
       if(limit_2 == 1){
         if(input(PIN_D1)){
           motor_s(2,'O','>');
           limit_2 = 0;
         }
         else{
           motor_s(2,'I','<');
         }
        }
    } while((limit_1 != 0)||(limit_2 != 0));

    char end = 'c';

    printf("%c",end);
}
void r_datos(int *repeat, int32 *time_motor){
  char bucle[2];
    for (int i = 0; i < 2;  i++) {
      bucle[i] = getc();
    }
    *repeat = bucle[0];
    *time_motor = bucle[1];
}
void corte_maquina(int32 times){
  do {
    motor_s(1,'I','>');
  } while((input(l_2)) != 1);

  motor_s(1,'O',' ');
  motor_s(2,'I','A');
  delay_ms(times);
  motor_s(2,'O',' ');
  do {
    motor_s(1,'I','<');
  } while((input(l_1)) != 1);
  motor_s(1,'O',' ');
  motor_s(2,'I','V');
  delay_ms(times + times);
  motor_S(2,'O',' ');
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
              case 'A':{ // DIRECCION ADELANTE
                output_high(motor_pin_3);
                output_low(motor_pin_4);
                break;
              }
              case 'V':{ // DIRECCION ATRAS
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
void cut_start(int16 time_motor){
  int avance_motor = 400;
  do {
    motor_s(1,'I','<');
  } while((input(l_1)) != 1);
   motor_s(1,'O',' ');
   motor_s(2,'I','V');
   delay_ms(time_motor * avance_motor);
   motor_s(2,'O',' ');

}
void on_cortadora(void){

  output_high(PIN_B4);
}
void off_cortadora(void){

  output_low(PIN_B4);
}
