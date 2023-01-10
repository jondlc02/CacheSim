/*Trace file format:
- s = save address to cache
- l = load address from cache
- 32 bit hex address
- random last number that literally doesn't matter

Algorithm:
1) Receive memory address from trace
2) Process save or load
3) Remove offset (log(line size)) from address
4.1) Store in cache
4.2) Find in cache
5.1) If in cache, process hit to total hit rate
5.2) If not in cache, process miss and add to cache
6) If cache is full, use replacement policy (FIFO or least recently used).

Fully associative -> only has tag and offset
- Look in every line for tag

Direct Mapped -> need to calculate line number and offset
- Num lines = cache size/line size
- line size = log(numLines) bits
- Tag -> Line# -> Offset
- Look for block in appropriate line
- FIFO applies individually per line

Set Associative -> needs to calculate set# and offset
- Looks through entire set, regardless of how many lines per set (don't really understand
how that differs from Direct Mapped.
- Tag -> set# -> offset
- Num of set based on num of lines per set (can either be hardcoded or taken as an input)
	- Num sets = (bytes/cache) / [(bytes/line * lines/set)]
	- Set field = log(numSets) bits
- FIFO applies individually per line per set

Storage calculated by kiloBYTE
- 1 byte = 8 bits
- Size of main memory is 2^32

Each 
*/

#include "cacheSim.h"
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <time.h>
using namespace std;

int main()
{
	clock_t clk;
	string leave = "y";
	while (leave == "y")
	{
		string fileName;
		unsigned int blockSize;
		unsigned int numBlocks;
		unsigned int numSets;
		cout << "How many sets would you like in the cache? (Must be positive power of 2)" << endl;
		cin >> numSets;
		cout << "How many blocks per set would you like in the cache? (Must be positive power of 2)" << endl;
		cin >> numBlocks;
		cout << "How big is a block in bytes? (Must be positive power of 2 > 4)" << endl;
		cin >> blockSize;
		if (numSets == 1)
		{
			cout << "\nYou will be simulating a Fully Associated Cache" << endl;
		}
		else if (numSets > 1 && numBlocks == 1)
		{
			cout << "You will be simulating a Directly Mapped Cache" << endl;
		}
		else if (numSets > 1 && numBlocks > 1)
		{
			cout << "You will be simulating a " << numSets;
			cout << "-way Set Associated Cache" << endl;
		}
		cout << "With a size of " << numSets * numBlocks * blockSize << "bytes" << endl;

		cout << "\nMake sure you put the trace file in the trace folder." << endl;
		cout << "Please type the name of the trace file you would like to input:" << endl;
		cin >> fileName;
		ifstream traceFile("trace/" + fileName + ".trace");


		cacheSim cache(numSets, numBlocks, blockSize);

		clk = clock();

		while (!traceFile.eof())
		{
			string shit;
			char op;
			string ads;
			getline(traceFile, shit);
			if (shit.empty())
			{
				break;
			}
			op = shit.at(0);
			for (unsigned int i = 2; i < 12; i++)
			{
				ads += shit.at(i);
			}
			cache.processAddress(op, ads);
		}

		clk = clock() - clk;
		cout << "\nHit Rate: ";
		cout << cache.getHitRate() << endl;
		float missRate = 1 - cache.getHitRate();
		cout << "Miss Rate: " << missRate << endl;
		cout.precision(4);
		cout << "It took " << (double)clk / CLOCKS_PER_SEC << "s to finish going through the instructions" << endl;

		cout << "\n\nWould you like to continue (enter y for yes, enter n for otherwise)" << endl;
		cin >> leave;

	}
	return 0;
}