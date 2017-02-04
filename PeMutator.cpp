#include "include.h"
PeMutator* PeMut = new PeMutator();




bool PeMutator::Mutate(BYTE* inBuf, DWORD SizeToMut, bool MutAll)
{
	for (int i = 0; i < SizeToMut - 16; i += length_disasm(&inBuf[i]))
	{
		if (length_disasm(&inBuf[i]) <= 0)
		{
			printf("%s \n", "Error Disassembling");
			return 0;
		}

		++InstructionCount;

		if (inBuf[i] == 0x55)
			if (inBuf[i + 1] == 0x8B)
				if (inBuf[i + 2] == 0xEC)
					if (inBuf[i + 3] == 0x83) // add esp, -x
						if (inBuf[i + 4] == 0xC4)
							if (((char)inBuf[i + 5] <= 0) && (inBuf[i + 5] != 0x80)) // -128 sux
								if (rand() % 2 || MutAll)
								{
									inBuf[i] = 0xC8;
									inBuf[i + 1] = -inBuf[i + 5];
									inBuf[i + 2] = 0;
									inBuf[i + 3] = 0;
									inBuf[i + 4] = 0x90;
									inBuf[i + 5] = 0x90;
									++MutationCount;
								}

		// enter
		if (inBuf[i] == 0x55)
			if (inBuf[i + 1] == 0x8B)
				if (inBuf[i + 2] == 0xEC)
					if (inBuf[i + 3] == 0x81) // add esp, xxxxxxxx
						if (inBuf[i + 4] == 0xC4)
							if ((inBuf[i + 7] == 0xFF) && (inBuf[i + 8] == 0xFF))
								if (rand() % 2 || MutAll)
								{
									inBuf[i] = 0xC8;
									long t = -((long)inBuf[i + 5] + (inBuf[i + 6] << 8));
									inBuf[i + 1] = t;
									inBuf[i + 2] = t >> 8;
									inBuf[i + 3] = 0;
									inBuf[i + 4] = 0x90;
									inBuf[i + 5] = 0x90;
									inBuf[i + 6] = 0x90;
									inBuf[i + 7] = 0x90;
									inBuf[i + 8] = 0x90;
									++MutationCount;
								}

		// enter
		if (inBuf[i] == 0x55)
			if (inBuf[i + 1] == 0x8B)
				if (inBuf[i + 2] == 0xEC)
					if (inBuf[i + 3] == 0x83) // sub esp, x
						if (inBuf[i + 4] == 0xEC)
							if ((signed)inBuf[i + 5] >= 0)
								if (rand() % 2 || MutAll)
								{
									inBuf[i] = 0xC8;
									inBuf[i + 1] = inBuf[i + 5];
									inBuf[i + 2] = 0;
									inBuf[i + 3] = 0;
									inBuf[i + 4] = 0x90;
									inBuf[i + 5] = 0x90;
									++MutationCount;
								}

		// mov esp, ebp
		// pop ebp
		// leave
		if (inBuf[i] == 0x8B)
			if (inBuf[i + 1] == 0xE5)
				if (inBuf[i + 2] == 0x5D)
					if (rand() % 2 || MutAll)
					{
						inBuf[i] == 0xC9;
						inBuf[i + 1] == 0x90;
						inBuf[i + 2] == 0x90;
						++MutationCount;
					}

		// 10001001 11xxxyyy     ; mov r1,r2
		// 01010xxx 01011yyy     ; push r2 // pop r1
		// 10001011 11xxxyyy     ; mov r1,r2
		// 01010yyy 01011xxx     ; push r2 // pop r1
		if ((inBuf[i + 1] & 0xC0) == 0xC0)
			if ((inBuf[i] & 0xFD) == 0x89)
				if (rand() % 2 || MutAll)
				{
					BYTE t = inBuf[i];
					inBuf[i] = 0x50 | ((inBuf[i + 1] >> (t == 0x89 ? 3 : 0)) & 7);
					inBuf[i + 1] = 0x58 | ((inBuf[i + 1] >> (t == 0x89 ? 0 : 3)) & 7);
					++MutationCount;
				}

		//if (xxx==yyy)
		//001100xx 11xxxyyy     ; xor r1,r1
		//001010xx 11xxxyyy     ; sub r1,r1
		if (((inBuf[i + 1] & 0xC0) == 0xC0) && (((inBuf[i + 1] >> 3) & 7) == (inBuf[i + 1] & 7)))
			if (((inBuf[i] & 0xFC) == 0x30) || ((inBuf[i] & 0xFC) == 0x28))
				if (rand() % 2 || MutAll)
				{
					inBuf[i] ^= 0x30 ^ 0x28;
					++MutationCount;
				}

		//if (xxx==yyy)
		//0000100x 11xxxyyy     ; or r1,r1
		//1000010x 11xxxyyy     ; test r1,r1
		if (((inBuf[i + 1] & 0xC0) == 0xC0) && (((inBuf[i + 1] >> 3) & 7) == (inBuf[i + 1] & 7)))
			if (((inBuf[i] & 0xFE) == 0x08) || ((inBuf[i] & 0xFE) == 0x84))
				if (rand() % 2 || MutAll)
				{
					inBuf[i] ^= 0x08 ^ 0x84;
					++MutationCount;
				}

		// AND Reg, Reg
		// OR Reg, Reg
		if (((inBuf[i] & 0xFE) == 0x08) || ((inBuf[i] & 0xFE) == 0x20))
			if (((inBuf[i + 1] & 0xC0) == 0xC0) && (((inBuf[i + 1] >> 3) & 7) == (inBuf[i + 1] & 7)))
				if (rand() % 2 || MutAll)
				{
					inBuf[i] ^= 0x08 ^ 0x20;
					++MutationCount;
				}

		// TEST Reg, Reg
		// AND Reg, Reg
		if (((inBuf[i] & 0xFE) == 0x20) || ((inBuf[i] & 0xFE) == 0x84))
			if (((inBuf[i + 1] & 0xC0) == 0xC0) && (((inBuf[i + 1] >> 3) & 7) == (inBuf[i + 1] & 7)))
				if (rand() % 2 || MutAll)
				{
					inBuf[i] ^= 0x20 ^ 0x84;
					++MutationCount;
				}


		//10000011 11000xxx c   ; add r1,c
		//10000011 11101xxx -c  ; sub r1,-c
		if (inBuf[i] == 0x83)
			if (((inBuf[i + 1] & 0xF8) == 0xC0) || ((inBuf[i + 1] & 0xF8) == 0xE8))
				if (inBuf[i + 2] != 0x80)
					//if ((inBuf[i + 1] & 7) == 4) // ESP only. May cause errors otherwise (CF)
						if (rand() % 2 || MutAll)
						{
							inBuf[i + 1] ^= 0x28;
							inBuf[i + 2] = -inBuf[i + 2];
							++MutationCount;
						}

		// ADD Reg1, Imm32
		// SUB Reg1, -Imm32
		if (inBuf[i] == 0x81)
			if (((inBuf[i + 1] & 0xF8) == 0xC0) || ((inBuf[i + 1] & 0xF8) == 0xE8))
				if (inBuf[i + 2] != 0x80)
					//if ((inBuf[i + 1] & 7) == 4) // ESP only. May cause errors otherwise (CF)
						if (rand() % 2 || MutAll)
						{
							inBuf[i + 1] ^= 0x28;
							*(int*)&inBuf[i + 2] = -(*(int*)&inBuf[i + 2]);
							++MutationCount;
						}


		//1101000x 11tttxxx     ; ttt r1,1 (ROL,ROR,RCL,RCR,SHL/SHA,SHR,SAR)
		//1101000x 11tttxxx     ;
		//1100000x 11tttxxx c   ; ttt r1,c
		if (((inBuf[i] & 0xFE) == 0xD0) && (inBuf[i + 2] == inBuf[i]))
			if (((inBuf[i + 1] & 0xC0) == 0xC0) && (inBuf[i + 3] == inBuf[i + 1]))
				if (rand() % 2 || MutAll)
				{
					++MutationCount;
					inBuf[i] ^= 0x10;
					inBuf[i + 2] = 2;
					inBuf[i + 3] = 0x90;
					if (rand() % 2 || MutAll)
					{
						inBuf[i + 3] = inBuf[i + 2];
						inBuf[i + 2] = inBuf[i + 1];
						inBuf[i + 1] = inBuf[i];
						inBuf[i] = 0x90;
					}
				}

		//if (aaa==bbb)
		//if (ddd==aaa)
		//001100x1 11aaabbb     ; xor/sub r1,r1
		//100010x0 11cccddd     ; mov r1l,r2l
		//00001111 10110110 11aaaddd ; movzx r1,r2l
		if (((inBuf[i] & 0xFD) == 0x31) || ((inBuf[i] & 0xFD) == 0x29))
			if ((inBuf[i + 1] & 0xC0) == 0xC0)
				if ((inBuf[i + 3] & 0xC0) == 0xC0)
					if ((((inBuf[i + 1] >> 3) ^ inBuf[i + 1]) & 7) == 0)
						if ((inBuf[i + 1] & 7)<4)
							if ((inBuf[i + 2] & 0xFD) == 0x88)
								if (((inBuf[i + 3] >> (inBuf[i + 2] == 0x88 ? 0 : 3)) & 7) == (inBuf[i + 1] & 7))
									if (rand() % 2 || MutAll)
									{
										++MutationCount;
										inBuf[i + 2] = 0xC0 |
											(inBuf[i + 1] & 0x38) |
											((inBuf[i + 3] >> (inBuf[i + 2] == 0x88 ? 3 : 0)) & 7);
										inBuf[i + 1] = 0xB6;
										inBuf[i] = 0x0F;
										inBuf[i + 3] = 0x90;
										if (rand() % 2 || MutAll)
										{
											inBuf[i + 3] = inBuf[i + 2];
											inBuf[i + 2] = inBuf[i + 1];
											inBuf[i + 1] = inBuf[i];
											inBuf[i] = 0x90;
										}
									}

		// mov r1, fs:[0]
		// xor r1,r1
		// mov r1,fs:[r1]
		if (inBuf[i] == 0x64)
			if (inBuf[i + 1] == 0x67)
				if (inBuf[i + 2] == 0x8B)
					if ((inBuf[i + 3] & 0xC7) == 0x06)
						if (inBuf[i + 4] == 0)
							if (inBuf[i + 5] == 0)
								if (rand() % 2 || MutAll)
								{
									++MutationCount;
									inBuf[i] = 0x33;
									inBuf[i + 1] = 0xC0 | (inBuf[i + 3] & 0x38) | (inBuf[i + 3] >> 3) & 7;
									inBuf[i + 2] = 0x64;
									inBuf[i + 4] = (inBuf[i + 3] & 0x38) >> 3;
									inBuf[i + 3] = 0x8B;
									inBuf[i + 5] = 0x90;
									if (rand() % 2 || MutAll)
									{
										inBuf[i + 5] = inBuf[i + 4];
										inBuf[i + 4] = inBuf[i + 3];
										inBuf[i + 3] = inBuf[i + 2];
										inBuf[i + 2] = inBuf[i + 1];
										inBuf[i + 1] = inBuf[i];
										inBuf[i] = 0x90;
									}
								}

		// mov eax, fs:[0]
		// xor eax,eax
		// mov eax, fs:[r1]
		if (inBuf[i] == 0x64)
			if (inBuf[i + 1] == 0x67)
				if (inBuf[i + 2] == 0xA1)
					if (inBuf[i + 3] == 0)
						if (inBuf[i + 4] == 0)
							if (rand() % 2 || MutAll)
							{
								++MutationCount;
								inBuf[i] = 0x33;
								inBuf[i + 1] = 0xC0;
								inBuf[i + 2] = 0x64;
								inBuf[i + 3] = 0x8B;
								inBuf[i + 4] = 0x00;
							}

		// or ecx,-1
		// xor ecx,ecx//dec ecx
		if (inBuf[i] == 0x83)
			if ((inBuf[i + 1] & 0xF8) == 0xC8)
				if (inBuf[i + 2] == 0xFF)
					if (rand() % 2 || MutAll)
					{
						++MutationCount;
						inBuf[i] = 0x33;
						BYTE t = inBuf[i + 1] & 7;
						inBuf[i + 1] = 0xC0 | t | (t << 3);
						inBuf[i + 2] = 0x48 | t;
					}

		// Inverse ADD/SUB/AND/OR/XOR/MOV/CMP Reg1, Reg2
		if (inBuf[i] <= 0x3B || inBuf[i] == 0x8B || inBuf[i] == 0x89)
			if (inBuf[i] & 9 == 1)
				if (length_disasm(&inBuf[i]) == 2)
					if (inBuf[i + 1] & 0xC0 == 0xC0)
						if (rand() % 2 || MutAll)
						{
							++MutationCount;
							inBuf[i] ^= 2;
							BYTE reg1 = inBuf[i + 1] & 7;
							BYTE reg2 = ((inBuf[i + 1] & 0x38) >> 3);
							inBuf[i + 1] = (reg1 << 3) + 0xC0 + reg2;
						}
		
		
		// Instruction shrinking (not needed for compiled pe's though, they are already optimized)
		/*if ((inBuf[i] & 0xFD) == 0x81)
			if ((inBuf[i + length_disasm(&inBuf[i])] & 0xFD) == 0x81)
				if ((inBuf[i + 1] & 7) == (inBuf[i + 1 + length_disasm(&inBuf[i])] & 7))
				{
					int FirstIMM;
					int SecondIMM;
					
					inBuf[i] == 0x83 ? FirstIMM = inBuf[i + 2] : FirstIMM = *(int*)&inBuf[i + 2];
					inBuf[i + length_disasm(&inBuf[i])] == 0x83 ? SecondIMM = inBuf[i + 2] : SecondIMM = *(int*)&inBuf[i + 2];
					
					//printf("%d\n%d\n", FirstIMM, SecondIMM);
					DWORD FirstOP = inBuf[i + 1] / 8;
					DWORD SecondOP = inBuf[i + 1 + length_disasm(&inBuf[i])] / 8;
					int result = OperationCalculation(FirstOP, SecondOP, FirstIMM, SecondIMM);
					//printf("%d\n%d\n", result, FirstIMM);
					//system("pause");
					if (result == -2)
						continue;
					else if (result == -1)
					{
						int j = length_disasm(&inBuf[i]);
						for (int x = 0; x < j; ++x)
							inBuf[i + x] = 0x90;
						continue;
					}
					
					*(int*)&inBuf[i + 2] = FirstIMM;
					int j = length_disasm(&inBuf[i]);
					int c = length_disasm(&inBuf[i] + j);
					for (int x = 0; x < c; ++x)
						inBuf[i + j + x] = 0x90;

					i -= j;    // Go back for iteration.
				}*/

	}
	printf("%d Mutations Done!\n", MutationCount);
	return 1;
}


int PeMutator::OperationCalculation(DWORD &FirstOP, DWORD SecondOP, int &FirstIMM, int SecondIMM)
{
	/*
	0x18 = ADD
	0x19 = OR
	0x1C = AND
	0x1D = SUB
	0x1E = XOR
	0x1F = CMP
	*/
	// Returns:
	// -2 if it isnt shrinkable
	// -1 if first instruction can be eliminated

	switch (FirstOP)
	{
	case 0x1F:
		return -1;
	case 0x18:
		if (SecondOP == 0x18)
		{
			FirstIMM += SecondIMM;
			return 0;
		}
		else if (SecondOP == 0x1D)
		{
			FirstIMM -= SecondIMM;
			return 0;
		}
		break;
	case 0x19:
		if (SecondOP == 0x19)
		{
			FirstIMM |= SecondIMM;
			return 0;
		}
		break;
	case 0x1C:
		if (SecondOP == 0x1C)
		{
			FirstIMM &= SecondIMM;
			return 0;
		}
		break;
	case 0x1D:
		if (SecondOP == 0x1D)
		{
			FirstIMM -= SecondIMM;
			return 0;
		}
		else if (SecondOP == 0x18)
		{
			FirstIMM += SecondIMM;
			return 0;
		}
		break;
	case 0x1E:
		if (SecondOP == 0x1E)
		{
			FirstIMM ^= SecondIMM;
			return 0;
		}
		break;
	default:
		return -2;
	}
	return -2;

}



