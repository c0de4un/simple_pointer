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

// Mark as Declared, in cases of Forward Declaration
#define _C0DE4UN_FAST_PTR_DECL_

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
	 *
	 * @version 0.1.7
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
		T * mObject;

		/* Shared between Pointers Instances Counter */
		counter_t * mCounter;

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
		explicit fast_ptr( T *const pObject = nullptr ) noexcept
			: mObject( pObject ),
			mCounter( pObject != nullptr ? new counter_t( 1 ) : nullptr )
		{
		}

		/*
		 * fast_ptr const copy constructor
		*/
		fast_ptr( const fast_ptr<T> & pOther ) noexcept
			: mObject( pOther.mObject ),
			mCounter( pOther.mCounter )
		{

			// Increase Pointers-Instances Counter
			( *mCounter )++;

		}

		/*
		 * fast_ptr const copy operator
		*/
		fast_ptr<T> & operator=( const fast_ptr<T> & pOther ) noexcept
		{

			// Cancel if self-copy
			if ( this == &pOther )
				return( *this );

			// Copy values
			mObject = pOther.mObject;
			mCounter = pOther.mCounter;

			// Increase Pointers-Instances Counter
			( *mCounter )++;

			// Return
			return( *this );

		}

		/* fast_ptr move constructor */
		fast_ptr( fast_ptr && pOther )
			: mObject( nullptr ),
			mCounter( nullptr )
		{

			// Copy values
			mObject = pOther.mObject;
			mCounter = pOther.mCounter;

			// Reset moved
			pOther.mObject = nullptr;
			pOther.mCounter = nullptr;

		}

		/* @deleted fast_ptr move assignment operator */
		fast_ptr & operator=( fast_ptr && pOther )
		{

			// Cancel if self-copy
			if ( this == &pOther )
				return( *this );

			// Copy values
			mObject = pOther.mObject;
			mCounter = pOther.mCounter;

			// Reset moved
			pOther.mObject = nullptr;
			pOther.mCounter = nullptr;

			// Return
			return( *this );

		}

		/* fast_ptr Destructor */
		~fast_ptr( ) noexcept
		{

			// Decrease counter
			if ( mCounter != nullptr )
			{

				// Get reference to the counter
				counter_t & counter_lr = *mCounter;

				// Decrease Pointers Instances Counter
				counter_lr--;

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

		}

		// ===========================================================
		// Methods & Operators
		// ===========================================================

		/* Assign (set) object to store. */
		void operator=( T *const pObject ) noexcept
		{

			// Cancel
			if ( mObject == pObject || pObject == nullptr )
				return;

			// Set pointer-value
			mObject = pObject;

			// Update instances counter
			if ( *mCounter > 1 )
			{// Other 'smart-pointers' stores previous object
				// New instances counter for new Object
				mCounter = new counter_t( 1 );
			}

		}

		///* Assignment (set, share) object from other pointer-instance */
		//void operator=( const fast_ptr<T> pOther ) noexcept
		//{

		//	// Cancel
		//	if ( this == &pOther || pOther.mObject == nullptr || mObject == pOther.mObject )
		//		return;

		//	// Delete previous object
		//	if ( *mCounter < 2 )
		//	{

		//		// Delete object
		//		delete mObject;

		//		// Delete instances counter
		//		delete mCounter;

		//	}
		//	else // Decrease previous object instances counter
		//		( *mCounter )--;

		//	// Set new Object (pointer-value)
		//	mObject = pOther.mObject;

		//	// Copy instances-counter pointer-value (address)
		//	mCounter = pOther.mCounter;

		//	// Increase instances counter for new object
		//	( *mCounter )++;

		//}

		/* Returns 'reference'. (!) Don't call on null-value. */
		T & getRef( ) noexcept
		{ return( *mObject ); }

		/* Returns 'raw-pointer' */
		T *const getPtr( ) noexcept
		{ return( mObject ); }

		/* Returns 'raw-pointer' to the object instance, can be null. Same as #getReference. */
		T *const operator*( ) noexcept
		{ return( mObject ); }

		/* Returns number of pointer-'instances' */
		const counter_t & count( ) const noexcept
		{ return( *mCounter ); }

		/* Returns true if 'pointer' is nullptr */
		const bool operator==( nullptr_t ) const noexcept
		{ return( mObject == nullptr ); }

		/* Returns true if 'pointer' is not nullptr */
		const bool operator!=( nullptr_t ) const noexcept
		{ return( mObject != nullptr ); }

		/* Pointer address access operator */
		T *const operator->( ) noexcept
		{ return( mObject ); }

		/* Returns true if this instance stores same object as given one. */
		const bool operator==( const fast_ptr<T> & pOther ) const noexcept
		{ return( mObject == pOther.mObject ); }

		/* Returns true if given object instance is the same as the stored one. */
		const bool operator==( T *const pObject ) const noexcept
		{ return( mObject == pObject ); }

		// -------------------------------------------------------- \\

	};

}

#endif // !_C0DE4UN_FAST_PTR_HXX_