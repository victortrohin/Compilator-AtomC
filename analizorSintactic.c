#include <stdio.h>
#include <stdbool.h>

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

bool stmCompound(){

}

bool expr(){

}

bool exprAssign(){

}

bool exprOr(){

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


bool exprAnd(){

}

bool exprEq(){

}

bool exprRel(){

}

bool exprAdd(){

}

bool exprMul(){

}

bool exprCast(){

}

bool exprUnary(){

}

bool exprPostfix(){

}

bool exprPrimary(){

}


int main(int argc, char const *argv[])
{
    /* code */
    return 0;
}
