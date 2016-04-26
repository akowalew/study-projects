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
BTN_IN			.set		P1IN

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

TAB_JOHNSON	.byte	0x00, 0x01, 0x03, 0x07, 0x0F, 0x1F, 0x3F, 0x7F, 0xFF, 0xFE, 0xFC, 0xF8, 0xF0, 0xE0, 0xC0, 0x80
;-------------------------------------------------------------------------------
;	Program Constants
;-------------------------------------------------------------------------------
BTN_LOAD		.set	0x01
BTN_INC			.set	0x08

LOAD_CHANGED	.set	0x01
INC_CHANGED		.set	0x02
IS_TO_LOAD		.set	0x04

;-------------------------------------------------------------------------------
; RESET INTERRUPT VECTOR
;-------------------------------------------------------------------------------

RESET       mov.w   #__STACK_END,	SP       ; Initialize stackpointer
StopWDT     mov.w   #WDTPW|WDTHOLD,	&WDTCTL  ; Stop watchdog timer

			;----------------
			; Configure ports
			;----------------

			bic.b	#BTN_LOAD|BTN_INC, 	&P1DIR	;	Port1 jako WEJSCIE. PRZYCISKI
			mov.b	#0xFF, 	&SEG7_DIR				;	Port2 jako wejscie. WYSWIETLACZ
			mov.b	#0x00, 	&HEX_DIR				;	Port3 jako wejcie. HEX_IN

			;----------------
			; Configure interrupts
			;----------------
			bis.b	#BTN_LOAD|BTN_INC, 	&BTN_IES	;	Przycisk1, HIGH -> LOW
			bis.b	#BTN_LOAD|BTN_INC, 	&BTN_IE	;	Przycisk1, Interrupt Enable

			;----------------
			; Configure variables
			;----------------
			mov.b	#0x00, LICZNIK
			mov.b	#0x00, FLAGI

			mov.b	LICZNIK, &SEG7_OUT

			eint								; 	Global Enable Interrupts
			jmp MainLoop

;-------------------------------------------------------------------------------
; Main loop here
;-------------------------------------------------------------------------------
MainLoop
			bit.b	#LOAD_CHANGED, FLAGI	; Is LOAD_BTN interrupt flag set ?
			jz		LOAD_NOT_CHANGED		; if not, branch
			bic.b	#LOAD_CHANGED, FLAGI

			call 	#BtnLoadChanged			; BtnLoad is changed, we have to check it

LOAD_NOT_CHANGED
			bit.b	#INC_CHANGED, FLAGI		; Is 	BTN_INC interrupt flag set?
			jz		INC_NOT_CHANGED			; if not, branch
			bic.b	#INC_CHANGED, FLAGI

			call	#BtnIncChanged

INC_NOT_CHANGED
			bit.b	#IS_TO_LOAD, FLAGI		; Is LOAD_BTN pressed ?
			jz		TO_SLEEP 		; If not, branch

			call	#LOAD					; Execute asynchronic LOAD function

			bit.b	#BTN_LOAD, &P1IN
			jz 		MainLoop

			bic.b	#IS_TO_LOAD, FLAGI
			jmp 	MainLoop				; We have to don't care about INC_BTN if LOAD_BTN is pressed

TO_SLEEP
			;--------------------
			; GO SLEEEEEEP!!!
			;--------------------
			bit.b	#IS_TO_LOAD|LOAD_CHANGED|INC_CHANGED, FLAGI
			jnz		MainLoop

			bis		#CPUOFF|SCG1|SCG0|OSCOFF, SR

			jmp MainLoop

;-------------------------------------------------------------------------------
;	INT_PORT1 Interrupt Routine
;-------------------------------------------------------------------------------
INT_PORT1
	;----------------
	; Check LOAD_BTN
	;----------------
			bit.b	#BTN_LOAD, &BTN_IFG		; Is LOAD_BTN interrupt flag set?
			jz		NOT_LOAD_INT_FLAG		; branch if not

			bic.b	#BTN_LOAD, &BTN_IFG
			bic.b	#BTN_LOAD, &BTN_IE

			bis.b	#LOAD_CHANGED, FLAGI

	;----------------
	; Check INC_BTN
	;----------------
NOT_LOAD_INT_FLAG

			bit.b	#BTN_INC, &BTN_IFG		; IS INC_BTN INT FLAG?
			jz		NOT_INC_INT_FLAG		; branch if not

			bic.b	#BTN_INC, &BTN_IFG
			bic.b	#BTN_INC, &BTN_IE

			bis.b	#INC_CHANGED, FLAGI

NOT_INC_INT_FLAG
CANCEL_SLEEP

			; ------------------------
			; DO SOMETHING TO CANCEL SLEEP
			; ------------------------

			bic		#CPUOFF|SCG1|SCG0|OSCOFF, 0(SP)
			reti

;-------------------------------------------------------------------------------
;	BtnLoadChanged procedure
;-------------------------------------------------------------------------------
BtnLoadChanged
			call 	#DEBOUNCE				; First, wait for debounce, because button state is changed
			dint

			bit.b	#BTN_LOAD, &BTN_IN
			jnz		LOAD_NOT_PRESSED

			bis.b	#IS_TO_LOAD, FLAGI
			jmp 	AFTER_LOAD_CHECK

LOAD_NOT_PRESSED
AFTER_LOAD_CHECK
			bis.b	#BTN_LOAD, &BTN_IE

			eint
			ret

;-------------------------------------------------------------------------------
;	BtnIncChanged procedure
;-------------------------------------------------------------------------------
BtnIncChanged
			call 	#DEBOUNCE				; wait for debounce, because button state is changed
			dint

			bit.b	#BTN_INC, &BTN_IN
			jnz		INC_NOT_PRESSED

			bit.b	#IS_TO_LOAD, FLAGI
			jnz		INC_NOT_PRESSED

			call 	#INCREMENT

INC_NOT_PRESSED
AFTER_INC_CHECK
			bis.b	#BTN_INC, &BTN_IE
			eint
			ret

;-------------------------------------------------------------------------------
;	LOAD procedure
;-------------------------------------------------------------------------------
LOAD
			push R4
			push R5

			mov.b	&HEX_IN, R4
			and.b	#0x0F, R4

			mov.w	#TAB_JOHNSON, R5
			add.w	R4, R5

			mov.b	@R5, &LICZNIK

			mov.b	LICZNIK, &SEG7_OUT

			pop R5
			pop R4
			ret

;-------------------------------------------------------------------------------
;	INCREMENT procedure
;-------------------------------------------------------------------------------
INCREMENT
			rla.b	LICZNIK
			addc.b	#0x00, LICZNIK
			xor.b	#0x01, LICZNIK

			mov.b	LICZNIK, &SEG7_OUT

			ret

;-------------------------------------------------------------------------------
;	DEBOUNCE procedure
;-------------------------------------------------------------------------------
DEBOUNCE
			push R4
			mov.w		#0x3415, R4
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


