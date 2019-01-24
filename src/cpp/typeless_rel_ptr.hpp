/*
* Copyright © 2018 Denis Zyamaev (code4un@yandex.ru) All rights reserved.
* Authors: Denis Zyamaev (code4un@yandex.ru)
* All rights reserved.
* API: C++ 11
* License: see LICENSE.txt
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
* 1. Redistributions of source code must retain the above copyright notice,
* this list of conditions and the following disclaimer.
* 2. Redistributions in binary form must display the names 'Denis Zyamaev' and
* in the credits of the application, if such credits exist.
* The authors of this work must be notified via email (code4un@yandex.ru) in
* this case of redistribution.
* 3. Neither the name of copyright holders nor the names of its contributors
* may be used to endorse or promote products derived from this software
* without specific prior written permission.
*
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS ``AS
* IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
* THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
* PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL COPYRIGHT HOLDERS OR CONTRIBUTORS
* BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
* CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
* SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
* INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
* CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
* ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
* POSSIBILITY OF SUCH DAMAGE.
*/

#pragma once

// Include STL atomic
#include <atomic> // std::atomic

// Include STL mutex
#include <mutex> // std::mutex, std::unique_ptr

// Include STL map
#include <map> // std::map

// Include iostream
#include <iostream> // std::cout, std::cin:: std::endl

// Include stdlib
#include <cstdlib>

namespace c0de4un
{

	// -------------------------------------------------------- \\

	// ===========================================================
	// Types
	// ===========================================================

// Enable structure-data (fields, variables) alignment (by compilator) to 1 byte
#pragma pack( push, 1 )

	/*
	 * typeless_rel_ptr_data - type-independent structure to store shared between 'smart-pointers' data.
	*/
	struct typeless_rel_ptr_data final
	{

		// -------------------------------------------------------- \\

		// ===========================================================
		// Fields
		// ===========================================================

		/* Instances counters */
		std::atomic<unsigned int> mCounter;

		/* Stored Object Instance */
		void * mObject;

		// ===========================================================
		// Constructor & destructor
		// ===========================================================

		/* typeless_rel_ptr_data default constructor */
		typeless_rel_ptr_data( )
			: mCounter( 0 ),
			mObject( nullptr )
		{

			// Print Log
			std::cout << "typeless_rel_ptr_data::constructor" << std::endl;

		}

		/* typeless_rel_ptr_data destructor */
		~typeless_rel_ptr_data( )
		{

			// Print Log
			std::cout << "typeless_rel_ptr_data::destructor" << std::endl;

		}

		// ===========================================================
		// Deleted
		// ===========================================================

		/* @deleted typeless_rel_ptr_data const copy constructor */
		typeless_rel_ptr_data( const typeless_rel_ptr_data & ) = delete;

		/* @deleted typeless_rel_ptr_data const copy assignment operator */
		typeless_rel_ptr_data operator=( const typeless_rel_ptr_data & ) = delete;

		/* @deleted typeless_rel_ptr_data move constructor */
		typeless_rel_ptr_data( typeless_rel_ptr_data && ) = delete;

		/* @deleted typeless_rel_ptr_data move assignment operator */
		typeless_rel_ptr_data & operator=( typeless_rel_ptr_data && ) = delete;

		// -------------------------------------------------------- \\

	};

	/*
	 * typeless_rel_ptr_cache - stores rel_ptr_data instances (cache, pool).
	*/
	struct typeless_rel_ptr_cache final
	{

		// -------------------------------------------------------- \\

		// ===========================================================
		// Fields
		// ===========================================================

		/* Pointers instances */
		std::map<void const*, typeless_rel_ptr_data> mPointersData;

		/* Mutex */
		std::mutex mMutex;

		/* unique_lock */
		std::unique_lock<std::mutex> mLock;

		// ===========================================================
		// Constructor & destructor
		// ===========================================================

		/* typeless_rel_ptr_cache default constructor */
		typeless_rel_ptr_cache( )
			: mPointersData( ),
			mMutex( ),
			mLock( mMutex, std::defer_lock )
		{

			// Print Log
			std::cout << "typeless_rel_ptr_cache::constructor" << std::endl;

		}

		/* typeless_rel_ptr_cache destructor */
		~typeless_rel_ptr_cache( )
		{

			// Print Log
			std::cout << "typeless_rel_ptr_cache::destructor" << std::endl;

		}

		// ===========================================================
		// Deleted
		// ===========================================================

		/* @deleted typeless_rel_ptr_cache const copy constructor */
		typeless_rel_ptr_cache( const typeless_rel_ptr_cache & ) = delete;

		/* @deleted typeless_rel_ptr_cache const copy assignment operator */
		typeless_rel_ptr_cache operator=( const typeless_rel_ptr_cache & ) = delete;

		/* @deleted typeless_rel_ptr_cache move constructor */
		typeless_rel_ptr_cache( typeless_rel_ptr_cache && ) = delete;

		/* @deleted typeless_rel_ptr_cache move assignment operator */
		typeless_rel_ptr_cache & operator=( typeless_rel_ptr_cache && ) = delete;

		// -------------------------------------------------------- \\

	};

// Restore structure-data alignment to default (8-byte on MSVC)
#pragma pack( pop )

	// -------------------------------------------------------- \\

	// ===========================================================
	// Fields
	// ===========================================================

	/* Cache */
	static typeless_rel_ptr_cache mCache;

	// ===========================================================
	// Getter & Setter
	// ===========================================================

	/*
	 * Search for 'relative pointer' data for specific Object.
	 *
	 * (?) Does nothing, if Object is null.
	 *
	 * @thread_safety - thread-safe, synchronization (thread-lock) used.
	 * @param pObject - 'raw-pointer' to a Object.
	 * @return - data for 'shared-pointer', or null.
	 * @throws - can throw exception:
	 * - mutex ;
	 * - null (access-violation, etc) ;
	*/
	static typeless_rel_ptr_data *const getData( void *const pObject )
	{

		// Cancel
		if ( pObject == nullptr )
			return( nullptr );

		// Print DEBUG to a Console
		std::cout << "trel_ptr::getData - address=" << pObject << std::endl;

		// Lock Cache
		mCache.mLock.lock( );

		// Get Data using Object-address as key
		typeless_rel_ptr_data * result_lp( &mCache.mPointersData[pObject] );

		// Set Data's Object 'raw-pointer' value
		if ( result_lp->mObject == nullptr )
			result_lp->mObject = pObject; // Copy address

		// Increase 'pointers' counter
		result_lp->mCounter++;

		// Unlock Cache
		mCache.mLock.unlock( );

		// Return result
		return( result_lp );

	}

	// ===========================================================
	// Methods
	// ===========================================================

	/*
	 * Removes Data associated with the given Object.
	 *
	 * @thread_safety - thread-safe, synchronization (thread-lock) used.
	 * @param pObject - 'raw-pointer' to a Object.
	 * @throws - can throw exception:
	 * - mutex ;
	 * - null (access-violation, etc) ;
	*/
	template <typename T>
	static void removeData( void *const pObject )
	{

		// Cancel
		if ( pObject == nullptr )
			return;

		// Print DEBUG to a Console
		std::cout << "trel_ptr::removeData - address=" << pObject << std::endl;

		// Lock Cache
		mCache.mLock.lock( );

		// Search
		std::map<void const*, typeless_rel_ptr_data>::const_iterator dataPos = mCache.mPointersData.find( pObject );

		// Remove Data
		if ( dataPos != mCache.mPointersData.cend( ) )
		{

			// Remove Data from a map
			mCache.mPointersData.erase( dataPos );

			// Delete Object instance
			delete (T*const)pObject;

		}

		// Unlock Cache
		mCache.mLock.unlock( );

	}

	// -------------------------------------------------------- \\

	/*
	 * trel_ptr - type-independent 'relative-pointer'. An alternative (simple)
	 * to STL 'shared_ptr'.
	 * 
	 * @version 0.0.1
	*/
	template <typename T>
	class trel_ptr final
	{

	private:

		// -------------------------------------------------------- \\

		// ===========================================================
		// Fields
		// ===========================================================

		/* Data */
		typeless_rel_ptr_data * mData;

		// -------------------------------------------------------- \\

	public:

		// -------------------------------------------------------- \\

		// ===========================================================
		// Constructors
		// ===========================================================

		/*
		 * trel_ptr constructor
		 * 
		 * @param pObject - 'raw-pointer' to a Object to store.
		 * @throws - can throw exception (bad_alloc, mutex, etc).
		*/
		trel_ptr( T *const pObject = nullptr )
			: mData( getData( (void*const) pObject ) )
		{

			// Print Log
			std::cout << "trel_ptr::constructor, Object address=" << pObject << std::endl;

		}

		/*
		 * trel_ptr copy constructor
		 * 
		 * @param pObject - 'raw-pointer' to a Object to store.
		 * @throws - can throw exception (bad_alloc, mutex, etc).
		*/
		trel_ptr( trel_ptr & pOther )
			: mData( nullptr )
		{

			// Get Data
			mData = pOther.mData;

			// Update instances counter
			if ( mData != nullptr )
			{

				mData->mCounter++;

				// Print Log
				std::cout << "trel_ptr::copy-constructor, Object address=" << mData->mObject << std::endl;

			}
			else
				std::cout << "trel_ptr::copy-constructor, null-Object" << std::endl;

		}

		/*
		 * trel_ptr move constructor
		 * 
		 * @param pObject - 'raw-pointer' to a Object to store.
		 * @throws - can throw exception (bad_alloc, mutex, etc).
		*/
		trel_ptr( trel_ptr && pOther )
			: mData( nullptr )
		{

			// Get Data
			mData = pOther.mData;

			// Reset
			pOther.mData = nullptr;

			// Print Log
			if ( mData != nullptr )
				std::cout << "trel_ptr::move-constructor, Object address=" << mData->mObject << std::endl;

		}

		/* trel_ptr destructor */
		~trel_ptr( )
		{

			// Print Log
			if ( mData != nullptr )
				std::cout << "trel_ptr::destructor, Object address=" << mData->mObject << std::endl;
			else
				std::cout << "trel_ptr::destructor, null data" << std::endl;

			// Update Data
			if ( mData != nullptr )
			{

				// Decrease instances counter
				if ( mData->mCounter > 1 )
					mData->mCounter--;
				else // Remove Data & Release Object
					removeData<T>( mData->mObject );

			}

		}

		// ===========================================================
		// Methods & Operators
		// ===========================================================

		/*
		 * trel_ptr copy assignment operator.
		 * 
		 * (!) Does nothing if same instance called.
		 * 
		 * @thread_safety - thread-safe, atomic-counter used.
		 * @param pOther - trel_ptr instance to copy from.
		 * @return - 'this' trel_ptr.
		 * @throws - can throw exception (bad_alloc, mutex, acces-violation, etc).
		*/
		trel_ptr & operator=( trel_ptr & pOther )
		{

			// Cancel
			if ( this == &pOther )
				return( *this );

			// Print Log
			std::cout << "trel_ptr::copy-assignment operator, Object address=" << mData != nullptr ? mData->mObject : 0 << std::endl;

			// Set Data
			mData = pOther.mData;

			// Increase instances counter
			if ( mData != nullptr )
				mData->mCounter++;

		}

		/*
		 * trel_ptr move assignment operator.
		 *
		 *
		*/
		trel_ptr & operator=( trel_ptr && pOther )
		{

			// Cancel
			if ( this == &pOther )
				return( *this );

			// Print Log
			std::cout << "trel_ptr::move-assignment operator, Object address=" << mData != nullptr ? mData->mObject : 0 << std::endl;

			// Set Data
			mData = pOther.mData;

			// Reset
			pOther.mData = nullptr;

		}

		/* Returns instances counter */
		const unsigned int count( )
		{
			//Cancel
			if ( mData == nullptr )
				return( 0 );

			// Copy value
			const unsigned int result_( mData->mCounter );

			// Return result
			return( result_ );

		}

		T *const operator*( )
		{ return( mData != nullptr ? mData->mObject : nullptr ); }

		/* Returns true if 'pointer' is nullptr */
		const bool operator==( nullptr_t ) const noexcept
		{ return( mData == nullptr || mData->mObject == nullptr ); }

		/* Returns true if 'pointer' is not nullptr */
		const bool operator!=( nullptr_t ) const noexcept
		{ return( mData != nullptr || mData->mObject != nullptr ); }

		/* Pointer address access operator */
		T *const operator->( ) noexcept
		{ return( mData != nullptr ? mData->mObject : nullptr ); }

		/* Returns true if this instance stores same object as given one. */
		const bool operator==( const trel_ptr<T> & pOther ) const noexcept
		{ return( mData == pOther.mData ); }

		/* Returns true if given object instance is the same as the stored one. */
		const bool operator==( T *const pObject ) const noexcept
		{ return( mData != nullptr ? pObject == mData->mObject : pObject == nullptr ); }

		T *const get( )
		{

			//
			if ( mData == nullptr )
				return( nullptr );

			// 
			void *const object_lp( mData->mObject );

			// 
			return( (T *const)object_lp );

		}

		// -------------------------------------------------------- \\

	}; // trel_ptr

	// -------------------------------------------------------- \\

} // namespace c0de4un