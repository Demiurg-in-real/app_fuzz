#include<iostream>
#include<vector>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<errno.h>
#include<csignal>
#include<fcntl.h>
#include<sys/inotify.h>

using namespace std; 
int kuku;
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
		void fuzzing1(string name){}//last github update smth broke in this fuzzer. Can't make anything with it
		void fuzzing2(string name, string way_cases, string way_output);
};

ordprgr::ordprgr(int n, int m, char** fir){
	size = n+1;
	parametres_comp = new char*[n+2];
	parametres_fuzz = new char*[m];
	buf = new char[256*256];
	parametres_comp[0] = NULL;
	parametres_comp[n+1] = NULL;
	for(int i = 1; i<n+1; i++) parametres_comp[i] = fir[i-1];
}

ordprgr::~ordprgr(){
	delete [] parametres_comp;
	delete [] parametres_fuzz;
	delete [] buf;
	parametres_comp = NULL;
	parametres_fuzz = NULL;
	buf = NULL;
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
			if( answer1.find("error") < answer1.size() || answer2.find("error") < answer2.size()){
				return false;
			}
			else{
				return true;
			}
	}
}

void ordprgr::fuzzing2(string name, string way_cases, string way_output){
	int pip1[2];
	int pip2[2];
	pid_t check;

	if(pipe(pip1) == (-1) || pipe(pip2) == (-1)){
		perror("pipe");
		exit(EXIT_FAILURE);
	}

	delete [] parametres_fuzz;
	first = way_to_afl + "afl-fuzz";

	parametres_fuzz = new char* [9];
	parametres_fuzz[0] = const_cast<char*>(first.c_str());
	parametres_fuzz[1] = const_cast<char*>("-i");
	parametres_fuzz[2] = const_cast<char*>(way_cases.c_str()); // here will be the way to cases
	parametres_fuzz[3] = const_cast<char*>("-o");
	parametres_fuzz[4] = const_cast<char*>(way_output.c_str()); // here will be the way to output
	parametres_fuzz[5] = const_cast<char*>("--");
	parametres_fuzz[6] = const_cast<char*>(name.c_str());
	parametres_fuzz[7] = const_cast<char*>("@@");
	parametres_fuzz[8] = NULL;

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
			cout<<"hi!"<<endl;
			fcntl(pip2[0],F_SETFL, O_NONBLOCK);
			kuku=check;
			for (int i = 0; i<60*0.1; i++){
				sleep(1);
				read(pip2[0], buf, 256*256-1);
				answer1 = buf;
				read(pip1[0], buf, 256*256-1);
				answer2 = buf;
				cout<<answer1<<" - "<<i<<endl;
				if(answer1.find("1 uniq crash") <= answer1.size() || answer2.find("error") <= answer2.size()){
					kill(check,15);
					break;
				}
			}
			kill(check,15);
			cout<<"hey!"<<endl;
			if(close(pip1[1]) == (-1) || close(pip1[0]) == (-1) || close(pip2[1]) == (-1) || close(pip2[0]) == (-1)){
				perror("close");
				std::cout<<"Some troubles with closing of pipe"<<std::endl;
			}
	}
}

class vatson{
	public:
		vatson();
		~vatson();
		void check();
	private:
		vector<int>buf;
		inotify_event *krek;
};

vatson::vatson(){
	buf.resize(2);
	buf[0] = inotify_init();
	if(buf[0] == (-1)){
		perror("inotify_init");
		exit(EXIT_FAILURE);
	}
}

vatson::~vatson(){
	for(int i=buf.size()-1;i == (-1) ; i--){
	       inotify_rm_watch(buf[0], buf[i]);
	}	       
	buf.clear();
}
void vatson::check(){
	char buffer[sizeof(struct inotify_event)+256+1];
	buf[1] = inotify_add_watch(buf[0], "", IN_CREATE | IN_DELETE);
	if(buf[1] == (-1)){
		perror("inotify_add_watch");
	}
	cout<<read(buf[0], buffer, sizeof(struct inotify_event)+256)<<endl;
	krek = reinterpret_cast<inotify_event*>(buffer);
	if(krek->mask == IN_CREATE)cout<<"kek";
}

void handler(int sig){//временная мера, чтобы убивать запущенное приложение
	kill(kuku,15);
	exit(-1);
}

int main(int argc, char* argv[]){
	signal(SIGTERM, handler);
	ordprgr mem(0,0,argv);
	if(mem.compilate("","afl")){
		cout<<"No\n";
	}
	else{
		cout<<"Yes\n";
	}
	vatson kkk;
	kkk.check();
//	mem.fuzzing2("","","");
	return 0;
}
