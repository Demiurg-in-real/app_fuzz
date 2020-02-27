#include<unistd.h>
#include<iostream>
#include<cstdio>
#include<string>
#include<cstring>
#include<vector>
#include<sys/types.h>
#include<sys/wait.h>
#include<csignal>
#include<fcntl.h>
#include<stdint.h>
int kuku;
using namespace std;
class ordprgr{
	private:
		int timer = 5;//parameter for compiling wait
		int how_long = 600;//parameter for fuzzing time
		string way_to_afl = ".../AFL/";//without specifying this point, everything breaks down
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

ordprgr::ordprgr(string inp, vector<string>& param){
	param.resize(param.size()+2);
	param[param.size()-2] = "-o";
	param[param.size()-1] = "target";
	parametres_comp = new char*[param.size()+3];
	if( inp == "file") {
		parametres_fuzz = new char*[9];
		parametres_fuzz[8] = NULL;
	}
	else parametres_fuzz = new char*[8];
	buf = new char[256*256];
	parametres_fuzz[7] = NULL;
	parametres_comp[param.size()+2] = NULL;
}

ordprgr::~ordprgr(){
	delete [] parametres_comp;
	delete [] parametres_fuzz;
	delete [] buf;
	buf = NULL;
	parametres_comp = NULL;
	parametres_fuzz = NULL;
}

bool ordprgr::compilate(string compiler, string type, string name, vector<string>& param){
	int pip1[2];
	int pip2[2];

	int size1;

	pid_t check;

	if(pipe(pip1) == (-1) || pipe(pip2) == (-1)){
		perror("pipe");
		exit(EXIT_FAILURE);
	}

	if(type == "hong"){
		compiler = way_to_hong + compiler;
//		if(name.find(".cpp") < name.size()) compiler = way_to_hong + "hfuzz-cc/hfuzz-" + pp;
//		else compiler = way_to_hong + "hfuzz-fcc/hfuzz-" + compiler;
	}
	if(type == "afl"){
		compiler = way_to_afl + "afl-" + compiler;
	}
	parametres_comp[0] = const_cast<char*>(compiler.c_str());
	for(int i=1; i<param.size()+1; i++) parametres_comp[i] = const_cast<char*>(param[i-1].c_str());
	parametres_comp[param.size()+1] = const_cast<char*>(name.c_str());
	check = vfork();
	switch(check){
		case -1:
			perror("vfork");
			exit(EXIT_FAILURE);
		case 0:
			dup2(pip1[1],1);
			dup2(pip2[1],2);
			if( close(pip1[0]) == (-1) || close(pip2[0]) == (-1)){
				perror("close");
				exit(EXIT_FAILURE);
			}
			execve(parametres_comp[0], parametres_comp, environ);
			perror("execve");
			exit(EXIT_FAILURE);
		default:
			if(close(pip1[1]) == (-1) || close(pip2[1]) == (-1) ){
				perror("close");
				std::cout<<"Some troubles with closing of pipe"<<std::endl;
			}
			sleep(timer);
			read(pip1[0], buf, 256*256-1);
			answer1 = buf;
			memset(buf,0,answer1.size());
			read(pip2[0], buf, 256*256-1);
			answer2 = buf;
			memset(buf,0,answer2.size());
			if( answer1.find("error") < answer1.size() || answer2.find("error") < answer2.size()){
				return false;
			}
			else{
				return true;
			}
	}
}

bool ordprgr::fuzzing2(string way, string input_method){
	int pip1[2];
	int pip2[2];
	pid_t check;

	if(pipe(pip1) == (-1) || pipe(pip2) == (-1)){
		perror("pipe");
		exit(EXIT_FAILURE);
	}
	string fuzzer = way_to_afl + "afl-fuzz";
	string input = way + "/in";
	string outpu = way + "/out";
	string targ = way + "/target";
	parametres_fuzz[0] = const_cast<char*>((fuzzer).c_str());
	parametres_fuzz[1] = const_cast<char*>("-i");
	parametres_fuzz[2] = const_cast<char*>(input.c_str()); // here will be the way to cases
	parametres_fuzz[3] = const_cast<char*>("-o");
	parametres_fuzz[4] = const_cast<char*>(outpu.c_str()); // here will be the way to output
	parametres_fuzz[5] = const_cast<char*>("--");
	parametres_fuzz[6] = const_cast<char*>(targ.c_str());
	if(input_method == "file") parametres_fuzz[7] = const_cast<char*>("@@");
	check = vfork();
	switch(check){
		case -1:
			perror("vfork");
			exit(EXIT_FAILURE);
		case 0:
			dup2(pip1[1],1);
			dup2(pip2[1],2);
			if( close(pip1[0]) == (-1) || close(pip2[0]) == (-1)){
				perror("close");
				exit(EXIT_FAILURE);
			}
			execve(parametres_fuzz[0], parametres_fuzz, environ);
			perror("execve");
			exit(EXIT_FAILURE);
		default:
			fcntl(pip1[0], F_SETFL, O_NONBLOCK);
			fcntl(pip2[0],F_SETFL, O_NONBLOCK);
			kuku=check;
			for (int i = 0; i<how_long; i++){
				sleep(1);
				read(pip1[0], buf, 256*256-1);
				answer1 = buf;
				memset(buf,0,answer1.size());
				read(pip2[0], buf, 256*256-1);
				answer2 = buf;
				memset(buf,0,answer2.size());
				if(i==0){
					if(answer1.find("PROGRAM ABORT") <= answer1.size()){
						cout<<"Troubles with starting the fuzzer"<<endl<<answer1<<endl;;
						kill(check,15);
						result = true;
						return result;
					}
				}
				if(answer1.find("1 uniq crashes found") <= answer1.size() || answer2.find("1 uniq crashes found") <= answer2.size()){
					kill(check,15);
					result = true;
					break;
				}
			}
			if(close(pip1[1]) == (-1) || close(pip1[0]) == (-1) || close(pip2[1]) == (-1) || close(pip2[0]) == (-1)){
				perror("close");
				std::cout<<"Some troubles with closing of pipe"<<std::endl;
			}
			if(result){
				kill(check,15);
				result=false;
				return true;
			}
			return false;
	}
}

void handler(int sig){
	cout<<strerror(sig);
	kill(kuku, SIGTERM);
	exit(0);
}

int main(){
	signal(SIGTERM, handler);
	signal(SIGSEGV, handler);
	vector<string> heh(2);
	heh[0] = "-std=c99";
	heh[1] = "-pthread";
	string compiler = "gcc";
	string name = "writer.c";
	string input_method = "file";
	ordprgr kek(input_method, heh);
	char way[256];
	getcwd(way,256);
	string Way = way;
	cout<<Way<<endl;
	if(kek.compilate(compiler, "afl", name, heh)){
		cout<<"Succes compile!"<<endl;
		if(kek.fuzzing2(Way, input_method)) cout<<"Fuzzer found something!\n";
		else cout<<"Nothing was found..."<<endl;
	}
	else{
		cout<<"Some troubles..."<<endl;
	}
	return 0;
}
