/*
 * Copyright © 2018 Denis Zyamaev. Email: (code4un@yandex.ru)
 * License: MIT (see "LICENSE" file)
 * Author: Denis Zyamaev (code4un@yandex.ru)
 * API: C++ 11
*/

// Pragma
#pragma once

#ifndef _C0DE4UN_RELATIVE_PTR_HXX_
#define _C0DE4UN_RELATIVE_PTR_HXX_

#ifdef _C0DE4UN_MULTITHREADING_ENABLED_ // Multithreading Mode
// Include std::atomic
#include <atomic>

// Include std::mutex
#include <mutex>

#endif // !_C0DE4UN_MULTITHREADING_ENABLED_

// Include std::map
#include <map>

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

	/* Pointer-Data, unique for each stored object instance, shared between pointers of such object */
	template <typename T>
	struct PointerData
	{

		/* Counter of pointers instances */
		counter_t mCounter;

		/* Stored object instance */
		T * mObject;

		/* PointerData Constructor */
		explicit PointerData( )
			: mCounter( 0 ),
			mObject( nullptr )
		{

		}

		/* PointerData Destructor */
		~PointerData( ) noexcept
		{
		}

	};

	/* Cache for all instances of relative_ptr */
	template <typename T>
	struct Cache
	{

		/* Pointers instances data */
		std::map<T*, PointerData<T>> mPointersData;

		/* Mutex to synchronize access to pointers-data */
		std::mutex mPointersMutex;

		/* Cache Constructor */
		explicit Cache( ) noexcept
			: mPointersData( ),
			mPointersMutex( )
		{

		}

		/* Cache Destructor */
		~Cache( ) noexcept
		{

		}

	};

	// -------------------------------------------------------- \\

	/*
	 * relative_ptr - version of shared-pointer with ability to retrieve & copy using
	 * previously cached (stored) object instance (item) with same address.
	*/
	template <typename T>
	class relative_ptr final
	{

	private:

		// -------------------------------------------------------- \\

		// ===========================================================
		// Types
		// ===========================================================



		// ===========================================================
		// Fields
		// ===========================================================

		/* Pointers Cache */
		static Cache<T> mCache;

		/* 'raw-pointer' to shared between all pointers, of the same object instance, pointer-data */
		PointerData<T> & mData;

		// ===========================================================
		// Getter & Setter
		// ===========================================================

		/* Search pointer-info for given object instance */
		static PointerData<T> & getInfo( T *const pObject ) noexcept
		{

#ifdef _C0DE4UN_MULTITHREADING_ENABLED_
			// Lock
			std::unique_lock<std::mutex> pointersLock_( mCache.mPointersMutex );
#endif // _C0DE4UN_MULTITHREADING_ENABLED_

			// Return
			return( mCache.mPointersData[pObject] );

		}

		// ===========================================================
		// Methods
		// ===========================================================

		/* Removes pointers data from cache */
		static void removeInfo( PointerData<T> & pData ) noexcept
		{

#ifdef _C0DE4UN_MULTITHREADING_ENABLED_
			// Lock
			std::unique_lock<std::mutex> pointersLock_( mCache.mPointersMutex );
#endif // _C0DE4UN_MULTITHREADING_ENABLED_

			// Delete instance
			delete pData.mObject;

			// Erase
			mCache.mPointersData.erase( pData.mObject );

		}

		// -------------------------------------------------------- \\

	public:

		// -------------------------------------------------------- \\

		// ===========================================================
		// Constructor & Destructor
		// ===========================================================

		/*
		 * relative_ptr Constructor
		*/
		explicit relative_ptr( T *const pObject ) noexcept
			: mData( getInfo( pObject ) )
		{

			// Set Object instance to store, if not set (first call for  this instance)
			if ( mData.mObject == nullptr )
				mData.mObject = pObject;

			// Increase instances counter
			mData.mCounter++;

#ifdef DEBUG
			std::cout << "relative_ptr created, instances count=" << mData.mCounter << std::endl;
#endif // DEBUG

		}

		/* relative_ptr const copy constructor */
		explicit relative_ptr( const relative_ptr & pOther ) noexcept
			: mData( pOther.mData )
		{

			// Increase instances counter
			mData.mCounter++;

#ifdef DEBUG
			std::cout << "relative_ptr copied, instances count=" << mData.mCounter << std::endl;
#endif // DEBUG

		}

		/* relative_ptr const copy operator */
		relative_ptr operator=( const relative_ptr & pOther ) noexcept
		{

			// Cancel if self-copy
			if ( this == &pOther )
				return( *this );

			// Set data
			mData = pOther.mData;

			// Increase instances counter
			mData.mCounter++;

		}

		/* relative_ptr Destructor */
		~relative_ptr( ) noexcept
		{

			// Decrease instances counter
			mData.mCounter--;

#ifdef DEBUG
			std::cout << "relative_ptr deleted, instances counter=" << mData.mCounter << std::endl;
#endif // DEBUG

			// Last instance
			if ( mData.mCounter < 1 )
				removeInfo( mData );

		}

		// -------------------------------------------------------- \\

	};

	template <typename T>
	Cache<T> relative_ptr<T>::mCache;

}

#endif // !_C0DE4UN_RELATIVE_PTR_HXX_