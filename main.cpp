/*
 * main.cpp

 *
 *  Created on: 20 maj 2015
 *      Author: dicker
 */
#include <iostream>
#include <fstream>
#include <cstring>

#include "headers/date_asn.h"
#include "headers/date2_asn.h"
#include "headers/integer_asn.h"
#include "headers/string_asn.h"
#include "headers/asn_sequence.h"
#include "headers/czlowiek_asn.h"

using namespace std ;



int main()
{
	fstream f ;
	f.open("file", fstream::out) ;
	f << "30161A02BBDD1A02CCEE0201DC3009020133020144020177" ;
	f.close() ;

	f.open("file", fstream::in) ;
	czlowiek_asn czlowiek ;
	cout << czlowiek.try_read_from_stream(f) << endl ;
	cout << czlowiek.is_readable() << ' ' << (int)czlowiek.get_length()  << endl ;

	if(czlowiek.is_readable())
	{
		cout << czlowiek.imie.get_value() << endl <<
				czlowiek.naz
	}

	f.close() ;

	return 0 ;
}



