        .syntax unified

	      .include "../setup.s"

				b polling														/* Run polling solution */

polling:
				ldr r4, [r2, #GPIO_DIN]							// Read buttons
				lsl r4, r4, #8											// Shift into corresponding LED position
				str r4, [r1, #GPIO_DOUT]						// Output on LEDs

				b polling
