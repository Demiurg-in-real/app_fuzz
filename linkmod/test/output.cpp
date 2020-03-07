#include "output.h"
#include "test.h"
react::react(std::string way){ where_the_log = way;}
void react::seterror(std::string from, std::string what, std::string way){
	er.module = from;
	er.error = what;
	getcwd(Way, PATH_MAX);
	chdir(way.c_str());
	outwrite(true);
	chdir(Way);
	int i = (unsigned long)7;
	memset(Way, 0 , PATH_MAX);
}
void react::success(std::string way){
	getcwd(Way, PATH_MAX);
	chdir(way.c_str());
	outwrite(false);
	chdir(Way);
	memset(Way,0,PATH_MAX);
}
void react::outwrite(bool heh){
	std::fstream director("result.txt", director.out | director.trunc);
	if(heh) director<<"Step - "<<er.module<<std::endl<<"What happened - "<<er.error<<std::endl;
	else director<<"Success!"<<std::endl;
	director.close();
}	

