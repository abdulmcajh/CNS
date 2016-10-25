#include<iostream>
#include<vector>
#include<map>
using namespace std;
void multiply(int K[3][3],string& p)
{
    string t="aaa";
    for(int i=0;i<3;i++)
    {
        int sum=0;
        for(int j=0;j<3;j++)
        {
           sum+=K[i][j]*(p[j]-'a');
        }
        t[i]=((sum%26)+'a');
    }

    p.assign(t);
}
string hill_encryption(int K[3][3],string s)
{
    string cipher="";
    for(int i=0;i<s.length();i=i+3)
    {
        string t(s,i,3);
        multiply(K,t);

        cipher=cipher+t;
    }
    return cipher;
}
string hill_decryption(int Kinv[3][3],string s)
{
    string plain="";
    for(int i=0;i<s.length();i=i+3)
    {
        string t(s,i,3);
        multiply(Kinv,t);
        plain=plain+t;
    }
    return plain;
}
void run_hill()
{
    cout<<"Hill Cipher"<<endl<<endl;
    int K[3][3]={{17,17,5},{21,18,21},{2,2,19}};
    int Kinv[3][3]={{4,9,15},{15,17,6},{24,0,17}};
    string text;
    cin>>text;
    string cipher=hill_encryption(K,text);
    cout<<"cipher:"<<cipher<<endl;
    string plain=hill_decryption(Kinv,cipher);
    cout<<"Plain:"<<plain<<endl;
}

string otp_encryption(string key,string text)
{
    string cipher(text.length(),'a');
    for(int i=0;i<text.length();i++)
    {
        cipher[i]='a'+((key[i]-'a')^(text[i]-'a'));
    }
    return cipher;

}
string otp_decryption(string key,string text)
{
    string plain(text.length(),'a');
    for(int i=0;i<text.length();i++)
    {
        plain[i]='a'+((key[i]-'a')^(text[i]-'a'));
    }
    return plain;
}
void run_otp()
{
     cout<<"one time pad Cipher"<<endl<<endl;
    string text,key;
    cout<<"Enter Key:"<<endl;
    cin>>key;
    cout<<"Enter plain text:"<<endl;
    cin>>text;
    string cipher=otp_encryption(key,text);
    cout<<"cipher:"<<cipher<<endl;
    string plain=otp_decryption(key,cipher);
    cout<<"Plain:"<<plain<<endl;
}
string polyalpha_encryption(string key,string text)
{
    string cipher(text.length(),'a');
    int klen=key.length();
    for(int i=0;i<text.length();i++)
    {
        cipher[i]='a'+((key[i%klen]-'a'+text[i]-'a')%26);
    }
    return cipher;

}
string polyalpha_decryption(string key,string text)
{
    string plain(text.length(),'a');
    int klen=key.length();
    for(int i=0;i<text.length();i++)
    {
        plain[i]='a'+((26-key[i%klen]+text[i])%26);
    }
    return plain;
}
void run_polyalpha()
{
    string text,key;
    cout<<"Enter Key:"<<endl;
    cin>>key;
    cout<<"Enter plain text:"<<endl;
    cin>>text;
    string cipher=polyalpha_encryption(key,text);
    cout<<"cipher:"<<cipher<<endl;
    string plain=polyalpha_decryption(key,cipher);
    cout<<"Plain:"<<plain<<endl;
}
void fillmat(char M[5][5],string s)
{
	vector<int> keys(26,0);
	int k=0,j=-1;
	for(int i=0;i<s.length();i++)
	{
		if(keys[s[i]-'a']==0)
		{
			j=j+1;
			if(j==5)
			{
				k++;
				if(k==5)
				k=0;
				j=0;
			}
			M[k][j]=s[i];

			keys[s[i]-'a']=1;

			if(s[i]=='i'||s[i]=='j')
			{
				keys[8]=1;
				keys[9]=1;
			}
		}
	}
	for(int i=0;i<keys.size();i++)
	{
		if(keys[i]==0)
		{
			j=j+1;
			if(j==5)
			{
				k++;
				if(k==5)
				k=0;
				j=0;
			}
			M[k][j]=i+'a';
			if(i==8)
			{
				keys[9]=1;
			}
		}

	}
}
string playfair_encryption(string s,char M[5][5],map< char,pair<int,int> > RM)
{
    for(int i=0;i<s.length()-1;i+=2)
	{
		char a= s[i];
		char b = s[i+1];
		int r1 = RM[a].first;
		int c1 = RM[a].second;
		int r2 = RM[b].first;
		int c2 = RM[b].second;
		if(r1 == r2)
		{
			c1=(c1+1)%5;
			s[i] = M[r1][c1];
			c2=(c2+1)%5;
			s[i+1]= M[r2][c2];
		}
		else if(c1 == c2)
		{
			r1=(r1+1)%5;
			s[i] = M[r1][c2];
			r2=(r2+1)%5;
			s[i+1]= M[r2][c2];
		}
		else
		{
			s[i] = M[r1][c2];
			s[i+1] = M[r2][c1];
		}
	}
	return s;
}
string playfair_decryption(string s,char M[5][5],map< char,pair<int,int> > RM)
{
    for(int i=0;i<s.length()-1;i+=2)
	{
		char a= s[i];
		char b = s[i+1];
		int r1 = RM[a].first;
		int c1 = RM[a].second;
		int r2 = RM[b].first;
		int c2 = RM[b].second;
		if(r1 == r2)
		{
			c1=(5+c1-1)%5;
			s[i] = M[r1][c1];
			c2=(5+c2-1)%5;
			s[i+1]= M[r2][c2];
		}
		else if(c1 == c2)
		{
			r1=(5+r1-1)%5;
			s[i] = M[r1][c2];
			r2=(5+r2-1)%5;
			s[i+1]= M[r2][c2];
		}
		else
		{
			s[i] = M[r1][c2];
			s[i+1] = M[r2][c1];
		}
	}
	return s;
}

void run_playfair()
{
    string key,text;
    cout<<"Enter Key:\n";
    cin>>key;
    cout<<"Enter text:\n";
    cin>>text;
    char M[5][5];
    map<char,pair<int,int> >RM;
    fillmat(M,key);

    int i=0,j=1;
    while(i<text.length() && j<text.length())
    {
        if(text[i]==text[j])
        {
            text.insert(j,1,'x');
        }
        i=i+2;
        j=j+2;
    }
    if(i<text.length() && j>=text.length())
        text.append("x");

    cout<<"preprocessed text:"<<text<<endl;


    for(int i=0;i<5;i++)
    {
        for(int j=0;j<5;j++)
        {
            RM[M[i][j]]=pair<int,int>(i,j);
        }
    }

    string cipher=playfair_encryption(text,M,RM);
    cout<<"cipher:"<<cipher<<endl;

    string plain=playfair_decryption(cipher,M,RM);
    cout<<"Plain:"<<plain<<endl;

}
int main()
{
   // run_hill();
    //run_polyalpha();
    //run_otp();
    run_playfair();


return 0;
}
