#include<iostream>
#include<gmp.h>
#include<string>
using  namespace std;
struct point
{
	mpz_t x;
	mpz_t y;
	point()
	{
		mpz_init(x);
		mpz_init(y);
	}
};
void pointadd(struct point* R,struct point* P,struct point* Q,mpz_t a,mpz_t p)
{
	mpz_t m,t,t1;
	mpz_inits(m,t,t1,NULL);
	
	//calculate m
	mpz_sub(m,Q->y,P->y);
	mpz_sub(t,Q->x,P->x);
	mpz_invert(t,t,p);
	mpz_mul(m,m,t);
	mpz_mod(m,m,p);
	
		
	//calculate Xr
	mpz_pow_ui(t1,m,2);
	mpz_sub(t1,t1,P->x);
	mpz_sub(R->x,t1,Q->x);
	mpz_mod(R->x,R->x,p);
	
	//calculate Yr
	mpz_sub(t,P->x,R->x);
	mpz_mul(t1,m,t);
	mpz_sub(R->y,t1,P->y);
	mpz_mod(R->y,R->y,p);
	


}
void pointdoub(struct point* R,struct point* P,mpz_t a,mpz_t p)
{
	mpz_t m,t,t1;
	mpz_inits(m,t,t1,NULL);
	//calculate m
	mpz_pow_ui(m,P->x,2);
	mpz_mul_ui(m,m,3);
	mpz_add(m,m,a);
	mpz_mul_ui(t,P->y,2);
	mpz_invert(t,t,p);
	mpz_mul(m,m,t);
	mpz_mod(m,m,p);
	
	//calculate Xr
	mpz_add(t,P->x,P->x);
	mpz_pow_ui(t1,m,2);
	mpz_sub(R->x,t1,t);
	mpz_mod(R->x,R->x,p);
	
	//calculate Yr
	mpz_sub(t,P->x,R->x);
	mpz_mul(t1,m,t);
	mpz_sub(R->y,t1,P->y);
	mpz_mod(R->y,R->y,p);

	
}
int getlen(char a[])
{
	int i=0;
	for(i=0;a[i]!='\0';i++);
	return i;
}
void scalarmul(struct point* R,struct point* P,mpz_t a,mpz_t p,mpz_t k)
{
	char bit[512];
	mpz_get_str(bit,2,k);
	
	struct point temp,Q;
	int j=getlen(bit);
	
	j=j-1;
	
	
	while(bit[j]=='0')
	{
		//cout<<j<<" "<<bit[j]<<endl;
		pointdoub(&temp,P,a,p);
		mpz_set(P->x,temp.x);
		mpz_set(P->y,temp.y);
		j=j-1;
	}
	mpz_set(Q.x,P->x);
	mpz_set(Q.y,P->y);

	pointdoub(&temp,P,a,p);
	mpz_set(P->x,temp.x);
	mpz_set(P->y,temp.y);
	for(int i=j-1;i>=0;i--)
	{
		
		if(bit[i]=='1')
		{
			pointadd(&temp,&Q,P,a,p);
			mpz_set(Q.x,temp.x);
			mpz_set(Q.y,temp.y);
		}
		
		
		pointdoub(&temp,P,a,p);
		mpz_set(P->x,temp.x);
		mpz_set(P->y,temp.y);
		
	}
	
	mpz_set(R->x,Q.x);
	mpz_set(R->y,Q.y);
	
}
int main()
{
	
	struct point R,P,Q;
	mpz_t a,p,k;
	mpz_inits(a,p,k,NULL);

	printf("Enter p:");
	gmp_scanf("%Zd",p);
	printf("Enter a:");
	gmp_scanf("%Zd",a);
	
	printf("Enter Xp:");
	gmp_scanf("%Zd",P.x);
	printf("Enter Yp:");
	gmp_scanf("%Zd",P.y);
	
	printf("Enter k:");
	gmp_scanf("%Zd",k);
	
	scalarmul(&R,&P,a,p,k);
	gmp_printf("%Zd\n",R.x);
	gmp_printf("%Zd\n",R.y);
	return 0;
	
	printf("Enter Xq:");
	gmp_scanf("%Zd",Q.x);
	printf("Enter Yq:");
	gmp_scanf("%Zd",Q.y);
	
	pointadd(&R,&P,&Q,a,p);
	gmp_printf("%Zd\n",R.x);
	gmp_printf("%Zd\n",R.y);
	
}
