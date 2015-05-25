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

public :

	integer_asn dzien, miesiac, rok ;

	date_asn(bool opt = false) ;
	void set_value(long ldzien, long lmiesiac, long lrok) ;
	void get_value(long &ldzien, long &lmiesiac, long &lrok) ;

};

#endif /* HEADERS_DATE_ASN_H_ */
