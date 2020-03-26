#include "injector.h"

typedef void(__cdecl* t_mono_thread_attach)(injector::MonoDomain*);
t_mono_thread_attach fnThreadAttach;
typedef  injector::MonoDomain* (__cdecl* t_mono_get_root_domain)(void);
t_mono_get_root_domain fnGetRootDomain;
typedef injector::MonoAssembly* (__cdecl* t_mono_assembly_open)(const char*, injector::MonoImageOpenStatus*);
t_mono_assembly_open fnAssemblyOpen;
typedef injector::MonoImage* (__cdecl* t_mono_assembly_get_image)(injector::MonoAssembly*);
t_mono_assembly_get_image fnAssemblyGetImage;
typedef injector::MonoClass* (__cdecl* t_mono_class_from_name)(injector::MonoImage*, const char*, const char*);
t_mono_class_from_name fnClassFromName;
typedef injector::MonoMethod* (__cdecl* t_mono_class_get_method_from_name)(injector::MonoClass*, const char*, int);
t_mono_class_get_method_from_name fnMethodFromName;
typedef injector::MonoObject* (__cdecl* t_mono_runtime_invoke)(injector::MonoMethod*, void*, void**, injector::MonoObject**);
t_mono_runtime_invoke fnRuntimeInvoke;


void injector::find_mono_funcs(HMODULE mono_module) {
	fnThreadAttach = (t_mono_thread_attach)GetProcAddress(mono_module, "mono_thread_attach");
	fnGetRootDomain = (t_mono_get_root_domain)GetProcAddress(mono_module, "mono_get_root_domain");
	fnAssemblyOpen = (t_mono_assembly_open)GetProcAddress(mono_module, "mono_assembly_open");
	fnAssemblyGetImage = (t_mono_assembly_get_image)GetProcAddress(mono_module, "mono_assembly_get_image");
	fnClassFromName = (t_mono_class_from_name)GetProcAddress(mono_module, "mono_class_from_name");
	fnMethodFromName = (t_mono_class_get_method_from_name)GetProcAddress(mono_module, "mono_class_get_method_from_name");
	fnRuntimeInvoke = (t_mono_runtime_invoke)GetProcAddress(mono_module, "mono_runtime_invoke");
}

void injector::inject( ) {
	std::string assemblyDir;
	HMODULE mono;
	MonoDomain* domain;
	MonoAssembly* assembly;
	MonoImage* image;
	MonoClass* klass;
	MonoMethod* method;

	mono = LoadLibraryW(mono_dll);
	find_mono_funcs(mono);

	printf("[+] mono dll found at %ls\n\n", mono_dll);
	printf("[~] mono_thread_attach: 0x%p\n", fnThreadAttach);
	printf("[~] mono_get_root_domain: 0x%p\n", fnGetRootDomain);
	printf("[~] mono_assembly_open: 0x%p\n", fnAssemblyOpen);
	printf("[~] mono_assembly_get_image: 0x%p\n", fnAssemblyGetImage);
	printf("[~] mono_class_from_name: 0x%p\n", fnClassFromName);
	printf("[~] mono_class_get_method_from_name: 0x%p\n", fnMethodFromName);
	printf("[~] mono_runtime_invoke: 0x%p\n", fnRuntimeInvoke);

	domain = fnGetRootDomain();
	fnThreadAttach(domain);

	assemblyDir.append("C:/Windows/mono_dlls/");
	printf("[~] filename: ");
	std::string filename;
	std::cin >> filename;
	assemblyDir.append(filename);

	assembly = fnAssemblyOpen(assemblyDir.c_str(), NULL);
	image = fnAssemblyGetImage(assembly);
	printf("[~] namespace: ");
	std::string ns;
	std::cin >> ns;
	printf("[~] class: ");
	std::string cls;
	std::cin >> cls;
	klass = fnClassFromName(image, ns.c_str(), cls.c_str());
	printf("[~] method: ");
	std::string method_str;
	std::cin >> method_str;
	method = fnMethodFromName(klass, method_str.c_str(), 0);
	system("cls");

	fnRuntimeInvoke(method, NULL, NULL, NULL);

	printf("[+] attached to mono thread\n");
	printf("[~] assembly dir: %s\n", assemblyDir.c_str());
	printf("[~] loaded namespace: %hs\nloaded class: %hs\n", ns.c_str(), cls.c_str());
	printf("[~] loaded method: %hs\n", method_str.c_str());
	printf("[~] injection complete.\n");
}