// ConsoleApplication5.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include <iostream>
#include <vector>
#include <botan/sha2_32.h>
#include "wallet.h"
using namespace std;
using namespace Botan;

secure_vector<byte> somefunction(std::vector<byte> input) {
	SHA_256 sha;
	return sha.process(input);
}


	int main(int argc, char *argv[])
	{
		Wallet newOne;
		vector<byte> input;
		for (auto i = 0; i < 10; i++)
		{
			input.push_back('m');
		}
		secure_vector<byte> output = somefunction(input);
		for (auto i = 0; i < output.size(); i++)
		{
			cout << output[i] << endl;
		}
		return 0;
	
	}
	/**/


