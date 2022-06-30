#include <REGX52.H>

sbit Clock_Slave = P0^4;	   //USTAWIENIE PINOW CLOCK DLA SLAVE
sbit ACK_Slave = P0^5;		   //USTAWIENIE PINOW ACK DLA SLAVE
sbit Clock_Master = P0^6;	   //USTAWIENIE PINOW CLOCK DLA MASTER
sbit ACK_Master = P0^7;		   //USTAWIENIE PINOW ACK DLA MASTER
bit BM = 1;					   //USTAWIENIE ZMIENNEJ BIT DLA MASTER

volatile unsigned char LiczbaMaster = 0x00;		   //ZMIENNE DLA MASTER I SLAVE
unsigned char LiczbaSlave = 0x00;
unsigned char bajt = 0x00;

void Delay(int i)		  //FUNKCJA SLUZACA DO OPOZNIANIA DZALANIA PROGRAMU
{
 unsigned int k,l,m;
 for(l = 0; l < i; l++)
 {
  k = 500;
  m = 1000;
  k = m*l;
 }
}

void Int0() interrupt 0	   //FUNKCJA DO PRZERWAN
{
 LiczbaMaster++;
 BM = 0;
}

void zapiszBajt(unsigned char bajt)	   //FUNKCJA DO ZAPISU POJEDYNCZEGO BAJTU
{
 while(ACK_Master == 0) Delay(1);
 Clock_Master = 1;
 Delay(1);
 P2 = bajt;
 Delay(1);
 Clock_Master = 0;
 while(ACK_Master == 1) Delay(1);
 Clock_Master = 1;
 while(ACK_Master == 0) Delay(1);
 P2 = 0xFF;
}

void initialize()		  //FUNKCJA DO INICJALIZOWANIA PRZERWAN
{
 LiczbaMaster = 0;
 IT0 = 1;		   //INT0 aktywne zero
 EX0 = 1;		   //Wlaczenia INT0
 EA = 1;		   //Wlaczenie wszstkich przerwan
}

void main()
{
 initialize();
 P1 = 0;
 ACK_Slave = 1;
 Clock_Master = 1;
 while(P0_0 == 1)	  //MASTER
 {
  while(BM) Delay(10);
  P1 = LiczbaMaster;
  zapiszBajt(LiczbaMaster);
  BM = 1;
 }
 while(P0_0 == 0)	 //SLAVE
 {
  while(Clock_Slave == 1) Delay(1);
  ACK_Slave = 0;
  P1 = P2;
  Delay(1);
  ACK_Slave = 1;
 }
}