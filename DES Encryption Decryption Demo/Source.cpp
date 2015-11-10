#include "stdafx.h"
#include "Header.h"
DES::DES()
{
	a = 3;
}
bitset<64> DES::blockEncryption(bitset<64> &plaintext, bitset<64>& key) {
	bitset<64> afterIniPer = 0, tmp64;
	bitset<64> afterFinalPer = 0, after16Round;
	bitset<32> plainL32, plainR32, tmp32;
	bitset<56> key56;
	bitset<28> keyL28, keyR28;
	unsigned int iniPermu[] = { 58,50,42,34,26,18,10,2
		,60,52,44,36,28,20,12,4
		,62,54,46,38,30,22,14,6
		,64,56,48,40,32,24,16,8
		,57,49,41,33,25,17,9,1
		,59,51,43,35,27,19,11,3
		,61,53,45,37,29,21,13,5
		,63,55,47,39,31,23,15,7 };
	unsigned int FinalPermu[] = { 40,8,48,16,56,24,64,32
		,39,7,47,15,55,23,63,31
		,38,6,46,14,54,22,62,30
		,37,5,45,13,53,21,61,29
		,36,4,44,12,52,20,60,28
		,35,3,43,11,51,19,59,27
		,34,2,42,10,50,18,58,26
		,33,1,41,9,49,17,57,25 };
	unsigned int keyReduce[] = { 57,49,41,33,25,17,9,1
		,58,50,42,34,26,18,10,2
		,59,51,43,35,27,19,11,3
		,60,52,44,36,63,55,47,39
		,31,23,15,7,62,54,46,38
		,30,22,14,6,61,53,45,37
		,29,21,13,5,28,20,12,4 };

	//initial permutation
	for (int i = 0; i < 64; i++) {
		afterIniPer[63 - i] = plaintext[64 - iniPermu[i]];
	}
	//key 64-bit to 56-bit & split into two helves
	for (int i = 0; i < 56; i++) {
		key56[55 - i] = key[64 - keyReduce[i]];
		if (55 - i >= 28) keyL28[27 - i] = key56[55 - i];
		else keyR28[55 - i] = key56[55 - i];
	}
	/*cout << "iniPer" << afterIniPer;
	cout << "\nkey 56: " << key56 << endl;
	cout << "key L:  " << keyL28 << endl;
	cout << "key R:  " << keyR28 << endl;*/

	//divide plaintext into two helves
	for (int i = 0; i < 64; i++) {
		if ((63 - i) >= 32) plainL32[31 - i] = afterIniPer[63 - i];
		else plainR32[63 - i] = afterIniPer[63 - i];
	}
	/*cout << "\nP 56: " << afterIniPer << endl;
	cout << "P L:  " << plainL32 << endl;
	cout << "P R:  " << plainR32 << endl;*/

	for (int i = 0; i < 16; ++i) {
		tmp32 = plainR32;
		plainR32 = funct(plainR32, keyGen(keyL28, keyR28, i));
		plainR32 ^= plainL32;
		plainL32 = tmp32;
		/*cout << "P L:  " << plainL32 << endl;
		cout << "P R:  " << plainR32 << endl;*/
	}

	after16Round = plainR32.to_ullong();
	after16Round <<= 32;
	tmp64 = plainL32.to_ulong();
	after16Round |= tmp64;
	//cout << "after16R \n" << after16Round<<endl;

	//Final permutation
	for (int i = 0; i < 64; i++) {
		afterFinalPer[63 - i] = after16Round[64 - FinalPermu[i]];
	}
	return afterFinalPer;
}
bitset<32> DES::sbox(bitset<48> raw) {
	bitset<48> tmp48 = 63;
	bitset<32>  after = 0;
	bitset<6> tmpIn = 0;
	bitset<32> tmp32;
	bitset<2> tmp2 = 0;
	int sBox[8][4][16] = { { { 14,4,13,1,2,15,11,8,3,10,6,12,5,9,0,7 }
		,{ 0,15,7,4,14,2,13,1,10,6,12,11,9,5,3,8 }
		,{ 4,1,14,8,13,6,2,11,15,12,9,7,3,10,5,0 }
		,{ 15,12,8,2,4,9,1,7,5,11,3,14,10,0,6,13 } }

		,{ { 15,1,8,14,6,11,3,4,9,7,2,13,12,0,5,10 }
		,{ 3,13,4,7,15,2,8,14,12,0,1,10,6,9,11,5 }
		,{ 0,14,7,11,10,4,13,1,5,8,12,6,9,3,2,15 }
		,{ 13,8,10,1,3,15,4,2,11,6,7,12,0,5,14,9 } }

		,{ { 10,0,9,14,6,3,15,5,1,13,12,7,11,4,2,8 }
		,{ 13,7,0,9,3,4,6,10,2,8,5,14,12,11,15,1 }
		,{ 13,6,4,9,8,15,3,0,11,1,2,12,5,10,14,7 }
		,{ 1,10,13,0,6,9,8,7,4,15,14,3,11,5,2,12 } }

		,{ { 7,13,14,3,0,6,9,10,1,2,8,5,11,12,4,15 }
		,{ 13,8,11,5,6,15,0,3,4,7,2,12,1,10,14,9 }
		,{ 10,6,9,0,12,11,7,13,15,1,3,14,5,2,8,4 }
		,{ 3,15,0,6,10,1,13,8,9,4,5,11,12,7,2,14 } }

		,{ { 2,12,4,1,7,10,11,6,8,5,3,15,13,0,14,9 }
		,{ 14,11,2,12,4,7,13,1,5,0,15,10,3,9,8,6 }
		,{ 4,2,1,11,10,13,7,8,15,9,12,5,6,3,0,14 }
		,{ 11,8,12,7,1,14,2,13,6,15,0,9,10,4,5,3 } }

		,{ { 12,1,10,15,9,2,6,8,0,13,3,4,14,7,5,11 }
		,{ 10,15,4,2,7,12,9,5,6,1,13,14,0,11,3,8 }
		,{ 9,14,15,5,2,8,12,3,7,0,4,10,1,13,11,6 }
		,{ 4,3,2,12,9,5,15,10,11,14,1,7,6,0,8,13 } }

		,{ { 4,11,2,14,15,0,8,13,3,12,9,7,5,10,6,1 }
		,{ 13,0,11,7,4,9,1,10,14,3,5,12,2,15,8,6 }
		,{ 1,4,11,13,12,3,7,14,10,15,6,8,0,5,9,2 }
		,{ 6,11,13,8,1,4,10,7,9,5,0,15,14,2,3,12 } }

		,{ { 13,2,8,4,6,15,11,1,10,9,3,14,5,0,12,7 }
		,{ 1,15,13,8,10,3,7,4,12,5,6,11,0,14,9,2 }
		,{ 7,11,4,1,9,12,14,2,0,6,10,13,15,3,5,8 }
	,{ 2,1,14,7,4,10,8,13,15,12,9,0,3,5,6,11 } } };
	//cout << "sbox: ";
	for (int i = 7; i >= 0; --i) {
		tmpIn = (tmp48&(raw >> (6 * i))).to_ulong();
		tmp2[0] = tmpIn[0]; tmp2[1] = tmpIn[5];
		tmpIn >>= 1; tmpIn[4] = 0;
		tmp32 = sBox[7 - i][tmp2.to_ulong()][tmpIn.to_ulong()];
		//cout<< sBox[7-i][tmp2.to_ulong()][tmpIn.to_ulong()]<<" ";
		if (i != 7)after <<= 4;
		after |= tmp32;
		//cout << tmpIn<<endl;
	}
	//cout << endl;
	return after;
}
bitset<32> DES::funct(bitset<32> R, bitset<48> key) {
	bitset<48>afterExpan;
	bitset<32>afterSbox32;
	bitset<32> output32;
	unsigned int expansion[] = { 32,1,2,3,4,5
		,4,5,6,7,8,9
		,8,9,10,11,12,13
		,12,13,14,15,16,17
		,16,17,18,19,20,21
		,20,21,22,23,24,25
		,24,25,26,27,28,29
		,28,29,30,31,32,1 };
	unsigned int fPermu[] = { 16,7,20,21,29,12,28,17
		,1,15,23,26,5,18,31,10
		,2,8,24,14,32,27,3,9
		,19,13,30,6,22,11,4,25 };

	//R 32-bit expands to 48-bit
	for (int i = 0; i < 48; ++i) {
		afterExpan[47 - i] = R[32 - expansion[i]];
	}
	//cout << "E=" << afterExpan << endl;
	afterExpan ^= key;
	afterSbox32 = sbox(afterExpan);
	//Func permutation
	for (int i = 0; i < 32; ++i) {
		output32[31 - i] = afterSbox32[32 - fPermu[i]];
	}
	return output32;
}
bitset<48> DES::keyGen(bitset<28> &Lkey, bitset<28> &Rkey, int round) {
	bitset<56> tmp56, afterLeftShift;
	bitset<48> result;
	bitset<2> tmpL2, tmpR2;
	unsigned int CP[] = { 14,17,11,24,1,5,3,28,15,6,21,10
		,23,19,12,4,26,8,16,7,27,20,13,2
		,41,52,31,37,47,55,30,40,51,45,33,48
		,44,49,39,56,34,53,46,42,50,36,29,32 };
	round++;
	// In round 1,2,9,16, both halves only need to be rotated left by 1-bit
	if (round == 1 || round == 2 || round == 16 || round == 9) {
		tmpL2[0] = Lkey[27]; tmpR2[0] = Rkey[27];
		Lkey <<= 1; Rkey <<= 1; Lkey[0] = tmpL2[0]; Rkey[0] = tmpR2[0];
	}
	else {
		tmpL2[0] = Lkey[26]; tmpR2[0] = Rkey[26];
		tmpL2[1] = Lkey[27]; tmpR2[1] = Rkey[27];
		Lkey <<= 2; Rkey <<= 2;
		Lkey[0] = tmpL2[0]; Rkey[0] = tmpR2[0];
		Lkey[1] = tmpL2[1]; Rkey[1] = tmpR2[1];
	}
	/*cout << "Lkey:" << Lkey;
	cout << "\nRkey:" << Rkey<<endl;*/
	afterLeftShift = Lkey.to_ulong();
	afterLeftShift <<= 28;
	tmp56 = Rkey.to_ulong();
	afterLeftShift |= tmp56;

	for (int i = 0; i < 48; ++i) {
		result[47 - i] = afterLeftShift[56 - CP[i]];
	}

	return result;
}
bitset<64> DES::blockDecryption(bitset<64>ciphertext, bitset<64> key) {
	bitset<64> afterIniPer = 0, tmp64;
	bitset<64> afterFinalPer = 0, after16Round;
	bitset<32> plainL32, plainR32, tmp32;
	bitset<56> key56;
	bitset<28> keyL28, keyR28;
	bitset<48> subKeySet[16];
	unsigned int iniPermu[] = { 58,50,42,34,26,18,10,2
		,60,52,44,36,28,20,12,4
		,62,54,46,38,30,22,14,6
		,64,56,48,40,32,24,16,8
		,57,49,41,33,25,17,9,1
		,59,51,43,35,27,19,11,3
		,61,53,45,37,29,21,13,5
		,63,55,47,39,31,23,15,7 };
	unsigned int FinalPermu[] = { 40,8,48,16,56,24,64,32
		,39,7,47,15,55,23,63,31
		,38,6,46,14,54,22,62,30
		,37,5,45,13,53,21,61,29
		,36,4,44,12,52,20,60,28
		,35,3,43,11,51,19,59,27
		,34,2,42,10,50,18,58,26
		,33,1,41,9,49,17,57,25 };
	unsigned int keyReduce[] = { 57,49,41,33,25,17,9,1
		,58,50,42,34,26,18,10,2
		,59,51,43,35,27,19,11,3
		,60,52,44,36,63,55,47,39
		,31,23,15,7,62,54,46,38
		,30,22,14,6,61,53,45,37
		,29,21,13,5,28,20,12,4 };

	//initial permutation
	for (int i = 0; i < 64; i++) {
		afterIniPer[63 - i] = ciphertext[64 - iniPermu[i]];
	}

	//key 64-bit to 56-bit & split into two helves
	for (int i = 0; i < 56; i++) {
		key56[55 - i] = key[64 - keyReduce[i]];
		if (55 - i >= 28) keyL28[27 - i] = key56[55 - i];
		else keyR28[55 - i] = key56[55 - i];
	}
	
	//divide ciphertext into two halves
	for (int i = 0; i < 64; i++) {
		if ((63 - i) >= 32) plainR32[31 - i] = afterIniPer[63 - i];
		else plainL32[63 - i] = afterIniPer[63 - i];
	}
	
	//gnerating 16 sub-keys
	for (int i = 0; i < 16; ++i) {
		subKeySet[i] = keyGen(keyL28, keyR28, i);
	}
	//start 16 round iteration
	for (int i = 0; i < 16; ++i) {
		tmp32 = plainL32;
		plainL32 = funct(plainL32, subKeySet[15-i]);
		plainL32 ^= plainR32;
		plainR32 = tmp32;
		/*cout << "P L:  " << plainL32 << endl;
		cout << "P R:  " << plainR32 << endl;*/
	}

//merge 2 helves into one 64-bit
#pragma region merge
	after16Round = plainL32.to_ullong();
	after16Round <<= 32;
	tmp64 = plainR32.to_ulong();
	after16Round |= tmp64;
	//cout << "after16R \n" << after16Round<<endl;
#pragma endregion

	

	//Final permutation
	for (int i = 0; i < 64; i++) {
		afterFinalPer[63 - i] = after16Round[64 - FinalPermu[i]];
	}
	return afterFinalPer;
}