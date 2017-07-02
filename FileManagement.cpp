#include "include.h"



FileManagement::FileManagement(const std::string FileName) : m_FileName(FileName)
{
	if (!ReadExistingFile())
	{
		printf("Error: 0x%X \n", GetLastError());
		system("pause");
		return;
	}

	if (!GetTextSection())
	{
		printf("Getting .text section failed\n");
		system("pause");
		return;
	}
}


FileManagement::~FileManagement()
{
	if (!VirtualFree(m_pBytes, m_dwFileSize, MEM_DECOMMIT))
	{
		printf("%s \n", "Memory Deallocation Could Not Be Performed");
		system("pause");
		return;
	}
	CloseHandle(m_hFile);
	m_pBytes = nullptr;
	m_hFile = nullptr;
}


bool FileManagement::GetTextSection()
{
	PIMAGE_DOS_HEADER pDosHeader = reinterpret_cast<PIMAGE_DOS_HEADER>(m_pBytes);
	if (pDosHeader->e_magic != IMAGE_DOS_SIGNATURE)
		return 0;
	PIMAGE_NT_HEADERS pNtHeader = reinterpret_cast<PIMAGE_NT_HEADERS>((DWORD)pDosHeader + pDosHeader->e_lfanew);
	if (pNtHeader->Signature != IMAGE_NT_SIGNATURE)
		return 0;

	PIMAGE_OPTIONAL_HEADER pOpt = reinterpret_cast<PIMAGE_OPTIONAL_HEADER>(&pNtHeader->OptionalHeader);
	PIMAGE_SECTION_HEADER sec = reinterpret_cast<PIMAGE_SECTION_HEADER>((DWORD)pOpt + pNtHeader->FileHeader.SizeOfOptionalHeader);

	for (int i = 0; i < pNtHeader->FileHeader.NumberOfSections; ++i)
	{
		PIMAGE_SECTION_HEADER pSection = sec + i;
		if (!strcmp(reinterpret_cast<const char*>(pSection->Name), ".text"))
		{
			pTextSection = pSection;
			return 1;
		}
	}
	return 0;
}

bool FileManagement::ReadExistingFile()
{
	m_hFile = CreateFile(m_FileName.c_str(), GENERIC_READ, NULL, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (m_hFile == INVALID_HANDLE_VALUE)
	{
		printf("%s \n", "No Access To File");
		return 0;
	}

	m_dwFileSize = GetFileSize(m_hFile, NULL);
	if (m_dwFileSize == INVALID_FILE_SIZE)
	{
		printf("%s \n", "Could Not Get File Size");
		CloseHandle(m_hFile);
		return 0;
	}

	m_pBytes = VirtualAlloc(NULL, m_dwFileSize, MEM_COMMIT, PAGE_READWRITE);
	if (m_pBytes == NULL)
	{
		printf("%s \n", "No Memory Could Be Allocated For The Code");
		CloseHandle(m_hFile);
		return 0;
	}

	if (!ReadFile(m_hFile, m_pBytes, m_dwFileSize, NULL, NULL))
	{
		printf("%s \n", "Could Not Read Bytes");
		CloseHandle(m_hFile);
		return 0;
	}

	return 1;
}


bool FileManagement::WriteNewFile(const std::string NewFileName, void* Bytes, const DWORD NumberOfBytesToWrite)
{
	m_hNewFile = CreateFile(NewFileName.c_str(), GENERIC_WRITE, NULL, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (m_hNewFile == INVALID_HANDLE_VALUE)
	{
		printf("%s \n", "Could Not Create New File");
		printf("%d \n", GetLastError());
		return 0;
	}

	if (!WriteFile(m_hNewFile, Bytes, NumberOfBytesToWrite, NULL, NULL))
	{
		CloseHandle(m_hNewFile);
		printf("%s \n", "Could Not Write To New File");
		printf("%d \n", GetLastError());
		return 0;
	}
	CloseHandle(m_hNewFile);
	return 1;
}


DWORD FileManagement::GetSize() const
{
	return m_dwFileSize;
}

HANDLE FileManagement::GetHandle() const
{
	return m_hFile;
}

void* FileManagement::GetBytePointer() const
{
	return m_pBytes;
}
