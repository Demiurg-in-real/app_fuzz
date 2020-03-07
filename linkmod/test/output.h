#ifndef OUTPUT_MESS
#define OUTPUT_MESS
#include<iostream>
#include<string>
#include<limits.h>
#include<cstring>
#include<unistd.h>
#include<fstream>
//#include "config.h"
struct kuku{
	std::string way;
}config;
#include "test.h"
//string kuku = "/root/Desktop/app_fuzz/linkmod/test";
class react{
	private:
		struct errs{
			std::string module;
			std::string error;
		} er;
		void outwrite(bool heh);
		char Way[PATH_MAX];
		std::string where_the_log;
	public:
		react(std::string way);
		void seterror(std::string from, std::string what, std::string way);
		void success(std::string way);
		void log(std::string message);
} resur(config.way);//gg wp
//#include "output.tpp"
#endif
