;-------------------------------------------------------------------------------
;       Implementacja programowa licznika 8bitowego kodu Johsnona z wykorzystaniem
;   przycisku asynchronicznego ładowania oraz działającego zboczem przycisku inkrementacji
;   przy użyciu modułów systemu SML3, w tym mikrokontrolera MSP430F16xx.
;
;   Program został napisany tak, by zapewnić możliwie elastyczną obsługę przerwań
;   związanych z przyciskami, jak i żeby zachować możliwie wysoką energooszczędność
;   mikrokontrolera.
;
;   Założenia projektowe:
;       Moduł z przyciskami - podłączony do portu 1 mikrokontrolera
;       Moduł 7SEG z dekoderem - port 2 mikrokontrolera
;       Moduł przełączników szesnastkowych - port 3 mikrokontrolera
;
;   Oba przyciski wywołują przerwania, powodując tym samym ustawienie odpowiedniej flagi w programie
;   i wyjście ze stanu snu. Dzięki temu, można wtedy w pętli głównej sprawdzić, co nastąpiło w przerwaniu
;   oraz podjąć odpowiednie działanie. Przycisk ładowania, dopóki nie zostanie puszczony, powoduje
;   nieprzerwane niczym ciągłe ładowanie. Przycisk inkrementacji działa tylko wtedy, gdy nie jest
;   jednocześnie wciśnięty przycisk ładowania.
;
;   Autorzy : Adam Kowalewski, Maciej Kłos.
;-------------------------------------------------------------------------------
            .cdecls C,LIST,"msp430.h"       ; Include device header file
			.text                           ; Assemble into program memory
            .global RESET
            .retain
            .retainrefs
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
LOAD_FLAG		.set	0x04

;-------------------------------------------------------------------------------
; RESET INTERRUPT VECTOR
;-------------------------------------------------------------------------------

RESET       mov.w   #__STACK_END,	SP       ; Initialize stackpointer
StopWDT     mov.w   #WDTPW|WDTHOLD,	&WDTCTL  ; Stop watchdog timer

;----------------
; Configure ports
;----------------

bic.b	#BTN_LOAD|BTN_INC, 	&P1DIR		;	Port1 jako WEJSCIE. PRZYCISKI
mov.b	#0xFF, 	&SEG7_DIR			;	Port2 jako wejscie. WYSWIETLACZ
mov.b	#0x00, 	&HEX_DIR			;	Port3 jako wejcie. HEX_IN

;----------------
; Configure interrupts
;----------------
bis.b	#BTN_LOAD|BTN_INC, 	&BTN_IES	;	Przycisk1, HIGH -> LOW
bis.b	#BTN_LOAD|BTN_INC, 	&BTN_IE		;	Przycisk1, Interrupt Enable

;----------------
; Configure variables
;----------------
mov.b	#0x00, LICZNIK
mov.b	#0x00, FLAGI

mov.b	LICZNIK, &SEG7_OUT

eint						; 	Global Enable Interrupts
jmp MainLoop


;-------------------------------------------------------------------------------
; Main loop here
;-------------------------------------------------------------------------------
MainLoop
	dint    ; critical section begins

	bit.b	#LOAD_CHANGED, FLAGI		; Is LOAD_BTN interrupt flag set ?
	jz	AFTER_LOAD_CHECK		; if not, branch
	bic.b	#LOAD_CHANGED, FLAGI

	call 	#BtnLoadChanged			; BtnLoad is changed, we have to check it
AFTER_LOAD_CHECK

	bit.b   #INC_CHANGED, FLAGI     	; Is BTN_INC interrupt flag set?
	jz      AFTER_INC_CHECK          	; if not, branch
	bic.b   #INC_CHANGED, FLAGI

	call    #BtnIncChanged
AFTER_INC_CHECK

	eint    ; critical section ends

	bit.b   #LOAD_FLAG, FLAGI       	; Is LOAD_BTN pressed ?
	jz      TO_SLEEP             		; If not, branch

	call    #LOAD                   	; Execute asynchronic LOAD function
	jmp     MainLoop
TO_SLEEP

	tst.b	FLAGI                   	; Is something changed ?
	jnz	MainLoop        		; There are some flags. branch

	; GO SLEEEEEEP!!!
	bis	#CPUOFF|SCG1|SCG0|OSCOFF, SR    ; enter LPM4 mode

	jmp MainLoop

;-------------------------------------------------------------------------------
;	INT_PORT1 Interrupt Routine
;-------------------------------------------------------------------------------
INT_PORT1

	bit.b	#BTN_LOAD, &BTN_IFG		; Is LOAD_BTN interrupt flag set?
	jz	NOT_LOAD_INT_FLAG		; branch if not

	bic.b	#BTN_LOAD, &BTN_IFG
	bic.b	#BTN_LOAD, &BTN_IE

	bis.b	#LOAD_CHANGED, FLAGI

NOT_LOAD_INT_FLAG

	bit.b	#BTN_INC, &BTN_IFG		; IS INC_BTN INT FLAG?
	jz	NOT_INC_INT_FLAG		; branch if not

	bic.b	#BTN_INC, &BTN_IFG
	bic.b	#BTN_INC, &BTN_IE

	bis.b	#INC_CHANGED, FLAGI

NOT_INC_INT_FLAG

	bic	#CPUOFF|SCG1|SCG0|OSCOFF, 0(SP) ; CANCEL SLEEP, exit LPM4 mode.
	reti

;-------------------------------------------------------------------------------
;	BtnLoadChanged procedure
;-------------------------------------------------------------------------------
BtnLoadChanged
	mov.b   #BTN_LOAD, R15          ; Insert arg for DEBOUNCE fun - BTN_LOAD pin
	call 	#DEBOUNCE		; Debounce and check btn state. State is stored in R15

	bic.b   #LOAD_FLAG, FLAGI       ; clear LOAD_FLAG

	tst.b   R15                     ; Check state of BTN_LOAD - from DEBOUNCE function
	jnz     LOAD_NOT_PRESSED        ; if not pressed, branch

	bis.b	#LOAD_FLAG, FLAGI

LOAD_NOT_PRESSED
	bis.b	#BTN_LOAD, &BTN_IE      ; re-enable interrupt for BTN_LOAD

	ret

;-------------------------------------------------------------------------------
;	BtnIncChanged procedure
;-------------------------------------------------------------------------------
BtnIncChanged
	mov.b   #BTN_INC, R15           ; Insert arg for DEBOUNCE fun - BTN_INC pin
	call 	#DEBOUNCE		; Debounce and check btn state. State is stored in R15

	tst.b   R15                     ; Check state of BTN_INC - from DEBOUNCE function
	jnz	INC_NOT_PRESSED         ; if not pressed - branch

	bit.b	#LOAD_FLAG, FLAGI       ; If BTN_LOAD is also pressed
	jnz	INC_NOT_PRESSED         ; branch

	call 	#INCREMENT              ; if not - do increment

INC_NOT_PRESSED
	bis.b	#BTN_INC, &BTN_IE       ; re-enable interrupt for BTN_INC
	ret

;-------------------------------------------------------------------------------
;	LOAD procedure
;-------------------------------------------------------------------------------
LOAD
	push R4
	push R5

	mov.b	&HEX_IN, R4              ; get value from HEX switch
	and.b	#0x0F, R4                ; and only from one

	mov.w	#TAB_JOHNSON, R5         ; calculate position of our code in array
	add.w	R4, R5

	mov.b	@R5, &LICZNIK            ; get code from array

	mov.b	LICZNIK, &SEG7_OUT       ; display code in 7SEG

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

	mov.b	LICZNIK, &SEG7_OUT       ; display code in 7SEG

	ret

;-------------------------------------------------------------------------------
;	DEBOUNCE procedure
;   It checks button's state 45 times with 1ms delay between samples
;   and then decides if button is really pressed or not.
;
;   Button mask pin , which is to check should be stored in R15
;   As a result, function writes to R15:
;       0 - if button is pressed
;       1 - if button is not pressed
;-------------------------------------------------------------------------------
DEBOUNCE
	push R4
	push R5
	push R6

	mov.b	#0, R4    ; button states counter
	mov.b	#45, R5   ; 45 times checking

SAMPLE_CHECK_LOOP
	bit.b   R15, BTN_IN ; check button state
	jnz     SAMPLE_PRESSED  ; if it is not pressed - branch

	dec.b	R4  ;   if it is pressed - decrement
	jmp     AFTER_SAMPLE_CHECK

SAMPLE_PRESSED
	inc.b       R4  ; if it is not pressed - increment

AFTER_SAMPLE_CHECK

	mov.b       #200, R6 ; Delay constant - 1ms
DELAY_LOOP
	dec.b       R6 ; 2 cycles
	jnz         DELAY_LOOP ; 2 cycles

	; after 1ms delay loop
	dec.b       R5
	jnz         SAMPLE_CHECK_LOOP ; make one more sample

	; after 45 samples
	; decide now, if button is really pressed, or not

	mov.b       #0, R15 ; assume, it's pressed
	tst.b       R4  ; test the sum, if it's negative or not
	jn          BUTTON_IS_PRESSED  ; it's negative, we're right, branch
	mov.b       #1, R15; It's not negative. Button is not pressed

BUTTON_IS_PRESSED

	pop R6
	pop R5
	pop R4

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



