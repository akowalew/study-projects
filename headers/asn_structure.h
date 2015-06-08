/** \file asn_structure.h
    \brief Deklaracja klasy asn_structure oraz throw_error_e, uint8_t
*/
#ifndef HEADERS_ASN_STRUCTURE_H_
#define HEADERS_ASN_STRUCTURE_H_

#include <istream>
#include <ostream>
#include <cctype>
#include <vector>
#include <typeinfo>

enum throw_error_e //! Typ wyliczeniowy potrzebny przy rzucaniu wyjątków
{
	/** /brief Błąd strumienia wejścia
	 * 	Występuje, gdy próbujemy np. wziąć ze strumienia oktet wymagany, a go tam nie ma */
	STREAM_FAIL,
	/** \brief Błąd danych w strumieniu wejściowym
	 * 	Występuje, gdy np. */
	DATA_FAIL,

	OBJECT_FAIL
};
typedef unsigned char uint8_t ; //!< Typ całkowity 8 bitowy, pojedynczy oktet

/** \brief Klasa abstrakcyjna, klasa podstawowa dla wszystkich pozostałych
 *
 * 	Klasa ta jest punktem wyjścia dla każdej klasy pochodnej używanej w programie.
 	Są w niej zapisane najważniejsze informacje dotyczące pojedynczego obiektu
 	istniejącego w ASN.1 oraz najważniejsze funkcje wirtualne do jego obsługi
 */
class asn_structure
{
protected :

	uint8_t tag ; //!< TAG, w postaci oktetu
	bool data_is_optional ; //!< Czy dany obiekt może być opcjonalny?
	bool data_can_read ;	//!< Czy można czytać aktualnie z danego obiektu?

	/** Przyjaźń ta wymagana jest z tego względu, że klasa sekwencji,
		gdy przykładowo wczytuje dane cały czas poprawnie i nagle napotka błąd(więc nie załaduje się), musi
		poprzednie obiekty zaznaczyć jako nieużywalne, ponieważ nie jest używalna w całości. */
	friend class asn_sequence ; //!< Przyjaźń z klasą sekwencji

public :

	/** \brief Konstruktor
	 * 	@param opt sygnalizuje, czy dany obiekt będzie mógł być opcjonalnym */
	asn_structure(uint8_t ttag, bool opt )
		: tag(ttag), data_is_optional(opt) , data_can_read(false) { }

	bool is_optional() const { return data_is_optional ; } //!< Czy dany obiekt może być opcjonalnym?
	virtual bool is_readable() const { return data_can_read ; } //!< Czy można czytać z danego obiektu?

	uint8_t get_tag() const { return tag ; }	//!< Funkcja zwraca TAG danego typu obiektu

	/** \brief Zwraca LENGTH obiektu
	 *
	 	@todo	Dodać obsługę LENGTH > 127, czyli zajmującej więcej niż jeden oktet
	 */
	virtual uint8_t get_length() const
		{ return 0 ; }

	/** \brief Spróbuj załadować daną ze strumienia wejściowego
	 *
	 *  @warning Założenie jest takie, że w przypadku niepowodzenia, funkcja musi
	 *  cofnąć w strumieniu tyle oktetów, ile z niego wydobyła w trakcie próby wczytania.
	 *
	 * 	@param 	iss Strumień wejściowy z ustaloną pozycją, od której zaczynamy czytać
	 * 	@retval ilość pobranych ze strumienia oktetów w wyniku pomyślnego wczytania
	 * 	@retval -1 gdy zaszło niepowodzenie wczytywania
	 * 	@throw	STREAM_FAIL, DATA_FAIL
	 * 	@see STREAM_FAIL, DATA_FAIL
	 *
	 */
	virtual int try_read_from_stream( std::istream& iss)
		throw(throw_error_e) = 0 ;

	/** \brief Zapisz wszystko do strumienia
	 *
	 * 	Zapisuje kolejno [TAG][LENGTH][CONTENT] w postaci oktetów
	 * 	@param oss	Strumień wyjściowy, do którego ładowane będą dane
	 */
	virtual void write_to_stream(std::ostream& oss) const = 0  ;

	/**
	 * \brief Czy dwie struktury są sobie równe?
	 *
	 * Sprawdzamy równość atrybutów bazy, czyli klasy asn_structure. Sprawdzamy równość tag'ów, is_optional, is_readable.
	 */
	virtual bool operator==(const asn_structure& comp) const
	{
		return ( (get_tag() == comp.get_tag()) &&
				 (is_optional() == comp.is_optional()) &&
				 (is_readable() == comp.is_readable())) ;
	}

	virtual ~asn_structure() { };

	virtual asn_structure& operator= (const asn_structure &obj) throw(throw_error_e) ;
private :

	bool try_read_tag_and_check(std::istream& iss, uint8_t tag)
		throw(throw_error_e) ;
	void try_read_length_contents(std::istream& iss, std::vector<uint8_t> &wektor_zawartosc)
		throw(throw_error_e) ;

protected:
	/**
	 * @warning Funkcja nie sprawdza poprawności danych. Musimy sami sprawdzić, czy
	 * podaliśmy poprawną daną.
	 * @param c Znak będący obowiązkowo cyfrą HEX
	 * @return	Liczba naturalna z zakresu <0;15>, odpowiednik tego HEX
	 */
	uint8_t hextou8(char c) const ; //!< Konwersja z HEX do LICZBY dziesiętnej
	/**
	 *  @warning Funkcja nie sprawdza poprawności danych. Musimy sami sprawdzić, czy
	 *  podaliśmy poprawną daną.
	 * 	@param u8	liczba dziesiętna z zakresu <0;15>
	 * 	@return 	znak w postaci pojedynczej cyfry HEX
	 */
	char	u8tohex(uint8_t u8) const ; //!< Konwersja z LICZBY dziesiętnej do HEX

	/** \brief Sprawdź poprawność i załaduj zawartość do wektora
	 *
	 * 	Funkcja pomocnicza. Sprawdza poprawność tag'u, pobiera długość i ładuje dane
	 * 	do wektora. Wartość LENGTH odczytamy z wielkości (size) wektora.
	 * 	@warning Funkcja oddaje znaki do strumienia, na wypadek niezgodności
	 * 	@param iss Strumień wejściowy, od którego rozpoczynamy wczytywanie
	 * 	@param wektor_zawartość Do niego zapisany zostanie CONTENT
	 * 	@retval true jeśli operacja się powiodła i w wektorze mamy CONTENT
	 * 	@retval	false jeśli operacja się nie udała.
	 * 	@see DATA_FAIL, STREAM_FAIL
	 */
	bool try_read_all(std::istream &iss, std::vector<uint8_t> &wektor_zawartosc)
		throw(throw_error_e) ;
	/** \brief Odczytuje ze strumienia jeden oktet
	 *
	 * @param iss strumień, od którego wczytamy dwa znaki
	 * @return oktet 8-bit powstały z dwóch znaków HEX
	 * @see STREAM_FAIL
	 */
	uint8_t read_octet(std::istream& iss) const
		throw(throw_error_e)  ;
	/** \brief Zapisuje do strumienia TAG i LENGTH w postaci oktetów HEX
	 *
	 * @param oss strumień wyjściowy, do którego ładujemy dane
	 */
	void write_tag_length(std::ostream &oss) const ;


};

#endif /* HEADERS_ASN_STRUCTURE_H_ */
