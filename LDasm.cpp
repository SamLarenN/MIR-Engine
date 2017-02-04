

#include "include.h"

/* length_disasm */
unsigned int length_disasm(void * opcode0) {

	unsigned char* opcode = (unsigned char*)opcode0;

	unsigned int flag = 0;
	unsigned int ddef = 4, mdef = 4;
	unsigned int msize = 0, dsize = 0;

	unsigned char op, modrm, mod, rm;

prefix:
	op = *opcode++;

	/* prefix */
	if (CHECK_PREFIX(op)) {
		if (CHECK_PREFIX_66(op)) ddef = 2;
		else if (CHECK_PREFIX_67(op)) mdef = 2;
		goto prefix;
	}

	/* two byte opcode */
	if (CHECK_0F(op)) {
		op = *opcode++;
		if (CHECK_MODRM2(op)) flag++;
		if (CHECK_DATA12(op)) dsize++;
		if (CHECK_DATA662(op)) dsize += ddef;
	}

	/* one byte opcode */
	else {
		if (CHECK_MODRM(op)) flag++;
		if (CHECK_TEST(op) && !(*opcode & 0x38)) dsize += (op & 1) ? ddef : 1;
		if (CHECK_DATA1(op)) dsize++;
		if (CHECK_DATA2(op)) dsize += 2;
		if (CHECK_DATA66(op)) dsize += ddef;
		if (CHECK_MEM67(op)) msize += mdef;
	}

	/* modrm */
	if (flag) {
		modrm = *opcode++;
		mod = modrm & 0xc0;
		rm = modrm & 0x07;
		if (mod != 0xc0) {
			if (mod == 0x40) msize++;
			if (mod == 0x80) msize += mdef;
			if (mdef == 2) {
				if ((mod == 0x00) && (rm == 0x06)) msize += 2;
			}
			else {
				if (rm == 0x04) rm = *opcode++ & 0x07;
				if (rm == 0x05 && mod == 0x00) msize += 4;
			}
		}
	}

	opcode += msize + dsize;

	return opcode - (unsigned char *)opcode0;
}
