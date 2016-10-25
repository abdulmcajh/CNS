#include<stdio.h>
#include<gmp.h>

void gcd(mpz_t r,mpz_t a,mpz_t b)
{
	mpz_t t,x;
	mpz_init(t);
	mpz_init(x);
	while(mpz_cmp(b,x)!=0)
	{
		mpz_set(t,b);
		mpz_mod(b,a,b);
		mpz_set(a,t);
	}
	mpz_clear(t);
	mpz_clear(x);
	mpz_set(r,a);
	
}
int main()
{
	mpz_t a,b,r;
	mpz_init(a);
	mpz_init(b);
	mpz_init(r);

	printf("Enter a:");
	gmp_scanf("%Zd",a);
	printf("Enter b:");
	gmp_scanf("%Zd",b);
	
	
	mpz_abs(a,a);
	mpz_abs(b,b);
	gcd(r,a,b);
	
	
	
	gmp_printf("GCD : %Zd \n",r);	
	mpz_clear(a);
	mpz_clear(b);
	mpz_clear(r);

}
