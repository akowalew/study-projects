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
RESET       mov.w   #__STACK_END,	SP       ; Initialize stackpointer
StopWDT     mov.w   #WDTPW|WDTHOLD,	&WDTCTL  ; Stop watchdog timer

;----------------
; Konfiguracja portow
;----------------
			bic.b	#BTN_LOAD|BTN_INC, 	&P1DIR	;	Port1 jako WEJSCIE. PRZYCISKI
			mov.b	#0xFF, 	&P2DIR				;	Port2 jako wejscie. WYSWIETLACZ
			mov.b	#0x00, 	&P3DIR				;	Port3 jako wejcie. HEX_IN

;----------------
; Konfiguracja przerwania przycisku1, INKREMENTACJA, przycisku 8 LADOWANIE
;----------------
			bis.b	#BTN_LOAD|BTN_INC, 	&P1IES	;	Przycisk1, HIGH -> LOW
			bis.b	#BTN_LOAD|BTN_INC, 	&P1IE	;	Przycisk1, Interrupt Enable
			eint								; 	Global Enable Interrupts

;----------------
; Konfiguracja zmiennych
;----------------
			mov.b	#0x00, LICZNIK
			mov.b	#0x00, FLAGI

			jmp MainLoop

;-------------------------------------------------------------------------------
; Main loop here
;-------------------------------------------------------------------------------
MainLoop

			bit.b	#BTN_LOAD, FLAGI ;  CZY PRZYCISK LOAD ZOSTAL NACISNIETY?
			jz		NOT_LOAD

			bic.b	#BTN_INC, &P1IE
			call	#DEBOUNCE
LADUJ		call 	#LOAD

			bit.b	#BTN_LOAD, &P1IN 	; CZY PRZYCISK LOAD DALEJ JEST NACISNIETY?
			jz 		LADUJ				; JESLI DALEJ NACISNIETY, TO LADUJ

			call 	#DEBOUNCE

			; KASOWANIE FLAG, ITD...
			bic.b	#BTN_LOAD, FLAGI 	; SKASUJ FLAGE LOAD
			bic.b	#BTN_LOAD, &P1IFG	; SKASUJ INT FLAGE PRZYCISKU LOAD
			bis.b	#BTN_LOAD, &P1IE	; URUCHOM PONOWNIE PRZERWANIE LOAD

			jmp 	MainLoop

NOT_LOAD
			bit.b	#BTN_INC, FLAGI ; 	CZY PRZYCISK INCREMENT ZOSTAL NACISNIETY?
			jz		NOT_INC

			call	#DEBOUNCE
ZWIEKSZ		call 	#INCREMENT



			; KASOWANIE FLAG, ITD...
			bic.b	#BTN_INC, FLAGI	; SKASUJ FLAGE INCREMENT
			bic.b	#BTN_INC, &P1IFG
			bis.b	#BTN_INC, &P1IE	; URUCHOM PRZERWANIE INCREMENT

NOT_INC
			bis.b	#BTN_INC, &P1IE
			jmp MainLoop

;-------------------------------------------------------------------------------
;	INT_PORT1 Interrupt Routine
;-------------------------------------------------------------------------------
INT_PORT1
			bic.b	#BTN_LOAD|BTN_INC, &P1IFG	; KASUJ FLAGI PRZERWANIA PORTU1, BTN1 | BTN2


			bit.b	#BTN_LOAD, &P1IN	; SPRAWDZ PRZYCISK1, LADOWANIE

			jnz		NIE_BTN1		; JESLI NIE BYL WCISNIETY, TO WYSKOCZ
			bis.b	#BTN_LOAD, FLAGI	; USTAW FLAGE LOAD, BO BYL WCISNIETY
			bic.b	#BTN_LOAD, &P1IE	; WYLACZ PRZERWANIE DLA TEGO PRZYCISKU
			jmp 	NIE_BTN2

NIE_BTN1
			bit.b	#BTN_INC, &P1IN	; sprawdz PRZYCISK2, INCREMENT

			jnz NIE_BTN2
			bis.b	#BTN_INC, FLAGI	; USTAW FLAGE INCREMENT
			bic.b	#BTN_INC, &P1IE	; WYLACZ PRZERWANIE DLA TEGO PRZYCISKU

NIE_BTN2
			reti

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
;	Variables
;-------------------------------------------------------------------------------
			.data
LICZNIK		.byte	0xF0
FLAGI		.byte 	0x00

;-------------------------------------------------------------------------------
;	Constants
;-------------------------------------------------------------------------------
BTN_LOAD	.set	0x01
BTN_INC		.set	0x80


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

