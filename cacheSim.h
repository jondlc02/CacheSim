#include "set.h"
#include <fstream>
#include <iostream>
#include <vector>
#include <deque> //pop_front and push_back for regular FIFO use
using namespace std;

class cacheSim 
{
private: 
	unsigned int numSets;		
	unsigned int numBlocks;
	unsigned int blockSize;

	unsigned int setSize;
	unsigned int setFieldSize;
	unsigned int offsetSize;
	unsigned int totalCount;
	unsigned int hitCount;
	vector<SetCache> sets;	//collection of sets

public:
	cacheSim();
	cacheSim(unsigned int&, unsigned int&, unsigned int&);
	string hex2B(string&);
	unsigned int b2Dec(string);
	void processAddress(char&, string&);
	bool find(unsigned int&, string&);
	void store(unsigned int&, string&);
	float getHitRate();
	int getTotalCount();
};