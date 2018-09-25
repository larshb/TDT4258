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

				//b polling														/* Run polling solution */

				mov r4, #0x22222222									/* Interrupt setup */
				str r4, [r3, #GPIO_EXTIPSELL]				// Set GPIO Interrupt pins
				mov r4, #0xff
				str r4, [r3, #GPIO_EXTIFALL]				// Trigger on falling edge (button press)
				str r4, [r3, #GPIO_EXTIRISE]				// Trigger on rising edge (button release)
				str r4, [r3, #GPIO_IEN]							// Enable interrupt generation
				ldr r4, =#0x802
				ldr r5, =ISER0
				str r4, [r5]												// Enable interrupt handling

				ldr r4, =CMU_BASE
				mov r5, 0x108 											
				str r5, [r4, #CMU_OSCENCMD]					
				ldr r5, [r4, #CMU_CMD]							// Enable LF crystal oscillator (LFXO), and disable HF
				orr r5, r5, 0x4 										// Select LFXO as HFCLK
				str r5, [r4, #CMU_CMD]

				mov r4, 7														/* Low-power enhancements */
				ldr r5, =EMU_BASE
				str r4, [r5, #EMU_MEMCTRL]					// Disable RAM
				mov r4, 6
				ldr r5, =SCR
				str r4, [r5]												// Set energy mode 2 (deep sleep)

				b clk_debug

				wfi																	// Wait for interrupts (sleep)

polling:
				ldr r4, [r2, #GPIO_DIN]							// Read buttons
				lsl r4, r4, #8											// Shift into corresponding LED position
				str r4, [r1, #GPIO_DOUT]						// Output on LEDs

				b polling

clk_debug:
				ldr r4, =CMU_BASE
				ldr r5, [r4, #0x02c] // CMU_STATUS
				str r5, [r1, #GPIO_DOUT]
				b clk_debug
	
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

				b new_solution

				// TODO: Move this duplicate code into subroutine
				ldr r4, [r2, #GPIO_DIN]							// Read buttons
				lsl r4, r4, #8											// Shift into corresponding LED position
				str r4, [r1, #GPIO_DOUT]						// Output on LEDs

new_solution:
				ldr r4, [r2, #GPIO_DIN]

				/* Buttons are identified as inverted one-hot (eg. 1110 1111) */
				cmp r4, 0xfe	/* SW1 */
				beq btn_ll
				cmp r4, 0xfd	/* SW2 */
				beq btn_lu
				cmp r4, 0xfb	/* SW3 */
				beq btn_lr
				cmp r4, 0xf7	/* SW4 */
				beq btn_ld
				cmp r4, 0xef	/* SW5 */
				beq btn_rl
				cmp r4, 0xdf	/* SW6 */
				beq btn_ru
				cmp r4, 0xbf	/* SW7 */
				beq btn_rr
				cmp r4, 0x7f  /* SW8 */
				beq btn_rd
				b done

btn_ll:	and r6, #0xFE00 /* Turn on leftmost */
				b done
btn_lu:	mov r6, 0x0000 /* All on */
				b done
btn_lr:	and r6, #0x7F00 /* Turn on rightmost */
				b done
btn_ld:	mov r6, 0xFF00 /* All off */
				b done
btn_rl:	lsr r7, r6, 1 /* Move left (wrap around) */
				lsl r8, r6, 7
				orr r6, r7, r8
				b done
btn_ru:	eor r6, r6, 0xFF00 /* Invert */
				b done
btn_rr:	lsl r7, r6, 1 /* Move right (wrap around) */
				lsr r8, r6, 7
				orr r6, r7, r8
				b done
btn_rd:	b dance
				b done

done:		str r6, [r1, #GPIO_DOUT]
				bx lr

dance:
				mov r9, 10
dance_loop:
				sub r9, r9, 1
				lsr r7, r6, 1 /* Move left (wrap around) */
				lsl r8, r6, 7
				orr r6, r7, r8
				str r6, [r1, #GPIO_DOUT]
				mov r10, 0xFF00
busy_delay:
				sub r10, r10, 1
				cmp r10, 0
				bne busy_delay
				cmp r9, 0
				bne dance_loop
				bx lr

	
	/////////////////////////////////////////////////////////////////////////////
	
        .thumb_func
dummy_handler:  
        b .  // do nothing
