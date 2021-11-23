unsigned contagem;
void DELAY_ms(unsigned int ms_Count)
{
    unsigned int i,j;
    for(i=0;i<ms_Count;i++)
    {
        for(j=0;j<1000;j++);
    }
}

void interrupt(){
     if(INTCON.INTF==1){
           TRISD = 0x00;
           PORTD = 0xa1;  //exibe a letra d no display de 7 seg
           DELAY_ms(5000);  //delay de 5s
           INTCON.INTF = 0; //limpa o flag.
     }
     if(INTCON.TMR0IF==1){
          contagem++;       //para cada interrpção do TMR0 incrementa a variável contagem
          TMR0 = 0;
          INTCON.TMR0IF = 0;  //limpa o overflow.
     }
}

int main() {
    char seg_code[]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90};
    int i, j;
    INTCON.GIE =1; //interrupcao geral
    INTCON.INTE=1; //interrupcao INT
    INTCON.TMR0IE = 1; //habilita interrupção do TMR0;

    TMR0 = 0; // TIMER0 inicia com o valor 0;
    OPTION_REG = 0b10000010; // Modo Temporizador, prescaler 1:8;
    //tempo = 4/(8*10**6 )* 8 * 256 = 1ms
    /* Configure the ports D as output */
    TRISD = 0x00;
    i=0;
    while (1)
    {
      PORTD = seg_code[i];
      if(PORTC.RC0==1){
        if(contagem == 1000){
            contagem = 0;
            i++;
            if(i==10){
              i=0;
              PORTB=1;
              INTCON.INTF=1;
            }
        }

      }else{
        if(contagem == 1000){
              contagem = 0;
              i++;
              if(i==5){
                i=0;
                PORTB=1;
                INTCON.INTF=1;
              }
         }
      }
    }
}