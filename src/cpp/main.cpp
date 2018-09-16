/*
 * Copyright © 2018 Denis Zyamaev. Email: (code4un@yandex.ru)
 * License: MIT (see "LICENSE" file)
 * Author: Denis Zyamaev (code4un@yandex.ru)
 * API: C++ 11
*/

// Include STL
#include <iostream>
#include <cstdlib>

#define _MULTITHREADING_ENABLED_

// Include SimplePointer
#include "SimplePointer.hpp"

/* MAIN */
int main( int argC, char** argV )
{
	// Print Hello to Console
	std::cout << "Simple Pointer Example Started" << std::endl;

	// Call method
	createData( );

	// Print Bye to Console
	std::cout << "Simple Pointer Example Finished" << std::endl;

	// Pause Console-Window
	std::cin.get( );

	// Return OK
	return( 0 );
}

/* Creates 'pointer' */
static void createData( ) noexcept
{
	// Create temporary pointer
	c0de4un::SimplePointer<int> simple_ptr = c0de4un::SimplePointer::create( 777 );
}