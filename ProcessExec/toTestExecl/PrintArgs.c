#include <stdio.h>

int main(int argc, char* args[]) {
    for(int i = 0;i<argc;++i){
        printf("PrintArgs args[%d]:%s\n",i,args[i]);
    }
    printf("\n");
    return 0;
}
