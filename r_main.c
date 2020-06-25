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
* Creation Date: 13/01/2018
* author(s)    : Nikhil, switaja, Karthik, Chaithanya
***********************************************************************************************************************/

/***********************************************************************************************************************
Pragma directive
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

extern unsigned char Rx_data_arr0[400], Rx_data_arr1[100],Rx_data_arr2[100];
extern unsigned char Rx_count0, Rx_count1,Rx_count2;
extern unsigned char Rx_ST_Flag0, Rx_ST_Flag1,Rx_ST_Flag2;
extern unsigned char Rx_data0, Rx_data1,Rx_data2;

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
	
	
    while (1U)
    {
        ;
    }
    /* End user code. Do not edit comment generated here */
}

/* Start user code for adding. Do not edit comment generated here */

void Device_Init( void )
{
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
