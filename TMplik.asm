ORG 1800h
RESET:
	DI
	
	; INICJUJ STOS
	LD SP, 0x1FFF
	
	;PIERWSZY TYP PRZERWAN
	IM 1
	
	; INICJUJ LICZNIK
	LD A, 0xF0
	LD (ZMIENNA), A
	
	; WYZERUJ ZNACZNIK PRZERWANIA
	LD A, 0x00
	LD (ZMIENNA_INT), A
	
	CALL WYSWIETL_ZMIENNA
	
	EI
	
	;SKOCZ DO PETLI GLOWNEJ
	JR LOOP
	DS 0x1838-$, 0

INT_LADOWANIE:
	PUSH AF
	
	; pobierz stan HEXa
	IN A, (00h)
	
	; zapisz do zmiennej
	LD (ZMIENNA), A
	
	; USTAW ZNACZNIK
	LD A, 0xFF
	LD (ZMIENNA_INT), A
	
	OUT (0x00), A
	
	POP AF
	EI
	RETI
	
LOOP:
	; AKTYWNIE OCZEKUJEMY NA NACISNIECIE
	
	OCZEKIWANIE:
		IN A, (00h)
		BIT 4, A
		JR NZ, OCZEKIWANIE

	LD A, (ZMIENNA_INT)
	BIT 0, A
	LD A, 0x00
	LD (ZMIENNA_INT), A
	JR NZ, NACISNIETY

	; INKREMENTUJ LICZNIK
	CALL INKREMENTUJ

	; wypisz na ledy wartosc zmiennej
	CALL WYSWIETL_ZMIENNA
	
	NACISNIETY:
	; PRZYCISK NACISNIETY
	; ODCZEKUJEMY DEBOUNCING
	CALL DELAY

	OCZEKIWANIE2:
		IN A, (0x00)
		BIT 4, A
		JR Z, OCZEKIWANIE2
	
	; PRZYCISK PUSZCZONY
	; ODCZEKUJEMY DEBOUNCING
	CALL DELAY
	
	; zapetl program
	JR LOOP

DELAY:
	; odloz na stos rejestry
	PUSH AF
	PUSH BC
	; wyczysc te, z których bedziemy korzystac
	; wyzeruj akumulator
	XOR A
	; LADUJEMY REJESTR B WARTOSCIA 30(dec)
	; da nam to czas mniej wiecej 50ms, 
	; co wystarczy na odczekanie bouncingu
	LD B, 0x1E

	; zewnetrzna petla
	INCREMENT_OUTER:
		INCREMENT_INNER:
			INC A
			JR NZ, INCREMENT_INNER
		DEC B	; dekrementuj B
		JR NZ, INCREMENT_OUTER	; jesli zero, to koncz
	
	; odczekal okolo 50ms
	POP BC
	POP AF
	RET

INKREMENTUJ:

	PUSH AF
	LD A, (ZMIENNA)
	BIT 3, A
	 
	SCF ;SET CARRY FLAG
	CCF ;INVERT CARRY FLAG(RESET CF)
	
	; TUTAJ CF=0
	; JEŒLI BIT3, A == 0
	; TO USTAW CF
	
	JR NZ, PO_MODYFIKACJI_CF
	SCF ; SET CARRY FLAG
	
	PO_MODYFIKACJI_CF:
	
	; PRZESUN W LEWO, UZUPELNIJ NASZYM CARRY Z PRAWEJ
	RLA ;
	LD (ZMIENNA), A
	POP AF
	RET

WYSWIETL_ZMIENNA:
	PUSH AF
	; wypisz na ledy wartosc zmiennej
	LD A, (ZMIENNA)
	OUT (0x00), A
	POP AF
	RET

ZMIENNA:	
DB 0xA0
ZMIENNA_INT:
DB 0x0A
WARTOSC_JOHNSON:
DB 0x00, 0x01, 0x03, 0x07, 0x0F, 0x0E, 0x0C, 0x08
