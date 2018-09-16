/*
 * Copyright © 2018 Denis Zyamaev. Email: (code4un@yandex.ru)
 * License: MIT (see "LICENSE" file)
 * Author: Denis Zyamaev (code4un@yandex.ru)
 * API: C++ 11
*/

// Pragma
#pragma once

#ifndef _C0DE4UN_SIMPLE_POINTER_HPP_
#define _C0DE4UN_SIMPLE_POINTER_HPP_

#ifdef _MULTITHREADING_ENABLED_ // Multithreading Mode

// Include std::atomic
#include <atomic>

// Include std::mutex
#include <mutex>

#else // Single (One) Thread Mode
#endif // !_MULTITHREADING_ENABLED_

// Include std::vector
#include <vector>

namespace c0de4un
{

	/*
	 * SimplePointer - 'smart pointer' alternative to STL shared pointer (std::shared_ptr).
	 *
	 * @features:
	 * - optionally allows to avoid using atomic-counter ;
	 * - configurable counter-type (int, char, short, etc) ;
	 * - additional built-in method to search & copy pointer, if given object already 'shared' ;
	 * - synchronized (thread-lock) access to instances of pointers & types (if multi-threading enabled), to
	 * avoid 'data-races' when acquiring pointer (copy) ;
	 *
	 * @usage:
	 * - try find pointer for already stored object with #search ;
	 * - get pointer to new object (same as std::make_shared) with #create ;
	 * - release (delete) all pointers & object with #release ;
	 *
	 * @notes:
	 * - to reduce binary size, version without template can be used, but it will increase
	 * CPU usage (same thread-lock called for all types of objects) ;
	 * - using single (one) thread mode greatly increase speed (no atomics, no thread-locks) ;
	*/
	template <typename T>
	class SimplePointer final
	{// TODO SimplePointer

	public:

		// -------------------------------------------------------- \\

		// ===========================================================
		// Configs
		// ===========================================================

		/* Numeric-type to count instances of pointers */
#ifndef _MULTITHREADING_ENABLED_
		using counter_t = unsigned char;
#else // _MULTITHREADING_ENABLED_
		using counter_t = std::atomic<unsigned char>;
#endif // !_MULTITHREADING_ENABLED_

		// ===========================================================
		// Constructors & Destructor
		// ===========================================================

		/* SimplePointer Destructor */
		~SimplePointer( ) noexcept
		{
			// 
		}

		// ===========================================================
		// Getter & Setter
		// ===========================================================

		/*
		 * 
		*/
		SimplePointer<T> search( T *const pInstance )
		{
			//
		}

		/*
		 * Tries to count pointers for given instance of object.
		 *
		 * (?) For better performance, use other #count method
		*/
		static counter_t & count( T *const pInstance ) noexcept
		{// TODO SimplePointer::count
			//
		}

		/*
		 * Tries to count pointers.
		 *
		 * @thread_safety - synchronization used (tread-lock), may cause pause (wait) if
		 * there are a lot of other pointers.
		 * @return - amount (number, count) of pointers shared same object (item) instance.
		*/
		counter_t & count( ) const noexcept
		{// TODO
			//
		}

		// ===========================================================
		// Methods
		// ===========================================================

		/*
		 * Move arguments
		*/
		template <typename C, typename... Args>
		static SimplePointer<C> create( Args&&... pArgs )
		{
			// 
			//T *const instance_lp = new T( pArgs );
		}

		/*
		 * Coping arguments
		*/
		template <typename C, typename... Args>
		static SimplePointer<C> create( Args... pArgs )
		{
			// 
			//T *const instance_lp = new T( pArgs );
		}

		/*
		 * Tries to create copy (not clone) of given pointer.
		 *
		 * @thread_safety - synchronization (thread-lock) used for accessing
		 * pointers-collection.
		 * @param pInstance - pointer instance to create copy from.
		 * @return - pointer. Instances counter increased.
		*/
		static SimplePointer<T> copy( const SimplePointer<T> pInstance )
		{// TODO SimplePointer::copy

		}

		// -------------------------------------------------------- \\

	private:

		// -------------------------------------------------------- \\

		// ===========================================================
		// Configs
		// ===========================================================

		/* Numeric-type for Object (stored instance) ID */
		using item_id_t = unsigned char;

		/* Numeric type-alias for pointer instance ID (instance of pointer) */
		using pointer_id_t = unsigned char;

		// ===========================================================
		// Constants
		// ===========================================================



		// ===========================================================
		// Fields
		// ===========================================================

		/*
		 * Container (vector, collection) for pointers.
		*/
		static std::vector<SimplePointer<T>> pointers;

		/*
		 * ID for item (instance of object shared between pointers).
		 *
		 * Unique for each pointer, but same for instance of object.
		 * Used for faster add/remove (insert/erase) of pointers.
		 *
		*/
		const item_id_t itemID;

		/*
		 * Pointer-ID. Used for faster pointer removal, when 
		 * its destroyed (released).
		 * Unique per each pointer.
		*/
		const pointer_id_t pointerID;

		// ===========================================================
		// Constructors & Destructor
		// ===========================================================

		/*
		 * SimplePointer Constructor
		 *
		 * @throws - may throw allocation-exception.
		*/
		explicit SimplePointer( )
		{
		}

		/* @deleted SimplePointer const copy constructor */
		explicit SimplePointer( const SimplePointer & ) noexcept = delete;

		/* @deleted SimplePointer const copy '=' operator */
		SimplePointer operator=( const SimplePointer & ) noexcept = delete;

		/* @deleted SimplePointer move constructor */
		explicit SimplePointer( SimplePointer && ) noexcept = delete;

		/* @deleted SimplePointer move '= 'operator */
		SimplePointer operator=( SimplePointer && ) noexcept = delete;

		// -------------------------------------------------------- \\

	};

}

// Mark as Declared, in cases of Forward Declaration
#define _C0DE4UN_SIMPLE_POINTER_DECL_

#endif // !_C0DE4UN_SIMPLE_POINTER_HPP_