#include <iostream>

#include <windows.h>
#include <wincrypt.h>

#pragma comment(lib, "advapi32.lib")


template <typename T>
void PRINT_FUNC(T *pszStr)
{
#ifdef UNICODE
#  define os std::wcout
#else
#  define os std::cout
#endif

	os << pszStr << std::endl;
}

void EnumProviderNames(void)
{
	DWORD dwIndex = 0, dwType = 0, cbName = 0;
	while (CryptEnumProviders(dwIndex,	// Next prov index
		NULL,							// Reserved
		0,								// Reserved
		&dwType,						// Prov type
		NULL,							// Get prov name size in bytes
		&cbName))						// Prov name size
	{
		TCHAR *pszName = NULL;
		if (!(pszName = (PTCHAR)LocalAlloc(LMEM_ZEROINIT, cbName)))
		{
			std::cout << "LocalAlloc failed" << std::endl;
			exit(1);
		}

		if (CryptEnumProviders(dwIndex++,
			NULL,
			0,
			&dwType,
			(LPTSTR)pszName,
			&cbName))
		{
			PRINT_FUNC(pszName);
		}
		else
		{
			std::cout << "CryptEnumProviders failed" << std::endl;
			exit(1);
		}
		LocalFree(pszName);
	}
}

void EnumProviderTypes(void)
{
	DWORD dwIndex = 0, dwProvType = 0, cbName = 0;
	while (CryptEnumProviderTypes(
		dwIndex,
		NULL,			// Reserved
		0,				// Reserved
		&dwProvType,
		NULL,			// Get name length in bytes
		&cbName))
	{
		TCHAR *pszName = NULL;
		if (!(pszName = (PTCHAR)LocalAlloc(LMEM_ZEROINIT, cbName)))
		{
			std::cout << "LocalAlloc failed" << std::endl;
			exit(1);
		}

		if (CryptEnumProviderTypes(dwIndex++,
			NULL,
			0,
			&dwProvType,
			pszName,
			&cbName))
		{
			PRINT_FUNC(pszName);
		}
		else
		{
			std::cout << "CryptEnumProviderTypes failed" << std::endl;
			exit(1);
		}
		LocalFree(pszName);
	}
}

int main()
{
	std::cout << "Enumerating prov names:" << std::endl;
	EnumProviderNames();

	std::cout << std::endl;
	std::cout << "Enumerating prov types" << std::endl;
	EnumProviderTypes();
}