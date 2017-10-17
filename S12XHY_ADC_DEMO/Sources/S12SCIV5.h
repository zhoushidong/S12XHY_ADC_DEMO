/******************************************************************************
                                                   COPYRIGHT (c) FREESCALE 2005  
                                                                          
File Name          :    $RCSfile: S12SCIV5.h,v $                          
                                                                          
Current Revision :      $Revision: 1.0 $                                  
                                                                          
PURPOSE: header file for S12P SCI register block                          
                                                                          
DESCRIPTION:  Defines the SCI register block as a datastructure of type tSCI.       
                                                                          
UPDATE HISTORY                                                            
REV  AUTHOR     DATE       DESCRIPTION OF CHANGE                          
---  ------    --------    ---------------------                          
1.0  r28318    11/06/09    - Adapted from S12/S08 SCI header files                      

     *******************************************************************
     * File created by: Freescale East Kilbride MCD Applications Group *
     *******************************************************************

                                                                          
******************************************************************************/
/*===========================================================================*/
/* Freescale reserves the right to make changes without further notice to any*/
/* product herein to improve reliability, function, or design. Freescale does*/
/* not assume any  liability arising  out  of the  application or use of any */
/* product,  circuit, or software described herein;  neither  does it convey */
/* any license under its patent rights  nor the  rights of others.  Freescale*/
/* products are not designed, intended,  or authorized for use as components */
/* in  systems  intended  for  surgical  implant  into  the  body, or  other */
/* applications intended to support life, or  for any  other application  in */
/* which the failure of the Freescale product  could create a situation where*/
/* personal injury or death may occur. Should Buyer purchase or use Freescale*/
/* products for any such intended  or unauthorized  application, Buyer shall */
/* indemnify and  hold  Freescale  and its officers, employees, subsidiaries,*/
/* affiliates,  and distributors harmless against all claims costs, damages, */
/* and expenses, and reasonable  attorney  fees arising  out of, directly or */
/* indirectly,  any claim of personal injury  or death  associated with such */
/* unintended or unauthorized use, even if such claim alleges that  Freescale*/
/* was negligent regarding the  design  or manufacture of the part. Freescale*/
/* and the Freescale logo* are registered trademarks of Freescale Ltd.       */
/*****************************************************************************/

#ifndef S12SCIV5         /*prevent duplicated includes */
#define S12SCIV5

/************************* Include Files *************************************/
#include "MC9S12XHY256.h"
//#include "S12P_peripherals.h"

/************************* typedefs ******************************************/

/************************* function prototypes *******************************/
void SCI_Config(tU16);
void PutChar (char);
char GetChar(void);
void EchoChar(void); 
char TestRx(tU08 txValue);
void DisplayString (signed char *textPointer);
void DisplayByte (unsigned char displayByte);
char EchoGetChar(void);
unsigned char Number_2_ASCII(unsigned char Nibble);
unsigned char ASCII_2_Number(unsigned char ascii_char);
void DisplayInt(unsigned int number, unsigned char field );
void DisplayLong(unsigned long number, unsigned char field );

#endif /*S12SCIV5 */

/************************* macros *******************************/
#define _CR DisplayString("\r")
#define _LF DisplayString("\n")
#define NEW_LINE DisplayString("\r\n")
#define FF DisplayString("\f")
#define LF NEW_LINE							/* for compatibility */
