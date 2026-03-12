/*******************************************************************************
  Main Source File

  Company:
    Microchip Technology Inc.

  File Name:
    main.c

  Summary:
    This file contains the "main" function for a project.

  Description:
    This file contains the "main" function for a project.  The
    "main" function calls the "SYS_Initialize" function to initialize the state
    machines of all modules in the system
 *******************************************************************************/

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include <stddef.h>                     // Defines NULL
#include <stdbool.h>                    // Defines true
#include <stdlib.h>                     // Defines EXIT_FAILURE
#include <string.h>                     // String
#include "definitions.h"                // SYS function prototypes


// *****************************************************************************
// *****************************************************************************
// Section: Main Entry Point
// *****************************************************************************
// *****************************************************************************

// Buffer to hold incoming Bluetooth data
uint8_t rxData;
bool prevButtonState = true; // True means HIGH (unpressed due to pull-up)

int main ( void )
{
    /* Initialize all modules */
    SYS_Initialize ( NULL );

    // 1. Submit the very first read request to the SERCOM module before the loop starts
    SERCOM0_USART_Read(&rxData, 1);
    
    while ( true )
    {
        
        /* * 2. Check if the read operation is complete. 
         * If it is NOT busy, it means the hardware successfully received our 1 byte.
         */
        
        if (!SERCOM0_USART_ReadIsBusy()) 
        {
            // 3. Optional but recommended: Check that no framing or overrun errors occurred
            if (SERCOM0_USART_ErrorGet() == USART_ERROR_NONE)
            {
                // 4. Process the received command
                switch(rxData) {
                    case 'W': // Forward
                        LED_Set();
                        for(volatile int i = 0; i < 500000; i++); 
                        break;
                    case 'S': // Reverse
                        LED_Set();
                        break;
                    case '0': // Stop
                        LED_Clear();
                        break;
                    default:
                        // Stop or Handle invalid command
                        break;
                }
            }
            
        // 5. IMPORTANT: Submit the next read request so we are ready for the next command
        SERCOM0_USART_Read(&rxData, 1);
        }
        
        bool currentButtonState = SW_Get();
        
        if (prevButtonState == true && currentButtonState == false){
            char echo[] = "Button Pressed\n";
            SERCOM0_USART_Write((void*)echo, strlen(echo));
            
            for(volatile int i = 0; i < 500000; i++);
            
            while(SW_Get() == false) 
            {
                SYS_Tasks(); // Keep background MCC tasks running
            }
            
            for(volatile int i = 0; i < 500000; i++);
            
        }
        
        prevButtonState = currentButtonState;
        /* Maintain state machines of all polled MPLAB Harmony modules. */
        SYS_Tasks( );
        
    }

    /* Execution should not come here during normal operation */

    return ( EXIT_FAILURE );
}


/*******************************************************************************
 End of File
*/

