/*
 * Copyright © 2018 Denis Zyamaev. Email: (code4un@yandex.ru)
 * License: MIT (see "LICENSE" file)
 * Author: Denis Zyamaev (code4un@yandex.ru)
 * API: C++ 11
*/

// Pragma
#pragma once

#ifndef _C0DE4UN_FAST_PTR_HXX_
#define _C0DE4UN_FAST_PTR_HXX_

#ifdef _C0DE4UN_MULTITHREADING_ENABLED_ // Multithreading Mode
// Include std::atomic
#include <atomic>
#endif // !_C0DE4UN_MULTITHREADING_ENABLED_

// DEBUG
#ifdef DEBUG
#include <iostream>
#include <cstdlib>
#endif
// DEBUG

namespace c0de4un
{

	// -------------------------------------------------------- \\

	// ===========================================================
	// Types
	// ===========================================================

#ifdef _C0DE4UN_MULTITHREADING_ENABLED_
#ifndef _C0DE4UN_POINTERS_COUNTER_TYPE_DECL_
	/* Type-Alias for instances counter */
	using counter_t = std::atomic<unsigned short>;

#define _C0DE4UN_POINTERS_COUNTER_TYPE_DECL_
#endif // _C0DE4UN_POINTERS_COUNTER_TYPE_DECL_
#else

#ifndef _C0DE4UN_POINTERS_COUNTER_TYPE_DECL_
	/* Type-Alias for instances counter */
	using counter_t = unsigned short;

#define _C0DE4UN_POINTERS_COUNTER_TYPE_DECL_
#endif // _C0DE4UN_POINTERS_COUNTER_TYPE_DECL_

#endif // _C0DE4UN_MULTITHREADING_ENABLED_

	// -------------------------------------------------------- \\

	/*
	 * fast_ptr - simple & fast shared pointer.
	*/
	template <typename T>
	class fast_ptr final
	{

	private:

		// -------------------------------------------------------- \\

		// ===========================================================
		// Fields
		// ===========================================================

		/* Sharable, between pointers instances, object instance */
		T *const mObject;

		/* Shared between Pointers Instances Counter */
		counter_t *const mCounter;

		// -------------------------------------------------------- \\

	public:

		// -------------------------------------------------------- \\

		// ===========================================================
		// Constructors & Destructor
		// ===========================================================

		/*
		 * fast_ptr Constructor with initial value
		 *
		 * @param pObject - object instance to store
		*/
		explicit fast_ptr( T *const pObject ) noexcept
			: mObject( pObject ),
			mCounter( new counter_t( 1 ) )
		{

#ifdef DEBUG
			std::cout << "new fast_ptr created" << std::endl;
#endif // DEBUG

		}

		/*
		 * fast_ptr const copy constructor
		*/
		explicit fast_ptr( const fast_ptr<T> & pOther ) noexcept
			: mObject( pOther.mObject ),
			mCounter( pOther.mCounter )
		{

			// Increase Pointers-Instances Counter
			( *mCounter )++;

#ifdef DEBUG
			std::cout << "fast_ptr copied, instances_stored=" << *mCounter << std::endl;
#endif // DEBUG

		}

		/*
		 * fast_ptr const copy operator
		*/
		fast_ptr<T> operator=( const fast_ptr<T> & pOther ) noexcept
		{
			// Cancel if self-copy
			if ( this == &pOther )
				return( *this );

			// Copy values
			mObject = pOther.mObject;
			mCounter = pOther.mCounter;

			// Increase Pointers-Instances Counter
			( *mCounter )++;

#ifdef DEBUG
			std::cout << "fast_ptr copied, instances_stored=" << *mCounter << std::endl;
#endif // DEBUG

		}

		/* fast_ptr Destructor */
		~fast_ptr( ) noexcept
		{

			// Get reference to the counter
			counter_t & counter_lr = *mCounter;

			// Decrease Pointers Instances Counter
			counter_lr--;

#ifdef DEBUG
			std::cout << "fast_ptr deleted, instances_stored=" << counter_lr << std::endl;
#endif // DEBUG

			// If last instance
			if ( counter_lr < 1 )
			{
				// Delete Object
				if ( mObject != nullptr )
					delete mObject;

				// Delete counter
				delete mCounter;
			}


		}

		// ===========================================================
		// Methods & Operators
		// ===========================================================

		T *const operator*( ) noexcept
		{ return( mObject ); }

		const counter_t & count( ) const noexcept
		{ return( *mCounter ); }

		// -------------------------------------------------------- \\

	};

}

// Mark as Declared, in cases of Forward Declaration
#define _C0DE4UN_FAST_PTR_DECL_

#endif // !_C0DE4UN_FAST_PTR_HXX_