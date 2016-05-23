ORG	1800h

;---------------------------------------------
; RESET, start programu z pod adresu 0
;---------------------------------------------
RESET:
	LD	SP, 0x2000 ; inicjuj stos
	IM	1 ; pierwszy typ przerwan

	; inicjuj licznik
	LD 	A, 0x00
	LD 	(ZMIENNA), A
	OUT	(0x00), A ; wyswietl wynik na ledach
	
	LD	(ZMIENNA_INT), A ; zaladuj zmienna_int zerem

	EI
	JR	LOOP ; skocz do petli glownej
	
	
DS	0x1838-$
;-------------------------------------------------
; INT, procedura przerwania maskowalnego, asynchroniczne ladowanie
;-------------------------------------------------
INT_LADOWANIE:
	EX 	AF, AF'	; alternatywnie : PUSH AF
	EXX

	IN 	A, (00h) ; pobierz stan HEXa, wartosc w kodzie NKB
	
	; tutaj trzeba to przerobic na kod johnsona	
	; pierw przerabiamy wczytana wartosc z hexa na kod NKB 3bit
	LD 	B, 0x07 ; maska trzech pierwszych bitow
	AND 	B ; wyzeruj pozostale	
	LD 	HL, WARTOSCI_JOHNSON ; zaladuj do HL adres poczatku tablicy
	LD 	C, A ; wrzuc nasz kod przesuniecia do C
	LD 	B, 0x00 ;  B wyzeruj
	ADD 	HL, BC ; dodaj przesuniecie, wynik w HL

	LD 	A, (HL) ; obliczylismy miejsce w tablicy
	LD 	(ZMIENNA), A ; teraz ladujemy ta wartosc do zmiennej
	
	OUT 	(0x00), A; wyswietl wynik na ledach	

	IN	A, (00h) ; pobierz stan hexa
	BIT	INC_BTN, A ; sprawdz, czy czasem teraz nie jest wcisly
	JR	NZ, NIEWCISNIETY ; jesli nie, to skocz
	
	LD 	A, FLAGA ; jesli tak, ustaw flage
	LD 	(ZMIENNA_INT), A
NIEWCISNIETY:
	EX 	AF, AF' ; jeslibysmy uzyli PUSH, to teraz POP AF
	EXX

	EI
	RETI

;-------------------------------------------------
; Petla glowna programu
;-------------------------------------------------
LOOP:
	OCZEKIWANIE: 	; aktywnie oczekujemy na nacisniecie
		IN 	A, (00h) ; pobierz stan hexa
		BIT 	INC_BTN, A ; sprawdz, czy nacisnieto przycisk
		JR 	NZ, OCZEKIWANIE ; jesli nie nacisnieto, to sprawdz ponownie
	
	; przycisk nacisniety.
	; pierw sprawdzamy, czy nie byly oba wcisniete
	DI	
	LD 	A, (ZMIENNA_INT) ; pobierz stan flagi	
	BIT 	FLAGA, A ; sprawdz stan 
	
	JR 	NZ, NIE_INKREMENTUJEMY; omijamy inkrementowanie, jesli flaga ustawiona
	CALL 	INKREMENTUJ ; je¶li nie, inkrementujemy licznik
	JR	PO_INKREMENTACJI
NIE_INKREMENTUJEMY:
	RES 	FLAGA, A ; skasuj flage
	LD 	(ZMIENNA_INT), A
PO_INKREMENTACJI:
	EI
	
	CALL 	DELAY ; przycisk nacisniety. Odczekujemy debouncing
	
	OCZEKIWANIE2: ; drugie oczekiwanie, aby przycisk zostal puszczony
		IN 	A, (0x00) ; zaladuj stan hexa
		BIT 	INC_BTN, A ; sprawdz stan przycisku
		JR 	Z, OCZEKIWANIE2 ; jesli nadal jest wcisniety, powtarzaj
		
	CALL 	DELAY ; przycisk puszczony, odczekaj
	
	JR 	LOOP ; zapetl program
	
	
;-------------------------------------------------
; Delay - procedura odczekujaca okolo 50ms
;-------------------------------------------------
DELAY:
	PUSH 	AF ; odloz na stos rejestry
	PUSH 	BC
	
	XOR 	A ; wyzeruj akumulator
	LD 	B, 0x1E	; w przyblizeniu 50ms

	INCREMENT_OUTER: ; zewnetrzna petla
		INCREMENT_INNER:
			INC 	A
			JR 	NZ, INCREMENT_INNER
		DEC 	B ; dekrementuj B
		JR 	NZ, INCREMENT_OUTER ; jesli zero, to koncz
	
	POP 	BC
	POP 	AF
	RET

;-------------------------------------------------
; INKREMENTUJ - inkrementowanie liczniku Johnsona z wyswietleniem jego stanu
; 	aby to zrobic dla czterobitowego kodu, musimy przesuwac w prawo
;	i jednoczesnie uzupelniac bit zero negacja czwartego bitu(ktory wychodzi).
;-------------------------------------------------
INKREMENTUJ:
	PUSH 	AF
	LD 	A, (ZMIENNA) ; pobierz zmienna
	BIT 	3, A ; sprawdz stan czwartego bitu
	 
	SCF 	;SET CARRY FLAG
	CCF 	;INVERT CARRY FLAG(RESET CF)
	; TUTAJ CF=0
	; JESLI BIT3, A == 0
	; TO USTAW CF
	JR 	NZ, PO_MODYFIKACJI_CF
	SCF 	; SET CARRY FLAG
	
PO_MODYFIKACJI_CF:
	RLA 	; PRZESUN W LEWO, UZUPELNIJ NASZYM CARRY Z PRAWEJ

	LD 	(ZMIENNA), A ; uaktualnij zmienna
	OUT 	(0x00), A ; wyswietl wynik na ledach

	POP 	AF
	RET

;-------------------------------------------------
; Dane programu
;-------------------------------------------------
ZMIENNA: ; zmienna - licznik
DB 0xA0
ZMIENNA_INT: ; zmienna - znacznik przerwania
DB 0x0A
WARTOSCI_JOHNSON: ; stablicowane wartosci kodu Johnsona
DB 0x00, 0x01, 0x03, 0x07, 0x0F, 0x0E, 0x0C, 0x08

IO_BTN:		equ	0x05
INC_BTN:	equ	4
FLAGA:		equ	0
