/* ###################################################################
**     Filename    : main.c
**     Project     : ee128_mini
**     Processor   : MK64FN1M0VLL12
**     Version     : Driver 01.01
**     Compiler    : GNU C Compiler
**     Date/Time   : 2024-12-02, 12:44, # CodeGen: 0
**     Abstract    :
**         Main module.
**         This module contains user's application code.
**     Settings    :
**     Contents    :
**         No public methods
**
** ###################################################################*/
/*!
** @file main.c
** @version 01.01
** @brief
**         Main module.
**         This module contains user's application code.
*/         
/*!
**  @addtogroup main_module main module documentation
**  @{
*/         
/* MODULE main */


#include "Cpu.h"
#include "Events.h"
#include "Pins1.h"
#include "SS1.h"
#include "CsIO1.h"
#include "IO1.h"
/* Including shared modules, which are used for whole project */
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"
#include "PDD_Includes.h"
#include "Init_Config.h"
#include "MK64F12.h"
// Sets Block_size to 1 byte for easy communicating
#define BLOCK_SIZE 1
/* User includes (#include below this line is not maintained by Processor Expert) */
uint8_t InpData[BLOCK_SIZE];
volatile bool DataReceivedFlag = FALSE;
volatile LDD_SPIMASTER_TError ComError = 0U;
LDD_TError Error;
LDD_TDeviceData *MySPIPtr;
/*lint -save  -e970 Disable MISRA rule (6.3) checking. */
int main(void)
/*lint -restore Enable MISRA rule (6.3) checking. */
{
  /* Write your local variable definition here */

  /*** Processor Expert internal initialization. DON'T REMOVE THIS CODE!!! ***/
  PE_low_level_init();
  /*** End of Processor Expert internal initialization.                    ***/
	// Setting GPIO and Output/Input pins
	SIM_SCGC5 |= SIM_SCGC5_PORTC_MASK;

	PORTC_GPCLR = 0x009F0100;
	// Initializing;
	GPIOC_PDDR = 0x00FF;
	uint32_t delay;
	// SPI communicating with SS1 library for slave capabilities
	MySPIPtr = SS1_Init(NULL);
	LDD_TDeviceData *SS1_DeviceData;

	unsigned long i = 0;
  for(;;)
    {
	  LDD_TError status = SS1_ReceiveBlock(MySPIPtr, InpData, BLOCK_SIZE);
	  // Prints for debugging
	          if (status == ERR_OK) {
	              while (!DataReceivedFlag); // Wait until data is fully received

	              // Process received data
	              printf("Received: %02X\n", InpData[0]); // Print as hex

	              DataReceivedFlag = FALSE; // Reset flag
	          }

	  // Motor state logic
      if(InpData[0] == 0x81){
    	  GPIOC_PDOR = 0x00;}
      else if(InpData[0] == 0x01){
    	  GPIOC_PDOR = 0x96;
    	  for(i=0; i < 10000; i++);
      	  GPIOC_PDOR = 0x9A;
      	  for(i=0; i < 10000; i++);
      	  GPIOC_PDOR = 0x99;
      	  for(i=0; i < 10000; i++);
      	  GPIOC_PDOR = 0x95;
      	  for(i=0; i < 10000; i++);
      }
      else if(InpData[0] == 0x03){
    	  GPIOC_PDOR = 0x96;
    	  for(i=0; i < 5000; i++);
      	  GPIOC_PDOR = 0x9A;
      	  for(i=0; i < 5000; i++);
      	  GPIOC_PDOR = 0x99;
      	  for(i=0; i < 5000; i++);
      	  GPIOC_PDOR = 0x95;
      	  for(i=0; i < 5000; i++);
      }
    }
   return 0;


  /*** Don't write any code pass this line, or it will be deleted during code generation. ***/
  /*** RTOS startup code. Macro PEX_RTOS_START is defined by the RTOS component. DON'T MODIFY THIS CODE!!! ***/
  #ifdef PEX_RTOS_START
    PEX_RTOS_START();                  /* Startup of the selected RTOS. Macro is defined by the RTOS component. */
  #endif
  /*** End of RTOS startup code.  ***/
  /*** Processor Expert end of main routine. DON'T MODIFY THIS CODE!!! ***/
  for(;;){}
  /*** Processor Expert end of main routine. DON'T WRITE CODE BELOW!!! ***/
} /*** End of main routine. DO NOT MODIFY THIS TEXT!!! ***/

/* END main */
/*!
** @}
*/
/*
** ###################################################################
**
**     This file was created by Processor Expert 10.5 [05.21]
**     for the Freescale Kinetis series of microcontrollers.
**
** ###################################################################
*/
