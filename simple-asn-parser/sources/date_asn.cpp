/*
 * data_ur_asn.cpp
 *
 *  Created on: 20 maj 2015
 *      Author: dicker
 */

#include "../headers/date_asn.h"

date_asn::date_asn(bool opt_dzien, bool opt_miesiac, bool opt_rok, bool opt)
	: asn_sequence(opt), dzien(opt_dzien), miesiac(opt_miesiac), rok(opt_rok)
{
	add_item(&dzien) ;
	add_item(&miesiac) ;
	add_item(&rok) ;
}

void date_asn::set_value(long ldzien, long lmiesiac, long lrok)
{
	dzien.set_value(ldzien) ;
	miesiac.set_value(lmiesiac) ;
	rok.set_value(lrok) ;
}

void date_asn::get_value(long &ldzien, long &lmiesiac, long &lrok)
{
	ldzien = dzien.get_value() ;
	lmiesiac = miesiac.get_value() ;
	lrok = rok.get_value() ;
}
