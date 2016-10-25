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
void pac(char s[],string &key)
{
	for(int i=0; s[i]!='\0'; i++)
	{
		int k = key[i]-'a';
		int c = s[i]-'a';
		s[i] = ((c+k)%26)+'a';
	}
}
void pad(char s[],string &key)
{
	for(int i=0; s[i]!='\0'; i++)
	{
		int k = key[i]-'a';
		int c = s[i]-'a';
		s[i] = ((26+(c-k))%26)+'a';
	}
}
void otpc(char s[],string key)
{
	for(int i=0;s[i]!='\0' ; i++)
	{
		int k,c;
		if(key[i] >='a' && key[i] <= 'z')
		k = key[i]-'a';
		else
		k = 26;
		if(s[i] >='a' && s[i] <= 'z')
		c = s[i]-'a';
		else
		c = 26;
		c = ((c+k)%27);
		if(c == 26)
		s[i] = ' ';
		else
		s[i] = c+'a';
	}
}
void otpd(char s[],string key)
{
	for(int i=0; s[i]!='\0'; i++)
	{
		int k,c;
		if(key[i] >='a' && key[i] <= 'z')
		k = key[i]-'a';
		else
		k = 26;
		if(s[i] >='a' && s[i] <= 'z')
		c = s[i]-'a';
		else
		c = 26;
		cout<<c<<" "<<k<<" ";
		c = ((27+(c-k))%27);
		cout<<c<<"\n";
		if(c == 26)
		s[i] = ' ';
		else
		s[i] = c+'a';
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

	
	cout<<"enter key :";
	string key;
	getline(cin,key);
	pac(s,key);
	cout<<"poly alphabetic encryped : "<<pt<<"\n";
	pad(s,key);
	cout<<"poly alphabetic decryped : "<<ct<<"\n";

	cout<<"enter plain text to be one time pad ciphered : ";
	getline(cin,pt);
	cout<<"enter key :";
	getline(cin,key);
	otpc(pt,key);
	cout<<"one time pad encryped text is : "<<pt<<"\n";
	cout<<"enter onetimepad ciphered text to be deciphered : ";
	getline(cin,ct);
	cout<<"enter key :";
	getline(cin,key);
	otpd(ct,key);
	cout<<"plain text is : "<<ct<<"\n";
	
	
	
}
