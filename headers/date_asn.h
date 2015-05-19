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

	date_asn() : dzien(true), miesiac(true), rok(true)
		{

		}
};



#endif /* HEADERS_DATE_ASN_H_ */
