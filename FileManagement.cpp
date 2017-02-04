#include "include.h"
FileManagement* FManager = new FileManagement();


void FileManagement::OnSetup(const char* FileName)
{
	if (!ReadExistingFile(FileName))
	{
		printf("%d \n", GetLastError());
		CloseHandle(hFile);
		system("pause");
		return;
	}
}

FileManagement::FileManagement()
{
}


FileManagement::~FileManagement()
{
	if (!VirtualFree(pBytes, dFileSize, MEM_DECOMMIT))
	{
		printf("%s \n", "Memory Deallocation Could Not Be Performed");
		system("pause");
		return;
	}
	CloseHandle(hFile);
	pBytes = nullptr;
	hFile = nullptr;
}


bool FileManagement::ReadExistingFile(const char* FileName)
{
	hFile = CreateFile(FileName, GENERIC_READ, NULL, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		printf("%s \n", "No Access To File");
		return 0;
	}

	dFileSize = GetFileSize(hFile, NULL);
	if (dFileSize == INVALID_FILE_SIZE)
	{
		printf("%s \n", "Could Not Get File Size");
		CloseHandle(hFile);
		return 0;
	}

	pBytes = VirtualAlloc(NULL, dFileSize, MEM_COMMIT, PAGE_READWRITE);
	if (pBytes == NULL)
	{
		printf("%s \n", "No Memory Could Be Allocated For The Code");
		CloseHandle(hFile);
		return 0;
	}

	if (!ReadFile(hFile, pBytes, dFileSize, NULL, NULL))
	{
		printf("%s \n", "Could Not Read Bytes");
		CloseHandle(hFile);
		return 0;
	}

	return 1;
}


bool FileManagement::WriteNewFile(const char* NewFileName, void* Bytes, DWORD NumberOfBytesToWrite)
{
	hNewFile = CreateFile(NewFileName, GENERIC_WRITE, NULL, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hNewFile == INVALID_HANDLE_VALUE)
	{
		printf("%s \n", "Could Not Create New File");
		printf("%d \n", GetLastError());
		return 0;
	}

	if (!WriteFile(hNewFile, Bytes, NumberOfBytesToWrite, NULL, NULL))
	{
		CloseHandle(hNewFile);
		printf("%s \n", "Could Not Write To New File");
		printf("%d \n", GetLastError());
		return 0;
	}
	CloseHandle(hNewFile);
	return 1;
}


DWORD FileManagement::GetSize()
{
	return dFileSize;
}

HANDLE FileManagement::GetHandle()
{
	return hFile;
}

void* FileManagement::GetBytePointer()
{
	return pBytes;
}