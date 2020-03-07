#ifndef _PARSE_MODULE_H_
#define _PARSE_MODULE_H_
#include<sys/types.h>
#include<string>
#include<vector>
using namespace std;
class gitparse{
	public:
		void hashparse(string way);
		void getinfo();
		gitparse();
		~gitparse();
	private:
		string answer;
		string target;
		string compiler;
		string input_method;
		string hash;
		string author;
		vector<string>param;
		vector<string>cases;
		char* buf;
		char** search;
		int pip[2];
		pid_t check;
		int commitparse(string kuku);
		void raspar(string& che, string khe,vector<string>& kek);
};
#endif
