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
#include "headers/integer_asn.h"
using namespace std ;

int main()
{
	fstream f ;
	f.open("file", fstream::out) ;
	f << "30060201aa0201ff0301bb" ;
	f.close() ;

	f.open("file", fstream::in) ;
	date_asn incior ;
	cout << incior.try_read_from_stream(f) << endl ;
	long a, b, c ;
	incior.get_value(a, b, c) ;
	f.close() ;

	return 0 ;
}



