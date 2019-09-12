#include<iostream>
using namespace std;
class test {
	char str;
public:
	int vivod(char *s);
} __attribute__((packed));
int test::vivod(char* s){
//	cin<<str;
	cout << " Kek " << s;
	printf("%s\n",s);
	return 0;
}

int main(){
	test kek;
	char *st="Hi!";
	kek.vivod( st );
	cout << "Hm...";
	return 0;
}
