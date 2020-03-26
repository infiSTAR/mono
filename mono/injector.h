#pragma once
#include <windows.h>
#include <string>
#include <iostream>
#include <thread>
namespace injector {
	#define mono_dll L"mono-2.0-bdwgc.dll"

	typedef void MonoObject;
	typedef void MonoDomain;
	typedef void MonoAssembly;
	typedef void MonoImage;
	typedef void MonoClass;
	typedef void MonoMethod;
	typedef void MonoImageOpenStatus;

	void find_mono_funcs(HMODULE mono_module);
	void inject();
}