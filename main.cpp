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
	FManager->OnSetup(argv[1]);
	
	inBuff = (BYTE*)FManager->GetBytePointer();
	FileSize = FManager->GetSize();

	printf("Want To Morph All Instructions? '1' For All, '0' For Randomized:\n");
	std::cin >> AllMut;
	
	PeMut->Mutate(inBuff, FileSize, AllMut);

	printf("Enter Name Of New Mutated File (Needs .exe suffix):\n");
	std::cin >> newName;
	
	FManager->WriteNewFile(newName.c_str(), inBuff, FileSize);

	FManager->~FileManagement();
	printf("Succesfully Made New PE!\n");
	Sleep(3000);
	return 0;
}


