/*
 * Copyright © 2018 Denis Zyamaev. Email: (code4un@yandex.ru)
 * License: MIT (see "LICENSE" file)
 * Author: Denis Zyamaev (code4un@yandex.ru)
 * API: C++ 11
*/

#ifndef DEBUG
// Include STL
#include <iostream>
#include <cstdlib>
#endif // !DEBUG

#define _C0DE4UN_MULTITHREADING_ENABLED_

// Include fast_ptr
#include "fast_ptr.hxx"

// Include relative_ptr
#include "relative_ptr.hxx"

static void fast_ptr_test( ) noexcept
{

	// Create pointer
	c0de4un::fast_ptr<unsigned int> ptr_( new unsigned int( 777 ) );

	// Create pointer assignment-copy
	c0de4un::fast_ptr<unsigned int> ptr_copy( ptr_ );

}

static void relative_ptr_test( ) noexcept
{

	// Create 'raw-pointer'
	unsigned int *const raw_pointer = new unsigned int( 777 );

	// Create pointer
	c0de4un::relative_ptr<unsigned int> rel_ptr( raw_pointer );

	// Copy pointer with assignment operator
	c0de4un::relative_ptr<unsigned int> rel_ptr_copy( rel_ptr );

	// Retrieve pointer instance using raw-pointer
	c0de4un::relative_ptr<unsigned int> rel_ptr_2( raw_pointer );

}

/* MAIN */
int main( int argC, char** argV )
{
	// Print Hello to Console
	std::cout << "Simple Pointer Example Started" << std::endl;

	// Test fast_ptr
	fast_ptr_test( );

	// Test relative_ptr
	relative_ptr_test( );

	// Print Bye to Console
	std::cout << "Simple Pointer Example Finished" << std::endl;
	
	// Pause Console-Window
	std::cin.get( );

	// Return OK
	return( 0 );
}