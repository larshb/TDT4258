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
        ldr r1, =CMU_BASE										/* CMU configuration */
        ldr r2, [r1, #CMU_HFPERCLKEN0]			// Read current configuration
        mov r3, #1
        lsl r3, r3, #CMU_HFPERCLKEN0_GPIO		// Enable GPIO clock
        orr r2, r2, r3											// Append
        str r2, [r1, #CMU_HFPERCLKEN0]			// Store back

        ldr r1, =GPIO_PA_BASE 							// (LEDs,    H)
        ldr r2, =GPIO_PC_BASE 							// (buttons, L)
        ldr r3, =GPIO_BASE
        mov r6, #0xFF00;										// LEDs register

        mov r4, #2													/* LEDs */
        str r4, [r1, #GPIO_CTRL]						// Set drive strength high
        mov r4, #0x55555555
        str r4, [r1, #GPIO_MODEH]						// Set pins (8-15) to output

        mov r4, #0x33333333									// Buttons
        str r4, [r2, #GPIO_MODEL]						// Set pins (0-7) to input
        mov r4, #0xff
        str r4, [r2, #GPIO_DOUT]						// Enable pull-up
