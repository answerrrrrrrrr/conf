#include "expr.c"

int main(){
    /*char e[] = "(4-3)+1-2*1";*/
    /*char e[] = "((4+3)/(4-3))";*/
    /*char e[] = "(4- 0x3)*2+1==2+1";*/
    /*char e[] = "(4-2 0x3)+1";*/
    /*char e[] = "$ax";*/
    char e[] = "2+";

    init_regex();
    if(make_token(e)){
        printf("Token made!\n");
    }

    for(int i=0; i<nr_token; i++){
        printf("%s  ", tokens[i].str);
    }

    printf("\nnr_token: %d\n", nr_token);

    /*int matched;*/
    /*matched = check_parentheses(0, nr_token-1);*/
    /*printf("%d\n", matched);*/

    int op = get_dominant_operator(0, nr_token-1);
    printf("op index: %d\n", op);

    bool suc = true;


    printf("result: %d\n", eval(0, nr_token-1, &suc));
    printf("flag: %d\n", suc);
}
