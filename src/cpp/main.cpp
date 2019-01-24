/*
 * Copyright © 2018 Denis Zyamaev. Email: (code4un@yandex.ru)
 * License: MIT (see "LICENSE" file)
 * Author: Denis Zyamaev (code4un@yandex.ru)
 * API: C++ 11
*/

// Include iostream
#include <iostream>

// Include stdlib
#include <cstdlib>

// Include rel_ptr
//#include "rel_ptr.hpp"

// Include trel_ptr
#include "typeless_rel_ptr.hpp"

// Include Object
#include "objects/Object.hpp"

static void prel_ptr_type_test( void *const pObject )
{

	// 
	c0de4un::trel_ptr<void> copy2_( pObject );

	// Print to a Console
	std::cout << "void-pointer created, pointers count=" << copy2_.count( ) << std::endl;

}

static void prel_ptr_copy_test( c0de4un::trel_ptr<c0de4un::Object> & pObject )
{

	c0de4un::trel_ptr<c0de4un::Object> copy_ = pObject;
	
	// Print to a Console
	std::cout << "Object-pointer copied, pointers count=" << pObject.count( ) << std::endl;

}

static void trel_ptr_test( )
{

	// Create Object
	c0de4un::trel_ptr<c0de4un::Object> object_sp( new c0de4un::Object( ) );
	
	// Print to a Console
	std::cout << "Object created, pointers count=" << object_sp.count( ) << std::endl;
	
	prel_ptr_copy_test( object_sp );

	prel_ptr_type_test( object_sp.get( ) );

}

/* MAIN */
int main( )
{
	// Print Hello to Console
	std::cout << "Simple Pointer Example Started" << std::endl;

	// 
	trel_ptr_test( );

	// Print Bye to Console
	std::cout << "Simple Pointer Example Finished" << std::endl;
	
	// Pause Console-Window
	std::cin.get( );

	// Return OK
	return( 0 );
}