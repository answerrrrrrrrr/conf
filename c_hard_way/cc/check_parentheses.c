#include <stdio.h>
#include <stdbool.h>

char tokens[] = //"(qwe)1";
"(2 - 1)";
/*"(4 + 3 * (2 - 1))";*/
/*"4 + 3 * (2 - 1)";*/
/*"(4 + 3)) * ((2 - 1)";*/
/*"(4 + 3) * (2 - 1)";*/

bool check_parentheses(int p, int q, bool *success)
{
    /*if(tokens[p] != '(' || tokens[q] != ')'){*/
        /*return false;*/
    /*}*/

    /*int open = 0, i;*/
    /*for(i = p + 1; i < q; i++){*/
            /*if(tokens[i] == '('){*/
                        /*open++;*/
                    /*}*/
            /*else if(tokens[i] == ')'){*/
                        /*if(!open){*/
                                        /*return false;*/
                                    /*}*/
                        /*open--;*/
                    /*}*/
        /*}*/

    /*return open == 0 ? true : false;*/
    bool surrounded = true;

    if(tokens[p] != '(' || tokens[q] != ')'){
        surrounded = false;
    }

    int open = 0, i;
    for(i = p; i <= q; i++){    // start with p in case of ")...("
        if(tokens[i] == '('){
            open++;
        }
        else if(tokens[i] == ')'){
            open--;
            if(open < 0){
                *success = false;
                return false;
            }
            else if(open == 0 && i != q){    // close p
                surrounded = false;
            }
        }
    }

    if(open){
        *success = surrounded = false;
    }
    return surrounded;
 }

int main(){
    int len = sizeof(tokens)-1;
    printf("%d\n", len);

    bool *success ;
    bool sus = check_parentheses(0, len-1, success);
    printf("%d", sus);
}
