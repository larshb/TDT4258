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

iser0_addr:
	.long ISER0

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

	// Initialize I/O registers
	ldr r6, =GPIO_PA_BASE
	ldr r7, =GPIO_PC_BASE
	ldr r8, =GPIO_BASE

	// Enabling GPIO-clock
	ldr r1, =CMU_BASE
	ldr r2, [r1, #CMU_HFPERCLKEN0]
	mov r3, #1
	lsl r3, r3, #CMU_HFPERCLKEN0_GPIO
	orr r2, r2, r3
	str r2, [r1, #CMU_HFPERCLKEN0]

	// Setting drive strength on output pins
	mov r2, #0x2
	str r2, [r6, #GPIO_CTRL]

	// Set LEDs to output (pin 8-15)
	mov r2, #0x55555555
	str r2, [r6, #GPIO_MODEH]

	// Initialize LEDs
	mov r9, 0b0001000000000000
	eor r10, r9, 0xff00
	str r10, [r6, #GPIO_DOUT]

	// Setting buttons to input (pin 0-7)
	mov r2, #0x33333333
	str r2, [r7, #GPIO_MODEL]

	// Enable button pullup
	mov r2, #0xff
	str r2, [r7, #GPIO_DOUT]

	// Enable interrupt
	mov r2, #0x22222222
	str r2, [r8, #GPIO_EXTIPSELL]
	mov r2, #0xff
	str r2, [r8, #GPIO_EXTIFALL]
	str r2, [r8, #GPIO_EXTIRISE]
	str r2, [r8, #GPIO_IEN]
	ldr r2, =#0x802
	ldr r3, iser0_addr
	str r2, [r3]

	///////////////////
	// Energy saving //
	///////////////////

	// Change energy mode to energy mode 2
	mov r2, 6
	ldr r3, =SCR
	str r2, [r3]

	wfi	// wait for interrupt

	//b .

/////////////////////////////////////////////////////////////////////////////
//
// GPIO handler
// The CPU will jump here when there is a GPIO interrupt
//
/////////////////////////////////////////////////////////////////////////////

    .thumb_func
gpio_handler:
	ldr r2, [r8, #GPIO_IF]
	str r2, [r8, #GPIO_IFC]

	ldr r5, =GPIO_PA_BASE
	add r5, #GPIO_DOUT

	ldr r2, [r7, #GPIO_DIN]
	cmp r2, 0xff
	beq done
	cmp r2, 0xfe
	beq go_left
	cmp r2, 0xfb
	beq go_right

done:
	bx lr

go_left:
	cmp r9, 0x0100
	beq done
	lsr r9, r9, 1
	eor r10, r9, 0xff00
	str r10, [r6, #GPIO_DOUT]
	bx lr

go_right:
	cmp r9, 0x8000
	beq done
	lsl r9, r9, 1
	eor r10, r9, 0xff00
	str r10, [r6, #GPIO_DOUT]
	bx lr

/////////////////////////////////////////////////////////////////////////////

    .thumb_func
dummy_handler:
	b .  // do nothing
