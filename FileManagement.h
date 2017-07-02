
#pragma once
class FileManagement
{
public:
	FileManagement(const std::string FileName);
	~FileManagement();

	DWORD GetSize() const;
	HANDLE GetHandle() const;
	void* GetBytePointer() const;
	bool WriteNewFile(const std::string NewFileName, void* Bytes, const DWORD NumberOfBytesToWrite);

private:
	bool ReadExistingFile();
	bool GetTextSection();

private:
	void*				m_pBytes = nullptr;
	HANDLE				m_hFile = INVALID_HANDLE_VALUE;
	DWORD				m_dwFileSize;
	HANDLE				m_hNewFile = INVALID_HANDLE_VALUE;
	const std::string	m_FileName;

public:
	PIMAGE_SECTION_HEADER pTextSection;
};
