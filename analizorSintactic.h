#include "analizorLexical.h"
bool unit();
bool structDef();
bool varDef();
bool typeBase();
bool arrayDecl();
bool fnDef();
bool exprPostfixPrim();
bool fnParam();
bool stm();
bool stmCompound();
bool expr();
bool exprAssign();
bool exprOr();
bool exprOrPrim();
bool exprAndPrim();
bool exprAnd();
bool exprEq();
bool exprEqPrim();
bool exprRel();
bool exprRelPrim();
bool exprAdd();
bool exprAddPrim();
bool exprMul();
bool exprMulPrim();
bool exprCast();
bool exprUnary();
bool exprPostfix();
bool exprPrimary();

Token *iTk = NULL; // iteratorul în lista de atomi. Inițial pointează la primul atom dinlistă.
Token *consumedTk = NULL; // atomul care tocmai a fost consumat. Va fi folosit în etapele următoare ale compilatorului

const char *tkCodeName(int code){
    return stringAtoms[code];
}
// const char *tkCodeName(int code) - o funcție care primește ca parametru un cod de atom și îi returnează numele

bool consume(int code){
    printf("consume(%s)",tkCodeName(code));
    if(iTk->code==code){
        consumedTk=iTk;
        iTk=iTk->next;
        printf(" => consumed\n");
        return true;
    }
    printf(" => found %s\n",tkCodeName(iTk->code));
    return false;
}




//unit: ( structDef | fnDef | varDef )* END
bool unit(){
    iTk = tokens;
    printf("__unit\n");
    for(;;){ // buclă infinită
        if(structDef()){}
        else if(fnDef()){}
        else if(varDef()){}
        else break; // dacă nu se poate consuma nimic la iterația curentă, se iese din buclă
    }
    if(consume(END)){ // se ajunge la END chiar și dacă nu se consumă nimic în repetiție, deci ea este opțională

    return true;
    }

    tkerr(iTk, "unexpected token");
}

//structDef: STRUCT ID LACC varDef* RACC SEMICOLON
bool structDef(){
    Token *start = iTk;
    printf("__fnParam\n");
    if(consume(STRUCT)){
        if(consume(ID)){
            if(consume(LACC)){
                while(varDef());

                if (consume(RACC)){
                    if(consume(SEMICOLON)){
                        return true;
                    }
                }
                
            }
        }
    }
    iTk = start;
    return false;
}


//varDef: typeBase ID arrayDecl? SEMICOLON
bool varDef(){
    Token *start = iTk;
    printf("__varDef\n");
    if(typeBase()){
        if(consume(ID)){
            if(arrayDecl()){
            }
            if(consume(SEMICOLON)){
                    return true;
                }
        }
    }
    iTk = start;
    return false;
}

// typeBase: INT | DOUBLE | CHAR | STRUCT ID
bool typeBase(){
    Token *start = iTk;
    printf("__typeBase\n");
    if(consume(INT)){
        return true;
    }
    if(consume(DOUBLE)){
        return true;
    }
    if(consume(CHAR)){
        return true;
    }
    if(consume(STRUCT)){
        if(consume(ID)){
            return true;
        }
    }
    iTk = start;
    return false;
}

//arrayDecl: LBRACKET CT_INT? RBRACKET

bool arrayDecl(){
    Token *start = iTk;
    printf("__arrayDecl\n");
    if(consume(LBRACKET)){
        if(consume(CT_INT)){
        }
        if(consume(RBRACKET)){
                return true;
        }
    }
    iTk = start;
    return false;
}


/*
fnDef: ( typeBase | VOID ) ID
            LPAR ( fnParam ( COMMA fnParam )* )? RPAR
            stmCompound
*/
bool fnDef(){
    Token *start = iTk;
    printf("__fnDef\n");
    if(typeBase() || consume(VOID)){
        if(consume(ID)){
            if(consume(LPAR)){
                if(fnParam()){
                    for(;;){
                        if(consume(COMMA)){
                            if(fnParam()){
                                break;
                            }
                        }
                    }
                }else if(consume(RPAR)){
                    if(stmCompound()){
                        return true;
                    }
                }
                if(consume(RPAR)){
                    if(stmCompound()){
                        return true;
                    }
                }
            }
        }
    }
    iTk = start;
    return false;
}


//fnParam: typeBase ID arrayDecl?
bool fnParam(){
    Token *start = iTk;
    printf("__fnParam\n");
    if(typeBase()){
        if(consume(ID)){
            if(arrayDecl()){
                return true;
            }else{
                return true;
            }
        }
    }

    iTk = start;
    return false;
}

/*

stm: stmCompound
    | IF LPAR expr RPAR stm ( ELSE stm )?
    | WHILE LPAR expr RPAR stm
    | FOR LPAR expr? SEMICOLON expr? SEMICOLON expr? RPAR stm
    | BREAK SEMICOLON
    | RETURN expr? SEMICOLON
    | expr? SEMICOLON

*/

bool stm(){
    Token *start = iTk;
    printf("__stm\n");
    if(stmCompound()){
        return true;
    }

    if(consume(IF)){
        if(consume(LPAR)){
            if(expr()){
                if(consume(RPAR)){
                    if(stm()){
                        if(consume(ELSE)){
                            if(stm()){
                                return true;
                            }
                        }

                        return true;
                    }
                }
            }
        }
    }

    if(consume(WHILE)){
        if(consume(LPAR)){
            if(expr()){
                if(consume(RPAR)){
                    if(stm()){
                        return true;
                    }
                }
            }
        }
    }

    if(consume(FOR)){
        if(consume(LPAR)){
            if(expr()){
            }
            if(consume(SEMICOLON)){
                if(expr()){
                }
                if(consume(SEMICOLON)){
                    if(expr()){
                    }
                    if(consume(RPAR)){
                        if(stm()){
                            return true;
                        }
                    }
                }
            }

        }
    }

    if(consume(BREAK)){
        if(consume(SEMICOLON))
            return true;
    }

    if(consume(RETURN)){
        if(expr()){
        }
        if(consume(SEMICOLON)){
            return true;
        }
    }

    if(expr()){
    }
    if(consume(SEMICOLON)){
        return true;
    }
    
    iTk = start;
    return false;
}

//stmCompound: LACC ( varDef | stm )* RACC
bool stmCompound(){
    Token *start = iTk;
    printf("__typeBase\n");
    if(consume(LACC)){
        for(;;){
            if(varDef() || stm()){
            }else{
                break;
            }
        }
        if(consume(RACC)){
            return true;
        }
    }
    iTk = start;
    return false;
}


//expr: exprAssign
bool expr(){
    Token *start = iTk;
    printf("__expr\n");
    if(exprAssign())
        return true;
    iTk = start;
    return false;
}

//exprAssign: exprUnary ASSIGN exprAssign | exprOr
bool exprAssign(){
    Token *start = iTk;
    printf("__exprAssign\n");
    if(exprUnary()){
        if(consume(ASSIGN)){
            if(exprAssign()){
                return true;
            }
        }
    }

    if(exprOr()){
        return true;
    }
    iTk = start;
    return false;
}

bool exprOr(){
    Token *start = iTk;
    printf("__exprOr\n");
    if(exprAnd()){
        if(exprOrPrim()){
            return true;
        }
    }
    iTk = start;
    return false;
}

// exprOrPrim: OR exprAnd exprOrPrim | ε
// echivalent cu: exprOrPrim: ( OR exprAnd exprOrPrim )?
bool exprOrPrim(){
    Token *start = iTk;
    printf("__exprOrPrim\n");
    if(consume(OR)){ // prima alternativă: OR exprAnd exprOrPrim
        if(exprAnd()){
            if(exprOrPrim()){
                return true;
            }
        }
    }
    return true; // ε - exprOrPrim returneaza true chiar dacă nu consumă nimic
}

/*
exprAnd: exprAnd AND exprEq | exprEq => exprAnd:exprEq exprAndPrim
									=> exprAndPrim:AND exprEq exprAndPrim|eps
*/
bool exprAnd(){
    Token *start = iTk;
    printf("__exprAnd\n");
    if(exprEq()){
        if(exprAndPrim()){
            return true;
        }
    }
    iTk = start;
    return false;
}

bool exprAndPrim(){
    Token *start = iTk;
    printf("__exprAndPrim\n");
    if(consume(AND)){
        if(exprEq()){
            if(exprAndPrim()){
                return true;
            }
        }
    }

    return true;
}

/*
exprEq: exprEq ( EQUAL | NOTEQ ) exprRel | exprRel
	 => exprEq: exprRel exprEqPrim
	 => exprEqPrim: ( EQUAL | NOTEQ ) exprRel exprEqPrim | eps
*/

bool exprEq(){
    Token *start = iTk;
    printf("__exprEq\n");
    if(exprRel()){
        if(exprEqPrim()){
            return true;
        }
    }
    iTk = start;
    return false;
}

bool exprEqPrim(){
    Token *start = iTk;
    printf("__exprEqPrim\n");
    if(consume(EQUAL) || consume(NOTEQ)){
        if(exprRel()){
            if(exprEqPrim()){
                return true;
            }
        }
    }

    return true;
}

/*
exprRel: exprRel ( LESS | LESSEQ | GREATER | GREATEREQ ) exprAdd | exprAdd
	   =>exprRel: exprAdd exprRelPrim
	   =>exprRelPrim: ( LESS | LESSEQ | GREATER | GREATEREQ ) exprAdd exprRelPrim | eps
*/

bool exprRel(){
    Token *start = iTk;
    printf("__exprRel\n");
    if(exprAdd()){
        if(exprRelPrim()){
            return true;
        }
    }

    iTk = start;
    return false;
}

bool exprRelPrim(){
    Token *start = iTk;
    printf("__exprRelPrim\n");
    if(consume(LESS) || consume(LESSEQ) || consume(GREATER) || consume(GREATEREQ)){
        if(exprAdd()){
            if(exprRelPrim){
                return true;
            }
        }

        return true;
    }

    return true;
}

/*
exprAdd: exprAdd ( ADD | SUB ) exprMul | exprMul
	  => exprAdd: exprMul exprAddPrim	   
	  => exprAddPrim: ( ADD | SUB ) exprMul exprAddPrim | eps
*/
bool exprAdd(){
    Token *start = iTk;
    printf("__exprAdd\n");
    if(exprMul()){
        if(exprAddPrim()){
            return true;
        }
    }
    iTk = start;
    return false;
}

bool exprAddPrim(){
    Token *start = iTk;
    printf("__exprAddPrim\n");
    if(consume(ADD) || consume(SUB)){
        if(exprMul()){
            if(exprAddPrim()){
                return true;
            }
        }
    }
    return true;
}

/*
exprMul: exprMul ( MUL | DIV ) exprCast | exprCast
	  => exprMul: exprCast exprMulPrim
	  => exprMulPrim: ( MUL | DIV ) exprCast exprMulPrim
*/

bool exprMul(){
    Token *start = iTk;
    printf("__exprMul\n");
    if(exprCast()){
        if(exprMulPrim()){
            return true;
        }
    }
    iTk = start;
    return false;
}

bool exprMulPrim(){
    Token *start = iTk;
    printf("__exprMulPrim\n");
    if(consume(MUL) || consume(DIV)){
        if(exprCast()){
            if(exprMulPrim()){
                return true;
            }
        }
    }

    return true;
}

//exprCast: LPAR typeBase arrayDecl? RPAR exprCast | exprUnary
bool exprCast(){
    Token *start = iTk;
    printf("__exprCast\n");
    if(consume(LPAR)){
        if(typeBase()){
            if(arrayDecl()){
            }
            if(consume(RPAR)){
                if(exprCast()){
                    return true;
                }
            }
            
        }
    }

    if(exprUnary())
        return true;

    iTk = start;
    return false;
}

//exprUnary: ( SUB | NOT ) exprUnary | exprPostfix
bool exprUnary(){
    Token *start = iTk;
    printf("__exprUnary\n");
    if(consume(SUB) || consume(NOT)){
        if(exprUnary()){
            return true;
        }
    }

    if(exprPostfix()){
        return true;
    }
    iTk = start;
    return false;
}


/*
exprPostfix: exprPostfix LBRACKET expr RBRACKET | exprPostfix DOT ID | exprPrimary
		=>exprPostfix: exprPrimary exprPostfixPrim
	    =>exprPostfixPrim: LBRACKET expr RBRACKET exprPosfixPrim | DOT ID exprPostfixPrim | eps
*/
bool exprPostfix(){
    Token *start = iTk;
    printf("__exprPostfix\n");
    if(exprPrimary()){
        if(exprPostfixPrim()){
            return true;
        }
    }
    iTk = start;
    return false;
}

bool exprPostfixPrim(){
    Token *start = iTk;
    printf("__exprPostficPrim\n");
    if(consume(LBRACKET)){
        if(expr()){
            if(consume(RBRACKET)){
                if(exprPostfixPrim()){
                    return true;
                }
            }
        }
    }

    if(consume(DOT)){
        if(consume(ID)){
            if(exprPostfixPrim()){
                return true;
            }
        }
    }

    return true;
}

/*
exprPrimary: ID ( LPAR ( expr ( COMMA expr )* )? RPAR )?
         | CT_INT | CT_REAL | CT_CHAR | CT_STRING | LPAR expr RPAR
 */
bool exprPrimary(){
    Token *start=iTk;
    printf("__exprPrimary\n");
    if(consume(ID)){
        if(consume(LPAR)){
            if(expr()){
                while(consume(COMMA)){
                    if(!expr()) 
                        tkerr(iTk,"This expression expected expresion");}
            }
            if(!consume(RPAR)) 
                return false;
        }
        return true;
    }
    if(consume(CT_INT)) return true;
    if(consume(CT_REAL)) return true;
    if(consume(CT_CHAR)) return true;
    if(consume(CT_STRING)) return true;
    if(consume(LPAR)){
        if(expr()){
            if(consume(RPAR)) 
            return true;}}
    iTk=start;
    return false;
}
