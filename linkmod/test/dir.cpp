#include<dirent.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<fcntl.h>
#include<string>
#include<stdio.h>
int main(){
	std::string way = ("/root/Desktop/app_fuzz/linkmod/");
	DIR *p = opendir(way.c_str());
	if(p == NULL) perror("opendir");
	struct dirent *kek = readdir(p);
	struct stat f;
	while(kek != NULL) {
		stat((way+"/"+kek->d_name).c_str(), &f);
		if(S_ISDIR(f.st_mode) && (static_cast<std::string>(kek->d_name) != "." && static_cast<std::string>(kek->d_name) != ".."))printf("%s\n", kek->d_name);
		kek = readdir(p);
	}
	return 0;
}

