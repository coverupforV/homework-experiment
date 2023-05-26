#include <stdio.h>

int f_star, l_star;
int f[3][6];
int l[3][6];
int a[3][6] = { { 0,0,0,0,0,0},{ 0,7,9,3,4,80},{ 0,8,5,6,4,5} };/*第一条线和第二条线*/
int t[3][5] = { { 0,0,0,0,0},{ 0,2,3,1,3},{ 0,2,1,2,2}};/*第一个缓冲和第二个缓冲*/
int e[3] = { 0,2,4 };
int x[3] = { 0,3,6 };
 
/*最快的路线*/
void FastWay()
{
	int j;
	f[1][1] = e[1] + a[1][1];
	f[2][1] = e[2] + a[2][1];
	for (j = 2; j < 6; j++)
	{
		/*遍历上面的结点*/
		if (f[1][j - 1] + a[1][j] <= f[2][j - 1] + t[2][j - 1] + a[1][j])/*往上面直接走比较快*/
		{
			f[1][j] = f[1][j - 1] + a[1][j];
			l[1][j] = 1;/*走第一条路*/
		}
		else/*往下面直接走比较快*/
		{
			f[1][j] = f[2][j - 1] + t[2][j - 1] + a[1][j];
			l[1][j] = 2;/*走第2条路*/
		}
		/*遍历下面的结点*/
		if (f[2][j - 1] + a[2][j] <= f[1][j - 1] + t[1][j - 1] + a[2][j])
		{
			f[2][j] = f[2][j - 1] + a[2][j];
			l[2][j] = 2;
		}
		else
		{
			f[2][j] = f[1][j - 1] + t[1][j - 1] + a[2][j];
			l[2][j] = 1;
		}
	}
	/*走到最后如果上面的路比下面的路短*/
	if (f[1][5] + x[1] <= f[2][5] + x[2])
	{
		f_star = f[1][5] + x[1];
		l_star = 1;
	}
	else
	{
		f_star = f[2][5] + x[2];
		l_star = 2;
	}
}
 
/*打印路线*/
void PrintStations()
{
	FastWay();
	int j;
	int i = l_star;/*决定最后是哪条路*/
	for (j = 2; j < 6; j++)
	{
		i = l[i][j];
		printf("line %d,station %d\n", i, j-1);
	}
	printf("line %d,station %d\n", i, 5);
	printf("最短时间为：%d",f_star);
}

int main(){
    PrintStations();
    return 0;
}

