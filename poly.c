#include<iostream>
#include<vector>
using namespace std;

//irreducible polynomial
string rp="1011";
vector<char> mx(rp.begin(),rp.end());

//print polynomial
void print(vector<char> a)
{
	int n=a.size();
	for(int i=0;i<n;i++)
	{
		if(a[i]=='1')
			cout<<"x^"<<(n-i-1)<<"+";
	}
	cout<<"\b \n";
}

//add or sub polynomials
vector<char> poly_add(vector<char> f,vector<char> g)
{
	vector<char> r;
	for(int i=0;i<f.size();i++)
	{
		char x=((f[i]-'0')^(g[i]-'0'))+'0';
		r.push_back(x);
	}
	
	return r;
	
}

//calculates x*f(x)
vector<char> mul(vector<char> f)
{
	vector<char> r;
	f.erase(f.begin());
	f.push_back('0');
	r=f;
	if(f[0]=='1')
	{
		r=poly_add(f,mx);
	}
	return r;
}

//multiplication of polynomials
vector<char> poly_mul(vector<char> f,vector<char> g)
{
	vector<char> V[3];
	V[0]=f;
	for(int i=0;i<2;i++)
	{
		V[i+1]=mul(V[i]);
		cout<<"x^"<<i+1<<"*f(x) :";
		print(V[i+1]);
	}
	
	string t="000";                                                                                  
	vector<char> temp(t.begin(),t.end());
	
	for(int i=0;i<g.size();i++)
	{
		if(g[i]=='1')
		{
			temp=poly_add(temp,V[g.size()-i-1]);
		}
	}
	
	return temp;
	
	
}
int main()
{
	cout<<"Irreducible polynomial m(x) : ";
	print(mx);
	mx.erase(mx.begin());
	
	string a,b;
	cout<<"Enter polynomial f(x):";
	cin>>a;
	vector<char> f(a.begin(),a.end());
	cout<<" f(x): ";
	print(f);
	
	cout<<"Enter polynomial g(x):";
	cin>>b;
	vector<char> g(b.begin(),b.end());
	cout<<" g(x): ";
	print(g);
	
	vector<char> r=poly_add(f,g);
	cout<<"ADDITON/SUBTRACTION : ";
	print(r);
	r=poly_mul(f,g);
	cout<<"MULTIPLICATION :f(x)*g(x)  : ";
	print(r);
	return 0;
}
