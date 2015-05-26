#ifndef CZLOWIEK_ASN_H
#define CZLOWIEK_ASN_H

#include "asn_sequence.h"
#include "date_asn.h"
#include "integer_asn.h"
#include "string_asn.h"

#include <iostream>

class czlowiek_asn : public asn_sequence
{
public :
	string_asn imie ;
	string_asn nazwisko ;
	integer_asn wiek ;

	date_asn data_urodzenia ;

	czlowiek_asn(bool opt = false )
	:	asn_sequence(opt), imie(true), nazwisko(true), wiek(true), data_urodzenia(true)
	{
		add_item(&imie) ;
		add_item(&nazwisko) ;
		add_item(&wiek) ;
		add_item(&data_urodzenia) ;
	}

};

#endif // CZLOWIEK_ASN_H
