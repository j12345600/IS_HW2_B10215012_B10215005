#include <iostream>
#include <thread>
#include <vector>
#include "blockencrypt.h"
using namespace std;
void test_func(bitset<64> plain,bitset<64> key, bitset<64> vec[],int num)
{
    vec[num]=BlockEncrypt::blockEncryption(plain,key);
}
int main()
{
    bitset<64> test[2];
    //test.resize(2);
    bitset<64> plain1("0110110001100101011000010111001001101110011010010110111001100111");
    bitset<64> plain2("0110110001100101101000010111001001101110011010010110111001100111");
    bitset<64> key   ("0110001101101111011011010111000001110101011101000110010101110010");
    thread Thread1( test_func,plain1,key,0);
    thread Thread2( test_func,plain2,key,1 );
    cout << "main thread" << endl;

    Thread1.join();
    Thread2.join();
    for(int i=0;i<2;++i) cout<<test[i]<<endl;
    cout<<thread::hardware_concurrency();
    //bitset<64> result = BlockEncrypt::blockEncryption(plain1,key);

    //cout<< result;
    return 0;
}

