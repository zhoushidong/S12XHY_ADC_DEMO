/******************************************************************************
                                                 COPYRIGHT (c) FREESCALE 2007   
FILE NAME: S12HY_peripherals.h                                             
                                                                          
PURPOSE: external declaration of on-chip peripheral variables for MC9S12HY64.                    
                            
                                                                          
DESCRIPTION:  external declarations for control register blocks of on-chip
              peripherals.
              Include this file in programs that interact with on-chip        
              peripherals.                                                            
                                                                          
UPDATE HISTORY                                                            
REV      AUTHOR      DATE         DESCRIPTION OF CHANGE                    
---      ------      ---------    ---------------------                   
1.0      ttz890      15/10/07     Initial coding 
1.1      r28318      15/06/09     Modified to suit S12P
1.1      r28318      14/09/09     Modified to suit S12HY


      *****************************************************************
      *File created by: Freescale East Kilbride MCD Applications Group*
      *****************************************************************

                                                                          
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
                           /*prevent duplicated includes */
#ifndef per_S12HY_H       
#define per_S12HY_H


#if (__MWERKS__)
  #pragma MESSAGE DISABLE C1106
#endif
          
#include "S12_COMMON.h"
//#include "S12ADC12B10CV1.h"	 /* ATD */
//#include "S12MSCANV3.h"			 /* CAN */
//#include "S12CPMUV1.h"			 /* CPMU */
//#include "S12SDBGV2.h"			 /* DBG */
//#include "S12FTMRC128K1V1.h" /* FTMRC */
//#include "S12SINTV1.h"		   /* INT */
//#include "S12PMMCV1.h"			 /* MMC */
//#include "S12PPIMV1.h"		   /* PIM */
//#include "S12PWM8B6CV1.h"		 /* PWM */
#include "S12SCIV5.h"				 /* SCI */
//#include "S12SPIV5.h"				 /* SPI */
//#include "S12TIM16B8CV2.h"	 /* TIM */

#endif /* per_S12P_H */
