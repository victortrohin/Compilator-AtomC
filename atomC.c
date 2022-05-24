//Trohin Victor
#include "analizorSintactic.h"

int main(){
    const char * pathTestFile = "testAlex.c";
    readFromFile(pathTestFile);
    int codeTk = 0;
    do{
        codeTk = getNextToken();
    }while(codeTk != END);
	showAtoms();
	printf("_______________________________________________________________________________________________________________\n");
	unit();
	eliberareMemorie(tokens);
	return 0;
}
