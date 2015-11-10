#include<stdio.h>
#include <bitset>
#include<iostream>
#include<vector>
#include "Header.h"
#include <stdlib.h>
#include <string>
using namespace std;
int main(){
     bitset<64> key("1111111111111111111111111111111111111111111111111111111111111111");
     bitset<64>  IV("1111111111111111111111111111111111111111111111111111111111111111");
          string s;
     int encrypt=0;
     while(true){
         cout<<"(1) Encryption (2) Decryption  ";
         cin>>encrypt;
		 cout << IV << endl;
         /*Encryption*/
         if(encrypt==1){
            do{
                cout<<"Enter a 64-bit plaintext in binary to be ciphered: \n";
                cin>>s;
                if(s.size()!=64) cout<<"The number of bits is not 64\n";
              }while(s.size()!=64);
            bitset<64> plaintext(s);
            do{
                cout<<"Enter the key (64 bits)\n";
                cin>>s;
                if(s.size()!=64) cout<<"The number of bits is not 64\n";
            }while(s.size()!=64);
            bitset<64> key(s);
            cout<<"\nPlaintext is: \n"<<plaintext<<endl;
            cout<<"key is:\n"<<key<<endl;
            cout<<"Cipheredtext is:\n"<<DES::blockEncryption(plaintext,key)<<endl;
         }
         /*Decryption*/
         else if (encrypt==2){
             do{
                 cout<<"Enter a 64-bit cipheredtext in binary to be deciphered: \n";
                 cin>>s;
                 if(s.size()!=64) cout<<"The number of bits is not 64\n";
             }while(s.size()!=64);
             bitset<64> cipheredtext(s);
             do{
                 cout<<"Enter the key (64 bits)\n";
                 cin>>s;
                 if(s.size()!=64) cout<<"The number of bits is not 64\n";
                }while(s.size()!=64);
             bitset<64> key(s);
             cout<<"\nCipheredtext is:\n"<<cipheredtext<<endl;
             cout<<"key is:\n"<<key<<endl;
             cout<<"Plaintext is:\n"<<DES::blockDecryption(cipheredtext,key)<<endl;
         }
     }
}