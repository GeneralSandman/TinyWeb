#include <stdio.h>

struct data
{
    int a;
    int b;
};

typedef struct data data;

void fun(const data *d)
{
    // (void)d;
    printf("invoke fun \n");
}

int main()
{
    fun(NULL);
    return 0;
}