// ConsoleApplication2.cpp : 定義主控台應用程式的進入點。
//

#include "stdafx.h"
#include<stdio.h>
#include <bitset>
#include<iostream>
#include<vector>
#include <thread>
#include "Header.h"
#include <stdlib.h>
#include <fstream>
#include <windows.h>
#include <chrono>    // std::chrono::seconds
#include <string>
using namespace std;
void thread_task(bitset<64> IV, bitset<64> key, int i, int x);
bitset<64> mat240x480[240][480];


typedef unsigned char   BYTE;
typedef unsigned short  WORD;
typedef unsigned long   DWORD;
typedef long            LONG;

//#define MAXPIX 512

using namespace std;

int main() {
	bitset<64> key("1010111110101111101011111010111110101111101011111010111110101111");
	bitset<64> IV( "1111101011111010111110101111101011111010111110101111101011111010");
	BITMAPFILEHEADER	FileHeader;
	BITMAPINFOHEADER	InfoHeader;
	int ImageX, ImageY;
	BYTE   ByteBuf;
	int fix;
	int i, j, n;
	FILE *fp,*ini;
	FILE *out;
	char default[] = "cute-cougar-baby.bmp";
	char *filename = default;
	char *Output;
	char ECB[] = "outECB.bmp";
	char CBC[] = "outCBC.bmp";
	char OFB[] = "outOFB.bmp";
	char CTR[] = "outCTR.bmp";


	/* 開啟檔案 */
	errno_t err;
	err = fopen_s(&fp,filename, "rb");

	cout << "Open file " << filename << endl;
	cout << "key is " << key.to_string() << endl;
	cout << "IV is  " << IV<<endl;
	/* 先讀取檔頭資訊 */
	fread(&FileHeader, sizeof(BITMAPFILEHEADER), 1, fp);
	fread(&InfoHeader, sizeof(BITMAPINFOHEADER), 1, fp);

	
	/* 確定格式 */
	if (FileHeader.bfType != 'MB')
		return false;
	if (InfoHeader.biCompression != 0)
		return false;
	if (InfoHeader.biBitCount != 24)
		return false;

	/* 取得圖寬及圖高 */
	ImageX = InfoHeader.biWidth;
	ImageY = InfoHeader.biHeight;

	printf("ImageX:%d\n", ImageX);
	printf("ImageY:%d\n", ImageY);

	fclose(fp);

	int mode=1;//ECB CBC OFB CTR
	string s;
	s.resize(64);

	while (mode > 0) {
		cout << "(1)ECB (2)CBC (3)OFB (4)CTR (5) Change key (6) Change IV (7)ECB_DE (8)CBC_DE (9)OFB_DE (10)CTR_DE  ";
		cin >> mode;
		Output = ECB;
		switch (mode) {
			case 1:
				filename = default;
				Output = ECB;
				break;
			case 2:
				filename = default;
				Output = CBC;
				break;
			case 3:
				filename = default;
				Output = OFB;
				break;
			case 4:
				filename = default;
				Output = CTR;
				break;
			case 5:
				cout << "Enter a 64-bit key: \nOld: "<<key<<"\nNew: ";
				for (int i = 0; i < 64; i++) cin >> s[i];
				key = bitset<64>(s);
				break;
			case 6:
				cout << "Enter a 64-bit IV: \nOld: " << IV << "\nNew: ";
				for (int i = 0; i < 64; i++) cin >> s[i];
				IV = bitset<64>(s);
				break;
			case 7:
				filename = ECB;
				Output = "DE_ECB.bmp";
				break;
			case 8:
				filename = CBC;
				Output = "DE_CBC.bmp";
				break;
			case 9:
				filename = OFB;
				Output = "DE_OFB.bmp";
				break;
			case 10:
				filename = CTR;
				Output = "DE_CTR.bmp";
				break;
		}
		if (mode == 6 || mode == 5) {
			cout << "\nkey is " << key.to_string() << endl;
			cout << "IV is  " << IV << endl;
			continue;
		}

			err = fopen_s(&fp, filename, "rb");
			fread(&FileHeader, sizeof(BITMAPFILEHEADER), 1, fp);
			fread(&InfoHeader, sizeof(BITMAPINFOHEADER), 1, fp);

		err = fopen_s(&out, Output, "wb");
		fwrite(&FileHeader, 1, sizeof(BITMAPFILEHEADER), out);
		fwrite(&InfoHeader, 1, sizeof(BITMAPINFOHEADER), out);


		/* 圖入各像素資訊 */
		bitset<64> tmp64;
		std::thread threads[480];
		std::chrono::time_point<std::chrono::system_clock> start, end;
		start = std::chrono::system_clock::now();
		/* CTR 平行計算offset之後的VI */
		if (mode == 4||mode==10) {
			for (i = ImageY - 1; i >= 0; i--)	threads[i] = std::thread(thread_task,IV,key,i,240);
			for (auto& t: threads) {
					t.join();
					t.~thread();
			}
		}
			
		/* 載入圖片並且加密 */
		for (i = ImageY - 1; i >= 0; i--)
		{
			for (j = 0; j<(ImageX * 3 * 8) / 64; j++)
			{
				fread(&tmp64, sizeof(bitset<64>), 1, fp);
	#pragma region ECB			
				if(mode==1) mat240x480[j][i] = DES::blockEncryption(tmp64, key);     
				else if(mode==7) mat240x480[j][i] = DES::blockDecryption(tmp64, key);
	#pragma endregion
	#pragma region CBC	
				else if (mode == 2) {
					if (i == (ImageY - 1) && j == 0) {
						mat240x480[j][i] = DES::blockEncryption(tmp64^IV, key);
					}
					else {
						if(j==0) mat240x480[j][i] = DES::blockEncryption(tmp64^mat240x480[ImageX * 3 * 8 / 64-1][i+1], key);
						else mat240x480[j][i] = DES::blockEncryption(tmp64^mat240x480[j-1][i], key);
					}
				}
				else if(mode==8) mat240x480[j][i] = tmp64;//解密用
	#pragma endregion
	#pragma region OFB
				else if (mode == 3||mode==9) {
					bitset<64> key_IV64;
					if (i == (ImageY - 1) && j == 0) {
						key_IV64 = DES::blockEncryption(IV, key);
					}
					else key_IV64 = DES::blockEncryption(key_IV64, key);
					mat240x480[j][i] = tmp64^key_IV64;
				}
			
	#pragma endregion
	#pragma region CTR
				else if (mode == 4 || mode == 10) {
					mat240x480[j][i] ^= tmp64;
				}
				
	#pragma endregion
			}
		}
	#pragma region CBC	Decrypt
		if (mode == 8) {
			for (i = 0; i < ImageY; i++){
				for (j = (ImageX * 3 * 8) / 64-1; j>=0; j--){
					if (i == (ImageY - 1) && j == 0) {
						tmp64 = DES::blockDecryption(mat240x480[j][i], key);
						mat240x480[j][i] = tmp64^IV;
					}
					else if (j == 0) {
						tmp64 = DES::blockDecryption(mat240x480[j][i], key);
						mat240x480[j][i] = tmp64^mat240x480[ImageX * 3 * 8 / 64 - 1][i+1];
					}
					else {
					tmp64=DES::blockDecryption(mat240x480[j][i],key);
					mat240x480[j][i] = tmp64^mat240x480[j - 1][i];
					}
				}
			}
		}
		
	#pragma endregion

		/*輸出成圖檔*/
		for (i = ImageY - 1; i >= 0; i--)
		{
			for (j = 0; j<(ImageX * 3 * 8) / 64; j++)
			{
				tmp64 = mat240x480[j][i];
				fwrite(&tmp64, 1, sizeof(bitset<64>), out);
			}

		}
		end = std::chrono::system_clock::now();
		std::chrono::duration<double> elapsed_seconds = end - start;
		cout << "elapsed time: " << elapsed_seconds.count() << endl;
		fclose(out);
		fclose(fp);
	}
	

	system("pause");
}

void thread_task(bitset<64> IV, bitset<64> key, int i,int x) {
	int offset = (479-i)*x;
	bitset<64> key_IV64;
	for (int j = 0; j < x; ++j,++offset) {
		key_IV64 = IV.to_ullong() + offset;
		mat240x480[j][i] = DES::blockEncryption(key_IV64, key);
	}
}
