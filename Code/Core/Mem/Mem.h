// Mem.h
//------------------------------------------------------------------------------
#pragma once
#ifndef CORE_MEM_MEM_H
#define CORE_MEM_MEM_H

// Includes
//------------------------------------------------------------------------------
#include "Core/Env/Types.h"
#include "Core/Mem/MemTracker.h"

// Placement new/delete
//------------------------------------------------------------------------------
#define INPLACE_NEW new
inline void * operator new( size_t, void * ptr ) { return ptr; }
inline void * operator new[]( size_t, void * ptr ) { return ptr; }
inline void operator delete( void *, void * ) {}
inline void operator delete[]( void *, void * ) {}

// new/delete
//------------------------------------------------------------------------------
#if defined( MEMTRACKER_ENABLED )
	#define FNEW( code )		new ( __FILE__, __LINE__ ) code
	#define FNEW_ARRAY( code )	new ( __FILE__, __LINE__ ) code
	#define FDELETE				delete
	#define FDELETE_ARRAY		delete[]

	#define ALLOC( ... )		::AllocFileLine( __VA_ARGS__, __FILE__, __LINE__ )
	#define FREE( ptr )			::Free( ptr )
#else
	#define FNEW( code )		new code
	#define FNEW_ARRAY( code )	new code
	#define FDELETE				delete
	#define FDELETE_ARRAY		delete[]

	#define ALLOC( ... )		::Alloc( __VA_ARGS__ )
	#define FREE( ptr )			::Free( ptr )
#endif

// Alloc/Free
//------------------------------------------------------------------------------
void * Alloc( size_t size );
void * Alloc( size_t size, size_t alignment );
void * AllocFileLine( size_t size, const char * file, int line );
void * AllocFileLine( size_t size, size_t alignment, const char * file, int line );
void Free( void * ptr );

// global new/delete
//------------------------------------------------------------------------------
#if defined( __clang__ ) && defined( __WINDOWS__ )
	// TODO: resolve issue with Clang and inline new/delete
#elif defined( __OSX__ )
	// TODO: resolve issue with Clang and inline new/delete
#else
	#if defined( MEMTRACKER_ENABLED )
		inline void * operator new( size_t size, const char * file, int line ) { return AllocFileLine( size, file, line ); }
		inline void * operator new[]( size_t size, const char * file, int line ) { return AllocFileLine( size, file, line ); }
		inline void operator delete( void * ptr, const char *, int ) { return Free( ptr ); }
		inline void operator delete[]( void * ptr, const char *, int ) { return Free( ptr ); }
	#endif
	inline void * operator new( size_t size ) { return Alloc( size ); }
	inline void * operator new[]( size_t size ) { return Alloc( size ); }
	inline void operator delete( void * ptr ) { Free( ptr ); }
	inline void operator delete[]( void * ptr ) { Free( ptr ); }
#endif

//------------------------------------------------------------------------------
#endif // CORE_MEM_MEM_H
