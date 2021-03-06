//********************************************************************//
//             황동천 임베디드 창의 시계 2014-2-15  ATmega8535           //
//******************************************************************//
#define F_CPU 16000000UL
#include <mega8535.h>
#include <delay.h>
#include <DS1307.h>
#asm("sei"); 
unsigned char h,m,s;
unsigned char weekday,day,month,year;
int mode = 0;
int fnd_list[5] = {0x80,0x20,0x10,0x40,0x00};
int fnd_led[10] = {0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x98};
    //**************************************//
    //                FND설정               //
    //**************************************//
void fnd_first(int num){
    int tempb[4];
    int i;    
    tempb[3] = num/10;
    tempb[2] = num%10;
    for(i = 0; i <= 1; i++){
        PORTD = fnd_list[i];
        PORTA = fnd_led[tempb[i]];
        delay_ms(1);
        PORTD =  0x00;
        delay_ms(1);
    }        
}
void fnd_second(int num){
    int tempa[4];
    int i;
    tempa[1] = num/10;
    tempa[0] = num%10;    
    for(i = 2; i <= 3; i++){
        PORTD = fnd_list[i];
        PORTA = fnd_led[tempa[i]];
        delay_ms(1);
        PORTD =  0x00;
        delay_ms(1);
    }    
}
    //**************************************//
    //       외부 인터럽트 실행내용         //
    //**************************************//
interrupt [EXT_INT0] void ext_int0_isr(void){            
    if(h >= 23) h = 0;
    else h++;         
    rtc_set_time(h,m,s);
    delay_ms(1);    
}
interrupt [EXT_INT1] void ext_int1_isr(void){
    if(m >= 59){
        m = 0;
    }
    else m++;
    rtc_set_time(h,m,s);
    delay_ms(1);;           
}
interrupt [EXT_INT2] void ext_int2_isr(void){
    if(mode == 0) mode = 1;
    else if(mode == 1) mode = 0; 
    delay_ms(1); 
}
void main(void){     
    //**************************************//
    //              초기화/설정             //
    //**************************************//
    i2c_init();
    rtc_init(0,0,0);    
    DDRD = 0xf0;
    DDRC = 0x00;
    DDRA = 0xff; 
    MCUCR = 0b00001010;        
    MCUCSR = 0b0100000;
    GICR = 0b11100000;
    //**************************************//
    //             메인프로그램             //
    //**************************************// 
    while(1){   
        rtc_get_time(&h,&m,&s);
       
        if(mode == 0){
            fnd_first(h);
            fnd_second(m);
        }
        else if(mode == 1){
            fnd_first(99);
            fnd_second(99);
        }      
    }                  
}
