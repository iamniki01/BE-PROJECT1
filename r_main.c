/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only 
* intended for use with Renesas products. No other uses are authorized. This 
* software is owned by Renesas Electronics Corporation and is protected under 
* all applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING 
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT 
* LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE 
* AND NON-INFRINGEMENT.  ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.
* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS 
* ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE 
* FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR 
* ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE 
* BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software 
* and to discontinue the availability of this software.  By using this software, 
* you agree to the additional terms and conditions found by accessing the 
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2010, 2011 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* File Name    : r_main.c
* Version      : CodeGenerator for RL78/G13 V1.03.01 [11 Oct 2011]
* Device(s)    : R5F100LE
* Tool-Chain   : CA78K0R
* Description  : This file implements main function.

* author(s)    : Nikhil, switaja, Karthik, Chaithanya

***********************************************************************************************************************/

/***********************************************************************************************************************

***********************************************************************************************************************/
/* Start user code for pragma. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "r_cg_macrodriver.h"
#include "r_cg_cgc.h"
#include "r_cg_port.h"
#include "r_cg_serial.h"
#include "r_cg_adc.h"
/* Start user code for include. Do not edit comment generated here */

#include "ALCD64.c"
#include "ADC64.c"

#include "GSM.c"


#define L_Motor_Data1 P5.0
#define L_Motor_Data2 P5.1
#define R_Motor_Data1 P5.2
#define R_Motor_Data2 P5.3


#define RF_Tx0 P3.0
#define RF_Tx1 P3.1
#define RF_Tx2 P5.5

//#define SW1 P4.0
#define Shop_Complete_SW P4.1
//#define Trolley_Park_SW P4.2
// #define Ultrasonic_Sensor P4.3



#define RF_Tx0 P5.0
#define RF_Tx1 P5.1
#define RF_Tx2 P5.3


/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */


void Device_Init( void );

//>>>>>>> master
extern unsigned char Rx_data_arr0[400], Rx_data_arr1[100],Rx_data_arr2[100];
extern unsigned char Rx_count0, Rx_count1,Rx_count2;
extern unsigned char Rx_ST_Flag0, Rx_ST_Flag1,Rx_ST_Flag2;
extern unsigned char Rx_data0, Rx_data1,Rx_data2;


unsigned char UART_TX_ARR[14] = "    ";

unsigned char RFID_TAG[7][12]={"xxxxxxxxxxx"," 0008662584"," 0008685583"," 0008662442"," 0008662597"," 0008685738"," 0008685570"};

unsigned char Prod_Name[7][15] =   {"              ",
									"Deo Spray     ",
									"Note Books    ",
									"Biscuit       ",
									"Fruit Juice   ",
									"Red Label Tea ",
                                    "Dish Washer   ",
									};
unsigned char Prod_Cost_Str[7][4] =    {"   ","180","432","106"," 80"," 15"," 35"};
unsigned int Prod_Cost_Intg[7] =       {0, 180, 432, 106, 80, 15, 35};

unsigned char Prod_Status[7] = "OOOOOOO";
unsigned char Key1_Disp_Row4[21]    =	"TI:     NET:Rs      ";



unsigned char Product_Detect, Tag_Detect_Flag = 0, Loop_Back_Disp = 0;

unsigned char Prod_Sel_Count, Prod_Cancel_Posit, RFID_Flag;
unsigned char Tot_Shop_Cost_Str[5] = "0000";
unsigned int Tot_Shop_Cost, Tot_Shop_Cost_Temp;

unsigned char Print_Prod_Sel[9] = "0000000@";	


// unsigned char RFID_RDM6300_STR[30], RFID_RDM6300_Flag;
unsigned char RF_Sent_Flag;
unsigned char Shop_Complete;

void Device_Init(void);
// void RFID_RDM6300_Scan(void);
void Robot_Move_Req_Func(void);
void Robot_OFF(void);
void Forward_L_R(void);
void Rotate_L( void );
void Rotate_R(void);
void Reverse_L_R( void );
void Prod_Add( void );
void Prod_Cancelled( void );
void RFID_Tag_Compare( void );

void MSDelay11( unsigned int Milli_Sec );


unsigned int RF_Sent_Flag;

/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: main
* Description  : This function implements main function.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void main(void)
{
    /* Start user code. Do not edit comment generated here */

	unsigned int i, y;
	
	Device_Init( );
	MSDelay( 200 );

//************************ RFID-TEST-UART-0 *****************************************************************************//
    Rx_ST_Flag0 = 0;
	Rx_count0 = 0;
	
	ALCD_Comm( 0x01 );
	MSDelay( 200 );
	ALCD_Message( 0xC0, "   PUNCH CARD    " );		
	MSDelay( 500 );
	while(1)
	{
		if( Rx_ST_Flag0 == 1 )
		{
			MSDelay( 200 );							// 0.1 sec delay
			Rx_data_arr0[11] = '\0';
			ALCD_Message( 0xC0, Rx_data_arr0 );		// Testing
			MSDelay( 2000 );						// 1 sec delay

			Rx_ST_Flag0 = 0;						// Indicates UART receive of first byte 
			Rx_count0 = 0;							// Take UART Receiver array to zeroth location
			break;
		}
	} 
//*******************************************************************************************//	
	
	Forward_L_R();
	Rotate_R();
	Rotate_L();
	Reverse_L_R();
	
//*********************GSM CODE****************************************

	GSM_Init( );
	ALCD_Message( 0xC0, "   GSM ACTIVE   " );
	MSDelay(1000);

	GSM_Send_SMS( Mb_Num, GSM_SMS );
	MSDelay(5000);              			     // Testing
	ALCD_Message( 0xC0, " TEST SMS SENT  " );
	MSDelay(2000);						// 2 sec delay	

//******************************************************************************

	
	ALCD_Comm( 0x01 );
	MSDelay( 200 );
	
	Rx_ST_Flag0 = 0;						// Indicates UART receive of first byte 
	Rx_count0 = 0;							// Take UART Receiver array to zeroth location
	Rx_ST_Flag1 = 0;
	Rx_count1 = 0;
//	ALCD_Message(0x80,"  PROJECT START  ");
	while(1)
	{
		Robot_Move_Req_Func( );
		
		if( Rx_ST_Flag0 == 1 )
		{
			MSDelay( 200 );							// 0.1 sec delay
			Rx_data_arr0[11] = '\0';
			ALCD_Message( 0x80, Rx_data_arr0 );		// Testing
			MSDelay( 2000 );						// 1 sec delay

			RFID_Tag_Compare( );
			
			if( (Product_Detect>=1) && (Product_Detect<=6) )
			{
//ALCD_Message(0xD4,"2");
//MSDelay( 1000 );
				if( Prod_Status[Product_Detect] == 'O' )
				{
					Prod_Status[Product_Detect] = 'I';
					Prod_Add( );
				}
				else if( Prod_Status[Product_Detect] == 'I' )
				{
					Prod_Status[Product_Detect] = 'O';
					Prod_Cancelled( );
				}
			}
			else
			{
				ALCD_Message( 0x80, "    EXPIRED     " );				
			}
			
			Rx_ST_Flag0 = 0;
			Rx_count0 = 0;

		}
	
		if( Shop_Complete_SW == 0 )
		{
			Print_Prod_Sel[0] = 'P';
			ALCD_Message( 0x80, Print_Prod_Sel );
			GSM_Send_SMS( Mb_Num, Print_Prod_Sel );
			//			R_UART1_Send(Print_Prod_Sel,9);
			MSDelay(6000);	
			
			ALCD_Comm( 0x01 );
			MSDelay( 200 );
			ALCD_Message( 0x80, "THANKS" );
			MSDelay(2000);	
			
			Rx_ST_Flag1 = 0;
			Rx_count1 = 0;
			
/*			
			ALCD_Message( 0xC0, "PARK TROLLEY" );
			while( Trolley_Park_SW == 1 )
			{
				;
			}
			MSDelay( 100 );
			if( Trolley_Park_SW == 0 )
			{
				Ultrsonic_Obst_Flag = 0;
				ALCD_Message( 0xC0, "TROLLEY MOVING" );
				Forward_L_R();
				MSDelay11( 1000 );
				Forward_L_R();
				MSDelay11( 1000 );
				Forward_L_R();
				MSDelay11( 1000 );
				Rotate_R();
				MSDelay11( 1000 );
				Forward_L_R();
				MSDelay11( 1000 );
				Forward_L_R();
				MSDelay11( 1000 );
				Rotate_L();
				MSDelay11( 500 );
				Forward_L_R();
				MSDelay11( 500 );
				Forward_L_R();
				MSDelay11( 500 );
				ALCD_Message( 0xC0, "TROLLEY PARK SUCCESS" );
				Ultrsonic_Obst_Flag = 1;
				
			}*/
		}
	}

//=======
	
	Device_Init();
	MSDelay( 200 );
	
		RF_Tx0 = 1;
		RF_Tx1 = 1;
		RF_Tx2 = 1;
	MSDelay( 4000 );

		RF_Tx0 = 0;
		RF_Tx1 = 0;
		RF_Tx2 = 0;

	
	R_ADC_Start();
	MSDelay( 500 );
	ALCD_Comm( 0x01 );             // TO CLEAR THE SCREEN
	MSDelay( 500 );
	
	while(1)
    {
		RF_Tx0 = 0;
		RF_Tx1 = 0;
		RF_Tx2 = 0;
		RF_Sent_Flag = 0;
		
	    ADC64_Start(0x00);
	    ALCD_Message(0x80,Volt_Value);
	
		if ( Volt_Temp <= 145 )
		{
		    ALCD_Message( 0xC0, " BACKWARD "  );
	
			RF_Tx0 = 1;
			RF_Tx1 = 0;
			RF_Tx2 = 0;
		    MSDelay(5000);
			ALCD_Message( 0xC0, "           "  );	
			RF_Sent_Flag = 1;
		}
		else if( Volt_Temp >= 175 )
		{
			ALCD_Message( 0xC0, "  FORWARD  "  );	
			RF_Tx0 = 0;
			RF_Tx1 = 1;
			RF_Tx2 = 0;
		    MSDelay(5000);
			ALCD_Message( 0xC0, "           "  );	
			RF_Sent_Flag = 1;
		}
		
		
	    ADC64_Start(0x01);
	    ALCD_Message(0x85,Volt_Value);
		
		if( RF_Sent_Flag == 0 )
		{
			if (Volt_Temp <= 145) 
			{
				ALCD_Message( 0xC0, "  LEFT   "  );	
				RF_Tx0 = 1;
				RF_Tx1 = 1;
				RF_Tx2 = 0;
			    MSDelay(5000);
				ALCD_Message( 0xC0, "           "  );	
				RF_Sent_Flag = 1;
			}
			else if( Volt_Temp >= 185 )
			{
			    ALCD_Message( 0xC0, "  RIGHT   "  );	
				RF_Tx0 = 0;
				RF_Tx1 = 0;
				RF_Tx2 = 1;
			    MSDelay(5000);
				ALCD_Message( 0xC0, "           "  );	
				RF_Sent_Flag = 1;
			}
		}
		
	    MSDelay(2000);
	    
	}
	
//>>>>>>> master
	
    while (1U)
    {
        ;
    }
    /* End user code. Do not edit comment generated here */
}

/* Start user code for adding. Do not edit comment generated here */

void Device_Init( void )
{
//<<<<<<< iamniki01-Receiver_side
	MSDelay(200);
	ALCD_Init( );
	MSDelay(200);
	ALCD_Message( 0x94, "    WELCOME     " );
    ALCD_Message( 0xD4, "  SHOPPING MALL " );
	MSDelay(2000);


	R_SAU0_Create( );
	MSDelay(200);
	R_UART0_Start( );
 //	R_UART0_Send( "UART0 TEST", 10 );
 //	ALCD_Message( 0x80, "       U0       " );
	MSDelay(1000);

	R_UART1_Start( );
	MSDelay(200);
 //	R_UART1_Send( "UART1 TEST", 10 );
 //	ALCD_Message( 0x80, "         U1     " );
	MSDelay(1000);
	
//	R_SAU1_Create( );
//	MSDelay(200);
//R_UART2_Start( );
 //	R_UART2_Send( "UART2 TEST", 10 );
 //	ALCD_Message( 0x80, "        U2      " );
//	MSDelay(1000);
}

void MSDelay11( unsigned int Milli_Sec )
{
	 unsigned int x,y;

	for(x=0;x<Milli_Sec;x++)
	{
		for(y=0;y<3000;y++)
		{
		}
	}
}


void RFID_Tag_Compare( void )
{
	unsigned char i,j;
	
	
//	RFID_TAG[0][0] = Rx_data_arr0[0];
	RFID_TAG[0][1] = Rx_data_arr0[1];
	RFID_TAG[0][2] = Rx_data_arr0[2];
	RFID_TAG[0][3] = Rx_data_arr0[3];
	RFID_TAG[0][4] = Rx_data_arr0[4];
	RFID_TAG[0][5] = Rx_data_arr0[5];
	RFID_TAG[0][6] = Rx_data_arr0[6];
	RFID_TAG[0][7] = Rx_data_arr0[7];
	RFID_TAG[0][8] = Rx_data_arr0[8];
	RFID_TAG[0][9] = Rx_data_arr0[9];
	RFID_TAG[0][10] = Rx_data_arr0[10];
//	RFID_TAG[0][11] = Rx_data_arr0[11];

	Tag_Detect_Flag = 0;
	Product_Detect = 0;
	for( i=1; i<=6; i++ )
	{
		j = 1;
		while(1)
		{
			if( RFID_TAG[0][j] == RFID_TAG[i][j] )
			{
				j++;
				if( j >= 10 )
				{
//ALCD_Message(0xD4,"5");
//MSDelay( 1000 );
					Tag_Detect_Flag = 1;
					break;
				}
			}

			else
			{
				break;
			}
		}

		if( Tag_Detect_Flag == 1 )
		{
			Product_Detect = i;
			break;
		}
	}
}



void Prod_Add( void )
{

	ALCD_Message( 0x80, "ADDED       " );
//MSDelay( 1000 );

	
	Prod_Sel_Count = Prod_Sel_Count + 1;
	Print_Prod_Sel[Prod_Sel_Count] = Product_Detect + 48;
	ALCD_Comm( 0xC0 );	
	ALCD_Data( (Prod_Sel_Count-1) + 48 );
//MSDelay( 1000 );
		
	ALCD_Comm( 0x94 );	
	ALCD_Data( Prod_Sel_Count + 48 );
//MSDelay( 1000 );
	
	
	ALCD_Message( 0xC2, Prod_Name[Print_Prod_Sel[Prod_Sel_Count-1]-48] );
//MSDelay( 1000 );
    ALCD_Message( 0x96, Prod_Name[Print_Prod_Sel[Prod_Sel_Count]-48] );
//MSDelay( 1000 );
	
	ALCD_Message( 0xD1, Prod_Cost_Str[Print_Prod_Sel[Prod_Sel_Count-1]-48] );
//MSDelay( 1000 );	
	ALCD_Message( 0xA5, Prod_Cost_Str[Print_Prod_Sel[Prod_Sel_Count]-48] );
//MSDelay( 1000 );


	ALCD_Message( 0xD4, Key1_Disp_Row4 );
//MSDelay( 1000 );

	ALCD_Comm( 0xD8 );	
	ALCD_Data( Prod_Sel_Count + 48 );
//MSDelay( 1000 );
	

	Tot_Shop_Cost = Tot_Shop_Cost + Prod_Cost_Intg[Product_Detect];
	Tot_Shop_Cost_Temp = Tot_Shop_Cost;

	Tot_Shop_Cost_Str[3] = (Tot_Shop_Cost_Temp % 10) + 48;
	Tot_Shop_Cost_Temp = Tot_Shop_Cost_Temp / 10;
	Tot_Shop_Cost_Str[2] = (Tot_Shop_Cost_Temp % 10) + 48;
	Tot_Shop_Cost_Temp = Tot_Shop_Cost_Temp / 10;
	Tot_Shop_Cost_Str[1] = (Tot_Shop_Cost_Temp % 10) + 48;
	Tot_Shop_Cost_Temp = Tot_Shop_Cost_Temp / 10;
	Tot_Shop_Cost_Str[0] = (Tot_Shop_Cost_Temp % 10) + 48;
	ALCD_Message( 0xE4, Tot_Shop_Cost_Str );
//MSDelay( 1000 );

	
}


void Prod_Cancelled( void )
{
	unsigned char i;

	ALCD_Message( 0x80, "CANCEL       " );
//MSDelay( 1000 );


	i = 0;
	while( (Print_Prod_Sel[i]-48) != Product_Detect )		// to find which product removed
	{
		i++;
	}
	Prod_Cancel_Posit = i;
	while( i <= 7 )
	{
ALCD_Comm( 0xC0 );	
ALCD_Data( i + 48 );
//MSDelay( 1000 );

		Print_Prod_Sel[i] = Print_Prod_Sel[i+1];
		i++;
	}
	Prod_Sel_Count = Prod_Sel_Count - 1;

	ALCD_Comm( 0xC0 );	
	ALCD_Data( (Prod_Sel_Count-1) + 48 );
//MSDelay( 1000 );
	ALCD_Comm( 0x94 );	
	ALCD_Data( Prod_Sel_Count + 48 );
//MSDelay( 1000 );

	ALCD_Message( 0xC2, Prod_Name[Print_Prod_Sel[Prod_Sel_Count-1]-48] );
//MSDelay( 1000 );
	ALCD_Message( 0x96, Prod_Name[Print_Prod_Sel[Prod_Sel_Count]-48] );
//MSDelay( 1000 );

	ALCD_Message( 0xD1, Prod_Cost_Str[Print_Prod_Sel[Prod_Sel_Count-1]-48] );
//MSDelay( 1000 );
	ALCD_Message( 0xA5, Prod_Cost_Str[Print_Prod_Sel[Prod_Sel_Count]-48] );
//MSDelay( 1000 );


	ALCD_Message( 0xD4, Key1_Disp_Row4 );
	ALCD_Comm( 0xD8 );	
	ALCD_Data( Prod_Sel_Count + 48 );
	Tot_Shop_Cost = Tot_Shop_Cost - Prod_Cost_Intg[Product_Detect];
	Tot_Shop_Cost_Temp = Tot_Shop_Cost;
//MSDelay( 1000 );


	Tot_Shop_Cost_Str[3] = (Tot_Shop_Cost_Temp % 10) + 48;
	Tot_Shop_Cost_Temp = Tot_Shop_Cost_Temp / 10;
	Tot_Shop_Cost_Str[2] = (Tot_Shop_Cost_Temp % 10) + 48;
	Tot_Shop_Cost_Temp = Tot_Shop_Cost_Temp / 10;
	Tot_Shop_Cost_Str[1] = (Tot_Shop_Cost_Temp % 10) + 48;
	Tot_Shop_Cost_Temp = Tot_Shop_Cost_Temp / 10;
	Tot_Shop_Cost_Str[0] = (Tot_Shop_Cost_Temp % 10) + 48;
	ALCD_Message( 0xE4, Tot_Shop_Cost_Str );
//MSDelay( 1000 );

}


void Robot_Move_Req_Func(void)
{
	Ultrsonic_Obst_Flag = 0;
	if((RF_Tx0 == 1)&&(RF_Tx1 == 0)&&(RF_Tx2 == 0))
	{
ALCD_Message( 0xC0, "    BACKWARD    " );	
		Forward_L_R();
		MSDelay(1000);
		ALCD_Message( 0xC0, "                " );
	}	
	else if((RF_Tx0 == 0)&&(RF_Tx1 == 1)&&(RF_Tx2 == 0))
	{
ALCD_Message( 0xC0, "    FORWARD     " );	
		Reverse_L_R();
		MSDelay(1000);
		ALCD_Message( 0xC0, "                " );
	}	
	else if((RF_Tx0 == 1)&&(RF_Tx1 == 1)&&(RF_Tx2 == 0))
	{
		ALCD_Message( 0xC0, "     LEFT       "  );	
		Rotate_L();
		MSDelay(1000);
		ALCD_Message( 0xC0, "                " );
	}	
	else if((RF_Tx0 == 0)&&(RF_Tx1 == 0)&&(RF_Tx2 == 1))
	{
		ALCD_Message( 0xC0, "     RIGHT      "  );	
		Rotate_R();
		MSDelay(1000);
		ALCD_Message( 0xC0, "                " );
	}
	Ultrsonic_Obst_Flag = 1;
}


void Robot_OFF( )
{

	L_Motor_Data1 = 0;
	L_Motor_Data2 = 0;
	R_Motor_Data1 = 0;
	R_Motor_Data2 = 0;

}

void Forward_L_R( void )
{
	L_Motor_Data1 = 1;
	L_Motor_Data2 = 0;
	R_Motor_Data1 = 1;
	R_Motor_Data2 = 0;
	MSDelay(2000);

	L_Motor_Data1 = 0;
	L_Motor_Data2 = 0;
	R_Motor_Data1 = 0;
	R_Motor_Data2 = 0;
	MSDelay(100);

}
void Rotate_L( void )
{
	L_Motor_Data1 = 0;
	L_Motor_Data2 = 1;
	R_Motor_Data1 = 1;
	R_Motor_Data2 = 0;
	MSDelay(2000);

	L_Motor_Data1 = 0;
	L_Motor_Data2 = 0;
	R_Motor_Data1 = 0;
	R_Motor_Data2 = 0;
	MSDelay(100);

}


void Rotate_R( void )
{
	L_Motor_Data1 = 1;
	L_Motor_Data2 = 0;
	R_Motor_Data1 = 0;
	R_Motor_Data2 = 1;
	MSDelay(2000);

	L_Motor_Data1 = 0;
	L_Motor_Data2 = 0;
	R_Motor_Data1 = 0;
	R_Motor_Data2 = 0;
	MSDelay(100);

}


void Reverse_L_R( void )
{
	L_Motor_Data1 = 0;
	L_Motor_Data2 = 1;
	R_Motor_Data1 = 0;
	R_Motor_Data2 = 1;
	MSDelay(2000);

	L_Motor_Data1 = 0;
	L_Motor_Data2 = 0;
	R_Motor_Data1 = 0;
	R_Motor_Data2 = 0;
	MSDelay(100);

}




	MSDelay( 20 );							// 0.2 sec delay
	ALCD_Init( );

	MSDelay( 200 );	  
	ALCD_Message( 0x80, "     WELCOME    "  );// 0.2 sec delay
	MSDelay( 2000 );	
	ALCD_Message( 0xC0, "  SHOPPING MALL "  );
	MSDelay( 1000 );
	
	R_SAU0_Create();
	R_UART0_Start();
	MSDelay( 500 );
	
	R_UART1_Start();
	MSDelay( 500 );
	
	R_SAU1_Create( );
	R_UART2_Start();
	MSDelay( 500 );
	
	R_ADC_Create();
	R_ADC_Set_OperationOn();
	R_ADC_Start();
	MSDelay( 500 );
	
	
}

/* End user code. Do not edit comment generated here */
