//--------------------------------------------------------------------------------------------------------------------
// 417_Radio_GSM.c
//--------------------------------------------------------------------------------------------------------------------

// AUTH:	SHASHI KIRAN
// DATE:	09/09/2013

// MODIFIED BY:
// MODIFIED DATE:

// Program to 

// Target: PROJECT BOARD of RENESAS G13 R5F100LEA (PES-49-1212)

// Tool chain: CubeSuite+

//--------------------------------------------------------------------------------------------------------------------
// Includes
//--------------------------------------------------------------------------------------------------------------------



//--------------------------------------------------------------------------------------------------------------------
// Global VARIABLES
//--------------------------------------------------------------------------------------------------------------------

extern unsigned char Rx_data_arr0[400], Rx_data_arr1[100], Rx_data_arr2[100];
extern unsigned char Rx_count0, Rx_count1, Rx_count2;
extern unsigned char Rx_ST_Flag0, Rx_ST_Flag1, Rx_ST_Flag2;
extern unsigned char Rx_data0, Rx_data1, Rx_data2;



unsigned char GSM_Send_SMS_Comm[] = "AT+CMGS=";
unsigned char GSM_Rx_SMS_Comm[] = "AT+CNMI=2,2,0,0,0";
unsigned char Rx_Mob_Num[11] = "0000000000";

char GSM_SMS[]="SYSTEM STARTS1234";
char Mb_Num[11]="9481265024";



unsigned char colon = 0x22;
unsigned char CARRIAGE = 0x0D;
unsigned char CntrlZ = 0x1A;


unsigned char PC_Data_SMS_Format[5] = "xx00";


//--------------------------------------------------------------------------------------------------------------------
// Global CONSTANTS
//--------------------------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------------------------
// Function PROTOTYPES
//--------------------------------------------------------------------------------------------------------------------

void GSM_Init( void );
void GSM_Send_SMS( char *Mb_Num1, char *SMS );
void GSM_Rx_SMS( void );
void SMS_Rx_Poll( void );
void St_Stepper_Anti_Clk( unsigned int Rotate_Cycles );
void St_Stepper_clock( unsigned int Rotate_Cycles );
void GPS_Read_SMS_Send( void );



//--------------------------------------------------------------------------------------------------------------------
// void GSM_Init( void )
//--------------------------------------------------------------------------------------------------------------------
// Function Name:	void GSM_Init( void )
// Arguments	:	No arguments
// Return Value	:	No return value
// Description	:	This function is used to test the GSM module. If we send a command AT then
// the GSM module will reply OK.

void GSM_Init( void )
{
	R_UART1_Send( "A", 1 );
	MSDelay(3);
	R_UART1_Send( "T", 1 );
	MSDelay(3);
	R_UART1_Send( &CARRIAGE, 1 );				// ASCII value of CARRIAGE RETURN
	MSDelay(3);
}

void GSM_Send_SMS( char *Mb_Num1, char *SMS )
{
	unsigned char i;

// SMS send command	is sent through UART
	for( i=0; GSM_Send_SMS_Comm[i] != '\0'; i++ )
	{
		R_UART1_Send( &GSM_Send_SMS_Comm[i], 1 );
		MSDelay(3);
	}
	
	R_UART1_Send( &colon,1 );				// ASCII value of "
	MSDelay(3);

		
	for( i=0; Mb_Num1[i] != '\0'; i++ )
	{
		R_UART1_Send( &Mb_Num1[i], 1 );
		MSDelay(3);
	}

	R_UART1_Send( &colon, 1 );				// ASCII value of "
	MSDelay(3);
	R_UART1_Send( &CARRIAGE, 1 );				// ASCII value of CARRIAGE RETURN
	MSDelay(3);
//	UART_Tx_char( 0x0A );				// ASCII value of LINE FEED
	MSDelay(4000);						


// Text message is sent through UART
	for( i=0; SMS[i] != '\0'; i++ )
	{
		R_UART1_Send( &SMS[i], 1 );
		MSDelay(3);
	}

	R_UART1_Send( &CntrlZ, 1 );				// UART transmission starts
	MSDelay(3);
}

void GSM_Rx_SMS( void )
{
	unsigned char i;

// SMS send command	is sent through UART
	
	for( i=0; GSM_Rx_SMS_Comm[i] != '\0'; i++ )
	{
		R_UART1_Send( &GSM_Rx_SMS_Comm[i],1 );
		MSDelay(3);
	}

	R_UART1_Send( &CARRIAGE, 1 );				// ASCII value of CARRIAGE RETURN
	MSDelay(3);
}


void SMS_Rx_Poll( void )
{
	unsigned char i, j; 

	// At this stage, we assume that the SMS as been received.
	// The received SMS contains lots of extra informations, which are not needed. So check for -->  22"	


	// To retrive the mobile number of received SMS
	i = 0;
	while( Rx_data_arr1[i] != '\0' )
	{
		if( (Rx_data_arr1[i] == '+') && (Rx_data_arr1[i+1] == '9') && (Rx_data_arr1[i+2] == '1'))
			break;
		i++;
	}
	// To store the mobile number of received SMS
	i = i+3;
	for( j=0; j<10; j++,i++ )
		Rx_Mob_Num[j] = Rx_data_arr1[i];

//	ALCD_Message( 0xC0, Rx_Mob_Num );
	MSDelay(2000);

	i = 0;
	while( Rx_data_arr1[i] != '\0' )
	{
		if( (Rx_data_arr1[i] == '2') && (Rx_data_arr1[i+1] == '2') && (Rx_data_arr1[i+2] == '"'))
			break;
		
		i++;
	}

										
	// Here we are storing the SMS received at 5th location of array of structure.
	// The received SMS format is -- Message(Max 30), 3 space
	i = i+5;
	j = i;
	while( i<(j+6) )
	{
	// Scan for 3 space to come out of this while loop
		if( Rx_data_arr1[i] == 'V' )
		{
			if( (Rx_data_arr1[i+1] == 'S') )
			{
				ALCD_Message( 0x80, "  VEHICLE START  " );
				MSDelay(2000);
			}
			break;
		}
		
		i++;
	}
}



