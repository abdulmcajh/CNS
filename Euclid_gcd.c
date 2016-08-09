#include<stdio.h>
#include<gmp.h>
#include<time.h>

void gcd(mpz_t a,mpz_t b,mpz_t res)
{
	mpz_t t;
	mpz_init(t);
	
	while(mpz_cmp_ui(b,0)!=0)
	{
		mpz_set(t,b);
		mpz_mod(b,a,b);
		mpz_set(a,t);
	}
	
	mpz_clear(t);
	mpz_set(res,a);
}
int main()
{
	gmp_randstate_t state;
	//gmp_randinit_default(state);
	mpz_t a,b,res;
	mpz_inits(a,b,res,NULL);
	
	for(int bits=10;bits<100001;bits=bits*10)
	{
		gmp_randinit_default(state);
		mpz_urandomb(a,state,bits);
		//gmp_printf("a: %Zd \n",a);
		mpz_urandomb(b,state,bits);
		//gmp_printf("b: %Zd \n",b);
	
		clock_t start = clock();
	
		gcd(a,b,res);
		
		clock_t stop = clock();
		
		double elapsed = (double)(stop - start) * 1000.0 / CLOCKS_PER_SEC;
		
		gmp_printf("GCD: %Zd \n",res);
	
		printf(" bits %d --- Time elapsed in ms: %f \n",bits, elapsed);
		
	}
	
	mpz_clears(a,b,res,NULL);
}
