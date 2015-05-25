/*
 * asn_sequence.cpp
 *
 *  Created on: 20 maj 2015
 *      Author: dicker
 */

#include "../headers/asn_sequence.h"

int asn_sequence::try_read_from_stream( std::istream &iss) throw(throw_error_e)
{

	data_can_read = false ;

	uint8_t r_length ; // ilość oktetów, którą odczytamy z nagłówka

	try
	{
		// pierw wczytujemy TAG
		// sprawdzamy, czy to jest tag SEQUENCE
		if(read_octet(iss)  != tag)
		{
			// musimy oddać znaki do strumienia, dwa znaki
			iss.unget() ; iss.unget() ;

			// nie pasuje nam to!
			return -1 ;
		}

		// teraz ładujemy LENGTH
		r_length = read_octet(iss) ; // wczytanie dwóch znaków
		// zakładamy formę krótką, czyli wielkość <= 127 !!!
		if(r_length > 127)
			throw(DATA_FAIL) ;
	}
	catch(throw_error_e &err)
	{
		throw(err) ;
	}

	// czytając sekwencję nie powinniśmy wczytywać od razu zawartości, bo to nam nic nie da
	// lepiej jak będziemy wysyłać kolejne porcje strumienia do kolejnych obiektów.
	// Tym samym odbieramy od każdego z obiektów, ile oktetów wczytał, by w razie czego wiedzieć,
	// ile z nich oddać do strumienia. Poza tym, suma tych ilości powinna dać ilość LEN, jaką wczytalśmy

	// num_unget oznacza ilość oktetów dotychczas wczytanych
	// tmp oznacza ilość oktetów wczytanych podzcas jednej iteracji
	uint8_t num_unget = 0 ;
	int tmp ;

	for(unsigned int i = 0 ; i < wektor_elementow.size() ; i++)
	{
		/* przypadek następujący :
		 * odczytujemy nagłówek sekwencji, on deklaruje r_length, ale nawet po zrównaniu
		 * tej ilości nie mamy wypełnionej jeszcze do końca struktury.
		 * Więc jeśli okaże się, że pozostałe elementy są opcjonalne, to OK, mamy jednak załadowaną strukturę
		 * jeśli okaże się, że gdzieś tam jest element obowiązkowy, to mamy niedopasowanie!
		 */

		if(num_unget == r_length)
		{
			bool stan = true ;
			for(unsigned int j = i ; j < wektor_elementow.size() ; j++)
				if(wektor_elementow[j]->is_optional() == false)
				{
					stan = false ;
					break ;
				}

			if(stan == true)
			{
				// pozostałe pola są opcjonalne, czyli OK
				break ;
			}
			else
			{
				// nie wszystkie pozostałe pola są opcjonalne. mamy niedopasowanie, przerywamy

				// pozostaje nam tylko oddanie znaków tych, co dobrze się wczytały
				for(uint8_t k = 0 ; k < num_unget*2 ; k++) // oktet = 2 znaki
					iss.unget() ;

				// dodatkowo musimy oddać nasze znaki TAGu i LENGTHu, łącznie cztery
				iss.unget() ; iss.unget() ; iss.unget() ; iss.unget() ;

				// trzeba zaznaczyć we wszystkich poprzednich, że jednak nie można z nich czytać
				for(int k = i ; k >= 0 ; k--)
					wektor_elementow[k]->data_can_read = false ;

				return -1 ; // nie pasuje!
			}
		}


		try
		{ tmp = wektor_elementow[i]->try_read_from_stream(iss) ; }
		catch(throw_error_e &ex)
		{ throw ex; }

		if(tmp == -1)
		{
			// ELEMENT NIE PASUJE
			// trzeba spróbować przy następnej iteracji
			// strumień został już odpowiednio cofnięty, tak by kolejny
			// obiekt mógł śmiało z niego skorzystać

			// pytanie tylko, czy to czasem akurat nie jest obiekt NIEOPCJONALNY?!
			if(wektor_elementow[i]->data_is_optional)
				continue ; // jest opcjonalny, możemy opuścić i spróbować dla następnego
			else
			{//ok
				// element MUSI pasować, a tak nie jest.
				// mamy zatem niedopasowanie, trzeba zwrócić do strumienia odpowiednią ilość znaków

				// element, który próbowaliśmy wczytać i który się nie dał, już zwrócił, co trzeba
				// pozostaje nam tylko oddanie znaków tych, co dobrze się wczytały
				for(uint8_t j = 0 ; j < num_unget*2 ; j++) // oktet = 2 znaki
					iss.unget() ;

				// dodatkowo musimy oddać znaki TAGu i LENGTHu, łącznie cztery
				iss.unget() ; iss.unget() ; iss.unget() ; iss.unget() ;

				// trzeba zaznaczyć we wszystkich poprzednich, że jednak nie można z nich czytać
				for(int j = i ; j >= 0 ; j--)
					wektor_elementow[j]->data_can_read = false ;

				return -1 ; // nie pasuje!
			}
		}
		else
		{
			// mamy coś różnego od -1, czyli poprawne wczytanie i wiemy, ile wykorzystał znaków

			if(num_unget + tmp > r_length)
			{
				/*
				 * num_unget oznacza ilość dotychczas wczytanych oktetow
				 * tmp oznacza ilość oktetow wczytanych podzcas tej iteracji w wyniku poprawnego załadowania
				 * r_length to ilość oktetów, które odczytaliśmy z nagłówka LENGTH sekwencji
				 *
				 * nie można więc przekroczyć ilości znaków r_length
				 * mamy błąd całego pliku, więc mamy wyjątek
				 */
				throw(DATA_FAIL) ;
			}
			else
			{
				// ilość się zgadza, więc ok,
				num_unget += tmp ;
			}
		}

	}

	if(num_unget != r_length)
	{
		if(num_unget > r_length) // wczytaliśmy więcej niż powinniśmy, jest to błąd danych
			throw DATA_FAIL ;
		else // wczytaliśmy mniej, niż powinniśmy, więc być może te dane są dla innej sekwencji, mającej więcej pól
		{
			// pozostaje nam tylko oddanie znaków tych, co dobrze się wczytały
			for(uint8_t k = 0 ; k < num_unget*2 ; k++) // oktet = 2 znaki
				iss.unget() ;

			// dodatkowo musimy oddać nasze znaki TAGu i LENGTHu, łącznie cztery
			iss.unget() ; iss.unget() ; iss.unget() ; iss.unget() ;

			// trzeba zaznaczyć we wszystkich poprzednich, że jednak nie można z nich czytać
			for(unsigned int k = 0 ; k < wektor_elementow.size() ; k++)
				wektor_elementow[k]->data_can_read = false ;

			return -1 ; // nie pasuje!
		}
	}


	// wszystko ok!
	data_can_read = true ;
	length = num_unget ;

	return 1+1+r_length ;
}


uint8_t asn_sequence::get_length()
{
	if(!data_can_read)
		return 0 ;

	// założenie : suma wszystkich długości nie przekroczy 127 !!!
	uint8_t ret = 0 ;
	for(int i = 0 ; i < wektor_elementow.size() ; i++)
		if(wektor_elementow[i]->is_readable())
			ret += wektor_elementow[i]->get_length() ;
	return ret ;
}
