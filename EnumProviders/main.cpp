#include <iostream>
#include <memory>
#include <map>

#include <windows.h>
#include <wincrypt.h>

#pragma comment(lib, "advapi32.lib")



std::ostream& PRINT_FUNC(LPCSTR pszData)
{
	return std::cout << pszData;
}

std::wostream& PRINT_FUNC(LPCWSTR pszData)
{
	return std::wcout << pszData;
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
		std::shared_ptr<TCHAR> pszName(new TCHAR[cbName / sizeof(TCHAR)]);

		if (CryptEnumProviders(dwIndex++,
			NULL,
			0,
			&dwType,
			pszName.get(),
			&cbName))
		{
			PRINT_FUNC(pszName.get()) << std::endl;
		}
		else
		{
			std::wcout << "CryptEnumProviders failed" << std::endl;
			exit(1);
		}
	}
}

using PtrType = std::shared_ptr<TCHAR>;
using TypesDictionary = std::map<PtrType, DWORD>;

TypesDictionary EnumProviderTypes(void)
{
	TypesDictionary installedTypes;

	DWORD dwIndex = 0, dwProvType = 0, cbName = 0;
	while (CryptEnumProviderTypes(
		dwIndex,
		NULL,			// Reserved
		0,				// Reserved
		&dwProvType,
		NULL,			// Get name length in bytes
		&cbName))
	{
		std::shared_ptr<TCHAR> pszName(new TCHAR[cbName / sizeof(TCHAR)]);

		if (CryptEnumProviderTypes(dwIndex++,
			NULL,
			0,
			&dwProvType,
			pszName.get(),
			&cbName))
		{
			installedTypes.insert({ pszName, dwProvType });
		}
		else
		{
			std::wcout << "CryptEnumProviderTypes failed" << std::endl;
			exit(1);
		}
	}
	return installedTypes;
}

int main()
{
	std::wcout << "Enumerating prov names:" << std::endl;
	EnumProviderNames();

	std::wcout << std::endl;
	std::wcout << "Enumerating prov types" << std::endl;
	auto provTypes = EnumProviderTypes();
	for (const auto& type : provTypes)
	{
		PRINT_FUNC(type.first.get()) << std::endl;
	}
}