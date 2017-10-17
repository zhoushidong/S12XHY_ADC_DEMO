/******************************************************************************
*                                             COPYRIGHT (c) Freescale 2007   
* FILE NAME: target.h     REVISION 2.0                                        
*                                                                           
* PURPOSE: header file for EVB program  							 
*                                                                           
*******************************************************************************
*******************************************************************************  
**  THIS CODE IS ONLY INTENDED AS AN EXAMPLE FOR THE METROWERKS COMPILER AND **  
**      THE STAR12 EVB AND HAS ONLY BEEN GIVEN A MIMIMUM LEVEL OF TEST.      **  
**  IT IS PROVIDED 'AS SEEN' WITH NO GUARANTEES AND NO PROMISE OF SUPPORT.   **  
*******************************************************************************  
*******************************************************************************
*                                                                           
*                                                                           
* DESCRIPTION:  definitions for target specifics such as clock speed        
*                                                                           
* NOTE: Modify this file depending on the crystal frequency of the target   
*                                                                           
* AUTHOR: Martyn Gallop        LOCATION: EKB      LAST EDIT DATE: 01/03/02  
*                                                                           
* UPDATE HISTORY                                                            
* REV      	AUTHOR      DATE       	DESCRIPTION OF CHANGE                    
* ---      	------      ---------   ---------------------                    
* 1.0      	r32151    	03/06/01	                                         
* 1.1			r32151    	30/07/01 - Added KHz version of XTAL_FREQ
*											- Reformated file info section
* 1.2			r32151		09/10/01	- added equations from xtal.h
* 2.0			r32151  		01/03/02	- Rewritten to use 'long' values and 
*									  		  support rewrite of SetPLL => PLL_Set.
*											- PLL is disabled by default
* 2.1			r32151		13/10/03	- Added checks for ATD and PLL clocks
* 3.0			r32151		22/12/03 - Expanded memory remap support.
*                                  #included projecglobals.h.
*
* 3.1     b06320    18/06/07    Adjusted to suit S12XS               
* 3.2     r28318    15/06/07    Adjusted to suit S12P
*                                                                           
******************************************************************************/

/*===========================================================================*/
/* Freescale reserves the right to make changes without further notice to any */
/* product herein to improve reliability, function, or design. Freescale does */
/* not assume any  liability arising  out  of the  application or use of any */
/* product,  circuit, or software described herein;  neither  does it convey */
/* any license under its patent rights  nor the  rights of others.  Freescale */
/* products are not designed, intended,  or authorized for use as components */
/* in  systems  intended  for  surgical  implant  into  the  body, or  other */
/* applications intended to support life, or  for any  other application  in */
/* which the failure of the Freescale product  could create a situation where */
/* personal injury or death may occur. Should Buyer purchase or use Freescale */
/* products for any such intended  or unauthorized  application, Buyer shall */
/* indemnify and  hold  Freescale  and its officers, employees, subsidiaries, */
/* affiliates,  and distributors harmless against all claims costs, damages, */
/* and expenses, and reasonable  attorney  fees arising  out of, directly or */
/* indirectly,  any claim of personal injury  or death  associated with such */
/* unintended or unauthorized use, even if such claim alleges that  Freescale */
/* was negligent regarding the  design  or manufacture of the part. Freescale */
/* and the Freescale logo* are registered trademarks of Freescale Ltd.         */
/*****************************************************************************/

#ifndef TARGET_H        /*prevent duplicated includes*/
#define TARGET_H

/****User Defines ****/

									/* Oscillator frequency in kHz */
#define	OSCCLK_FREQ_KHZ		8000L	/* "L" postfix evaluated as a long value */

#define PLL_USED_FOR_BUS	/* Enable PLL. Bus frequency =   */
									/*   fosc * multiplier / divider */

#define PLL_DIVIDER  		1		/* Pll Reference divider value    */

#define REFCLK_RANGE	2 /* 0 = 1 to 2MHz  */
                        /* 1 = 2 to 6MHz  */
                        /* 2 = 6 to 12MHz */
                        /* 3 = >12MHz     */

#define PLL_MULTIPLIER  	4  		/* Pll multiplier value  */ 

#define VCO_RANGE	1 /* 0 = 32 to 48MHz, 1 = 48 to 64MHz VCO Range */								

#define PLL_POSTDIVIDER 1 /* Pll Post divider value*/


#define FCLK_PRESCALER 32          /* must be 1 to 32 */ 

#define TIMER_PRESCALER_BITS    7   /* value 0-7; prescaler = 2^^n = 16*/
								    /* Osc Timer period = 4us */
								    /* PLL Timer period = 1us */
/**** MCU Defines ****/

/* defined here intially for the sake of convenience. Ideally, the stack */
/* locations will be provided by the linker in the future */
#define CPU_VECTOR_BASE 0x3F /* 0x3F00 - 0x3FFF ; default is 0xFF, 0xFF00 - 0xFFFF*/

#define BUSCLK_FREQ_KHZ_MAX 32000L	/* "L" postfix evaluated as a long value */


	/* Bus frequency in kHz = fosc * (SYNR+1) / (REFDV+1) */
#define BUSCLK_FREQ_KHZ_PLL		(OSCCLK_FREQ_KHZ*((PLL_MULTIPLIER/PLL_DIVIDER)/PLL_POSTDIVIDER))
#if(BUSCLK_FREQ_KHZ_PLL > BUSCLK_FREQ_KHZ_MAX)
#error PLL clock out of range.
#endif /* Incorrect PLL frequency. */		

	/* Bus frequency in kHz = fosc / 2 */
#define BUSCLK_FREQ_KHZ_OSC		OSCCLK_FREQ_KHZ/2

#ifdef PLL_USED_FOR_BUS
#define BUSCLK_FREQ_KHZ BUSCLK_FREQ_KHZ_PLL
#else
#define BUSCLK_FREQ_KHZ BUSCLK_FREQ_KHZ_OSC
#endif

#define ECLK_OUT 0 /* Enable ECLK Set to 1 or comment out to disable ECLK */

#define ECLKDIV 1  /* must be 1 to 32 */

/* timer defines */
#define MS_COUNT_OSC	(BUSCLK_FREQ_KHZ_OSC / (1 << TIMER_PRESCALER_BITS))
#define MS_COUNT_PLL	(BUSCLK_FREQ_KHZ_PLL / (1 << TIMER_PRESCALER_BITS))

/* ATD defines */
#define ATD0_CLK_KHZ_OSC 1000	/* should be between 250 - 8333 KHz) */
/* Note - with 2MHz clock the max ATD clock freq is 1000KHz */

#define ATD0_CLK_KHZ_PLL 1000	/* should be between 500 - 2000 KHz) */
/* Note - with 50MHz clock the min ATD clock freq is 650KHz */

#define ATD0_PRESCALER_PLL (((BUSCLK_FREQ_KHZ_PLL/ATD0_CLK_KHZ_PLL)/2)-1)  
#if ((ATD0_PRESCALER_PLL > 0x20) || (ATD0_PRESCALER_PLL < 0))
#error ATD0 prescaler or oscillator clock out of range (1c).
#endif /* Incorrect ATDCLK frequency. */


/* SCI defines */ 
#define TERMINAL_BAUD_RATE	9600
#define SPARE_BAUD_RATE 	9600

#define TERMINAL_SCI SCI0
#define SPARE_SCI SCI1

#define TERMINAL_BAUD_OSC ((BUSCLK_FREQ_KHZ_OSC * 10) / (16 * (TERMINAL_BAUD_RATE / 100)))
#define SPARE_BAUD_OSC ((BUSCLK_FREQ_KHZ_OSC * 10) / (16 * (SPARE_BAUD_RATE / 100)))

#define TERMINAL_BAUD_PLL ((BUSCLK_FREQ_KHZ_PLL * 10) / (16 * (TERMINAL_BAUD_RATE / 100)))
#define SPARE_BAUD_PLL ((BUSCLK_FREQ_KHZ_PLL * 10) / (16 * (SPARE_BAUD_RATE / 100)))

/*CAN bit rate defines */
#define CAN0_KBPS 	50	/* 1000 kbps MAXIMUM */
#define CAN0_TQPB	8		/* 8 Tq per bit */

#if((OSCCLK_FREQ_KHZ / (CAN0_KBPS * CAN0_TQPB)) < 1)
#define CANBTR0_T2 (0x40)
#elif((OSCCLK_FREQ_KHZ % (CAN0_KBPS * CAN0_TQPB)) == 0)
#define CANBTR0_T2 (0x40 + (UINT8)(OSCCLK_FREQ_KHZ / (CAN0_KBPS * CAN0_TQPB)) - 1)
#else
#define CANBTR0_T2 (0x40 + (UINT8)(OSCCLK_FREQ_KHZ / (CAN0_KBPS * CAN0_TQPB)))
#endif

#define CANBTR1_T2 0x14	/* SAMP = 0, TSEG2 = 2, TSEG1 = 5, 8 Tq per bit */

/* IO Defines */
#define ALL_FULL_DRIVE		0x00   
#define ALL_REDUCED_DRIVE	0xFF

#define ALL_PULLS_OFF		0x00
#define ALL_PULLS_ON	      0xFF

#define ALL_PULLED_UP		0x00   
#define ALL_PULLED_DOWN		0xFF

#define ALL_INPUTS			0x00
#define ALL_OUTPUTS			0xFF	

#define ALL_LOW				0x00
#define ALL_HIGH			   0xFF


#endif /* TARGET_H */
