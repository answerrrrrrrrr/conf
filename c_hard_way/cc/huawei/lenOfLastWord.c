#include <stdio.h>
#define maxn 1000
#include <string.h>
int main()
{
    int number = 0,i,len;
    char s[maxn];
    gets(s);
    len = strlen(s);
    for(i = len-1; i >= 0; i--)
    {
        if((s[i] >= 'a' && s[i] <= 'z') || (s[i] >= 'A' && s[i] <= 'Z'))
        {
            if(s[i] == ' ')
            {
                number = len - 1 - i;
                printf("%d\n",number);
                return 1;
            }
        }
    }
    printf("%d\n",len);
    return 0;
}
