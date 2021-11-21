/*****************************************************************************************************
*                                            Header file                                             *
*****************************************************************************************************/
#include<REGX52.H>
#include<string.h>

#define LCD_PORT P1
sbit EN=P3^4;
sbit RS=P3^5;

/*****************************************************************************************************
*                                        Function Decleration                                        *
*****************************************************************************************************/
void lcd_Initial();
void lcd_command(char value);
void lcd_data(char value);
void delay_ms(unsigned long int Delay);
void delay_us(int Delay);
void uart_initial(void);
void lcd_display(unsigned char locn,const char *dat);
void rotate(unsigned int value1);
void msdelay(unsigned int time);

/*****************************************************************************************************
*                                        Variable Decleration                                        *
*****************************************************************************************************/
char Rec_Data;
char Rec_Array[17];
char rx_flg=0,j=0,i=0;
const char stored_ID1[12]="0800AED9F28D";
const char stored_ID2[12]="00800AA3765F";
const char stored_ID[12]="D0800AED9F28";
//const char stored_ID4[12]="0800AA3765F0";

/*****************************************************************************************************
*                                            Main code                                               *
*****************************************************************************************************/
void main()
{
uart_initial();                                             /* UART Initialization                  */
lcd_Initial();                                              /* LCD Initialization                   */
lcd_display(0x80,"RFID Based Toll ");
lcd_display(0xC0,"     System     ");
delay_ms(1000);
lcd_display(0x80,"Scan Your Tag to");
lcd_display(0xC0," Open the Gate  ");
while(1)
 {
  if(RI==1)
  {	
   RI=0;
   Rec_Data=SBUF;                                           /* Intake of receiveddata to variable  */
   Rec_Array[j]=Rec_Data;                                   /* RFID card no stored into array       */
   j++;                                                     /* Increment array index                */
   if (j==12)                                               /* Checking 12 digit no. rx complete    */
   {                 
    Rec_Array[j]='\0';                                      /* Adding null character                */
    j=0;
    lcd_command(0x01); delay_us(200);
    lcd_display(0x80,"Your            ");
    delay_ms(500);
    lcd_display(0x80,"ID:             ");
    lcd_display(0X83,Rec_Array);
    delay_ms(1000);
    delay_ms(1000);
    if(!strcmp(Rec_Array,stored_ID))                        /* Comparing with Stored ID             */
    {
     lcd_display(0x80,"Gate is Opening ");			/* Login sucess                         */
		 delay_ms(1000);
		 rotate(120);
     lcd_display(0x80,"Scan Your Tag to");
     lcd_display(0xC0," Open the Gate  ");
    }
		 else if(!strcmp(Rec_Array,stored_ID1))                        /* Comparing with Stored ID             */
    {
     lcd_display(0x80,"Gate is Opening ");                  /* Login sucess                         */
     delay_ms(1000);
		 lcd_display(0x80,"Scan Your Tag to");
     lcd_display(0xC0," Open the Gate  ");
		 rotate(120);
    }
     else if(!strcmp(Rec_Array,stored_ID2))                        /* Comparing with Stored ID             */
    {
     lcd_display(0x80,"Gate is Opening ");                  /* Login sucess                         */
     delay_ms(1000);
     rotate(120);
		 lcd_display(0x80,"Scan Your Tag to");
     lcd_display(0xC0," Open the Gate  ");
    }
	 	 else
    {
     lcd_display(0x80," Sorry Wrong ID ");                  /* Login failed                         */
     delay_ms(1000);
     lcd_display(0x80,"Scan your Tag to");
     lcd_display(0xC0," Open the Gate  ");
    }		  
   }               
  }
 }
}

/*===================================================================================================
* Function    : uart_initial
* Description : Function to initialise UART
* Parameters  : None
====================================================================================================*/
void uart_initial(void) interrupt 4
{
 TMOD=0X21;
 SCON=0X50;																						
 TF1=0;
 TR1=0;
 TH1=0XFD;
 TL1=0XFD;	    
 TR1=1;
}

/*=====================================================================================================
* Function    : lcd_Initial
* Description : Function to Initialize LCD
* Parameters  : None
====================================================================================================*/	
void lcd_Initial()
{                                                           /* ---LCD Intialization Commands-----   */
 lcd_command(0x30); delay_us(5);                            /* LCD Specification Command            */
 lcd_command(0x30); delay_us(5);                            /* LCD Specification Command            */
 lcd_command(0x30); delay_us(5);                            /* LCD Specification Command            */
 lcd_command(0x38);                                         /* LCD Double Line Display Command      */
 lcd_command(0x06);                                         /* LCD Auto Increment Location cmmd     */
 lcd_command(0x0C);                                         /* LCD Display ON Command               */
 lcd_command(0x01); delay_us(200);                          /* LCD Display Clear Cmdmand            */
}

/*=====================================================================================================
* Function    : lcd_command
* Description : Function to send a command to LCD
* Parameters  : value, contains the command to be send
====================================================================================================*/
void lcd_command(char value)
{ 
 RS=0;
 EN=1;
 LCD_PORT=value;
 delay_us(5);
 EN=0;
}

/*====================================================================================================
* Function    : lcd_data
* Description : Function to send a data to LCD
* Parameters  : value, contains the data to be send
====================================================================================================*/
void lcd_data(char value)
{ 
 RS=1;
 EN=1;
 LCD_PORT=value;
 delay_us(5);
 EN=0;
}

/*===================================================================================================
* Function    : delay_us
* Description : Function to provide delayin us for LCD
* Parameters  : Delay, provide delay needed
====================================================================================================*/
void delay_us(int Delay)    
{		
 while((--Delay)!=0);	
}

/*===================================================================================================
* Function    : delay_ms
* Description : Function to provide delayin MS for LCD
* Parameters  : Delay, provide delay needed
====================================================================================================*/
void delay_ms(unsigned long int Delay)
{
 Delay=Delay*15;
 while(--Delay!=0);
}

/*===================================================================================================
* Function    : lcd_display
* Description : Function to send a string of data in to LCD
* Parameters  : dat, contains the adddress of a string 
====================================================================================================*/
void lcd_display(unsigned char locn,const char *dat)
{
 lcd_command(locn);
 while(*dat!='\0')
 {
  lcd_data(*dat);
  dat++;
 }
}
void rotate(unsigned int value1)
{
	unsigned m,n;
	for(m=0;m<value1;m++)
    {
         P0 = 0x03;     //0011     
        msdelay(1);
        P0 = 0x06;         //0110
        msdelay(1);
        P0 = 0x0C;         //1100
        msdelay(1);
        P0 = 0x09;         //1001
        msdelay(1);
    }
		msdelay(1428);
		for(n=0;n<120;n++)
		{
         P0 = 0x09;     //1001
        msdelay(1);
        P0 = 0x0C;         //1100
        msdelay(1);
        P0 = 0x06;         //0110
        msdelay(1);
        P0 = 0x03;         //0011
        msdelay(1);
    }
}
void msdelay(unsigned int time)
    {
    unsigned i,j ;
    for(i=0;i<time;i++)    
    for(j=0;j<1275;j++);
    }
/****************************************************************************************************
*									    END of Micro RFID Program						 		                                      *
*****************************************************************************************************/
