#include "target.h"  /* includes device peripheral declarations */
#include "S12HY_peripherals.h"
#include <MC9S12XHY256.h>     /* derivative information */

/******************************************************************************
Function Name	:	SCI_Config
Engineer		:	r32151
Date			:	05/03/02

Parameters		:	NONE
Returns			:	NONE
Notes			:	Configures SCI0

The PutChar function puts out char to the SCI and can be seen on the TXD pin.
The EchoChar function takes in char from SCI (RXD) then puts it out TXD.
******************************************************************************/
void
SCI_Config(unsigned int prescaler)
{
			/* set SCI0 baud rate */
	SCI0BDH = (unsigned char)((prescaler>>8));
	SCI0BDL = (unsigned char)(prescaler);
			/* set SCI0 control register 1 */
  SCI0CR1 = 0x00;
			/* enable SCI0 transmit */
	SCI0CR2_TE = 1;
			/* enable SCI0 receive */
	SCI0CR2_RE = 1;
}

/******************************************************************************
Function Name	:	PutChar
Engineer		:	r32151
Date			:	22/01/02

Parameters		:	char to be output to SCI
Returns			:	NONE
Notes			:	
******************************************************************************/
void PutChar(char ch) 
{
	/* check SCI0 transmit data register is empty */
	while(SCI0SR1_TDRE == 0)
	{
	}	
	SCI0DRL = ch;	
}

/******************************************************************************
Function Name	:	GetChar
Engineer		:	r32151
Date			:	04/11/02

Parameters		:	char to be output to SCI
Returns			:	NONE
Notes			:	
******************************************************************************/
char GetChar(void) 
{
	/* check SCI0 transmit data register is empty */
	while(SCI0SR1_RDRF == 0)
	{
	}	
	return(SCI0DRL);	
}

/******************************************************************************
Function Name	:	EchoChar
Engineer		:	r32151
Date			:	22/01/02

Parameters		:	char to be output to SCI
Returns			:	NONE
Notes			:	
******************************************************************************/
void EchoChar(void) {
	PutChar(GetChar());	
}


/******************************************************************************
Function Name	:	TestRx
Engineer		:	r32151
Date			:	04/11/02

Parameters		:	 
Returns			:	NONE
Notes			:	
******************************************************************************/
char TestRx(tU08 txValue) 
{
  tU08 status;
	/* check SCI0 transmit data register is empty */
	while(SCI0SR1_RDRF == 0)
	{
	}	
	
	/* ADD ERROR TEST HERE TO SEE IF ANY ERRORS FLAGGED   */
      /* can check noise flag here */
      /* can check Framing error flag here */      
      /* can check Parity error flag here */
	if (SCI0DRL != txValue) {
	   status = FAIL;
	} 
	else {
	   status = PASS;
  }     
  return (status); 
}

/******************************************************************************
Function Name	:	DisplayString
Engineer		:	r58711
Date			:	14/02/02

Parameters		:	textString[]
Returns			:	NONE
Notes			:	Output text string as ASCII to SCI
******************************************************************************/
void DisplayString (signed char *textPointer)
{
		/* while not end of string */
	while(*textPointer != 0)
	{	
			/* write the character to the SCI interface */
		PutChar(*textPointer);
			/* increment to point at the next character in the string */
		textPointer++;
	}	
}

/******************************************************************************
Function Name	:	DisplayByte
Engineer		:	r58711
Date			:	14/02/02

Parameters		:	displayByte
Returns			:	NONE
Notes			:	Output variable SCI after conversion to ASCII
******************************************************************************/
void DisplayByte (unsigned char displayByte)
{
	unsigned char upperNibble, lowerNibble;
		/* Split data into nibbles for ASCII conversion */
	upperNibble = ((displayByte & 0xF0)>>4); 
	lowerNibble = (displayByte & 0x0F);
		/* Pass nibbles to SCI after passing for conversion to ASCII */
	PutChar(Number_2_ASCII(upperNibble));
	PutChar(Number_2_ASCII(lowerNibble));
}

/******************************************************************************
Function Name	:	EchoGetChar
Engineer		:	r32151
Date			:	04/11/02

Parameters		:	char to be output to SCI
Returns			:	NONE
Notes			:	
******************************************************************************/
char EchoGetChar(void) 
{
		/* check SCI1 receive data register is empty */
	while(SCI0SR1_RDRF == 0)
	{
	}	
	//echochar = TerminalPointer->SCID.Byte;
	PutChar(SCI0DRL);
	return(SCI0DRL);	
}

/******************************************************************************
Function Name	:	Number_2_ASCII
Engineer		:	r58711
Date			:	15.11.01

Parameters		:	Nibble of binary data (0000 to 1111) (unsigned char)
Returns			:	ASCII code for nibble (unsigned char)
*******************************************************************************/
unsigned char Number_2_ASCII(unsigned char Nibble)
{	
	/* If nibble is less than 9 (ie. a number) */
	if (Nibble <= 0x09) 
		/* Return the ASCII code for this number */
		return (Nibble + 0x30); 
	else
		/* Return the ASCII code for this letter */
		return (Nibble + 0x37); 
} 

/******************************************************************************
Function Name	:	ASCII_2_Number
Engineer		:	r58711
Date			:	15.11.01

Parameters		:	ASCII data byte (unsigned char)
Returns			:	numeric equivalent (unsigned char) of ASCII code 
*******************************************************************************/
unsigned char ASCII_2_Number(unsigned char ascii_char)
{
  byte number;	
	/* If character is 9 or less (ie. a number) */
	number = ascii_char - 0x30; 
	/* If character is greater than 9 (ie. hex number A,B,C,D,E,F) */
	if (number > 0x09) number -= 0x07;
	/* If chracter is greater than 9 and lower case (ie. hex number a,b,c,d,e,f) */
  if (number > 0x0F) number -= 0x20;	
  /* If charcter is non-numeric default to 0 */
  if (number > 0x0F) number = 0;
	return (number); 
} 

/******************************************************************************
Function Name	:	DisplayInt
Engineer		:	r32151
Date			:	04/09/02

Parameters		:	int count value to display
					unsigned char field width
Returns			:	NONE
Notes			:	Effectively perforns a number to string conversion and then
					outputs the string.
					if field width = 0 then the int is displayed left justified   
					with leading zeros supressed.
					if field width = 1-5 then lowest n digits are displayed
					with leading zeros.
					This routine does not check that the field size is valid.					
******************************************************************************/
void DisplayInt(unsigned int number, unsigned char field ) 
{
	char buf[6];
	char i = 5;

	buf[i] = 0;
			/* for upto 10 digits : max value for long = 4294967296 */
			/* using a do while loop to take care of the number = 0 */
	do
	{
				/* convert least significant character of the int into ascii -> string buffer */
		buf[--i] = (char)(number % 10) + '0';
				/* moves next digit to least significant position */
		number /= 10;
	} while (number != 0); 
				/* if field width is less than number of digits only o/p field width of string */
	if ((field < (5-i)) && (field != 0))
		i = 5 - field;
				/* if field width is more than number of digits add leading zeros */
	if (field > (5-i))
	{
		while ((5-i) < field)
	    {
				/* add leading zeros */ 
		buf[--i] = '0';
		}
	}
	DisplayString(&buf[i] );	
}
	
/******************************************************************************
Function Name	:	DisplayLong
Engineer		:	r32151
Date			:	04/09/02

Parameters		:	long count value to display
					unsigned char field width
Returns			:	NONE
Notes			:	Direct replacement for NumberDisplay().

					Effectively perforns a number to string conversion and then
					outputs the string.
					if field width = 0 then the int is displayed left justified   
					with leading zeros supressed.
					if field width = 1-10 then lowest n digits are displayed
					with leading zeros.
					This routine does not check that the field size is valid.
					
					Due to the 32 bit math routines for handling long values
					code size and execution time is significantly larger that
					for DisplayInt.
******************************************************************************/
void DisplayLong(unsigned long number, unsigned char field ) 
{
	char buf[11];
	char i = 10;
	char length = 0;

	buf[i] = 0;
			/* for upto 10 digits : max value for long = 4294967296 */
			/* using a do while loop to take care of the number = 0 */
	do
	{
				/* convert least significant character of the int into ascii -> string buffer */
		buf[--i] = (char)(number % 10) + '0';
				/* moves next digit to least significant position */
		number /= 10;
	} while (number != 0); 
				/* if field width is less than number of digits only o/p field width of string */
	if ((field < (10-i)) && (field != 0))
		i = 10 - field;
				/* if field width is more than number of digits add leading zeros */
	if (field > (10-i))
	{
		while ((10-i) < field)
	    {
				/* add leading zeros */ 
		buf[--i] = '0';
		}
	}
	DisplayString(&buf[i] );	
}	
