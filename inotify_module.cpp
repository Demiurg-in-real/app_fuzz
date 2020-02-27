#include<unistd.h>
#include<iostream>
#include<vector>
#include<sys/inotify.h>
#include<filesystem>
#include<typeinfo>
#include<cstring>
#include<fcntl.h>
using namespace std;
namespace fs = std::filesystem;
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
		vector<string> wd;
		string way;
		char *buf;
		int dream = 20; // how long to sleep
		int buf_len;
		int max_size = 256; // max len of way 
};
spotter::spotter(string Way){
	way = Way;
	wd.resize(2);
	wds.resize(2);
	buf_len = (1*(sizeof(struct inotify_event) + max_size + 1));
	buf = new char[buf_len];
	wds.at(0)= inotify_init1(/*IN_NONBLOCK |*/ IN_CLOEXEC);
//	fcntl(wds.at(0), F_SETFL, O_NONBLOCK);
	wd.at(1) = way;
	wds.at(1) = inotify_add_watch(wds.at(0), (wd.at(1)).c_str(), IN_CREATE|IN_MOVED_TO);
	parser(way);
}
spotter::~spotter(){
	for(int i = 1; i<wds.size();i++){
		if(inotify_rm_watch(wds.at(0), wds.at(i)) == (-1)){
			perror("inotify_rm_watch");
			dprintf(2, "Some troubles with removing watch descriptor from inotify fd\n");
		}
	}
	if(close(wds.at(0)) == (-1)){
		perror("close");
	}
	delete [] buf;
	buf = NULL;
}
void spotter::active(){
	int num;
	char *i;
	struct inotify_event *events;
	while(true){
		sleep(dream);//если read блокируется в ожидании информации, то есть ли смысл вообще "спать"?
		num = read(wds.at(0), buf,buf_len);//стоит ли убирать обработку ошибок? он ведь на этом дескрипторе 100% выкинет EAGAIN
		if(num == (-1)){
			perror("read");
			exit(EXIT_FAILURE);
		}
		if(num > 0){
			for( i = buf; i < buf+num;){
				events = reinterpret_cast<struct inotify_event*>(i);
				if( (events->wd) == wds.at(0)) parser(way);
				i += sizeof(struct inotify_event) + events->len;
			}
//			return;
		}
	}
}


void spotter::parser(string targ){
	fs::directory_entry p(targ);
	while(fs::is_empty(p.path())) sleep(1);
	fs::directory_iterator f(p);
	f = fs::begin(f);
	int i=0;
	for(f;f!=fs::end(f);f++) /*if(*f == wds.....)*/{
		if(fs::is_directory((*f).path()) && search(static_cast<string>((*f).path().c_str())+"/objects"))addcheck(static_cast<string>((*f).path().c_str()));
	}
//	for(auto/*fs::directory_entry*/ p :fs::directory_iterator(targ.c_str())){
//		cout<<p.path()<<endl<<i<<endl;
//		i++;
//	}
}
bool spotter::addcheck(string name){
	wd.resize(wd.size()+1);
	wds.resize(wds.size()+1);
	wd.at(wds.size()-1) =  name + "/objects";
	wds.at(wds.size()-1) = inotify_add_watch(wds.at(0), (wd.at(wd.size() -1)).c_str(), IN_CREATE|IN_MOVED_TO);
	if(wds.at(wds.size()-1) == (-1)){
		perror("inotify_add_watch");
		return false;
	}
	else {
		delete [] buf;
		buf_len = ((wds.size()-1) * (sizeof(struct inotify_event) * max_size + 1));
		buf = new char[buf_len];
		return true;
	}
}
bool spotter::search(string name){
	for(auto i = wd.begin(); i!=wd.end();i++){
		if (*i == name) return false;
	}
	return true;
}
int spotter::search(int numer){
	for(auto i = wds.begin(); i != wds.end(); i++){
		if( numer == (*i) ) return (*i);
	}
}


int main(){
	spotter kek("");
	kek.active();
	return 0;
}
