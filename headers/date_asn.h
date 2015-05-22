/*
 * data_ur_asn.h
 *
 *  Created on: 20 maj 2015
 *      Author: dicker
 */

#ifndef HEADERS_DATE_ASN_H_
#define HEADERS_DATE_ASN_H_

#include "asn_sequence.h"
#include "integer_asn.h"

class date_asn : public asn_sequence
{
	integer_asn dzien, miesiac, rok ;
public :

	date_asn(bool opt = false) : asn_sequence(opt), dzien(true), miesiac(true), rok(true)
	{
		add_item(&dzien) ;
		add_item(&miesiac) ;
		add_item(&rok) ;
	}

	void set_value(long ldzien, long lmiesiac, long lrok)
	{
		dzien.set_value(ldzien) ;
		miesiac.set_value(lmiesiac) ;
		rok.set_value(lrok) ;
	}

	void get_value(long &ldzien, long &lmiesiac, long &lrok)
	{
		ldzien = dzien.get_value() ;
		lmiesiac = miesiac.get_value() ;
		lrok = rok.get_value() ;
  	}

};



#endif /* HEADERS_DATE_ASN_H_ */
