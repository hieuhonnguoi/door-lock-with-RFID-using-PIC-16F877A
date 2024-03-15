#include <DoorLock.h>
#FUSES PUT,HS,NOWDT,NOPROTECT,NOLVP
#define LCD_RS_PIN      PIN_E0
#define LCD_RW_PIN      PIN_E1
#define LCD_ENABLE_PIN  PIN_E2
#define LCD_DATA4       PIN_D4
#define LCD_DATA5       PIN_D5
#define LCD_DATA6       PIN_D6
#define LCD_DATA7       PIN_D7

#define MO_KHOA         PIN_A0
#define DEN_DO        PIN_C0
#define DEN_VANG          PIN_C1
#define COI             PIN_C7
#include <lcd.c>
#define MFRC522_CS PIN_C2                 
#define MFRC522_SCK PIN_C3
#define MFRC522_SI  PIN_C4                           
#define MFRC522_SO  PIN_C5              
#define MFRC522_RST PIN_C6    
#include<Built_in.h> 

char master[]={0x63,0x08,0xB8,0x0F,0xDC};

char tam1[5]={0x00,0x00,0x00,0x00,0x00};
char tam2[5]={0x00,0x00,0x00,0x00,0x00};
char tam3[5]={0x00,0x00,0x00,0x00,0x00};
int1 THE_1=0;
int tt1, tt2, tt3,tt4;
CHAR UID[6];
UNSIGNED int TagType; 
int1 QUET_THE(char DATA[],char UID[])
{
   FOR (int i = 0; i < 5; i++)
   {
      if(UID[i]== DATA[i] )
      {
         THE_1=1;
      }
      else
      {
         THE_1=0;
         break;
      }
   }
   return THE_1;
}
int i,j,dem=0;

char enter_password[16]={'0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0'};



void write_eeprom_pass()
{
   for(i=0;i<16;i++)
   {
      write_eeprom(i,enter_password[i]);
   }
}


char keypad()
{  
   output_low(pin_B0);
   output_high(pin_B1);
   output_high(pin_B2);
   output_high(pin_B3);
   if(input(pin_B4)==0) return '1';
   else if(input(pin_B5)==0) return '2';
   else if(input(pin_B6)==0) return '3';
   else if(input(pin_B7)==0) return 'A';
   
   output_high(pin_B0);
   output_low(pin_B1);
   output_high(pin_B2);
   output_high(pin_B3);
   if(input(pin_B4)==0) return '4';
   else if(input(pin_B5)==0) return '5';
   else if(input(pin_B6)==0) return '6';
   else if(input(pin_B7)==0) return 'B';
   
   output_high(pin_B0);
   output_high(pin_B1);
   output_low(pin_B2);
   output_high(pin_B3);
   if(input(pin_B4)==0) return '7';
   else if(input(pin_B5)==0) return '8';
   else if(input(pin_B6)==0) return '9';
   else if(input(pin_B7)==0) return 'C';
   
   output_high(pin_B0);
   output_high(pin_B1);
   output_high(pin_B2);
   output_low(pin_B3);
   if(input(pin_B4)==0) return '*';
   else if(input(pin_B5)==0) return '0';
   else if(input(pin_B6)==0) return '#';
   else if(input(pin_B7)==0) return 'D';
   
   return 0xff;
}

char key_debouncing() //chong doi nut nhan
{
   char key;
   do
   {
      key = keypad(); //key = 'A'
   }
   while(key==0xff); // doi nguoi dung nhap va giu phim
   while(keypad()!=0xff); //doi cho toi khi nha phim keypad() = oxff
   return key;
}




void enter_pw()
{
   for(i=0;i<16;i++)
   {
      enter_password[i] = key_debouncing();
      if(enter_password[i]=='#') break;
      lcd_gotoxy(i+1,2);
      printf(lcd_putc,"%c",enter_password[i]);
   }
}

void empty_pw()
{
   for(i=0;i<16;i++)
   {
      enter_password[i] = '0';
   }
}

int compare()
{
   for(i=0;i<16;i++)
   {
      if(read_eeprom(i) != enter_password[i]) return 0; // sai mat khau
   }
   return 1; //dung mat khau
}


void open()
{
   lcd_putc('\f');
   lcd_gotoxy(5,1);
   printf(lcd_putc,"Welcome");
   output_high(MO_KHOA);
   for(i=5;i>0;i--)
   {
      lcd_gotoxy(1,2);
      printf(lcd_putc,"Can close in %d s",i);
      delay_ms(1000);
   }
   output_low(MO_KHOA);   
}
void canh_bao()
{
   output_low(COI);
   output_low(DEN_DO);
   delay_ms(5000);
   output_high(COI);
   output_high(DEN_DO);
}
void doc_the_RFID()
{  
   
   char tam2[]={0x00,0x00,0x00,0x00,0x00};
   char tam3[]={0x00,0x00,0x00,0x00,0x00};
   
   for (int i=0;i<5;i++)
   {
      tam2[i]=read_eeprom(0x20+i);
   }
   for (int i=0;i<5;i++)
   {
      tam3[i]=read_eeprom(0x30+i);
   }
}

void check_RFID()
{
  
   while(true)
   {
   IF (MFRC522_isCard (&TagType)) //Check any card
         {
         
         //Read ID 
         IF (MFRC522_ReadCardSerial (&UID))             
         {  
            for (int i=0;i<5;i++)
            {
               tam2[i]=read_eeprom(0x20+i);
            }
            for (int i=0;i<5;i++)
            {
               tam3[i]=read_eeprom(0x30+i);
            }
            tt1=QUET_THE(master,UID);
            tt2=QUET_THE(tam2,UID);
            tt3=QUET_THE(tam3,UID);
            if(tt1==1)
            {
               open();
               break;
            }
            else if(tt2==1)
            {
               open();
               break;
            }
            else if (tt3==1)
            {
               open();
               break;
            }
            else
            {
                  lcd_gotoxy(1,1);
                  printf(lcd_putc,"\f Wrong password");
                  canh_bao();
                  break;
             }
         }
        }
        
   }
   
}


int check_master()
{
   while(true)
   {
   IF (MFRC522_isCard (&TagType)) //Check any card
         {
         
         //Read ID 
         IF (MFRC522_ReadCardSerial (&UID))             
         {
            tt4 = QUET_THE(master,UID);
            
            if(tt4 == 1)
            {
               
               return 1;
            }  
            else 
            {
               
               return 0;
            }
            }
   }
   }
}


void them_the_RFID_1()
{
    while(true)
    {
        IF (MFRC522_isCard (&TagType)) //Check any card
        {
         
            //Read ID 
            IF (MFRC522_ReadCardSerial (&UID))  
            {
               for (i=0;i<5;i++)
               {
                  write_eeprom(0x20 + i,UID[i]);
                }
                break;
              }
          }
      }
      printf(lcd_putc,"\f SAVING...!!!");
      output_low(DEN_VANG);
      delay_ms(3000);
      output_high(DEN_VANG);
      break;
}

void them_the_RFID_2()
{
    while(true)
    {
        IF (MFRC522_isCard (&TagType)) //Check any card
        {
         
            //Read ID 
            IF (MFRC522_ReadCardSerial (&UID))  
            {
               for (i=0;i<5;i++)
               {
                  write_eeprom(0x30 + i,UID[i]);
                }
                break;
              }
          }
      }
      printf(lcd_putc,"\f SAVING...!!!");
      output_low(DEN_VANG);
      delay_ms(3000);
      output_high(DEN_VANG);
      break;
}


void xoa_the_RFID_1()
{
               for (i=0;i<5;i++)
               {
                  write_eeprom(0x20 + i,' ');
                }
                break;
      printf(lcd_putc,"\fDELETING...!!!");
      output_low(DEN_VANG);
      delay_ms(3000);
      output_high(DEN_VANG);
      break;
}
void xoa_the_RFID_2()
{
  
   for (i=0;i<5;i++)
   {
       write_eeprom(0x30 + i,' ');
    }
    break;

  
printf(lcd_putc,"\fDELETING...!!!");
output_low(DEN_VANG);
delay_ms(3000);
output_high(DEN_VANG);
break;
}



void main()
{
    
   lcd_init();
   set_tris_b(0b11111111);
   port_b_pullups(true);
   MFRC522_Init ();   
   while(TRUE)
   {
      lcd_putc('\f');
      lcd_gotoxy(1,1);
      printf(lcd_putc,"* to enter PW");
      lcd_gotoxy(1,2);
      printf(lcd_putc,"# to change PW"); 
      
      char check = key_debouncing(); //nguoi dung nhan '*' hay '#'
      if(check=='*')
      {
         lcd_putc('\f');
         lcd_gotoxy(1,1);
         printf(lcd_putc,"Enter password:");
         enter_pw();
         if(compare()==1)
         {
            lcd_gotoxy(1,1);
            printf(lcd_putc,"Correct password");
            empty_pw();
            delay_ms(2000);
            lcd_putc('\f');
            open();
            output_high(pin_c3);
            dem=0;
         }
         else if(compare()==0)
         {
            lcd_gotoxy(1,1);
            printf(lcd_putc,"Wrong password");
            dem++;
            if(dem==2) {
               output_low(DEN_VANG);
            }
            if(dem==3) {
               output_high(DEN_VANG);
               canh_bao();
               dem=0;
            }
            empty_pw();
            delay_ms(2000);
         }
         
      }
      else if(check=='#')
      {
         lcd_putc('\f');
         lcd_gotoxy(1,1);
         printf(lcd_putc,"Enter old PW:");
         enter_pw();
         if(compare()==1)
         {
            lcd_putc('\f');
            lcd_gotoxy(1,1);
            printf(lcd_putc,"Enter new PW:");
            empty_pw(); //rong mang enter_password
            enter_pw();
            write_eeprom_pass();
            empty_pw(); //rong mang enter_password
            lcd_putc('\f');
            lcd_gotoxy(1,1);
            printf(lcd_putc,"PW has been set");
            delay_ms(2000);
         }
         else if(compare()==0)
         {
            lcd_gotoxy(1,1);
            printf(lcd_putc,"Wrong password");
            empty_pw();
            delay_ms(2000);
         }
      }
      else if(check=='A')
      {
         printf(lcd_putc,"\fXIN MOI QUET THE");
         check_RFID();
      }
      else if(check=='B')
      {
         lcd_putc('\f');
         lcd_gotoxy(1,1);
         lcd_putc("1. ADD CARD 1");
         lcd_gotoxy(1,2);
         lcd_putc("2. ADD CARD 2");
         char kiemtra = key_debouncing();
         if(kiemtra =='1')
         {  
            printf(lcd_putc,"\fQUET THE MASTER");
             if(check_master())
            {
               lcd_putc('\f');
               lcd_gotoxy(1,1);
               lcd_putc("DUNG THE");
               lcd_gotoxy(1,2);
               lcd_putc("MOI THEM THE");
               delay_ms(2000);
               them_the_RFID_1();
            }
            else 
            {
               lcd_gotoxy(1,1);
               printf(lcd_putc,"\f Wrong password");
               canh_bao();
            }
         }
            
         
         else if(kiemtra =='2')
         {
            if(check_master())
            {
               lcd_putc('\f');
               lcd_gotoxy(1,1);
               lcd_putc("DUNG THE");
               lcd_gotoxy(1,2);
               lcd_putc("MOI THEM THE");
               delay_ms(2000);
               them_the_RFID_2();
            }
            else 
            {
               lcd_gotoxy(1,1);
               printf(lcd_putc,"\f Wrong password");
               canh_bao();
            }
         }
         
      }
      else if(check=='C')
      {
         lcd_putc('\f');
         lcd_gotoxy(1,1);
         lcd_putc("1. DELETE CARD 1");
         lcd_gotoxy(1,2);
         lcd_putc("2. DELETE CARD 2");
         char kiemtra_2 = key_debouncing();
         if(kiemtra_2=='1')
         {
            printf(lcd_putc,"\fQUET THE MASTER");
            if(check_master())
            {
               lcd_putc('\f');
               lcd_gotoxy(1,1);
               lcd_putc("DUNG THE");
               
               delay_ms(2000);
               xoa_the_RFID_1();
            }
            else 
            {
               lcd_gotoxy(1,1);
               printf(lcd_putc,"\f Wrong password");
               canh_bao();
            }
         }
         else if(kiemtra_2=='2')
         {
            printf(lcd_putc,"\fQUET THE MASTER");
            if(check_master())
            {
               lcd_putc('\f');
               lcd_gotoxy(1,1);
               lcd_putc("DUNG THE");
               
               delay_ms(2000);
               xoa_the_RFID_2();
            }
            else 
            {
               lcd_gotoxy(1,1);
               printf(lcd_putc,"\f Wrong password");
               canh_bao();
            }
         }
         }
      }
   }

