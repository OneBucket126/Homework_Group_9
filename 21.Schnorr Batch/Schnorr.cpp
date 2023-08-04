#define _CRT_SECURE_NO_WARNINGS 
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
int xy[22];
// 判断两个数是否互质
int isHuZhi(int a, int b) {
	return b == 0 ? a : isHuZhi(b, a % b);
}
int myPow(int a, int b, int m) {

	int res = 1;
	a %= m;
	while (b != 0) {
		if ((b & 1) == 1)
			res = (res * a) % m;
		a = (a * a) % m;
		b >>= 1;
	}
	printf("rea=%d", res);
	return res;
}
//求q 
int candp(int g, int p)
{
	for (int i = 0; i < p; i++) {
		for (int j = 1; j <= g; j++) {
			g = g * g;
		}
		if (g % p == 1) {
			printf("a=%d\n", i);
			return i;
		}
	}

}
// 求 a mod b 的逆元
void exGcd(int a, int b) {
	if (b == 0) {
		xy[0] = 1;
		xy[1] = 0;
	}
	else {
		exGcd(b, a % b);
		int x = xy[0];
		xy[0] = xy[1];
		xy[1] = x - (a / b) * xy[1];
	}
}
int main(){
	int p, q, g, x, y, s, k, e, ni, m, h, r, r1;

	printf("请输入大素数 p ");
	scanf("%d", &p);
	m = 16;
	h = 11;//生成元
	q = 11;
	g = 6;//candp(q,p);
	printf("g=%d", g);
	srand(time(NULL)); //随机数种子
	x = 10;//rand()%p-1+2 ;
	y = myPow(g, x, p); // y是公开密钥
	printf("y=%d\n", y);
	k = 9;//rand()%p-1+1 ;
	//	while (isHuZhi(k, p-1) != 1) {
	//        k = 11;//rand()%p-1+1 ;
	//    }
	printf("k=%d", k);
	// r = g^k mod p
	r = myPow(g, k, p);
	printf("r=%d\n", r);
	e = 13;
	s = (x * e + k) % q;
	printf("(e,s)=(%d,%d)", e, s);
	exGcd(y, p);
	ni = xy[0];
	printf("ni=%d\n", ni);
	r1 = myPow(g, s, p) * myPow(ni, e, p) % p;

	if (r == r1) {
		printf("有效");
	}
	else {
		printf("无效");
	}
}
