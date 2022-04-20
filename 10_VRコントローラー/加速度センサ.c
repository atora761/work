#define F_CPU 16000000UL
//#define _SFR_MEM8(mem_addr) _MMIO_BYTE(mem_addr)
#include <avr/io.h>
#include <avr/interrupt.h>    // 割込を行うためにインクルードします。
#include <util/delay.h>       // _delay_msを使用するためにインクルードします。
#include <avr/pgmspace.h>
#include <avr/sleep.h>
#include <inttypes.h>
//状態値
#define FOSC 16000000
#define BAUD 9600
#define MYUBRR FOSC/16/BAUD-1
#define    START 0x08    //スタート状態値
#define   RECEIVE 0x60    //自分あての受信状態値
#define Fscl 400000
#define MYTWBR (FOSC/Fscl - 16)/2
#define   MT_SLA_ACK  0x18  //アドレス:SLA_Wを正常送信チェック値
#define   MT_DATA_ACK 0x1c  //データを正常送信チェック値
#define MAX_STRING 100
int main(void){
  unsigned char string[MAX_STRING];
  float accel_x,accel_y,accel_z;
  float before=0;
  float accel[100];
  unsigned int ubrr=MYUBRR;
  UBRR0H = (unsigned char)(ubrr>>8);
  UBRR0L = (unsigned char) ubrr;
  UCSR0B =(1 << RXEN0)|(1 << TXEN0);
  UCSR0C = (1 << UCSZ00)|(1 << UCSZ01);
  char dump_h,dump_l;
  //Wire.begin();
  //状態初期状態
  TWSR=0x00;
  DDRB = 0x0F;  //0b00000011; //LED ピン　出力設定
  DDRD = 0xC0;  //0b1100 0000;  //LED ピン　出力設定
  PORTB = 0x00;
  TWDR = 0x00 ;
  TWBR = (unsigned char) MYTWBR;
  TWCR |= (1<<TWEN);
  sei(); //enable interrupt
  int i=0;
  //void twi_start(char *status)
  TWCR = ( 1<< TWINT )|(1<<TWSTA)|(1<<TWEN);//
  while( !( TWCR & (1<< TWINT) ) ); //②結果待ち
  while( ( TWSR & 0xF8 ) != START );  //状態0x08かどうかの確認
  //状態スタート　0x08
  //twi_send_byte_nack(0xD0,&twi_stat);
  TWDR = 0xD0 ;         // あて先アドレス設定
  TWCR = ( 1<< TWINT )|(1<<TWEN);
  while(  ! ( TWCR & ( 1<< TWINT ) ) ); //④あて先アドレス送信完了待ち
  //状態SLW+W送信　0x18
  //twi_send_byte_nack(0x6B,&twi_stat);
  TWDR = 0x6B ;
  TWCR = ( 1<< TWINT )| ( 1<< TWEN ) ;    //アドレス送信開始
  while (!(TWCR & (1<<TWINT)));
  if ((TWSR & 0xF8) != MT_DATA_ACK){}
  //状態データ送信　0x28
  //twi_send_byte_nack(0x00,&twi_stat);
  TWDR = 0x00 ;
  TWCR = ( 1<< TWINT )| ( 1<< TWEN ) ;    //アドレス送信開始
  while (!(TWCR & (1<<TWINT)));
  //状態データ送信　0x28
  TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWSTO);
  while(1){
    //x　100回分をサンプリング
    for(i=0;i<100;i++)
    {
      //twi_start(&twi_stat);
      TWCR=(1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
      while( !( TWCR & (1<< TWINT) ) ); //②結果待ち
      //状態再送開始条件　0x10
      //twi_send_byte_nack(0xD0,&twi_stat);
      TWDR = 0xD0 ;         // あて先アドレス設定
      TWCR = ( 1<< TWINT )|(1<<TWEN);
      while(  ! ( TWCR & ( 1<< TWINT ) ) ); //④あて先アドレス送信完了待ち
      //状態SLW+W送信　0x18
      // twi_send_byte_nack(0x3B,&twi_stat);
      TWDR = 0x43 ;
      TWCR = ( 1<< TWINT )| ( 1<< TWEN ) ;    //アドレス送信開始
      while (!(TWCR & (1<<TWINT)));
      //状態データ送信　0x28
      //twi_start(&twi_stat);
      TWCR=(1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
      while( !( TWCR & (1<< TWINT) ) ); //②結果待ち
      //状態再送開始条件　0x10
      //twi_send_byte_nack(0xD1,&twi_stat);
      TWDR = 0xD1 ;         // あて先アドレス設定
      TWCR = ( 1<< TWINT )|(1<<TWEN);
      while(  ! ( TWCR & ( 1<< TWINT ) ) ); //④あて先アドレス送信完了待ち
      //状態SLW+R受信　0x40
      //twi_recv_byte_nack(&dump_h,&twi_stat);
      TWCR =( ( 1<<TWINT )|  ( 1<<TWEN ) );    //ｽﾚｰﾌﾞ受信動作
      while( !( TWCR & (1<<TWINT) ) );  // 設定結果待ち
      dump_h=TWDR;
      //状態データバイト受信　0x50
      TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWSTO);
      //STOP
      //twi_start(&twi_stat);
      TWCR=(1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
      while( !( TWCR & (1<< TWINT) ) ); //②結果待ち
      //状態再送開始条件　0x10
      //twi_send_byte_nack(0xD0,&twi_stat);
      TWDR = 0xD0 ;         // あて先アドレス設定
      TWCR = ( 1<< TWINT )|(1<<TWEN);
      while(  ! ( TWCR & ( 1<< TWINT ) ) ); //④あて先アドレス送信完了待ち
      //状態SLW+W送信　0x18
      // twi_send_byte_nack(0x3B,&twi_stat);
      TWDR = 0x44 ;
      TWCR = ( 1<< TWINT )| ( 1<< TWEN ) ;    //アドレス送信開始
      while (!(TWCR & (1<<TWINT)));
      if ((TWSR & 0xF8) != MT_DATA_ACK){}
      //状態データ送信　0x28
      //twi_start(&twi_stat);
      TWCR=(1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
      while( !( TWCR & (1<< TWINT) ) ); //②結果待ち
      //状態再送開始条件　0x10
      //twi_send_byte_nack(0xD1,&twi_stat);
      TWDR = 0xD1 ;         // あて先アドレス設定
      TWCR = ( 1<< TWINT )|(1<<TWEN);
      while(  ! ( TWCR & ( 1<< TWINT ) ) ); //④あて先アドレス送信完了待ち
      //状態SLW+R受信　0x40
      //twi_recv_byte_nack(&dump_h,&twi_stat);
      TWCR =( ( 1<<TWINT )|  ( 1<<TWEN ) );    //ｽﾚｰﾌﾞ受信動作
      while( !( TWCR & (1<<TWINT) ) );  // 設定結果待ち
      dump_l=TWDR;
      //状態データバイト受信　0x50
      TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWSTO);
      //STOP
      accel_x = (float)((dump_h << 8) | (dump_l))/131.0;
      accel[i] = accel_x;
    }
    accel_x = 0;
    for(i = 0;i < 100;i++){
      accel_x += accel[i];
    }
    accel_x = (accel_x / 100.0);
    sprintf(string,"%3.4f \r",accel_x);
    while(!(UCSR0A & (1<<UDRE0)));
    UDR0 = string;
    Serial.print(accel_x);
//y 100回サンプリング
    for(i=0;i<100;i++){
      //twi_start(&twi_stat);
      TWCR=(1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
      while( !( TWCR & (1<< TWINT) ) ); //②結果待ち
      //状態再送開始条件　0x10
      //twi_send_byte_nack(0xD0,&twi_stat);
      TWDR = 0xD0 ;         // あて先アドレス設定
      TWCR = ( 1<< TWINT )|(1<<TWEN);
      while(  ! ( TWCR & ( 1<< TWINT ) ) ); //④あて先アドレス送信完了待ち
      //状態SLW+W送信　0x18
      // twi_send_byte_nack(0x3B,&twi_stat);
      TWDR = 0x45 ;
      TWCR = ( 1<< TWINT )| ( 1<< TWEN ) ;    //アドレス送信開始
      while (!(TWCR & (1<<TWINT)));
      //状態データ送信　0x28
      //twi_start(&twi_stat);
      TWCR=(1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
      while( !( TWCR & (1<< TWINT) ) ); //②結果待ち
      //状態再送開始条件　0x10
      //twi_send_byte_nack(0xD1,&twi_stat);
      TWDR = 0xD1 ;         // あて先アドレス設定
      TWCR = ( 1<< TWINT )|(1<<TWEN);
      while(  ! ( TWCR & ( 1<< TWINT ) ) ); //④あて先アドレス送信完了待ち
      //状態SLW+R受信　0x40
      //twi_recv_byte_nack(&dump_h,&twi_stat);
      TWCR =( ( 1<<TWINT )|  ( 1<<TWEN ) );    //ｽﾚｰﾌﾞ受信動作
      while( !( TWCR & (1<<TWINT) ) );  // 設定結果待ち
      dump_h=TWDR;
      //状態データバイト受信　0x50
      TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWSTO);
      //STOP
      //twi_start(&twi_stat);
      TWCR=(1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
      while( !( TWCR & (1<< TWINT) ) ); //②結果待ち
      //状態再送開始条件　0x10
      //twi_send_byte_nack(0xD0,&twi_stat);
      TWDR = 0xD0 ;         // あて先アドレス設定
      TWCR = ( 1<< TWINT )|(1<<TWEN);
      while(  ! ( TWCR & ( 1<< TWINT ) ) ); //④あて先アドレス送信完了待ち
      //状態SLW+W送信　0x18
      // twi_send_byte_nack(0x3B,&twi_stat);
      TWDR = 0x46 ;
      TWCR = ( 1<< TWINT )| ( 1<< TWEN ) ;    //アドレス送信開始
      while (!(TWCR & (1<<TWINT)));
      if ((TWSR & 0xF8) != MT_DATA_ACK){}
      //状態データ送信　0x28
      //twi_start(&twi_stat);
      TWCR=(1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
      while( !( TWCR & (1<< TWINT) ) ); //②結果待ち
      //状態再送開始条件　0x10
      //twi_send_byte_nack(0xD1,&twi_stat);
      TWDR = 0xD1 ;         // あて先アドレス設定
      TWCR = ( 1<< TWINT )|(1<<TWEN);
      while(  ! ( TWCR & ( 1<< TWINT ) ) ); //④あて先アドレス送信完了待ち
      //状態SLW+R受信　0x40
      //twi_recv_byte_nack(&dump_h,&twi_stat);
      TWCR =( ( 1<<TWINT )|  ( 1<<TWEN ) );    //ｽﾚｰﾌﾞ受信動作
      while( !( TWCR & (1<<TWINT) ) );  // 設定結果待ち
      dump_l=TWDR;
      //状態データバイト受信　0x50
      TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWSTO);
      //STOP
      accel_y = (float)((dump_h << 8) | (dump_l))/131.0;
      accel[i] = accel_y;
    }
    accel_y = 0;
    for(i = 0;i < 100;i++){
      accel_y += accel[i];
    }
    accel_y = (accel_y / 100.0);
    sprintf(string,"%3.4f \r",accel_y);
    while(!(UCSR0A & (1<<UDRE0)));
    UDR0 = string;
    Serial.print(accel_y);
//z 100回サンプリング
    for(i=0;i<100;i++){
      //twi_start(&twi_stat);
      TWCR=(1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
      while( !( TWCR & (1<< TWINT) ) ); //②結果待ち
      //状態再送開始条件　0x10
      //twi_send_byte_nack(0xD0,&twi_stat);
      TWDR = 0xD0 ;         // あて先アドレス設定
      TWCR = ( 1<< TWINT )|(1<<TWEN);
      while(  ! ( TWCR & ( 1<< TWINT ) ) ); //④あて先アドレス送信完了待ち
      //状態SLW+W送信　0x18
      // twi_send_byte_nack(0x3B,&twi_stat);
      TWDR = 0x47 ;
      TWCR = ( 1<< TWINT )| ( 1<< TWEN ) ;    //アドレス送信開始
      while (!(TWCR & (1<<TWINT)));
      //状態データ送信　0x28
      //twi_start(&twi_stat);
      TWCR=(1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
      while( !( TWCR & (1<< TWINT) ) ); //②結果待ち
      //状態再送開始条件　0x10
      //twi_send_byte_nack(0xD1,&twi_stat);
      TWDR = 0xD1 ;         // あて先アドレス設定
      TWCR = ( 1<< TWINT )|(1<<TWEN);
      while(  ! ( TWCR & ( 1<< TWINT ) ) ); //④あて先アドレス送信完了待ち
      //状態SLW+R受信　0x40
      //twi_recv_byte_nack(&dump_h,&twi_stat);
      TWCR =( ( 1<<TWINT )|  ( 1<<TWEN ) );    //ｽﾚｰﾌﾞ受信動作
      while( !( TWCR & (1<<TWINT) ) );  // 設定結果待ち
      dump_h=TWDR;
      //状態データバイト受信　0x50
      TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWSTO);
      //STOP
      //twi_start(&twi_stat);
      TWCR=(1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
      while( !( TWCR & (1<< TWINT) ) ); //②結果待ち
      //状態再送開始条件　0x10
      //twi_send_byte_nack(0xD0,&twi_stat);
      TWDR = 0xD0 ;         // あて先アドレス設定
      TWCR = ( 1<< TWINT )|(1<<TWEN);
      while(  ! ( TWCR & ( 1<< TWINT ) ) ); //④あて先アドレス送信完了待ち
      //状態SLW+W送信　0x18
      // twi_send_byte_nack(0x3B,&twi_stat);
      TWDR = 0x48 ;
      TWCR = ( 1<< TWINT )| ( 1<< TWEN ) ;    //アドレス送信開始
      while (!(TWCR & (1<<TWINT)));
      if ((TWSR & 0xF8) != MT_DATA_ACK){}
      //状態データ送信　0x28
      //twi_start(&twi_stat);
      TWCR=(1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
      while( !( TWCR & (1<< TWINT) ) ); //②結果待ち
      //状態再送開始条件　0x10
      //twi_send_byte_nack(0xD1,&twi_stat);
      TWDR = 0xD1 ;         // あて先アドレス設定
      TWCR = ( 1<< TWINT )|(1<<TWEN);
      while(  ! ( TWCR & ( 1<< TWINT ) ) ); //④あて先アドレス送信完了待ち
      //状態SLW+R受信　0x40
      //twi_recv_byte_nack(&dump_h,&twi_stat);
      TWCR =( ( 1<<TWINT )|  ( 1<<TWEN ) );    //ｽﾚｰﾌﾞ受信動作
      while( !( TWCR & (1<<TWINT) ) );  // 設定結果待ち
      dump_l=TWDR;
      //状態データバイト受信　0x50
      TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWSTO);
      //STOP
      accel_z = (float)((dump_h << 8) | (dump_l))/131.0;
      accel[i] = accel_z;
    }
    accel_z = 0;
    for(i = 0;i < 100;i++){
      accel_z += accel[i];
    }
    accel_z = (accel_z / 100.0);
    sprintf(string,"%3.4f \r",accel_z);
    while(!(UCSR0A & (1<<UDRE0)));
    UDR0 = string;
    Serial.print(accel_z);
    Serial.print("\n");
    _delay_ms(100);
  }
}
