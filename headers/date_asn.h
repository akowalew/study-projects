/**
 * \file date_asn.h
 * \brief Deklaracja klasy reprezentującej datę
 */
#ifndef HEADERS_DATE_ASN_H_
#define HEADERS_DATE_ASN_H_

#include "asn_sequence.h"
#include "integer_asn.h"

/**
 * \brief Klasa reprezentująca pojedynczą datę.
 *
 * Można tutaj przechować datę postaci DZIEŃ, MIESIĄC, ROK
 */
class date_asn : public asn_sequence
{
public :

	integer_asn dzien, //!< Dzień w postaci l. całkowitej
				miesiac, //!< Miesiac w postaci l. całkowitej
				rok ;	//!< Rok w postaci l. całkowitej

	/**
	 * \brief Konstruktor daty
	 *
	 * @param opt_dzien	Czy dzień ma być opcjonalny ?
	 * @param opt_miesiac Czy miesiąc ma być opcjonalny?
	 * @param opt_rok Czy rok ma być opcjonalny?
	 * @param opt Czy data, jako obiekt, ma być opcjonalna ?
	 */
	date_asn(bool opt_dzien = false , bool opt_miesiac = false , bool opt_rok = false, bool opt = false) ;
	void set_value(long ldzien, long lmiesiac, long lrok) ; //!< Zapisz datę, wszystkie jej pola
	/**
	 * \brief Pobierz datę, wszystkie jej pola
	 *
	 * Argumenty podane są jako referencje.
	 * @warning	Funkcja nie sprawdza poprawności, tzn. jeśli obiekt nie został wcześniej
	 * wczytany, to do danej zmiennej mogą zostać wczytane śmieci. Trzeba się upewnić,
	 * z jakich zmiennych dostaniemy sensowny wynik.
	 * @see is_readable()
	 */
	void get_value(long &ldzien, long &lmiesiac, long &lrok) ;
};

#endif /* HEADERS_DATE_ASN_H_ */
