#include<iostream>
#include<gmp.h>

int main()
{
	mpz_t p,q,n,t1,t2,phi,e,d,g,M,C;
	gmp_randstate_t state;
	mpz_inits(p,q,n,t1,t2,phi,e,d,g,M,C,NULL);
	gmp_randinit_default(state);
	//find p
	mpz_urandomb(p,state,512);
	mpz_nextprime(p,p);
	//find q
	mpz_urandomb(q,state,512);
	mpz_nextprime(q,q);
	gmp_printf("p = %Zd\n",p);
	gmp_printf("q = %Zd\n",q);
	//n=pq
	mpz_mul(n,p,q);
	//t1=p-1
	mpz_sub_ui(t1,p,1);
	//t2=q-1;
	mpz_sub_ui(t2,q,1);
	//phi=(p-1)(q-1)
	mpz_mul(phi,t1,t2);
	//select e such that gcd(phi,e)=1 and 1<e<phi
	while(mpz_cmp_ui(g,1)!=0)
	{
		mpz_urandomm(e,state,phi);
		mpz_gcd(g,phi,e);
	}
	//d=e^-1 mod phi
	mpz_invert(d,e,phi);
	//gmp_printf("e = %Zd\n",e);
	//gmp_printf("d = %Zd\n",d);
	
	printf("Enter M:");
	gmp_scanf("%Zd",M);
	//C=M^e mod n
	mpz_powm(C,M,e,n);	
	gmp_printf("C = %Zd\n",C);
	//M=C^d mod n
	mpz_powm(M,C,d,n);
	gmp_printf("M = %Zd\n",M);
	
	
}
