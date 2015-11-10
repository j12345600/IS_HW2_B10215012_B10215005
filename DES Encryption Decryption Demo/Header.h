#pragma once
#include "stdafx.h"
#include<stdio.h>
#include <bitset>
#include<iostream>

using namespace std;
class DES
{
public:

	int a;
	DES();
	static bitset<64> blockEncryption(bitset<64>&, bitset<64>&);
	static bitset<64> blockDecryption(bitset<64>, bitset<64>);
	static bitset<32> sbox(bitset<48>);
	static bitset<32> funct(bitset<32>, bitset<48>);
	static bitset<48> keyGen(bitset<28> &, bitset<28> &, int);
};