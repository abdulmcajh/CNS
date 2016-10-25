#include<iostream>
#include<gmp.h>
int main()
{
	mpz_t a[50],m[50];
	mpz_t mi,miinv,t,sum,x,M;
	mpz_inits(mi,miinv,t,sum,x,M,NULL);
	mpz_set_ui(M,1);
	int n;
	printf("Enter no.of equations:\n");
	scanf("%d",&n);
	printf("Enter a's:\n");
	for(int i=0;i<n;i++)
	{
		mpz_inits(a[i],m[i],NULL);
		printf("Enter a%d:\n",i);
		gmp_scanf("%Zd",a[i]);
		printf("Enter m%d:\n",i);
		gmp_scanf("%Zd",m[i]);
		mpz_mul(M,M,m[i]);
	}
	for(int i=0;i<n;i++)
	{
		gmp_printf("x mod %Zd =%Zd\n",m[i],a[i]);
	}
	gmp_printf("M = %Zd\n",M);
	for(int i=0;i<n;i++)
	{	
		mpz_fdiv_q(mi,M,m[i]);
		mpz_invert(miinv,mi,m[i]);
		mpz_mul(t,mi,miinv);
		mpz_addmul(sum,a[i],t);
	}
	mpz_mod(x,sum,M);
	gmp_printf("x = %Zd\n",x);
	
	
	return 0;
}
