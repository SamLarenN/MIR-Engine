#include "include.h"

BYTE* inBuff = nullptr;
DWORD FileSize;
bool AllMut = 1;
std::string newName;
std::string ExeName;


int main(int argc, char* argv[])
{
	if (argc < 2)
		return 2;

	srand(time(NULL));
	auto FManager = std::make_unique<FileManagement>("d.exe");
	
	inBuff = (BYTE*)(reinterpret_cast<DWORD>(FManager->GetBytePointer()) + FManager->pTextSection->PointerToRawData);
	FileSize = FManager->pTextSection->SizeOfRawData;

	printf("Want To Morph All Instructions? '1' For All, '0' For Randomized:\n");
	std::cin >> AllMut;
	
	PeMut->Mutate(inBuff, FileSize, AllMut);

	printf("Enter Name Of New Mutated File:\n");
	std::cin >> newName;
	newName += ".exe";
	
	FManager->WriteNewFile(newName.c_str(), FManager->GetBytePointer(), FManager->GetSize());

	FManager->~FileManagement();
	printf("Succesfully Made New PE!\n");
	Sleep(3000);
	return 0;
}
