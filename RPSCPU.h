#include <iostream>
#include <time.h>

using namespace std;

class RPSCPU{
	int pastChoices[1000];
public:
	int randomInput(){
		srand(time(NULL));

		int randNum = (rand() % 3) + 1;

		return randNum;
	}

};