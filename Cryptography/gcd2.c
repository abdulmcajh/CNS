#include<stdio.h>
#include<gmp.h>
#include<time.h>

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
	time_t t1,t2;
	mpz_t a,b,r;
	gmp_randstate_t state;
	mpz_init(a);
	mpz_init(b);
	mpz_init(r);
	gmp_randinit_default(state);
	mpz_urandomb(a,state,100);
	mpz_urandomb(b,state,100);
	gmp_printf("a = %Zd\n",a);
	gmp_printf("b = %Zd\n",b);
	
	mpz_abs(a,a);
	mpz_abs(b,b);
	
	clock_t t;
  	int f;
  	t = clock();

	gcd(r,a,b);
	
	t = clock() - t;
	
	
	gmp_printf("GCD : %Zd \n",r);	
	printf("time : %f milli seconds\n",(t*1000.0)/CLOCKS_PER_SEC);
	mpz_clear(a);
	mpz_clear(b);
	mpz_clear(r);

}
