;-------------------------------------------------------------------------------
; MSP430 Assembler Code Template for use with TI Code Composer Studio
;
;
;-------------------------------------------------------------------------------
            .cdecls C,LIST,"msp430.h"       ; Include device header file
            
			.text                           ; Assemble into program memory
            .global RESET
            .retain                         ; Override ELF conditional linking
                                            ; and retain current section
            .retainrefs                     ; Additionally retain any sections
                                            ; that have references to current
                                            ; section
                          					; Assemble into program memory.

;-------------------------------------------------------------------------------


;---------------------------------
; Port mnemonics Constants
;---------------------------------
BTN_DIR			.set		P1DIR
BTN_IFG			.set		P1IFG
BTN_IE			.set		P1IE
BTN_IES			.set		P1IES

SEG7_DIR		.set		P2DIR
SEG7_OUT		.set		P2OUT

HEX_DIR			.set		P3DIR
HEX_IN			.set		P3IN

;-------------------------------------------------------------------------------
;	Variables
;-------------------------------------------------------------------------------
			.data
LICZNIK		.byte	0x00
FLAGI		.byte 	0x00

;-------------------------------------------------------------------------------
;	Program Constants
;-------------------------------------------------------------------------------
BTN_LOAD		.set	0x01
BTN_INC			.set	0x80

LOAD_PRESSED	.set	0x01
INC_PRESSED		.set	0x02
LOAD_RELEASED	.set	0x04
INC_RELEASED	.set	0x08

;-------------------------------------------------------------------------------
; RESET INTERRUPT VECTOR
;-------------------------------------------------------------------------------

RESET       mov.w   #__STACK_END,	SP       ; Initialize stackpointer
StopWDT     mov.w   #WDTPW|WDTHOLD,	&WDTCTL  ; Stop watchdog timer

			;----------------
			; Configure ports
			;----------------

			bic.b	#BTN_LOAD|BTN_INC, 	&P1DIR	;	Port1 jako WEJSCIE. PRZYCISKI
			mov.b	#0xFF, 	&P2DIR				;	Port2 jako wejscie. WYSWIETLACZ
			mov.b	#0x00, 	&P3DIR				;	Port3 jako wejcie. HEX_IN

			;----------------
			; Configure interrupts
			;----------------
			bis.b	#BTN_LOAD|BTN_INC, 	&P1IES	;	Przycisk1, HIGH -> LOW
			bis.b	#BTN_LOAD|BTN_INC, 	&P1IE	;	Przycisk1, Interrupt Enable

			;----------------
			; Configure variables
			;----------------
			mov.b	#0x00, LICZNIK
			mov.b	#0x00, FLAGI

			eint								; 	Global Enable Interrupts
			jmp MainLoop

;-------------------------------------------------------------------------------
; Main loop here
;-------------------------------------------------------------------------------
MainLoop

			bit.b	#BTN_LOAD, &P1IFG		; Is LOAD_BTN interrupt flag set ?
			jz		LOAD_INT_FLAG_NOT_SET	; if not, branch

			call 	#BtnLoadChanged			; BtnLoad is changed, we have to check it

LOAD_INT_FLAG_NOT_SET
			bit.b	#LOAD_PRESSED, FLAGI	; Is LOAD_BTN pressed ?
			jz		LOAD_NOT_PRESSED 		; If not, branch

			call	#LOAD					; Execute asynchronic LOAD function

			jmp 	MainLoop				; We have to don't care about INC_BTN if LOAD_BTN is pressed

LOAD_NOT_PRESSED
			bit.b	#BTN_INC, &P1IFG		; Is 	BTN_INC interrupt flag set?
			jz		INC_INT_FLAG_NOT_SET	; if not, branch

			call	#BtnIncChanged

			jmp 	MainLoop				; we have to check one more time the whole loop

INC_INT_FLAG_NOT_SET

			;--------------------
			; GO SLEEEEEEP!!!
			;--------------------


			jmp MainLoop

;-------------------------------------------------------------------------------
;	INT_PORT1 Interrupt Routine
;-------------------------------------------------------------------------------
INT_PORT1
	;----------------
	; Check LOAD_BTN
	;----------------
			bit.b	#BTN_LOAD, &P1IFG		; Is LOAD_BTN interrupt flag set?
			jz		NOT_LOAD_INT_FLAG		; branch if not

			bic.b	#BTN_LOAD, &P1IE		; DISABLE LOAD INT
			bit.b	#BTN_LOAD, &P1IES		; IS HIGH -> LOW transition ?

			jnz		LOAD_SET_PRESSED		; If it's true, we have a pressed button. Let's branch

			bis.b	#LOAD_RELEASED, FLAGI	; If not, we have a released button.
			jmp 	LOAD_INVERT_EDGE		; branch to next step
LOAD_SET_PRESSED
			bis.b	#LOAD_PRESSED, FLAGI	; we have pushed button

LOAD_INVERT_EDGE
			xor.b	#BTN_LOAD, &P1IES		; Invert type of Edge on LOAD_BTN
			jmp 	CANCEL_SLEEP

	;----------------
	; Check INC_BTN
	;----------------
NOT_LOAD_INT_FLAG

			bit.b	#BTN_INC, &P1IFG		; IS INC_BTN INT FLAG?
			jnz		NOT_INC_INT_FLAG		; branch if not

			bic.b	#BTN_INC, &P1IE			; Disable INC_BTN interrupt
			bit.b	#BTN_INC, &P1IES		; Is High to Low transition ?

			jz		INC_INVERT_EDGE			; if not, branch, because INC_BTN is not pressed
			bis.b	#INC_PRESSED, FLAGI		; if it is true, Set the PRESSED flag on INC_BTN

INC_INVERT_EDGE
			xor.b	#BTN_INC, &P1IES		; Invert type of Edge on INC_BTN

NOT_INC_INT_FLAG
CANCEL_SLEEP

			; ------------------------
			; DO SOMETHING TO CANCEL SLEEP
			; ------------------------

			reti

;-------------------------------------------------------------------------------
;	BtnLoadChanged procedure
;-------------------------------------------------------------------------------
BtnLoadChanged
			call 	#DEBOUNCE				; First, wait for debounce, because button state is changed

			bit.b	#LOAD_RELEASED, FLAGI	; Is LOAD_BTN released?
			jz		LOAD_NOT_RELEASED		; If not, branch

			bic.b	#LOAD_PRESSED|LOAD_RELEASED|INC_PRESSED, FLAGI	; If it is released, Clear the flags
			bic.b	#BTN_INC, &P1IFG		; Clear also INC_BTN flag
LOAD_NOT_RELEASED
			bic.b	#BTN_LOAD, &P1IFG		; Before we re-enable interrupts on BTN_LOAD, we've to clear interrupt flag
			bis.b	#BTN_LOAD, &P1IE		; Enable interrupt on BTN_LOAD

			ret

;-------------------------------------------------------------------------------
;	BtnIncChanged procedure
;-------------------------------------------------------------------------------
BtnIncChanged
			call 	#DEBOUNCE				; wait for debounce, because button state is changed

			bit.b	#INC_PRESSED, FLAGI		; Is INC_BTN pressed?
			jz		INC_NOT_PRESSED			; if not, branch
			; if it is true, execute code below:
			call	#INCREMENT
			bic.b	#INC_PRESSED, FLAGI		; Clear flag
INC_NOT_PRESSED
			bic.b	#BTN_INC, &P1IFG		; Clear interrupt Flag, before re-enable
			bis.b	#BTN_INC, &P1IE			; Enable interrupt on this pin

			ret

;-------------------------------------------------------------------------------
;	LOAD procedure
;-------------------------------------------------------------------------------
LOAD
			mov.b	#0x00, LICZNIK
			mov.b	LICZNIK, &P2OUT
			ret

;-------------------------------------------------------------------------------
;	INCREMENT procedure
;-------------------------------------------------------------------------------
INCREMENT
			inc.b	LICZNIK
			mov.b	LICZNIK, &P2OUT
			ret

;-------------------------------------------------------------------------------
;	DEBOUNCE procedure
;-------------------------------------------------------------------------------
DEBOUNCE
			push R4
			mov.w		#0x8000, R4
DEB_DEC		dec.w		R4
			jnz 		DEB_DEC
			pop 		R4
			ret


;-------------------------------------------------------------------------------
; Stack Pointer definition
;-------------------------------------------------------------------------------
            .global __STACK_END
            .sect   .stack
            
;-------------------------------------------------------------------------------
; Interrupt Vectors
;-------------------------------------------------------------------------------
            .sect   ".reset"                ; MSP430 RESET Vector
            .short  RESET
            .sect	".int04"
            .short	INT_PORT1
