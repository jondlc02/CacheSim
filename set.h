#include <string>
#include <iostream>
#include <deque>
using namespace std;

struct SetCache
{
	deque<string> blocks;
	unsigned int setIdentity;
};