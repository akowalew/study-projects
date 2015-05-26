/*
 * date2_asn.cpp
 *
 *  Created on: 26 maj 2015
 *      Author: dicker
 */
#include "../headers/date2_asn.h"

date2_asn::date2_asn(bool opt) : asn_sequence(opt), dzien(true), miesiac(true), rok(false)
{
	add_item(&dzien) ;
	add_item(&miesiac) ;
	add_item(&rok) ;
}


