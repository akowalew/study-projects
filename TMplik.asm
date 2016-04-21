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
	OUT (0x00), A ; wyswietl wynik na ledach
	
	; WYZERUJ ZNACZNIK PRZERWANIA
	XOR A
	LD (ZMIENNA_INT), A
	
	EI
	
	;SKOCZ DO PETLI GLOWNEJ
	JR LOOP
	DS 0x1838-$

; procedura przerwania maskowalnego, asynchroniczne ladowanie
INT_LADOWANIE:
	; alternatywnie : PUSH AF
	EX AF, AF'
	EXX

	; pobierz stan HEXa, wartosc w kodzie NKB
	IN A, (00h)
	
	; tutaj trzeba to przerobic na kod johnsona	
	; pierw przerabiamy wczytana wartosc z hexa na kod NKB 3bit
	LD B, 0x07 ; maska trzech pierwszych bitow
	AND B ; wyzeruj pozostale	
	LD HL, WARTOSCI_JOHNSON ; zaladuj do HL adres poczatku tablicy
	LD C, A ; wrzuc nasz kod przesuniecia do C
	LD B, 0x00 ;  B wyzeruj
	ADD HL, BC ; dodaj przesuniecie, wynik w HL

	; obliczylismy miejsce w tablicy
	; teraz ladujemy ta wartosc do zmiennej
	LD A, (HL)
	LD (ZMIENNA), A

	; wyswietl wynik na ledach	
	OUT (0x00), A

	; USTAW ZNACZNIK przebywania w przerwaniu
	LD A, 0xFF
	LD (ZMIENNA_INT), A

	; jeslibysmy uzyli PUSH, to teraz POP AF
	EX AF, AF'
	EXX

	EI
	RETI
	
LOOP:
	; AKTYWNIE OCZEKUJEMY NA NACISNIECIE
	OCZEKIWANIE:
		IN A, (00h) ;pobierz stan hexa
		BIT 4, A ; sprawdz, czy nacisnieto przycisk
		JR NZ, OCZEKIWANIE ;jesli nie nacisnieto, to sprawdz ponownie

	; przycisk nacisniety

	; pierw sprawdzamy, czy nie wyszedl dopiero co z przerwania
	; zakladamy, iz przerwanie ustawia wszystkie bity danej zmiennej na 1
	LD A, (ZMIENNA_INT) ; pobierz stan zmiennej 
	BIT 0, A ; sprawdz stan ustawienia bitow zmiennej

	LD A, 0x00 ; w miedzy czasie wyzeruj zmienna znacznikowa przerwania
	LD (ZMIENNA_INT), A

	JR NZ, NACISNIETY ; omijamy inkrementowanie, skoro wyszlismy z przerwania

	; INKREMENTUJ LICZNIK
	CALL INKREMENTUJ

	; wypisz na ledy wartosc zmiennej
	LD A, (ZMIENNA)
	OUT (0x00), A

	
	NACISNIETY:
	; PRZYCISK NACISNIETY
	; ODCZEKUJEMY DEBOUNCING
	CALL DELAY

	; drugie oczekiwanie, aby przycisk zostal puszczony
	OCZEKIWANIE2:
		IN A, (0x00) ; zaladuj stan hexa
		BIT 4, A ; sprawdz stan przycisku
		JR Z, OCZEKIWANIE2 ; jesli nadal jest wcisniety, powtarzaj
	
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
; zadaniem procedury jest zinkrementowac licznik johsnona.
; aby to zrobic dla czterobitowego kodu, musimy przesuwac w prawo
; i jednoczesnie uzupelniac bit zero negacja czwartego bitu(ktory wychodzi).
	PUSH AF
	LD A, (ZMIENNA) ; pobierz zmienna
	BIT 3, A ; sprawdz stan czwartego bitu
	 
	SCF ;SET CARRY FLAG
	CCF ;INVERT CARRY FLAG(RESET CF)
	
	; TUTAJ CF=0
	; JESLI BIT3, A == 0
	; TO USTAW CF
	
	JR NZ, PO_MODYFIKACJI_CF
	SCF ; SET CARRY FLAG
	
	PO_MODYFIKACJI_CF:
	
	; PRZESUN W LEWO, UZUPELNIJ NASZYM CARRY Z PRAWEJ
	RLA ;

	LD (ZMIENNA), A ; uaktualnij zmienna
	POP AF
	RET

ZMIENNA:	
DB 0xA0
ZMIENNA_INT:
DB 0x0A
WARTOSCI_JOHNSON:
DB 0x00, 0x01, 0x03, 0x07, 0x0F, 0x0E, 0x0C, 0x08
