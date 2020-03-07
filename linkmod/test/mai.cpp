#include<string>
struct kuku{
	std::string way = "/root/Desktop/app_fuzz/linkmod/test/";
}config;
//#include "config.h"
#include "output.h"
int main(){
	config.way = "/root/Desktop/app_fuzz/linkmod/test";
	resur.success(config.way);
	return 0;
}
