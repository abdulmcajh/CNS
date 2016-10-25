#include<iostream>
#include<gmp.h>
void genkeys(mpz_t XA,mpz_t YA,mpz_t q,mpz_t g,gmp_randstate_t state)
{
	mpz_t t;
	mpz_init(t);
	//t=q-1
	mpz_sub_ui(t,q,1);
	//find XA 0<XA<q-1 (private key)
	mpz_urandomm(XA,state,t);
	//find YA=g^XA mod q (public Key)
	mpz_powm(YA,g,XA,q);
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
	//make mpz var of value 2
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
	//set g to a
	mpz_set(g,a);
	
}
void encryption(mpz_t C1,mpz_t C2,mpz_t YB,gmp_randstate_t state,mpz_t g,mpz_t q,mpz_t M)
{
	mpz_t K,k;
	mpz_inits(K,k,NULL);
	//Get session key k
	mpz_urandomm(k,state,q);
	//find K=YB^k mod q
	mpz_powm(K,YB,k,q);
	//find C1=g^k mod q
	mpz_powm(C1,g,k,q);
	//find C2=KM
	mpz_mul(C2,K,M);
	//find C2=KM mod q
	mpz_mod(C2,C2,q);
	
}
void decryption(mpz_t C1,mpz_t C2,mpz_t XB,mpz_t q,mpz_t M)
{
	mpz_t K;
	mpz_init(K);
	//find K=C1^XB mod q
	mpz_powm(K,C1,XB,q);
	//find Kinv
	mpz_invert(K,K,q);
	//find M=Kinv*K*C2
	mpz_mul(M,K,C2);
	mpz_mod(M,M,q);
}
int main()
{
	mpz_t XA,YA,XB,YB,C1,C2,M,q,p,g,K,tq,t;
	mpz_inits(XA,YA,XB,YB,C1,C2,M,q,p,g,K,tq,t,NULL);
	gmp_randstate_t state;
	gmp_randinit_default(state);
	mpz_urandomb(p,state,10);
	//find q such that q=2*p+1 and p and q are prime
	while(mpz_cmp_ui(t,1)!=0)
	{	
		//find p;
		mpz_nextprime(p,p);
		//find q=2*p+1 and check if q is prime
		mpz_mul_ui(tq,p,2);
		mpz_nextprime(q,tq);
		mpz_sub(t,q,tq);	
	}
	//find g
	gmp_printf("p = %Zd\n",p);
	gmp_printf("q = %Zd\n",q);
	geng(g,tq,p,q);
	gmp_printf("g = %Zd\n",g);
	
	
	//generate key pairs for A and B
	genkeys(XA,YA,q,g,state);
	gmp_printf("XA = %Zd\n",XA);
	gmp_printf("YA = %Zd\n",YA);
	genkeys(XB,YB,q,g,state);
	gmp_printf("XB = %Zd\n",XB);
	gmp_printf("YB = %Zd\n",YB);
	
	printf("Enter M:");
	gmp_scanf("%Zd",M);

	encryption(C1,C2,YB,state,g,q,M);
	gmp_printf("C1 = %Zd\n",C1);
	gmp_printf("C2 = %Zd\n",C2);

	decryption(C1,C2,XB,q,M);
	gmp_printf("M = %Zd\n",M);
}
