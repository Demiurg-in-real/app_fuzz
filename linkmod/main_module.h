#ifndef _COMPILE_MODULE_H_
#define _COMPILE_MODULE_H_
#include<string>
#include<vector>
using namespace std;
void handler(int sig);
class ordprgr{
	private:
		int timer = 5;//parameter for compiling wait
		int how_long = 600;//parameter for fuzzing time
		string way_to_afl = "/root/Desktop/AFL/";//without specifying this point, everything breaks down
		string way_to_hong;
		string answer1, answer2;
		char* buf;
		char** parametres_comp;
		char** parametres_fuzz;
		void inline freed(int siz);
		bool result;
	public:
		ordprgr(string inp, vector<string>& param);
		~ordprgr();
		bool compilate(string compiler, string type, string name, vector<string>& param);
		void fuzzing1(string name){}//last github update smth broke in this fuzzer. Can't make anything with it
		bool fuzzing2(string way, string input_method);
};
#endif
