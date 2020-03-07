#include "parse_module.h"
#include "main_module.h"
#include<signal.h>
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

string lasthash = "";
gitparse::gitparse(){
	param.resize(0);
	cases.resize(0);
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
	signal(SIGTERM, handler);
	cout<<"Begin of hashparse..."<<endl;
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
	if(hash == lasthash) {
		cout<<"It is the same commit< that was behind. Skipping"<<endl;
		return;
	}
	else lasthash = hash;
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
	cout<<hashres<<" - it is the name of directory with files"<<endl;
	mkdir(("/tmp/"+hashres).c_str(), S_ISUID|S_IRWXU|S_IRGRP|S_IWGRP);//имя папки hashres - заменится на другое, возможно - на читаемое из конфигов
	mkdir(("/tmp/"+hashres+"/in").c_str(), S_ISUID|S_IRWXU|S_IRGRP|S_IWGRP);
	fstream f(("/tmp/"+hashres+"/"+target), f.out|f.trunc);
	f<<answer;
	f.close();
	for(unsigned long i=0; i<cases.size(); i++){
		cout<<"Writing case number..."<<i<<endl;
		search[3] = const_cast<char*>(hashres.c_str());
		getinfo();
		hash = answer.substr(answer.find(cases[i].c_str())-41,40);
		search[3] = const_cast<char*>(hash.c_str());
		getinfo();
		cout<<"Writing cases..."<<answer<<endl;
		fstream fe(("/tmp/"+hashres+"/in/"+cases[i]), fe.out|fe.trunc);
		fe<<answer;
		fe.close();
	}
	cout<<"Target = "<<target<<endl<<"Compiler - "<<compiler<<endl<<"Input method - "<<input_method<<endl;
	cout<<"Params:"<<endl;
	for(unsigned long i=0; i<param.size();i++) cout<<param[i]<<endl;
	cout<<"Cases:"<<endl;
	for(unsigned long i=0; i<cases.size();i++) cout<<cases[i]<<endl;
	chdir(("/tmp/"+hashres).c_str());//для того, чтобы фаззер работал в директории с распарсенным проектом
	ordprgr finaltest(input_method, param);
	if(finaltest.compilate(compiler,"afl",target , param)){
		finaltest.fuzzing2(("/tmp/"+hashres), input_method);
	}
	else cout<<"Troubles with compiling..."<<endl;
	//при сборке из модулей программы - так и произойдёт
	chdir(now.c_str());
}

void gitparse::raspar(string& che, string khe,vector<string>& kek){
	string sub = che.substr(che.find(khe)+khe.size(), che.find(";")-khe.size());
	che.erase(che.find(khe),che.find(";")+1);
	int i = 0;
	while(sub.find(",") != string::npos){
		kek.resize(kek.size()+1);
		kek[i] = sub.substr(0,sub.find(","));
		sub.erase(0,kek[i].size()+1);
	//	cout<<kek[i]<<endl;
		i++;
	}
	kek.resize(kek.size()+1);
	kek[i] = sub;
}
int gitparse::commitparse(string kuku){
	kuku.erase(0,kuku.find("compiler"));
	while(kuku.find(" ") != string::npos)kuku.erase(kuku.find(" "),1); // clear from spaces
	compiler = kuku.substr(kuku.find("compiler=")+9, kuku.find(";")-9);
	kuku.erase(kuku.find("compiler="), kuku.find(";")+1);
	raspar(kuku,"parametres=",param);
	for(unsigned long i=0; i<param.size();i++){
		cout<<param[i]<<endl;
	}
	target = kuku.substr(kuku.find("target=")+7, kuku.find(";")-7);
	kuku.erase(kuku.find("target="), kuku.find(";")+1);
	raspar(kuku,"case=",cases);
	input_method = kuku.substr(kuku.find("input_method=")+13, kuku.find(";")-13);
	kuku.erase(kuku.find("input_method="), kuku.find(";")+1);
	return 0;
}

