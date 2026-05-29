#include <stdio.h>
#include <stdlib.h>

int a=1;
static int b=2;
void function() {
    int i=3;
    static int j=4;
    printf("i=%p\n",(void *)&i);
    printf("j=%p\n",(void *)&j);
}
int* bad(void) {
    static int x = 42;
    return &x;
}
int main() {
    printf("a=%p\n",(void *)&a);
    printf("b=%p\n",(void *)&b);
    int *c=malloc(sizeof(int));
    *c=3;
    printf("c=%p\n",(void *)c);
    free(c);
    function();
    printf("bad=%p",bad());
    return 0;
}

