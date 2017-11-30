#include <stdio.h>
int main(void){
    char ch;
    puts("Enter a character:");
    ch = getchar();
    if(ch>='A'&&ch<='Z')
        ch += 32;
    putchar(ch);
    return 0;
}
