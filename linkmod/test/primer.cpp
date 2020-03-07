#include<iostream>
#include<stdio.h>
template<typename kek> class Kek{
	public:
	Kek *parent;
	Kek *child;
	int *i;
	Kek(kek re): parent(nullptr), child(){};
};
int main(){
	Kek<int> a(4);
//	int (*s)(void *nul);
//	s(nullptr);
//	*a.i=5;
//	*a.i*=5;
//	printf("%16x\n", a);
	return 0;
}
