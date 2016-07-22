#include<stdio.h>

void modi(int a,int b,int barr[][a])
{
barr[0][0]=5;

}
int main()
{
int arr[2][2]={{1,2},{3,4}};
modi(2,2,arr);
printf("%d",arr[0][0]);
return 0;
}
