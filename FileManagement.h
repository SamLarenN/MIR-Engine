
#pragma once
class FileManagement
{
public:
	FileManagement();
	~FileManagement();
	void OnSetup(const char* FileName);

	DWORD GetSize();
	HANDLE GetHandle();
	void* GetBytePointer();
	bool WriteNewFile(const char* NewFileName, void* Bytes, DWORD NumberOfBytesToWrite);

private:
	bool ReadExistingFile(const char* FileName);

private:
	void*		pBytes;
	HANDLE		hFile;
	DWORD		dFileSize;

	HANDLE		hNewFile;
};

extern FileManagement* FManager;