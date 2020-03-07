#ifndef _SPOT_H_
#define _SPOT_H_
#include<vector>
#include<string>
#include "parse_module.h"
#include "main_module.h"
using namespace std;
class spotter{
	public:
		spotter(string Way);
		void active();
		~spotter();
	private:
		void go(){};
		void parser(string targ);
		void check(){};
		bool addcheck(string name);
		bool search(string name);
		int search(int numer);
		vector<int> wds;
		int mainwds;
		vector<string> wd;
		string way;
		char *buf;
		int dream = 2; // how long to sleep
		int buf_len;
		int max_size = 256; // max len of way 
};
#endif
