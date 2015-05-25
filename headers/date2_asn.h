/*
 * date2_asn.h
 *
 *  Created on: 26 maj 2015
 *      Author: dicker
 */

#ifndef DATE2_ASN_H_
#define DATE2_ASN_H_

#include "asn_sequence.h"
#include "integer_asn.h"
#include "string_asn.h"

class date2_asn : public asn_sequence
{

public :

	integer_asn dzien, miesiac;
	string_asn rok ;

	date2_asn(bool opt = false) : asn_sequence(opt), dzien(true), miesiac(true), rok(false)
	{
		add_item(&dzien) ;
		add_item(&miesiac) ;
		add_item(&rok) ;
	}

} ;

#endif /* DATE2_ASN_H_ */
