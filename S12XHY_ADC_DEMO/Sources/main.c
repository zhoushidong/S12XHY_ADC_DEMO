//#pragma LINK_INFO DERIVATIVE "mc9s12p128"

/******************************************************************************
													            Copyright (c) Freescale 2009
File Name    : $RCSfile: main.c,v $

Current Revision :	$Revision: 1.0 $

PURPOSE: main program entry.                       
                                                                          
                                                                       
DESCRIPTION:  function main() providing initial program entry.
                                                                          
UPDATE HISTORY                                                            
REV  AUTHOR    DATE        DESCRIPTION OF CHANGE                          
---  ------    --------    ---------------------                          
1.0  r28318    01/10/09    - initial coding
2.0  b30269    17/11/10    - Migrated to SC12XHY, CRG_8MHz function was added for clock
                             setup

     *******************************************************************
     * File created by: Freescale East Kilbride MSG Applications Group *
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

/************************* Include Files *************************************/
/*general includes */
#include <hidef.h>  /* also includes boolean definitions in stdtypes.h     */
#include <MC9S12XHY256.h>     /* derivative information */
#include "target.h" 
#include "S12HY_peripherals.h" /* includes peripherals definitions and FSL data types */


/************************* typedefs ******************************************/

/************************* #defines ******************************************/

/************************* Constants *****************************************/
#pragma CONST_SEG DEFAULT

#define CONST (BUSCLK_FREQ_KHZ / 6 /* loop cycles */ )

/************************* Global Variables **********************************/
#pragma DATA_SEG DEFAULT

/************************* function prototypes *******************************/
#pragma CODE_SEG DEFAULT
/************************* Functions *****************************************/
#pragma CODE_SEG DEFAULT


/**********************************************************
*	CRG_8MHz
*
*	Setup clock module for PLL Engaged using externals XTAL
*	For S12XHY112LQFP w/ 8MHz XTAL -  Bus Clk - 32MHz
* Suggestion: You can use iPLL calculator to speed up CRG 
*             registers configuration (look for S12XESW.zip).
**********************************************************/
void CRG_8MHz(void)
{

    SYNR             = 0x43;    /*64MHz VCO*/
    REFDV            = 0x80;    /*48 MHz <Ref Clock (8MHz) < 80MHz */
    POSTDIV          = 0x00;    /*FBus = Fvco/(2*POSTDIV) = 32MHz*/ 
    while(!CRGFLG_LOCK)
    {
       ; /* wait for VCO to stabilize */
    }			 
    CLKSEL_PLLSEL = 1;/* Switch clk to use PLL */
}


/******************************************************************************
Function Name  : main
Engineer       : r28318	
Date           : 09/06/09
Parameters     : NONE
Returns        : NONE
Notes          : main routine called by Startup.c. 
******************************************************************************/
void main(void) {
   unsigned int ContinuousResult, x, y;

	 CRG_8MHz();
	 
   SCI_Config(TERMINAL_BAUD_PLL);
   LF;
   DisplayString("DEMOS12PFAME Board Running");
   LF;

   PTR = 0x01;  /* Initialise PORT R 0 High; turn off LED */
   DDRR = 0x01;   /* PORTR 0 = output */

   PER1AD = 0xF4; /* Enable pullups on Switches 1-4, and Transceiver Force Off */
   ATDDIENL = 0xF0;
   
   /* Wait for pull ups to come into effect */
  	for (x=0; x<1000; x++)
  	{	
  		for (y=0; y<100; y++)
  		{} 
	  }

    /****** Select ATD via Reset & Switches *******/
    /*                                            */
    /*     Default = Single 12-Bit Conversion     */
    /* Reset + SW1 = Continuous 8-Bit Conversions */
    /* Reset + SW2 = 12-Bit Automatic Compare     */
    
      switch (PT1AD&0xF0)
      {
            
      case 0xD0: /* Reset + SW2 12-Bit Automatic Compare */
      
         DisplayString("12-Bit Automatic Compare, greater than 0x07FF");
         LF;
        
         /* 12-Bit Automatic Compare */
         /* Initialize ADC */
                      
         ATDCTL1 = 0x4F;    /* 12-Bit resolution */              

         ATDCTL3 = 0x90;    /* Right Justified Data, two conversions / sequence */              

         ATDCTL4 = 0x01;    /* 8MHz ATD Clock, 4 ATD Clock cycles sample time */              

         ATDDR0 = 0x07FF;   /* Comparison value */

         ATDCMPHT = 0x0001; /* Compare higher than on conversion AN0 */

         ATDCMPE = 0x0001;  /* Enable comparisons for conversion AN0 */

         ATDCTL5 = 0x20;    /* Start Continuous Conversions on PAD00 */              

         while(1)
         {
            while (!ATDSTAT0_SCF);     /* wait for conversion sequence to complete */
            ContinuousResult = ATDDR1; /* store AN1 conversion result to avoid overwriting */
            if (ATDSTAT2_CCF0)         /* Test to see if comparison is true */
              {
                PTR_PTR0 ^=1;     /* Toggle LED1 */
              }
            else
              {
                PTR_PTR0 = 1; /* Turn off LED1 */
              }
              
            DisplayString("PAD00 AN1 Conversion = 0x");
            DisplayByte((unsigned char)(ContinuousResult>>8));
            DisplayByte((unsigned char)ContinuousResult);
            LF;

            /* Wait to display conversion result */
            for (x=0; x<2000; x++)
            {	
              for (y=0; y<500; y++)
              {} 
            }
            ATDSTAT0_SCF = 1; /* Clear Sequence Complete Flag */
            ATDSTAT2_CCF0= 1; /* Clear Conversion Complete Flag */
         }
   
/*        This example performs two conversions per sequence, AN0 and AN1 on the same pin PAD00. 
          AN0 is used for the automatic compare. AN1 is written to the terminal window. With 
          automatic compare enabled for AN0, the ATDDR0 register is used to store the compare 
          value, the result will not be stored there at the end of conversion, but is lost. AN1 
          is written to the terminal window to give an indication of the value of AN0. 
          In an ideal environment with no noise AN0=AN1, however as there will be some noise present 
          it is possible for AN0 and AN1 to differ slightly. When AN0 and AN1 approach the compare 
          value stored in ATDDR0, differences between them may give the appearance that the automatic 
          compare feature is behaving erroneously 
*/

      break;
      
      case 0xE0: /* Reset + SW1 Continuous 8-Bit Conversions */
                    
         DisplayString("8-Bit Continuous Conversion");
         LF;

         /* Continuous 8-Bit Conversion */
         /* Initialize ADC */
                      
         ATDCTL1 = 0x0F;  /* 8-Bit resolution */              

         ATDCTL3 = 0x88;  /* Right Justified Data, Single conversion sequence */              

         ATDCTL4 = 0x01;  /* 8MHz ATD Clock, 4 ATD Clock cycles sample time */              

         ATDCTL5 = 0x20;  /* Start Continuous Conversions on PAD00 */              

         while(1)
         {
            while (!ATDSTAT0_SCF);  /* wait for conversion sequence to complete */
            ContinuousResult = ATDDR0;   /* store result in variable to avoid overwriting */
            ATDSTAT0_SCF = 1;            /* Clear Sequence Complete Flag for next conversion */
            DisplayString("PAD01 Conversion = 0x");
            DisplayByte((unsigned char)(ContinuousResult>>8));
            DisplayByte((unsigned char)ContinuousResult);
            LF;
            
            /* Wait to display conversion result */
            for (x=0; x<2000; x++)
            {	
              for (y=0; y<500; y++)
              {} 
            }
         }
      
      break;
      
      default: /* Reset Single 12-Bit Conversion */

         DisplayString("12-Bit Single Conversion");
         LF;
                
         /* Single 12-Bit Conversion */
         /* Initialize ADC */
                      
         ATDCTL1 = 0x4F;  /* 12-Bit resolution */              

         ATDCTL3 = 0x88;  /* Right Justified Data, Single conversion sequence */              

         ATDCTL4 = 0x01;  /* 8MHz ATD Clock, 4 ATD Clock cycles sample time */              

         ATDCTL5 = 0x00;  /* Start Single Conversion on PAD00 */              

         while(1)
         {
            while (!ATDSTAT0_SCF);  /* wait for conversion sequence to complete */
            DisplayString("PAD00 Conversion = 0x");
            DisplayByte((unsigned char)(ATDDR0>>8));
            DisplayByte((unsigned char)ATDDR0);
            LF;

            /* Wait to debounce SW4 */
            for (x=0; x<2000; x++)
            {	
              for (y=0; y<500; y++)
              {} 
            }
            
            while (PT1AD_PT1AD7); /* Wait for SW4 before starting next conversion */
            ATDCTL5 = 0x00;  /* Start Single Conversion on AN0; simultaneously clears SCF */              
            
         }
      }
      

   /* Loop Forever. Code should never reach here */
   while(1);
}


