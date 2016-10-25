#include<iostream>
#include<cstring>
#include<map>
#include<bits/stdc++.h>
using namespace std;
map<char,char> m,rm;
void ceaser_encrypt(char s[],int k)
{
	
	for(int i=0;s[i]!='\0';i++)
	{
		if(s[i]>='A' && s[i]<='Z')
			s[i]='A'+((int)s[i]-'A'+k)%(26);
		else if(s[i]>='a' && s[i]<='z')
			s[i]='a'+((int)s[i]-'a'+k)%(26);
	}
	
}
void ceaser_decrypt(char s[],int k)
{

	for(int i=0;s[i]!='\0';i++)
	{
		if(s[i]>='A' && s[i]<='Z')
			s[i]='A'+(26+(int)s[i]-'A'-k)%(26);
		else if(s[i]>='a' && s[i]<='z')
			s[i]='a'+(26+((int)s[i]-'a'-k))%(26);
	}
	
}
void fillmap()
{
	
	char t;
	for(int i=0;i<26;i++)
	{
		
		while(1)
		{
			t='a'+rand()%26;
			if(rm.find(t)==rm.end())
				break;
		}
		m['a'+i]=t;
		rm[t]='a'+i;
	}
	for(int i=0;i<26;i++)
	{
		cout<<(char)('a'+i)<<"-"<<m['a'+i]<<endl;
	}
}
void mono_encrypt(char s[])
{
	for(int i=0;s[i]!='\0';i++)
	{
		if(s[i]>='a' && s[i]<='z')
			s[i]=m[s[i]];
	}
}
void mono_decrypt(char s[])
{
	for(int i=0;s[i]!='\0';i++)
	{
		if(s[i]>='a' && s[i]<='z')
			s[i]=rm[s[i]];
	}
}

int main()
{

	char s[1000];
	cin.getline(s,1000);
	ceaser_encrypt(s,3);
	cout<<"CEASER ENCRYPT : "<<s<<endl;
	ceaser_decrypt(s,3);
	cout<<"CEASER DECRYPT : "<<s<<endl;
	
	cout<<endl;
	fillmap();
	mono_encrypt(s);
	cout<<"MONO ENCRYPT : "<<s<<endl;
	mono_decrypt(s);
	cout<<"MONO DECRYPT : "<<s<<endl;
	//int K[3][3]={{17,17,5},{21,18,21},{2,2,19}};


	
}
