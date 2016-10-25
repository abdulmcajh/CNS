#include<iostream>
#include<stdio.h>
#include<string.h>
#include<bitset>
using namespace std;
static const int ip[64]=
 {58,50,42,34,26,18,10,2,
60,52,44,36,28,20,12,4,
62,54,46,38,30,22,14,6,
64,56,48,40,32,24,16,8,
57,49,41,33,25,17,9,1,
59,51,43,35,27,19,11,3,
61,53,45,37,29,21,13,5,
63,55,47,39,31,23,15,7};

static const int ep[48] = {32,1,2,3,4,5,
			4,5,6,7,8,9,
			8,9,10,11,12,13,
			12,13,14,15,16,17,
			16,17,18,19,20,21,
			20,21,22,23,24,25,
			24,25,26,27,28,29,
			28,29,30,31,32,1};

static const int p[32] = {16,7,20,21,29,12,28,17,
			1,15,23,26,5,18,31,10,
			2,8,24,14,32,27,3,9,
			19,13,30,6,22,11,4,25};

static const int sbox[8][4][16] = {{{14,4,13,1,2,15,11,8,3,10,6,12,5,9,0,7},
			 		{0,15,7,4,14,2,13,1,10,6,12,11,9,5,3,8},
			 		{4,1,14,8,13,6,2,11,15,12,9,7,3,10,5,0},
			 		{15,12,8,2,4,9,1,7,5,11,3,14,10,0,6,13}},

					{{15,1,8,14,6,11,3,4,9,7,2,13,12,0,5,10},
        			{3,13,4,7,15,2,8,14,12,0,1,10,6,9,11,5},
        			{0,14,7,11,10,4,13,1,5,8,12,6,9,3,2,15},
        			{13,8,10,1,3,15,4,2,11,6,7,12,0,5,14,9}},

					{{10,0,9,14,6,3,15,5,1,13,12,7,11,4,2,8},
    	       		{13,7,0,9,3,4,6,10,2,8,5,14,12,11,15,1},
    	       		{13,6,4,9,8,15,3,0,11,1,2,12,5,10,14,7},
    				{1,10,13,0,6,9,8,7,4,15,14,3,11,5,2,12}},

					{{7,13,14,3,0,6,9,10,1,2,8,5,11,12,4,15},
    	       		{13,8,11,5,6,15,0,3,4,7,2,12,1,10,14,9},
    	       		{10,6,9,0,12,11,7,13,15,1,3,14,5,2,8,4},
    	       		{3,15,0,6,10,1,13,8,9,4,5,11,12,7,2,14}},

					{{2,12,4,1,7,10,11,6,8,5,3,15,13,0,14,9},
    	       		{14,11,2,12,4,7,13,1,5,0,15,10,3,9,8,6},
     	       		{4,2,1,11,10,13,7,8,15,9,12,5,6,3,10,14},
    	       		{11,8,12,7,1,14,2,13,6,15,0,9,10,4,5,3}},

					{{12,1,10,15,9,2,6,8,0,13,3,4,14,7,5,11},
    	       		{10,15,4,2,7,12,9,5,6,1,13,14,0,11,3,8},
    	        	{9,14,15,5,2,8,12,3,7,0,4,10,1,13,11,6},
    				{4,3,2,12,9,5,15,10,11,14,1,7,6,0,8,13}},

					{{4,11,2,14,15,0,8,13,3,12,9,7,5,10,6,1},
    	       		{13,0,11,7,4,9,1,10,14,3,5,12,2,15,8,6},
    	        	{1,4,11,13,12,3,7,14,10,15,6,8,0,5,9,2},
    	        	{6,11,13,8,1,4,10,7,9,5,0,15,14,2,3,12}},

					{{13,2,8,4,6,15,11,1,10,9,3,14,5,0,12,7},
    				{1,15,13,8,10,3,7,4,12,5,6,11,0,14,9,2},
    				{7,11,4,1,9,12,14,2,0,6,10,13,15,3,5,8},
    				{2,1,14,7,4,10,8,13,15,12,9,0,3,5,6,11}}};

static const int shiftnumber[16] = {1,1,2,2,2,2,2,2,1,2,2,2,2,2,2,1};

static const int pc1[56] = {57,49,41,33,25,17,9,
                            1,58,50,42,34,26,18,
                            10,2,59,51,43,35,27,
                            19,11,3,60,52,44,36,
                            63,55,47,39,31,23,15,
                            7,62,54,46,38,30,22,
                            14,6,61,53,45,37,29,
                            21,13,5,28,20,12,4};
static const int pc2[48] = {14,17,11,24,1,5,
                            3,28,15,6,21,10,
                            23,19,12,4,26,8,
                            16,7,27,20,13,2,
                            41,52,31,37,47,55,
                            30,40,51,45,33,48,
                            44,49,39,56,34,53,
                            46,42,50,36,29,32};
string subkey[16];
static int ipinv[64];

void initialpermutation(string& s)
{
    string t (64,'a');
    for(int i=0;i<64;i++)
    {
        t[i]=s[ip[i]-1];
    }
    s.assign(t);
}
void calculateinvip()
{
    for(int i=0;i<64;i++)
    {
        ipinv[ip[i]-1]=i+1;
    }
    /*for(int i=0;i<8;i++)
    {
        for(int j=0;j<8;j++)
        {
            cout<<ipinv[i*8+j]<<" ";
        }
        cout<<endl;
    }*/
}
void invinitialpermutation(string& s)
{
    string t (64,'a');
    for(int i=0;i<64;i++)
    {
        t[i]=s[ipinv[i]-1];
    }
    s.assign(t);
}
void expand(string& s)
{
     string t (48,'a');
    for(int i=0;i<48;i++)
    {
        t[i]=s[ep[i]-1];
    }
    s.assign(t);
}
void subsbox(string& s)
{
    string temp="";
    for(int i=0;i<8;i++)
    {
        string t (s,6*i,6);

         bitset<2> b1(t,0,2);
         bitset<4> b2(t,2,4);
        int r=b1.to_ulong();
        int c=b2.to_ulong();
        int x=sbox[i][r][c];
        bitset<4> a(x);
        temp=temp+a.to_string();
    }
    s.assign(temp);
}
void XOR(string& s,string k,int flag)
{
    if(flag==1)
    {
        //cout<<"text"<<s<<endl;
        //cout<<"key"<<k<<endl;
        bitset<48> a(s);
        bitset<48> b(k);
      //  bitset<48> c=t^b;
        s.assign((a^b).to_string());
    }
    else
    {
        bitset<32> a(s);
        bitset<32> b(k);
        bitset<32> c=a^b;
        s.assign(c.to_string());
    }


}
void permutation(string& s)
{
    string t (32,'a');
    for(int i=0;i<32;i++)
    {
        t[i]=s[p[i]-1];
    }
    s.assign(t);
}
void permutechoice1(string& s)
{
    string t (56,'a');
    for(int i=0;i<56;i++)
    {
        t[i]=s[pc1[i]-1];
    }
    s.assign(t);
}
void permutechoice2(string s,string& r)
{
    string t (48,'a');
    for(int i=0;i<48;i++)
    {
        t[i]=s[pc2[i]-1];
    }
    r.assign(t);
}
void circularshift(string& s,int round)
{
    string C(s,0,28);
    string D(s,28,28);
    for(int j=0;j<28;j++)
    {
        C[(28+j-shiftnumber[round])%28]=s[j];
        D[(28+j-shiftnumber[round])%28]=s[28+j];
    }

    s.assign(C+D);
}
void genrateroundkey(string& k,int round)
{
    circularshift(k,round);
    permutechoice2(k,subkey[round]);
}
void generatekeys(string k)
{
    permutechoice1(k);
    for(int i=0;i<16;i++)
    {
         genrateroundkey(k,i);
    }
}
void des_encrypt(string& s,string k)
{
    initialpermutation(s);

    for(int i=0;i<16;i++)
    {
        string L(s,0,32);
        string R(s,32,32);
        s.clear();
        s.assign(R);

        expand(R);
        //cout<<L+R<<endl;
        XOR(R,subkey[i],1);
        subsbox(R);
        permutation(R);

        XOR(L,R,0);
        s.append(L);
        // if(i==1)
        //    break;
    }
    //swap
    string Lf(s,0,32);
    string Rf(s,32,32);
    s.assign(Rf+Lf);

    invinitialpermutation(s);

}
void des_decrypt(string& s,string k)
{
    initialpermutation(s);


    for(int i=0;i<16;i++)
    {
        string L(s,0,32);
        string R(s,32,32);
        s.clear();
        s.assign(R);
        expand(R);
        XOR(R,subkey[15-i],1);
        subsbox(R);
        permutation(R);
        XOR(L,R,0);
        s.append(L);

    }

    string Lf(s,0,32);
    string Rf(s,32,32);
    s.assign(Rf+Lf);

    invinitialpermutation(s);

}
void char_to_bin(unsigned char a[],string& r)
{
    for(int i=0;i<8;i++)
    {
     bitset<8> t(a[i]);
     r=r+t.to_string();
    }
}
int main()
{
    unsigned char rk[8];
    unsigned char rs[8];
    calculateinvip();
    cout<<"Enter Key(8 bytes):";
    cin>>rk;
    cout<<"Enter text(8 bytes):";
    cin>>rs;
    string s,k;
    char_to_bin(rk,k);
    char_to_bin(rs,s);
    cout<<"KEY: \n"<<k<<endl;
    generatekeys(k);
    cout<<"Plain text: \n"<<s<<endl;

    des_encrypt(s,k);
    cout<<"Cipher text: \n"<<s<<endl;

    des_decrypt(s,k);
    cout<<"Decrypted text: \n"<<s<<endl;
return 0;
}
