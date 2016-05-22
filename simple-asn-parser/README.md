# PROI_ASN
Projekt na przedmiot Programowanie Obiektowe.

Naszym zadaniem było stworzenie kilku klas podstawowych, a następnie z nich utworzyć struktury, które
miały zdolność zapisywania całej swojej zawartości do pliku, a także odczytywania jej z pliku.
Inaczej mówiąc, należało stworzyć prosty parser(w moim przypadku operujący na standardzie ASN.1).
Oznacza to, iż wszystkie dane miały być składowane w plikach w postaci binarnej.

Głównym atutem aplikacji jest zastosowanie obiektowości z elementami polimorfizmu. Dzięki temu
zapis do pliku w większości przypadków oznaczał pojedncze wywołanie wirtualnej funkcji.
Podobnie rzecz miała się z odczytem z pliku.

Struktura aplikacji nie jest skomplikowana. Zasadniczo wyróżnia się dwie klasy podstawowe, będące
punktem wyjścia dla pozostałych obiektów. Jest to asn_structure, oraz asn_sequence.

asn_structure - to cegiełka, z której powstaje każdy typ elementarny. W moim przypadku,
  odziedziczyłem po tej klasie tworząc obiekty typu integer_asn, oraz string_asn
  
asn_sequence - to cegiełka, z której powstaje każdy typ złożony. W moim przypadku,
  na tej podstawie utworzyłem klasy typu date_asn oraz czlowiek_asn. Asn_sequence to analogia
  do klasycznych struktur.
  
Program nie wykonuje nic innego, niż uruchamia testy jednostkowych dla utworzonych przeze
mnie klas. Zapisuje do strumienia całe struktury, po czym odczytuje je, sprawdzając, 
czy przyniesie to zamierzony efekt.

Dodatkowe atuty to przeciążanie operatorów(np. operator porównania), oraz rzucanie wyjątków.
Te wszystkie funkcjonalności są sprawdzane przez testy jednostkowe w oparciu o GoogleTestUnit.

  
