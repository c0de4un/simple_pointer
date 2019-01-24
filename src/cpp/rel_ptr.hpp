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
	 * rel_ptr_data - structure to store shared between 'smart-pointers' data.
	*/
	template <typename T>
	struct rel_ptr_data final
	{

		// -------------------------------------------------------- \\

		// ===========================================================
		// Fields
		// ===========================================================

		/* Instances counters */
		std::atomic<unsigned int> mCounter;

		/* Stored Object Instance */
		T * mObject;

		// ===========================================================
		// Constructor & destructor
		// ===========================================================

		/* rel_ptr_data default constructor */
		rel_ptr_data( )
			: mCounter( 0 ),
			mObject( nullptr )
		{

			// Print Log
			std::cout << "rel_ptr_data::constructor" << std::endl;

		}

		/* rel_ptr_data destructor */
		~rel_ptr_data( )
		{

			// Print Log
			std::cout << "rel_ptr_data::destructor" << std::endl;

		}

		// ===========================================================
		// Deleted
		// ===========================================================

		/* @deleted rel_ptr_data const copy constructor */
		rel_ptr_data( const rel_ptr_data & ) = delete;

		/* @deleted rel_ptr_data const copy assignment operator */
		rel_ptr_data operator=( const rel_ptr_data & ) = delete;

		/* @deleted rel_ptr_data move constructor */
		rel_ptr_data( rel_ptr_data && ) = delete;

		/* @deleted rel_ptr_data move assignment operator */
		rel_ptr_data & operator=( rel_ptr_data && ) = delete;

		// -------------------------------------------------------- \\

	};

	/*
	 * rel_ptr_cache - stores rel_ptr_data instances (cache, pool).
	*/
	template <typename T>
	struct rel_ptr_cache final
	{

		// -------------------------------------------------------- \\

		// ===========================================================
		// Fields
		// ===========================================================

		/* Pointers instances */
		std::map<T const*, rel_ptr_data<T>> mPointersData;

		/* Mutex */
		std::mutex mMutex;

		/* unique_lock */
		std::unique_lock<std::mutex> mLock;

		// ===========================================================
		// Constructor & destructor
		// ===========================================================

		/* rel_ptr_cache default constructor */
		rel_ptr_cache( )
			: mPointersData( ),
			mMutex( ),
			mLock( mMutex, std::defer_lock )
		{

			// Print Log
			std::cout << "rel_ptr_cache::constructor" << std::endl;

		}

		/* rel_ptr_cache destructor */
		~rel_ptr_cache( )
		{

			// Print Log
			std::cout << "rel_ptr_cache::destructor" << std::endl;

		}

		// ===========================================================
		// Deleted
		// ===========================================================

		/* @deleted rel_ptr_cache const copy constructor */
		rel_ptr_cache( const rel_ptr_cache & ) = delete;

		/* @deleted rel_ptr_cache const copy assignment operator */
		rel_ptr_cache operator=( const rel_ptr_cache & ) = delete;

		/* @deleted rel_ptr_cache move constructor */
		rel_ptr_cache( rel_ptr_cache && ) = delete;

		/* @deleted rel_ptr_cache move assignment operator */
		rel_ptr_cache & operator=( rel_ptr_cache && ) = delete;

		// -------------------------------------------------------- \\

	};

// Restore structure-data alignment to default (8-byte on MSVC)
#pragma pack( pop )

	// ===========================================================
	// Fields
	// ===========================================================



	// ===========================================================
	// Types
	// ===========================================================

// Enable structure-data (fields, variables) alignment (by compilator) to 1 byte
#pragma pack( push, 1 )

	/*
	 * rel_ptr - custom 'relative-pointer', as an alternative to the STL 'shared_ptr'.
	 * 
	 * 
	 * 
	 * @version 0.0.1
	*/
	template <typename T>
	class rel_ptr final
	{

	private:

		// -------------------------------------------------------- \\

		// ===========================================================
		// Fields
		// ===========================================================

		/* Cache */
		static rel_ptr_cache<T> mCache;

		/* Data */
		rel_ptr_data<T> * mData;

		// ===========================================================
		// Getter & Setter
		// ===========================================================

		/*
		 * Allocate or search 'rel_ptr' data.
		 * 
		 * @thread_safety - thread-lock used.
		 * @return - rel_ptr_data.
		*/
		static rel_ptr_data<T> *const getData( T *const pObject )
		{

			// Print Log
			std::cout << "rel_ptr::getData" << std::endl;

			// Cancel
			if ( pObject == nullptr )
				return( nullptr );

			// Lock
			mCache.mLock.lock( );

			// Data
			rel_ptr_data<T> *const result_lr = &mCache.mPointersData[pObject];

			// 
			if ( result_lr->mObject == nullptr )
				result_lr->mObject = pObject;

			// Increase instances counter
			result_lr->mCounter++;

			// Unlock
			mCache.mLock.unlock( );

			// Return result
			return( result_lr );

		}

		// ===========================================================
		// Methods
		// ===========================================================

		/*
		 * Removes 'rel_ptr' data & delete Object.
		 * 
		 * @thread_Safety - thread-lock used.
		*/
		static void removeData( T *const pObject )
		{

			// Print Log
			std::cout << "rel_ptr::removeData" << std::endl;

			// Cancel
			if ( pObject == nullptr )
				return;

			// Lock
			mCache.mLock.lock( );

			// Search
			std::map<T const*, rel_ptr_data<T>>::const_iterator dataIterator_ = mCache.mPointersData.find( pObject );

			if ( dataIterator_ != mCache.mPointersData.cend( ) )
			{

				// Remove Data
				mCache.mPointersData.erase( pObject );

				// Delete Object
				delete pObject;

			}

			// Unlock
			mCache.mLock.unlock( );

		}

		// -------------------------------------------------------- \\

	public:

		// -------------------------------------------------------- \\

		// ===========================================================
		// Constructors
		// ===========================================================

		/*
		 * rel_ptr constructor
		 * 
		 * 
		*/
		rel_ptr( T *const pObject )
			: mData( getData( pObject ) )
		{

			// Print Log
			std::cout << "rel_ptr::constructor" << std::endl;

		}

		/*
		 * rel_ptr copy constructor
		 * 
		 * 
		*/
		rel_ptr( rel_ptr & pOther )
			: mData( nullptr )
		{

			// Print Log
			std::cout << "rel_ptr::copy-constructor" << std::endl;

			mData = getData( pOther.mData->mObject );

		}

		/*
		 * rel_ptr move constructor.
		 * 
		 * 
		*/
		rel_ptr( rel_ptr && pOther )
			: mData( nullptr )
		{

			// Print Log
			std::cout << "rel_ptr::move-constructor" << std::endl;

			// Set Data
			mData = pOther.mData;

			// Reset
			pOther.mData = nullptr;

		}

		/* rel_ptr destructor */
		~rel_ptr( )
		{

			// Print Log
			std::cout << "rel_ptr::destructor" << std::endl;

			// Manage Data
			if ( mData != nullptr )
			{

				// Decrease instances counter
				if ( mData->mCounter > 1 )
					mData->mCounter--;
				else // Remove Data & Release Object
					removeData( mData->mObject );

			}

		}

		// ===========================================================
		// Getter & Setter
		// ===========================================================

		// ===========================================================
		// Methods & Operators
		// ===========================================================

		/* rel_ptr copy assignment operator */
		rel_ptr & operator=( rel_ptr & pOther )
		{

			// Print Log
			std::cout << "rel_ptr::copy-assignment operator" << std::endl;

			// Cancel
			if ( pOther == *this )
				return( *this );

			// Set Data
			mData = getData( pOther.mData->mObject );

		}

		/* rel_ptr move assignment operator */
		rel_ptr & operator=( rel_ptr && pOther )
		{

			// Print Log
			std::cout << "rel_ptr::move-assignment operator" << std::endl;

			// Cancel
			if ( pOther == *this )
				return( *this );

			// Set Data
			mData = pOther.mData;

			// Reset
			pOther.mData = nullptr;

		}

		T & getRef( )
		{ return( *mData->mObject ); }

		T *const get( )
		{ return( mData != nullptr ? mData->mObject : nullptr ); }

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
		const bool operator==( const rel_ptr<T> & pOther ) const noexcept
		{ return( mData == pOther.mData ); }

		/* Returns true if given object instance is the same as the stored one. */
		const bool operator==( T *const pObject ) const noexcept
		{ return( mData != nullptr ? pObject == mData->mObject : pObject == nullptr ); }

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

		// -------------------------------------------------------- \\

	};

// Restore structure-data alignment to default (8-byte on MSVC)
#pragma pack( pop )

	// ===========================================================
	// Fields
	// ===========================================================

	/* Initialize variable value */
	template <typename T>
	rel_ptr_cache<T> rel_ptr<T>::mCache;

	// -------------------------------------------------------- \\

} // namespace c0de4un