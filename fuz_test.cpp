#include <iostream>
#include<unistd.h>
#include<sys/types.h>

using namespace std;
class run{
	pid_t forpid;
	pid_t parpid;
public:
	int get();
}__attribute__((packed));
int run::get(){

	switch(vfork()){
		case 0:
		execve("b.out", NULL, NULL);
		default:
		return 0;
	}
}
int main(){
	run like;
	like.get();
	cout << "Hello, Brothers!\n";
	return 0;
}
