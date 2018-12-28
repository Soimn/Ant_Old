// Ant Engine
// Copyright 2018 Sentience Games
//
// File Description:
// Macros handling symbol exporting

#pragma once

#ifdef ANT_COMPILER_MSVC
	#define ANT_HELPER_DLL_IMPORT __declspec(dllimport)
	#define ANT_HELPER_DLL_EXPORT __declspec(dllexport)
	#define ANT_HELPER_DLL_LOCAL
#elif ANT_COMPILER_GNU || ANT_COMPILER_CLANG
	#define ANT_HELPER_DLL_IMPORT __attribute__ ((visibility ("default")))
	#define ANT_HELPER_DLL_EXPORT __attribute__ ((visibility ("default")))
	#define ANT_HELPER_DLL_LOCAL  __attribute__ ((visibility ("hidden")))
#endif

#ifdef ANT_DLL_EXPORT
	#define ANT_API ANT_HELPER_DLL_EXPORT
	#define ANT_LOCAL ANT_HELPER_LOCAL
#else
	#define ANT_API ANT_HELPER_DLL_IMPORT
	#define ANT_LOCAL 
#endif
