#include "spotter_module.h"
#include<unistd.h>
#include<iostream>
#include<vector>
#include<sys/inotify.h>
#include<filesystem>
#include<typeinfo>
#include<cstring>
#include<fcntl.h>
#include<string>
#include<cstdint>
#include<sys/stat.h>
#include<sys/types.h>
#include<fstream>
#include<csignal>
#include<sys/wait.h>
using namespace std;
namespace fs = std::filesystem;

spotter::spotter(string Way){
	cout<<"Initializating Spotter..."<<endl; 
	way = Way;
	wd.resize(1);
	wds.resize(1);
	buf_len = (1*(sizeof(struct inotify_event) + max_size + 1));
	buf = new char[buf_len];
	mainwds = inotify_init1(IN_CLOEXEC);
	wd.at(0) = way;
	wds.at(0) = inotify_add_watch(mainwds, (wd.at(0)).c_str(), IN_CREATE|IN_MOVED_TO);
	parser(way);
	cout<<"Successful init of Spotter!"<<endl;
}
spotter::~spotter(){
	for(unsigned long i = 0; i<wds.size();i++){
		if(inotify_rm_watch(mainwds, wds.at(i)) == (-1)){
			perror("inotify_rm_watch");
			dprintf(2, "Some troubles with removing watch descriptor from inotify fd\n");
		}
	}
	if(close(mainwds) == (-1)){
		perror("close");
	}
	delete [] buf;
	buf = NULL;
}
void spotter::active(){
	int numread;
	char *i;
	struct inotify_event *events;
	while(true){
		numread = read(mainwds, buf,buf_len);
		if(numread == (-1)){
			perror("read");
			exit(EXIT_FAILURE);
		}
		if(numread > 0){
			for( i = buf; i != (buf+numread);){
				events = reinterpret_cast<struct inotify_event*>(i);
				cout<<events->name<<endl;	
				cout<<"Changes found in the directory - "<<(events->wd)<<" with wd - "<<wd.at(search(events->wd))<<endl;
				if( (events->wd) == wds.at(0)) parser(way);
				else{
					cout<<"Changes found in the repository - "<<wd.at(search(events->wd))<<" with wd - "<<events->wd<<endl;
					sleep(1);
					gitparse kek;
					kek.hashparse(wd.at(search(events->wd)));
				}
				i+= sizeof(struct inotify_event) + events->len;
			}
		}
	}
}


void spotter::parser(string targ){
	fs::directory_entry p(targ);
	while(fs::is_empty(p.path())) sleep(1);
	fs::directory_iterator f(p);
	for(f = fs::begin(f);f!=fs::end(f);f++) {
		if(fs::is_directory((*f).path()) && search(static_cast<string>((*f).path().c_str())+"/objects")){
			addcheck(static_cast<string>((*f).path().c_str()));
			cout<<"Working with directory - "<<(*f)<<endl;
		}
	}
}
bool spotter::addcheck(string name){
	wd.resize(wd.size()+1);
	wds.resize(wds.size()+1);
	wd.at(wd.size()-1) =  name + "/objects";
	cout<<"Adding object - "<<wd.at(wd.size() - 1)<<"..."<<endl;;
	wds.at(wds.size()-1) = inotify_add_watch(mainwds, (wd.at(wd.size() -1)).c_str(), IN_CREATE|IN_MOVED_TO);
	if(wds.at(wds.size()-1) == (-1)){
		cout<<"Fail!"<<endl;
		wds.resize(wds.size()-1);
		wd.resize(wd.size()-1);
		perror("inotify_add_watch");
		return false;
	}
	else {
		cout<<"Successfull! Watch descriptor of element and its name: "<<endl<<wds.at(wds.size()-1)<<" - "<<wd.at(wd.size()-1)<<endl;
		cout<<(wds.size()-1)<< " - " << (wd.size()-1)<<endl;
		cout<<"Resizing buf..."<<endl;
		delete [] buf;
		buf_len = ((wds.size()-1) * (sizeof(struct inotify_event) * max_size + 1));
		buf = new char[buf_len];
		if( buf != NULL)cout<<"Succesfull!"<<endl;
		else cout<<"Fail! Buf was not resized!"<<endl;
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
		if( numer == (*i) ) return ((*i)-1);//костыль....
	}
}

