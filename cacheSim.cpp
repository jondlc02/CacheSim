#include "cacheSim.h"
#include <vector>
#include <iostream>
#include <fstream>
using namespace std;

cacheSim::cacheSim()
{
	numSets = 0;
	numBlocks = 0;
	blockSize = 0;
	setSize = 0;
	totalCount = 0;
	hitCount = 0;
}

cacheSim::cacheSim(unsigned int& numSets, unsigned int& numBlocks, unsigned int& blockSize)
{
	this->numSets = numSets;
	this->numBlocks = numBlocks;
	this->blockSize = blockSize;
	
	this->setSize = numSets / numBlocks;
	this->setFieldSize = log2(numSets);
	this->offsetSize = log2(blockSize);	

	this->totalCount = 0;
	this->hitCount = 0;
	
	for (unsigned int i = 0; i < numSets; i++)
	{
		SetCache newSet;
		newSet.setIdentity = i;
		sets.push_back(newSet);
	}

}

unsigned int cacheSim::b2Dec(string bin)
{
	string num = bin;
	unsigned int dec_value = 0;

	int base = 1;

	int len = num.length();
	for (int i = len - 1; i >= 0; i--) {
		if (num[i] == '1')
			dec_value += base;
		base = base * 2;
	}

	return dec_value;
}

string cacheSim::hex2B(string& hexNum)
{
	string decodedNum;
	for (unsigned int i = 2; i < hexNum.size(); i++)
	{
		char hexDigit = hexNum.at(i);
		switch (hexDigit)
		{
		case 'a':
		case 'A': 
			decodedNum += "1010";
			break;
		case 'b':
		case 'B':
			decodedNum += "1011";
			break;
		case 'c':
		case 'C':
				decodedNum += "1100";
				break;
		case 'd':
		case 'D':
			decodedNum += "1101";
			break;
		case 'e':
		case 'E':
			decodedNum += "1110";
			break;
		case 'f':
		case 'F':
			decodedNum += "1111";
			break;
		case '1':
			decodedNum += "0001";
			break;
		case '2':
			decodedNum += "0010";
			break;
		case '3':
			decodedNum += "0011";
			break;
		case '4':
			decodedNum += "0100";
			break;
		case '5':
			decodedNum += "0101";
			break;
		case '6':
			decodedNum += "0110";
			break;
		case '7':
			decodedNum += "0111";
			break;
		case '8':
			decodedNum += "1000";
			break;
		case '9':
			decodedNum += "1001";
			break;
		default:
			decodedNum += "0000";
		}
	}
	return decodedNum;
}


void cacheSim::processAddress(char& option, string& address)
{	
	string binAddress = hex2B(address);
	string tag;
	string setField;
	unsigned int setDec;
	unsigned int tagSize = 32 - (setFieldSize + offsetSize);	//Gets tag size

	for (unsigned int i = 0; i < tagSize; i++)	//Finds tag from after 0x to right before
	{												//The offset
		tag.push_back(binAddress.at(i));
	}

	if (setFieldSize == 0)
	{
		setField = "0";
	}

	for (unsigned int i = tagSize; i < 32 - offsetSize; i++)
	{
		setField.push_back(binAddress.at(i));
	}

	setDec = b2Dec(setField);

	if (option == 's')
	{
		store(setDec, tag);
	}

	else if (option == 'l')
	{
		totalCount++;
		bool hit = find(setDec, tag);
		if (!hit)
		{
			store(setDec, tag);
		}
	}
}

bool cacheSim::find(unsigned int& set, string& tag)	//Searching algorithm.
{
	bool verify = false;

	for (unsigned int i = 0; i < sets.at(set).blocks.size(); i++)
	{
		if (tag == sets.at(set).blocks[i])
		{
			hitCount++;
			return true;
		}
	}


	return verify;
}

void cacheSim::store(unsigned int& set, string & tag)	//Loading algorithm, pretty simple for FA
{
	for (int i = 0; i < numSets; i++)
	{
		if (sets.at(i).setIdentity == set)
		{
			if (sets.at(i).blocks.size() == numBlocks * (blockSize / 4))	//If full, pop out first to enter
			{
				sets.at(i).blocks.pop_front();
			}
			sets.at(i).blocks.push_back(tag);
			break;
		}
	}
}

float cacheSim::getHitRate()
{
	if (totalCount == 0)
	{
		return 0;
	}
	return (float)hitCount / totalCount;
}

int cacheSim::getTotalCount()
{
	return totalCount;
}