/*
 * ADC_ass2.c
 *
 *  Created on: Jan 5, 2024
 *      Author: Abir Omara
 */


/* Lib layer */
#include "STD_TYPES.h"
#include "BIT_MATH.h"
#include <util/delay.h>
#define F_CPU   8000000UL

/* MCAL */
#include "DIO_interface.h"
#include "ADC_interface.h"


/* HAL */
#include "lcd_interface.h"
#include "KPD_interface.h"


u8 GetPassword (void);



int main ()
{

	u8 Password_Flag;
	u16 Local_u16AnalogValue,Local_u16DigitalValue,Local_u16TempValue;

	DIO_voidInit();
	LCD_voidInt();
	ADC_voidInit();


	Password_Flag=GetPassword();
	while (1)
	{

		if (Password_Flag ==1)
		{
			LCD_voidClearDisplay();
			LCD_u8GoToXY(1,0);
			LCD_voidWriteString("you tried 3     times");
			_delay_ms(1000);
			LCD_voidClearDisplay();
			LCD_u8GoToXY(1,0);
			LCD_voidWriteString("wait for 5 sec  and try later");
			_delay_ms(5000);
			LCD_voidClearDisplay();
			Password_Flag=GetPassword();

		}
		else if (Password_Flag ==0)
		{

			LCD_voidClearDisplay();
			LCD_u8GoToXY(1,0);
			LCD_voidWriteString("Temperature=");
			_delay_ms(2);

			ADC_u8GetDigitalValueSynchNonBlocking(ADC_u8_CHANNEL_0,&Local_u16DigitalValue);

			Local_u16AnalogValue=(u16) ((Local_u16DigitalValue*5000UL)/1024);
			Local_u16TempValue=Local_u16AnalogValue/10;
			LCD_u8GoToXY(1,13);
			LCD_voidWriteNumber(Local_u16TempValue);
			_delay_ms(1000);

		}
	}
	return 0;
}


u8 GetPassword (void)
{

	u8 Correct_password [4]={'1','1','1','1'};
	u8 Input_password [4];
	u8 Local_u8Counter=0,Local_u8Key;
	u8 Local_u8NumOfAttempts=0;
	u8 Local_u8NumOfMaxAttempts=3;
	u8 Local_u8Flag=0;

	for  (Local_u8NumOfAttempts=0; Local_u8NumOfAttempts< Local_u8NumOfMaxAttempts;Local_u8NumOfAttempts++)
	{
		LCD_u8GoToXY(1,0);
		LCD_voidWriteString("Enter password");
		while (Local_u8Counter != 4)
		{
			KPD_u8GetKeyState(&Local_u8Key);
			//if there are no switch pressed
			//no displaying on LCD
			if (Local_u8Key != KPD_u8_KEY_NOT_PRESSED)
			{
				LCD_u8GoToXY(2,Local_u8Counter);
				LCD_voidSendData(Local_u8Key);
				_delay_ms(200);
				LCD_u8GoToXY(2,Local_u8Counter);
				LCD_voidSendData('*');
				Input_password[Local_u8Counter]=Local_u8Key;
				Local_u8Counter++;
			}
		}
		if ((Input_password[0]==Correct_password[0]) && (Input_password[1]==Correct_password[1]) && (Input_password[2]==Correct_password[2])  &&(Input_password[3]==Correct_password[3]))
		{
			Local_u8Flag=0;
			break;
		}

		else
		{

			LCD_voidClearDisplay();
			LCD_u8GoToXY(1,0);
			LCD_voidWriteString("incorrect       password");
			_delay_ms(500);
			LCD_voidClearDisplay();
			//LCD_u8GoToXY(1,0);
			//LCD_voidWriteString("try again");
			//_delay_ms(1000);
			Local_u8Counter=0;
			Local_u8Flag=1;


		}

	}
	return Local_u8Flag;
}
