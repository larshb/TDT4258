        .syntax unified
	
	      .include "../efm32gg.s"
	      .include "../init.s"
				
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
				b .
	
        .thumb_func
dummy_handler:  
        b .  // do nothing
