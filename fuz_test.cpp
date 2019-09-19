#include <iostream>
#include<unistd.h>
#include<sys/types.h>

using namespace std;
class run{
	pid_t forpid;
	pid_t parpid;
public:
	void get();
}__attribute__((packed));
void run::get(){

	switch(vfork()){
		case 0:
		cout<<getpid();
		execve("b.out", NULL, NULL);
		default:
		cout << "again";
//		return 0;
	}
}
int main(){
	run like;
	like.get();
	cout << "Hello, Brothers!\n";
	return 0;
}
