        .syntax unified
	
	      .include "efm32gg.s"

	/////////////////////////////////////////////////////////////////////////////
	//
  // Exception vector table
  // This table contains addresses for all exception handlers
	//
	/////////////////////////////////////////////////////////////////////////////
	
        .section .vectors
	
	      .long   stack_top               /* Top of Stack                 */
	      .long   _reset                  /* Reset Handler                */
	      .long   dummy_handler           /* NMI Handler                  */
	      .long   dummy_handler           /* Hard Fault Handler           */
	      .long   dummy_handler           /* MPU Fault Handler            */
	      .long   dummy_handler           /* Bus Fault Handler            */
	      .long   dummy_handler           /* Usage Fault Handler          */
	      .long   dummy_handler           /* Reserved                     */
	      .long   dummy_handler           /* Reserved                     */
	      .long   dummy_handler           /* Reserved                     */
	      .long   dummy_handler           /* Reserved                     */
	      .long   dummy_handler           /* SVCall Handler               */
	      .long   dummy_handler           /* Debug Monitor Handler        */
	      .long   dummy_handler           /* Reserved                     */
	      .long   dummy_handler           /* PendSV Handler               */
	      .long   dummy_handler           /* SysTick Handler              */

	      /* External Interrupts */
	      .long   dummy_handler
	      .long   gpio_handler            /* GPIO even handler */
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   gpio_handler            /* GPIO odd handler */
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler

	      .section .text

	/////////////////////////////////////////////////////////////////////////////
	//
	// Reset handler
  // The CPU will start executing here after a reset
	//
	/////////////////////////////////////////////////////////////////////////////

	      .globl  _reset
	      .type   _reset, %function
        .thumb_func
_reset: 
				ldr r1, =CMU_BASE									// + CMU configuration
				ldr r2, [r1, #CMU_HFPERCLKEN0]			// Read current configuration
				mov r3, 1
				lsl r3, r3, #CMU_HFPERCLKEN0_GPIO		// Enable GPIO clock
				orr r2, r2, r3											// Append
				str r2, [r1, #CMU_HFPERCLKEN0]			// Store back

				ldr r1, =GPIO_PA_BASE 							// (LEDs,    H)
				ldr r2, =GPIO_PC_BASE 							// (buttons, L)
				ldr r3, =GPIO_BASE

				mov r4, #2												// + LEDs
				str r4, [r1, #GPIO_CTRL]						// Set drive strength high
				mov r4, #0x55555555
				str r4, [r1, #GPIO_MODEH]						// Set pins (8-15) to output

				mov r4, #0x33333333									// Buttons
				str r4, [r2, #GPIO_MODEL]						// Set pins (0-7) to input
				mov r4, #0xff
				str r4, [r2, #GPIO_DOUT]						// Enable pull-up

				//b polling													// + Run polling solution

				mov r4, #0x22222222								// + Interrupt setup
				str r4, [r3, #GPIO_EXTIPSELL]				// Set GPIO Interrupt pins
				mov r4, #0xff
				str r4, [r3, #GPIO_EXTIFALL]				// Trigger on falling edge (button press)
				str r4, [r3, #GPIO_EXTIRISE]				// Trigger on rising edge (button release)
				str r4, [r3, #GPIO_IEN]							// Enable interrupt generation
				ldr r4, =#0x802
				ldr r5, =ISER0
				str r4, [r5]												// Enable interrupt handling

				mov r4, 7													// + Low-power enhancements
				ldr r5, =EMU_BASE
				str r4, [r5, #EMU_MEMCTRL]					// Disable RAM
				mov r4, 6
				ldr r5, =SCR
				str r4, [r5]												// Set energy mode 2
				wfi																	// Wait for interrupts (sleep)

polling:
				ldr r4, [r2, #GPIO_DIN]							// Read buttons
				lsl r4, r4, #8											// Shift into corresponding LED position
				str r4, [r1, #GPIO_DOUT]						// Output on LEDs
				b polling
	
	/////////////////////////////////////////////////////////////////////////////
	//
  // GPIO handler
  // The CPU will jump here when there is a GPIO interrupt
	//
	/////////////////////////////////////////////////////////////////////////////
	
        .thumb_func
gpio_handler:  
				ldr r4, [r3, #GPIO_IF]							// Read source
				str r4, [r3, #GPIO_IFC]							// Clear interrupt

				// TODO: Move this duplicate code into subroutine
				ldr r4, [r2, #GPIO_DIN]							// Read buttons
				lsl r4, r4, #8											// Shift into corresponding LED position
				str r4, [r1, #GPIO_DOUT]						// Output on LEDs

				bx lr
	
	/////////////////////////////////////////////////////////////////////////////
	
        .thumb_func
dummy_handler:  
        b .  // do nothing
