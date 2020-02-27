#include<iostream>
#include<vector>
#include<string>
#include<unistd.h>
#include<fcntl.h>
#include<stdint.h>
#include<cstring>
#include<filesystem>
#include<sys/stat.h>
#include<sys/types.h>
#include<fstream>
using namespace std;
using namespace filesystem;
extern char** environ;
class gitparse{
	public:
		void hashparse(string way);
		void getinfo();
		int commitparse(string kek){return 0;};
		gitparse();
		~gitparse();
	private:
		string answer;
		string target;
		string compiler;
		string input_method;
		string hash;
		string author;
		char* buf;
		char** search;
		int pip[2];
		pid_t check;
};
gitparse::gitparse(){
	buf = new char[256*256];
	search = new char*[5];
	search[0] = const_cast<char*>("git");
	for(int8_t i = 1; i<5; i++) search[i]=NULL;
}
gitparse::~gitparse(){
	delete buf;
	delete search;
	buf = NULL;
	search = NULL;
}
void gitparse::getinfo(){
	if(pipe(pip) == (-1)){
		perror("pipe");
		exit(-1);
	}
	check = vfork();
	switch(check){
		case -1:
			perror("vfork");
			exit(-1);
		case 0:
			dup2(pip[1],1);
			if(close(pip[0]) == -1){
				perror("close");
				exit(-1);
			}
			execvpe(search[0], search, environ);
			perror("execve");
		default:
			usleep(100);
			answer.clear();
			memset(buf,0,256*256);
			while(read(pip[0], buf,256*256) == 0);
			answer=buf;
			if( close(pip[0]) == (-1) || close(pip[1]) == (-1)){
				perror("close");
				exit(-1);
			}
	}
}


void gitparse::hashparse(string way){
	vector<string>param(0);
	vector<string>cases(0);
	string now;
	string hashres;
	now.resize(256); // чтобы предотвратить учечку памяти
	now=getcwd(NULL,256);
	if(chdir(way.c_str()) == (-1))perror("chdir");
	search[1] = const_cast<char*>("log");
	search[2] = const_cast<char*>("-1");
	getinfo();
	commitparse(answer);//получение target, cases,param,input_method etc
	hash = answer.substr(answer.find("commit ")+7, 40);
	search[1] = const_cast <char*>("cat-file");
	search[2] = const_cast <char*>("-p");
	search[3] = const_cast <char*>(hash.c_str());
	getinfo();
	hash = answer.substr(answer.find("tree ")+5, 40);
	search[3] = const_cast<char*>(hash.c_str());
	getinfo();
	hashres = hash;
	hash = answer.substr(answer.find(target) -41,40);
	search[3] = const_cast<char*> (hash.c_str());
	getinfo();
	mkdir(("/tmp/"+hashres).c_str(), S_ISUID|S_IRWXU|S_IRGRP|S_IWGRP);//имя папки hashres - заменится на другое, возможно - на читаемое из конфигов
	mkdir(("/tmp/"+hashres+"/in").c_str(), S_ISUID|S_IRWXU|S_IRGRP|S_IWGRP);
	fstream f(("/tmp/"+hashres+"/"+target), f.out|f.trunc);
	f<<answer;
	f.close();
	for(int i=0; i<cases.size(); i++){
		search[3] = const_cast<char*>(hashres.c_str());
		getinfo();
		hash = answer.substr(answer.find(cases[i].c_str())-41,40);
		search[3] = const_cast<char*>(hash.c_str());
		getinfo();
		fstream f("/tmp/"+hashres+"/in/"+cases[i], f.out);
		f<<answer;
		f.close();
	}
	//тут должна происходить компияция с передачей параметров, а далее фазинг по аналогичной схеме
	//при сборке из модулей программы - так и произойдёт
	chdir(now.c_str());
}
int main(){
	gitparse kek;
	kek.hashparse("");
	return 0;
}
