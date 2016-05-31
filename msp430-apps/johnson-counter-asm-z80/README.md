# johnson-counter-asm-z80

Aplikacja napisana w Assemblerze do wykorzystania w mikroprocesorze Z80 firmy Zilog. Program realizuje funkcję licznika
Johnsona 4bitowego, który możemy inkrementować jednym przyciskiem reagującym zboczem oraz ładować asynchronicznie (w przerwaniu wywoływanem
niskim stanem logicznym) drugim przyciskiem. Stan liczników pobierany jest przy pomocy buforów wejściowych.
Stan licznika wyświetlany jest przy użyciu buforów zatrzaskujących na diodach LED.
