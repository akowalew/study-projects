/*
 * integer_asn.cpp
 *
 *  Created on: 20 maj 2015
 *      Author: dicker
 */

#include "../headers/integer_asn.h"

#include <stack>

void integer_asn::set_value(long val)
{
	value = val ;

	std::stack<uint8_t> stosik ;

	do
	{
		stosik.push(val & 0xFF) ;
		val >>= 8 ;
	}while(val) ;

	length = stosik.size() ;
}

int integer_asn::try_read_from_stream(std::istream& iss) throw(throw_error_e)
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
		value = 0 ;
		for(int i = 0 ; i < wektor_zawartosc.size() ; i++)
		{
			value |= wektor_zawartosc[i] ;
			value <<= 8 ;
		}
		value >>= 8 ;

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

void integer_asn::write_to_stream(std::ostream& oss)
{

}
