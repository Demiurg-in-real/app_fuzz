#include<iostream>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<errno.h>
#include<csignal>

using namespace std; 

class ordprgr{
	private:
		string way_to_afl = "";
		string way_to_hong;
		string cc="gcc", pp="g++";
		string first;
		string answer1, answer2;
		char* buf;
		char** parametres_comp;
		char** parametres_fuzz;
		int size;
	public:
		ordprgr(int n, int m, char** fir);
		~ordprgr();
		bool compilate(string name, string type);
		void fuzzing(string name, string type);
};

ordprgr::ordprgr(int n, int m, char** fir){
	size = n+1;
	parametres_comp = new char*[n+2];
//	parametres_fuzz = new char*[m];
	buf = new char[256*256];
	parametres_comp[0] = NULL;
	parametres_comp[n+1] = NULL;
	for(int i = 1; i<n+1; i++) parametres_comp[i] = fir[i-1];
}

ordprgr::~ordprgr(){
	delete [] parametres_comp;
//	delete [] parametres;
	delete [] buf;
}

bool ordprgr::compilate(string name, string type){
	int pip1[2];
	int pip2[2];
	pid_t check;

	if(pipe(pip1) == (-1) || pipe(pip2) == (-1)){
		perror("pipe");
		exit(EXIT_FAILURE);
	}

	if(type == "hong"){
		if(name.find(".cpp") < name.size()) first = way_to_hong + "hfuzz-cc/hfuzz-" + pp;
		else first = way_to_hong + "hfuzz-fcc/hfuzz-" + cc;
	}
	if(type == "afl"){
		if(name.find(".cpp") < name.size()) first = way_to_afl + "afl-" + pp;
		else first = way_to_afl + "afl-" + cc;
	}
	cout<<first<<endl<<name<<endl;
	parametres_comp[0] = const_cast<char*>(first.c_str());
	parametres_comp[size] = const_cast<char*>(name.c_str());
	cout<<parametres_comp[size]<<endl;
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
			sleep(2);
			read(pip1[0], buf, 256*256-1);
			answer1 = buf;
			read(pip2[0], buf, 256*256-1);
			answer2 = buf;
			cout<<answer1<<endl<<answer2;
			if( answer1.find("error") < (256*256) || answer2.find("error") < (256*256)){
				return false;
			}
			else{
				return true;
			}
	}
}


int main(int argc, char* argv[]){
	ordprgr mem(0,0,argv);
	if(mem.compilate("","")){
		cout<<"No\n";
	}
	else{
		cout<<"Yes\n";
	}
	return 0;
}
