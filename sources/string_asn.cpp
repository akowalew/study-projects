/*
 * string_asn.cpp
 *
 *  Created on: 20 maj 2015
 *      Author: dicker
 */
#include "../headers/string_asn.h"

int string_asn::try_read_from_stream( std::istream &iss) throw(throw_error_e)
{
	data_can_read = false ;
	bool state ;

	std::vector<uint8_t> wektor_zawartosc ;
	try
	{
		state = try_read_all(iss, wektor_zawartosc) ; // czy się zgadza TAG ?
	}
	catch(throw_error_e &ex) { throw ex ; }

	if(!state)
		return -1 ;
	else
	{
		// mamy zawartość, więc teraz odkodujemy to, co odczytaliśmy.
		// po prostu odczytujemy znaki
		value.resize(wektor_zawartosc.size()) ;
		for(int i = 0 ; i < wektor_zawartosc.size() ; i++)
			value[i] = wektor_zawartosc[i] ;

		length = wektor_zawartosc.size() ;
		data_can_read = true ; // można odczytać daną

		/* INTEGER jest tak prostym typem, że nie trzeba sprawdzać poprawności
		 * na poziomie wczytanych oktetów. Jednym słowem, zgadza się ilość, zgadza się tak
		 * więc taką też liczbę wczytujemy, dodatkowo zwracamy liczbę znaków, które wyjęliśmy
		 * ze strumienia.
		 */

		return 1+1+(length) ; // 1 TAGu, 1 LENGTH, +LENGTH zawartości, bo mamy LENGTH oktetów
		// każdy po dwa znaki
	}
}

void string_asn::set_value(std::string val)
{
	value = val ;
	length = val.size() ;
	data_can_read = true ;
}

