/** \file czlowiek_asn.h
 * 	\brief Deklaracja klasy czlowieka
 */

#ifndef CZLOWIEK_ASN_H
#define CZLOWIEK_ASN_H

#include "asn_sequence.h"
#include "date_asn.h"
#include "integer_asn.h"
#include "string_asn.h"

#include <iostream>

/** \brief Klasa opisująca człowieka, sekwencja
 *
 * 	Reprezentuje pojedynczego człowieka, przechowując informacje o jego pojedynczych cecach.
 */
class czlowiek_asn : public asn_sequence
{
public :
	string_asn imie ;	//!< Imię w postaci napisowej
	string_asn nazwisko ; //!< Nazwisko w postaci napisowej
	integer_asn wiek ;	//!< Wiek w postaci liczby całkowitej

	date_asn data_urodzenia ;	//!< Data urodzenia

	/**
	 *  \brief Konstruktor człowieka.
	 *
	 *  Wszystkie pola człowieka są definiowane jako opcjonalne.
	 *
	 *  @param opt decydujemy, czy człowiek, jako obiekt jest opcjonalny
	 */
	czlowiek_asn(bool opt = false )
	:	asn_sequence(opt), imie(true), nazwisko(true), wiek(true), data_urodzenia(true, true, true, true)
	{
		add_item(&imie) ;
		add_item(&nazwisko) ;
		add_item(&wiek) ;
		add_item(&data_urodzenia) ;
	}
};

#endif // CZLOWIEK_ASN_H
