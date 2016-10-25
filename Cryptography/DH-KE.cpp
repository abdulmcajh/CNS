#include<iostream>
#include<gmp.h>
void genkeys(mpz_t XA,mpz_t YA,mpz_t q,mpz_t g,gmp_randstate_t state)
{
	//find XA 0<XA<q (private key)
	mpz_urandomm(XA,state,q);
	//find YA=g^XA mod q (public Key)
	mpz_powm(YA,g,XA,q);
}
void secretkey(mpz_t K,mpz_t X,mpz_t Y,mpz_t q)
{
	//find K=Y^X mod q
	mpz_powm(K,Y,X,q);
}

void geng(mpz_t g,mpz_t tq,mpz_t p,mpz_t q)
{	
	//q=2*p+1;
	//q-1 has 2 factors 2 and p
	//tq=2*p
	mpz_t a,t1,t2,p2;
	mpz_inits(a,t1,t2,p2,NULL);
	gmp_randstate_t state;
	gmp_randinit_default(state);
	mpz_set_ui(p2,2);
	while(1)
	{
		//select a random a < q
		mpz_urandomm(a,state,q);
		mpz_powm(t1,a,p2,q);
		mpz_powm(t2,a,p,q);
		//a is primitive root if a^(2) mod q not equal 1 and a^(p) mod q not equal 1
		if((mpz_cmp_ui(t1,1)!=0)&&(mpz_cmp_ui(t2,1)!=0))
			break;
	}
	mpz_set(g,a);
	
}
int main()
{
	mpz_t XA,YA,XB,YB,q,p,g,K,tq,t;
	mpz_inits(XA,YA,XB,YB,q,p,g,K,tq,t,NULL);
	gmp_randstate_t state;
	gmp_randinit_default(state);
	mpz_urandomb(p,state,10);
	//find q such that q=2*p+1 and p and q are prime
	while(mpz_cmp_ui(t,1)!=0)
	{	
		//find p;
		mpz_nextprime(p,p);
		//find q=2*p+1 and q is prime
		mpz_mul_ui(tq,p,2);
		mpz_nextprime(q,tq);
		mpz_sub(t,q,tq);
		
	}
	//find g
	gmp_printf("p = %Zd\n",p);
	gmp_printf("q = %Zd\n",q);
	geng(g,tq,p,q);
	gmp_printf("g = %Zd\n",g);
	//return 0;
	
	//generate key pairs for A and B
	genkeys(XA,YA,q,g,state);
	gmp_printf("XA = %Zd\n",XA);
	gmp_printf("YA = %Zd\n",YA);
	genkeys(XB,YB,q,g,state);
	gmp_printf("XB = %Zd\n",XB);
	gmp_printf("YB = %Zd\n",YB);

	//find the secret key
	secretkey(K,XA,YB,q);
	gmp_printf("K = %Zd\n",K);
	secretkey(K,XB,YA,q);
	gmp_printf("K = %Zd\n",K);
}
