//Trohin Victor
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <ctype.h>

enum{ID,CT_INT, CT_REAL, COMMA, SEMICOLON, LPAR, RPAR, LBRACKET,
	RBRACKET, LACC, RACC, END, ADD, SUB, MUL, DIV, DOT, OR, AND, NOT, ASSIGN,
	EQUAL, NOTEQ, LESS, LESSEQ, GREATER, GREATEREQ, CT_CHAR, CT_STRING,
	BREAK, CHAR, DOUBLE, ELSE, FOR, IF, INT, RETURN, STRUCT, VOID, WHILE,
	LINECOMMENT};
char *stringAtoms[] = {"ID","CT_INT", "CT_REAL", "COMMA", "SEMICOLON", "LPAR",
	"RPAR", "LBRACKET", "RBRACKET", "LACC", "RACC", "END", "ADD", "SUB", "MUL", "DIV",
	"DOT", "OR", "AND", "NOT", "ASSIGN", "EQUAL", "NOTEQ", "LESS", "LESSEQ", "GREATER",
	"GREATEREQ", "CT_CHAR", "CT_STRING", "BREAK", "CHAR", "DOUBLE", "ELSE", "FOR",
	"IF", "INT", "RETURN", "STRUCT", "VOID", "WHILE", "LINECOMMENT"};

void err(const char *fmt, ...);
#define SAFEALLOC(var,Type) if((var=(Type*)malloc(sizeof(Type)))==NULL)err("not enough memory");

typedef struct _Token{
	int code; 					// codul (numele)
	union{
		char *text; 			// folosit pentru ID, CT_STRING (alocat dinamic)
		int i; 					// folosit pentru CT_INT, CT_CHAR
		double r; 				// folosit pentru CT_REAL
	};
	int line; 					// linia din fisierul de intrare
	struct _Token *next; 		// inlantuire la urmatorul AL
}Token;

Token* tokens = NULL;
Token* lastToken = NULL;
int globalLine = 1;
char buffer[4097];
char *pCrtCh = buffer;

void eliberareMemorie(Token * head)		//functie de eliberare a memoriei
{
   Token* tmp;

   while (head != NULL)
    {
		if(tmp->text != NULL)			//se elibereaza si zona de memorie alocata de String
			free(tmp->text);
       	tmp = head;
       	head = head->next;
       	free(tmp);
    }

}

Token *addTk(int code){
	Token *tk;
	SAFEALLOC(tk,Token)
	tk->code=code;
	tk->line=globalLine;
	tk->next=NULL;
	if(lastToken){
		lastToken->next=tk;
	}else{
		tokens=tk;
	}
	lastToken=tk;
	return tk;
}

void err(const char *fmt,...){
	va_list va;
	va_start(va,fmt);
	fprintf(stderr,"error: ");
	vfprintf(stderr,fmt,va);
	fputc('\n',stderr);
	va_end(va);
	exit(-1);
}

char* createString(const char *start, const char *end){
	int const len = end - start;
	char *createdString = (char*)malloc(sizeof(char)*(len + 1));
	strncpy(createdString, (start), (len));
	createdString[len] = '\0';
	return createdString;
}

void showAtoms(){
	Token *tk = tokens;
	while (tk != NULL){
		printf("%d %s ",tk->line, stringAtoms[tk->code] );
		if(tk->code == ID)
			printf("%s\n", tk->text);
		else if(tk->code == CT_INT)
			printf("%d\n", tk->i);
		else if(tk->code == CT_REAL)
			printf("%f\n", tk->r);
		else if(tk->code == CT_STRING)
			printf("%s\n", tk->text);
		else if(tk->code == CT_CHAR)
			printf("%c\n", tk->i);
		else	
			printf("\n");
		tk = tk->next;
	}

}

void tkerr(const Token *tk,const char *fmt,...){
	va_list va;
	va_start(va,fmt);
	fprintf(stderr,"error in line %d: ",tk->line);
	vfprintf(stderr,fmt,va);
	fputc('\n',stderr);
	va_end(va);
	exit(-1);
}

void readFromFile(const char* file){
    FILE *ptr = fopen(file, "rt");

    if(ptr == NULL){
        printf("Error with file");
        exit(1);
    }

    char str[100];
    while ( fgets(str, 100, ptr) != NULL){
        strcat(buffer, str);
    }
    fclose(ptr);
}

int getNextToken()
{
	int state=0,nCh;
	char ch;
	char* aux;
	const char *pStartCh;
	Token *tk;
	for(;;){ 								// bucla infinita
	ch=*pCrtCh;
	switch(state){
		case 0:
			if(ch>='0' && ch<='9'){				// testare tranzitii posibile din starea 0
				pStartCh = pCrtCh;
				pCrtCh++;
				state = 1;
			}else if(ch == '\''){ 			// CT_CHAR
				pCrtCh++;
				state = 12;
			}else if(ch == '"'){			//CT_STRING
				pStartCh = pCrtCh;
				pCrtCh++;			
				state = 15;
			}else if(ch == ','){			//COMMA
				pCrtCh++;
				state = 17;
			}else if(ch == ';'){			//SEMICOLON
				pCrtCh++;
				state = 18;
			}
			else if(ch == '('){				//LPAR
				pCrtCh++;
				state = 19;
			}else if(ch == ')'){				//RPAR
				pCrtCh++;
				state = 20;
			}else if(ch == '['){				//LBRACKET
				pCrtCh++;
				state = 21;
			}else if(ch == ']'){				//RBRACKET
				pCrtCh++;
				state = 22;
			}else if(ch == '{'){				//LACC
				pCrtCh++;
				state = 23;
			}else if(ch == '}'){				//RACC
				pCrtCh++;
				state = 24;
			}else if(ch == '+'){				//ADD
				pCrtCh++;
				state = 26;
			}else if(ch == '-'){				//SUB
				pCrtCh++;
				state = 27;
			}else if(ch == '*'){				//MUL
				pCrtCh++;
				state = 28;
			}else if(ch == '/'){				//DIV
				pCrtCh++;
				state = 48;
			}else if(ch == '.'){				//DOT
				pCrtCh++;
				state = 30;
			}else if(ch == '&'){				//AND
				pCrtCh++;
				state = 31;
			}else if(ch == '|'){				//OR
				pCrtCh++;
				state = 33;
			}else if(ch == '!'){				//NOT or NOTEQ
				pCrtCh++;
				state = 35;
			}else if(ch=='='){					//EQUAL or ASSIGN
				pCrtCh++;
				state=38;
			}else if(ch=='<'){					//LESS or LESSEQ
				pCrtCh++;
				state=41;
			}else if(ch=='>'){					//GREATER or GREATEREQ
				pCrtCh++;
				state=44;
			}else if((ch >= 'A' && ch <= 'Z') || (ch>='a' && ch <= 'z') ||ch=='_'){
				pStartCh=pCrtCh; 			// memoreaza inceputul ID-ului
				pCrtCh++; 					// consuma caracterul
				state=52; 					// trece la noua stare
			}else if(ch==' '||ch=='\r'||ch=='\t'){
				pCrtCh++; 					// consuma caracterul si ramane in starea 0
			}
			else if(ch=='\n'){ 				// tratat separat pentru a actualiza linia curenta
				globalLine++;
				pCrtCh++;
			}
			else if(ch=='\0'){ 				// sfarsit de sir
				addTk(END);
				return END;
			}
			else
				tkerr(addTk(END),"caracter invalid");
			break;
		case 1:
			if(ch>='0' && ch<='9')
				pCrtCh++;
			else if(ch == '.'){
				pCrtCh++;
				state = 3;
			}
			else if(ch == 'e' || ch == 'E'){
				pCrtCh++;
				state = 5;
			}
			else
				state=2;
			break;
		case 2:
			nCh = pCrtCh - pStartCh;
			tk=addTk(CT_INT);
			aux = createString(pStartCh,pCrtCh); 
			tk->i=atoi(aux);
			free(aux);
			return tk->code;
		case 3:
			if(ch>='0' && ch<='9'){
				pCrtCh++;
				state = 4;
			}
			else
				tkerr(addTk(END), "este nevoie macar de o cifra dupa punct");
		break;
		case 4:
			if(ch>='0' && ch<='9')
				pCrtCh++;
			else if(ch == 'e' || ch == 'E'){
				pCrtCh++;
				state = 5;
			}else
				state = 8;
		break;
		case 5:
			if(ch == '+' || ch == '-'){
				pCrtCh++;
				state = 6;
			}else
				state = 6;
		break;
		case 6:
			if(ch>='0' && ch<='9'){
				pCrtCh++;
				state = 7;
			}else
				state = 8;
		break;
		case 7:
			if(ch>='0' && ch<='9'){
				pCrtCh++;
				state = 8;
			}
			else
				state = 8;
		break;		
		case 8:
			nCh = pCrtCh - pStartCh;
			tk=addTk(CT_REAL);
			aux = createString(pStartCh, pCrtCh);
			tk->r=atof(aux);
			free(aux);
			return tk->code;
		case 12:
			if(ch != '\''){
				pCrtCh++;
				state = 13;
			}else
				tkerr(addTk(END), "CHAR nu poate contine caracterul [']");
		break;
		case 13:
			if(ch == '\'')
				state = 14;
			else
				tkerr(addTk(END), "CHAR nu poate avea mai mult de un caracter");
		break;
		case 14:
			tk = addTk(CT_CHAR);
			tk->i = *(pCrtCh-1);
			pCrtCh++;
			return tk->code;
		case 15:
			if(ch != '"'){	
				pCrtCh++;		
			}
			else{
				state = 16;
				pCrtCh++;
			}
				
		break;
		case 16:
			nCh = (pCrtCh + 1) - (pStartCh - 1);
			tk=addTk(CT_STRING);
			tk->text= createString(pStartCh + 1,pCrtCh - 1);
			return tk->code;
		case 17:
			addTk(COMMA);
			return COMMA;
		case 18:
			addTk(SEMICOLON);
			return SEMICOLON;
		case 19:
			addTk(LPAR);
			return LPAR;
		case 20:
			addTk(RPAR);
			return RPAR;
		case 21:
			addTk(LBRACKET);
			return LBRACKET;
		case 22:
			addTk(RBRACKET);
			return RBRACKET;
		case 23:
			addTk(LACC);
			return LACC;
		case 24:
			addTk(RACC);
			return RACC;
		case 26:
			addTk(ADD);
			return ADD;
		case 27:
			addTk(SUB);
			return SUB;
		case 28:
			addTk(MUL);
			return MUL;
		case 29:
			addTk(DIV);
			return DIV;
		case 30:
			addTk(DOT);
			return DOT;
		case 31:
			if(ch == '&'){
				pCrtCh++;
				state = 32;
			}else
				tkerr(addTk(END), "Se astepta al doilea caracter '&'");
		break;
		case 32:
			addTk(AND);
			return AND;
		case 33:
			if(ch == '|'){
				pCrtCh++;
				state = 34;
			}else
				tkerr(addTk(END), "Se astepta al doilea caracter '|'");
		break;
		case 34:
			addTk(OR);
			return OR;
		case 35:
			if(ch == '='){
				pCrtCh++;
				state = 37;
			}else
				state = 36;
		break;		
		case 36:
			addTk(NOT);
			return NOT;
		case 37:
			addTk(NOTEQ);
			return NOTEQ;
		case 38:
			if(ch == '='){
				pCrtCh++;
				state=39;
			}
			else
				state=40;
			break;
		case 39:
			addTk(EQUAL);
			return EQUAL;
		case 40:
			addTk(ASSIGN);
			return ASSIGN;
		case 41:
			if(ch == '='){
				pCrtCh++;
				state = 43;
			}else
				state = 42;
		break;
		case 42:
			addTk(LESS);
			return LESS;
		case 43:
			addTk(LESSEQ);
			return LESSEQ;
		case 44:
			if(ch == '='){
				pCrtCh++;
				state = 46;
			}else
				state = 45;
		break;
		case 45:
			addTk(GREATER);
			return GREATER;
		case 46:
			addTk(GREATEREQ);
			return GREATEREQ;
		case 48:
			if(ch == '/'){
				pCrtCh++;
				state = 50;
			}else
				state = 29;

		case 50:
			if(ch != '\n'){
				pCrtCh++; 					// consuma caracterul si ramane in starea 0
			}
			else if(ch=='\n'){ 				// tratat separat pentru a actualiza linia curenta
				globalLine++;
				pCrtCh++;
				state = 51;
			}		
		break;
		case 51:
			state = 0;
		break;
		case 52:
			if((ch >= 'A' && ch <= 'Z') || (ch>='a' && ch <= 'z') || ch=='_')
				pCrtCh++;
			else
				state=53;
			break;
		case 53:
			nCh=pCrtCh-pStartCh; 				// lungimea cuvantului gasit
												// teste cuvinte cheie
			if(nCh==5&&!memcmp(pStartCh,"break",5))
				tk=addTk(BREAK);
			else if(nCh==4&&!memcmp(pStartCh,"char",4))
				tk=addTk(CHAR);
			else if(nCh == 6 && !memcmp(pStartCh, "double", 6))
				tk=addTk(DOUBLE);
			else if(nCh == 4 && !memcmp(pStartCh, "else", 4))
				tk=addTk(ELSE);
			else if(nCh == 3 && !memcmp(pStartCh, "for", 3))
				tk=addTk(FOR);
			else if(nCh == 2 && !memcmp(pStartCh, "if", 2))
				tk=addTk(IF);
			else if(nCh == 3 && !memcmp(pStartCh, "int", 3))
				tk=addTk(INT);
			else if(nCh == 6 && !memcmp(pStartCh, "return", 6))
				tk=addTk(RETURN);
			else if(nCh == 6 && !memcmp(pStartCh, "struct", 6))
				tk=addTk(STRUCT);
			else if(nCh == 4 && !memcmp(pStartCh, "void", 4))
				tk=addTk(VOID);
			else if(nCh == 5 && !memcmp(pStartCh, "while", 5))
				tk=addTk(WHILE);
			else{ 								// daca nu este un cuvant cheie, atunci e un ID
				tk=addTk(ID);
				tk->text=createString(pStartCh,pCrtCh);	
			}
			return tk->code;

		}
	}
}

int main(){
    const char * pathTestFile = "testAlex.c";
    readFromFile(pathTestFile);
    int codeTk = 0;
    do{
        codeTk = getNextToken();
    }while(codeTk != END);
    showAtoms();
	eliberareMemorie(tokens);
	return 0;
}
