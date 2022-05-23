#include "analizorLexical.h"

Token *iTk; // iteratorul în lista de atomi. Inițial pointează la primul atom dinlistă.
Token *consumedTk; // atomul care tocmai a fost consumat. Va fi folosit în etapele următoare ale compilatorului


bool consume(int code){
    if(iTk->code==code){// dacă la poziția curentă avem codul cerut, consumăm atomul
        consumedTk=iTk;
        iTk=iTk->next;
        return true;
    }

    return false; // dacă la poziția curentă se află un atom cu un alt cod decât celcerut, nu are loc nicio acțiune
}




bool unit(){
    for(;;){ // buclă infinită
        if(structDef()){}
        else if(fnDef()){}
        else if(varDef()){}
        else break; // dacă nu se poate consuma nimic la iterația curentă, se iese din buclă
    }
    if(consume(END)){ // se ajunge la END chiar și dacă nu se consumă nimic în repetiție, deci ea este opțională

    return true;
    }
}

//structDef: STRUCT ID LACC varDef* RACC SEMICOLON
bool structDef(){
    if(consume(STRUCT)){
        if(consume(ID)){
            if(consume(LACC)){
                for(;;){
                    if(varDef()){}
                    else break;
                }
                if(consume(END)){
                    if (consume(RACC)){
                        if(consume(SEMICOLON)){
                            return true;
                        }
                    }
                }
            }
        }
    }

    return false;
}


//varDef: typeBase ID arrayDecl? SEMICOLON
bool varDef(){
    if(typeBase()){
        if(consume(ID)){
            if(arrayDecl()){
                if(consume(SEMICOLON)){
                   return true;
                }
            }else if(consume(SEMICOLON)){
                return true;
            }
        }
    }
    
    return false;
}

// typeBase: INT | DOUBLE | CHAR | STRUCT ID
bool typeBase(){
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

    return false;
}

//arrayDecl: LBRACKET CT_INT? RBRACKET

bool arrayDecl(){
    if(consume(LBRACKET)){
        if(consume(CT_INT)){
            if(consume(RBRACKET)){
                return true;
            }else if(consume(RBRACKET)){
                return true;
            }
        }
    }

    return false;
}


/*
fnDef: ( typeBase | VOID ) ID
            LPAR ( fnParam ( COMMA fnParam )* )? RPAR
            stmCompound
*/
bool fnDef(){
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

    return false;
}


//fnParam: typeBase ID arrayDecl?
bool fnParam(){
    if(typeBase()){
        if(consume(ID)){
            if(arrayDecl()){
                return true;
            }else{
                return true;
            }
        }
    }


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
    if(stmCompound()){
        return true;
    }

    if(consume(IF)){
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
                if(consume(SEMICOLON)){
                    if(expr()){
                        if(consume(SEMICOLON)){
                            if(expr()){
                                if(consume(RPAR)){
                                    if(stm()){
                                        return true;
                                    }
                                }
                            }  
                        }
                    }  
                }else if(consume(SEMICOLON)){
                    if(expr()){
                        if(consume(SEMICOLON)){
                            if(expr()){
                                if(consume(RPAR)){
                                    if(stm()){
                                        return true;
                                    }
                                }
                            }  
                        }
                    }else if(consume(SEMICOLON)){
                            if(expr()){
                                if(consume(RPAR)){
                                    if(stm()){
                                        return true;
                                    }
                                }
                            }else if(consume(RPAR)){
                                    if(stm()){
                                        return true;
                                    }
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
            if(consume(SEMICOLON)){
                return true;
            }
        }else if(consume(SEMICOLON)){
                return true;
            }
    }

    if(expr()){
        if(consume(SEMICOLON))
            return true;
    }else if(consume(SEMICOLON)){
        return true;
    }
    
    return false;
}

//stmCompound: LACC ( varDef | stm )* RACC
bool stmCompound(){
    if(consume(LACC)){
        if(varDef() | stm() ){
            if(consume(RACC)){
                return true;
            }
        }else if(consume(RACC)){
            return true;
        }
    }

    return false;
}


//expr: exprAssign
bool expr(){
    if(exprAssign())
        return true;

    return false;
}

//exprAssign: exprUnary ASSIGN exprAssign | exprOr
bool exprAssign(){
    if(exprUnary()){
        if(consume(ASSIGN)){
            if(exprAssign() | exprOr()){
                return true;
            }
        }
    }
    return false;
}

bool exprOr(){
    if(exprAnd()){
        if(exprOrPrim()){
            return true;
        }
    }

    return false;
}

// exprOrPrim: OR exprAnd exprOrPrim | ε
// echivalent cu: exprOrPrim: ( OR exprAnd exprOrPrim )?
bool exprOrPrim(){
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
    if(exprEq()){
        if(exprAndPrim()){
            return true;
        }
    }

    return false;
}

bool exprAndPrim(){
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
    if(exprRel()){
        if(exprEqPrim()){
            return true;
        }
    }

    return false;
}

bool exprEqPrim(){
    if(consume(EQUAL) || cosume(NOTEQ)){
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
    if(exprAdd()){
        if(exprRelPrim()){
            return true;
        }
    }
    return false;
}

bool exprRelPrim(){
    if(consume(LESS) || consume(LESSEQ) || consume(GREATER) || consume(GREATEREQ)){
        if(exprAdd()){
            if(exprRelPrim){
                return true;
            }
        }

        return true;
    }
}

/*
exprAdd: exprAdd ( ADD | SUB ) exprMul | exprMul
	  => exprAdd: exprMul exprAddPrim	   
	  => exprAddPrim: ( ADD | SUB ) exprMul exprAddPrim | eps
*/
bool exprAdd(){
    if(exprMul()){
        if(exprAddPrim()){
            return true;
        }
    }
    return false;
}

bool exprAddPrim(){
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
    if(exprCast()){
        if(exprMulPrim()){
            return true;
        }
    }

    return false;
}

bool exprMulPrim(){
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
    if(consume(LPAR)){
        if(typeBase()){
            if(arrayDecl()){
                if(consume(RPAR)){
                    if(exprCast()){
                        return true;
                    }
                }
            } else if(consume(RPAR)){
                    if(exprCast()){
                        return true;
                    }
                }
        }
    }

    if(exprUnary())
        return true;

    return false;
}

//exprUnary: ( SUB | NOT ) exprUnary | exprPostfix
bool exprUnary(){
    if(consume(SUB) || consume(NOT)){
        if(exprUnary()){
            return true;
        }
    }

    if(exprPostfix()){
        return true;
    }

    return false;
}


/*
exprPostfix: exprPostfix LBRACKET expr RBRACKET | exprPostfix DOT ID | exprPrimary
		=>exprPostfix: exprPrimary exprPostfixPrim
	    =>exprPostfixPrim: LBRACKET expr RBRACKET exprPosfixPrim | DOT ID exprPostfixPrim
*/
bool exprPostfix(){
    if(exprPrimary()){
        if(exprPostfixPrim()){
            return true;
        }
    }

    return false;
}

bool exprPostfixPrim(){
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

bool exprPrimary(){

}
