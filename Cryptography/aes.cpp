#include<iostream>
#include <stdio.h>
#include<string.h>

using namespace std;

static const uint8_t sbox[256] =   {
  //0     1    2      3     4    5     6     7      8    9     A      B    C     D     E     F
  0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,
  0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,
  0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,
  0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,
  0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,
  0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,
  0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,
  0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,
  0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,
  0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,
  0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,
  0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,
  0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,
  0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,
  0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,
  0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16 };

static const uint8_t rsbox[256] =
{ 0x52, 0x09, 0x6a, 0xd5, 0x30, 0x36, 0xa5, 0x38, 0xbf, 0x40, 0xa3, 0x9e, 0x81, 0xf3, 0xd7, 0xfb,
  0x7c, 0xe3, 0x39, 0x82, 0x9b, 0x2f, 0xff, 0x87, 0x34, 0x8e, 0x43, 0x44, 0xc4, 0xde, 0xe9, 0xcb,
  0x54, 0x7b, 0x94, 0x32, 0xa6, 0xc2, 0x23, 0x3d, 0xee, 0x4c, 0x95, 0x0b, 0x42, 0xfa, 0xc3, 0x4e,
  0x08, 0x2e, 0xa1, 0x66, 0x28, 0xd9, 0x24, 0xb2, 0x76, 0x5b, 0xa2, 0x49, 0x6d, 0x8b, 0xd1, 0x25,
  0x72, 0xf8, 0xf6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xd4, 0xa4, 0x5c, 0xcc, 0x5d, 0x65, 0xb6, 0x92,
  0x6c, 0x70, 0x48, 0x50, 0xfd, 0xed, 0xb9, 0xda, 0x5e, 0x15, 0x46, 0x57, 0xa7, 0x8d, 0x9d, 0x84,
  0x90, 0xd8, 0xab, 0x00, 0x8c, 0xbc, 0xd3, 0x0a, 0xf7, 0xe4, 0x58, 0x05, 0xb8, 0xb3, 0x45, 0x06,
  0xd0, 0x2c, 0x1e, 0x8f, 0xca, 0x3f, 0x0f, 0x02, 0xc1, 0xaf, 0xbd, 0x03, 0x01, 0x13, 0x8a, 0x6b,
  0x3a, 0x91, 0x11, 0x41, 0x4f, 0x67, 0xdc, 0xea, 0x97, 0xf2, 0xcf, 0xce, 0xf0, 0xb4, 0xe6, 0x73,
  0x96, 0xac, 0x74, 0x22, 0xe7, 0xad, 0x35, 0x85, 0xe2, 0xf9, 0x37, 0xe8, 0x1c, 0x75, 0xdf, 0x6e,
  0x47, 0xf1, 0x1a, 0x71, 0x1d, 0x29, 0xc5, 0x89, 0x6f, 0xb7, 0x62, 0x0e, 0xaa, 0x18, 0xbe, 0x1b,
  0xfc, 0x56, 0x3e, 0x4b, 0xc6, 0xd2, 0x79, 0x20, 0x9a, 0xdb, 0xc0, 0xfe, 0x78, 0xcd, 0x5a, 0xf4,
  0x1f, 0xdd, 0xa8, 0x33, 0x88, 0x07, 0xc7, 0x31, 0xb1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xec, 0x5f,
  0x60, 0x51, 0x7f, 0xa9, 0x19, 0xb5, 0x4a, 0x0d, 0x2d, 0xe5, 0x7a, 0x9f, 0x93, 0xc9, 0x9c, 0xef,
  0xa0, 0xe0, 0x3b, 0x4d, 0xae, 0x2a, 0xf5, 0xb0, 0xc8, 0xeb, 0xbb, 0x3c, 0x83, 0x53, 0x99, 0x61,
  0x17, 0x2b, 0x04, 0x7e, 0xba, 0x77, 0xd6, 0x26, 0xe1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0c, 0x7d };

static const uint8_t Rcon[10] = { 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1B, 0x36 };

static const uint8_t mix[4][4]={{0x02, 0x03, 0x01, 0x01},
                                {0x01, 0x02, 0x03, 0x01},
                                {0x01, 0x01, 0x02, 0x03},
                                {0x03, 0x01, 0x01, 0x02}
                                };
static const uint8_t rmix[4][4]={{0x0e, 0x0b, 0x0d, 0x09},
                                {0x09, 0x0e, 0x0b, 0x0d},
                                {0x0d, 0x09, 0x0e, 0x0b},
                                {0x0b, 0x0d, 0x09, 0x0e}
                                };

unsigned char key[16],ek[176];

unsigned char text[16];

int hex_to_int(unsigned char c){
        int first = c / 16 - 3;
        int second = c % 16;
        int result = first*10 + second;
        if(result > 9) result--;
        return result;
}

int hex_to_ascii(unsigned char c, unsigned char d){
        int high = hex_to_int(c) * 16;
        int low = hex_to_int(d);
        return high+low;
}

void shiftrow(unsigned char a[4][4])
{
    unsigned char temp[4];
    for(int i=1;i<4;i++)
    {
        for(int j=0;j<4;j++)
        {
            temp[j]=a[i][j];
        }
        for(int j=0;j<4;j++)
        {
            a[i][(4+j-i)%4]=temp[j];
        }
    }

}
void invshiftrow(unsigned char a[4][4])
{
    unsigned char temp[4];
    for(int i=1;i<4;i++)
    {
        for(int j=0;j<4;j++)
        {
            temp[j]=a[i][j];
        }
        for(int j=0;j<4;j++)
        {
            a[i][(j+i)%4]=temp[j];
        }
    }

}
void subsbox(unsigned char a[4][4])
{

    for(int i=0;i<4;i++)
    {
         for(int j=0;j<4;j++)
        {

            a[i][j]=sbox[a[i][j]];
        }
    }

}
void invsubsbox(unsigned char a[4][4])
{

    for(int i=0;i<4;i++)
    {
         for(int j=0;j<4;j++)
        {

           a[i][j]=rsbox[a[i][j]];
        }
    }
}
void addRoundKey(unsigned char a[4][4],int offset)
{
    for(int i=0;i<4;i++)
    {
        for(int j=0;j<4;j++)
        {
            a[j][i]=(unsigned char)(a[j][i]^ek[16*offset+4*i+j]);
        }
    }
}
unsigned char gfmul(unsigned char a,unsigned char b)
{
    unsigned char r[8];
    if(a==0x00 || b==0x00)
    {
        return 0x00;
    }
    else if(a==0x01)
    {
        return b;
    }
    else if(b==0x01)
    {
        return a;
    }
    else
    {
        r[0]=a;
        unsigned char sum=0x00;
        for(int i=1;i<8;i++)
        {
            if((r[i-1]>>7)&1)
            {
                r[i]=(r[i-1]<<1)^(0x1b);
            }
            else
            {
                r[i]=(r[i-1]<<1);
            }
        }
        for(int i=0;i<8;i++)
        {
            if((b>>i)&1)
            {
                sum=sum^r[i];
            }
        }
        return sum;
    }

}
void mixcolumns(unsigned char a[4][4])
{
    unsigned char t[4][4];
    for(int i=0;i<4;i++)
    {
        for(int j=0;j<4;j++)
        {
            unsigned char temp=0x00;
            for(int k=0;k<4;k++)
            {
                temp=temp^gfmul(mix[i][k],a[k][j]);
            }
            t[i][j]=temp;
        }
    }

    for(int i=0;i<4;i++)
    {
        for(int j=0;j<4;j++)
        {
            a[i][j]=t[i][j];
        }
    }

}
void invmixcolumns(unsigned char a[4][4])
{
    unsigned char t[4][4];
    for(int i=0;i<4;i++)
    {
        for(int j=0;j<4;j++)
        {
            unsigned char temp=0x00;
            for(int k=0;k<4;k++)
            {
                temp=temp^gfmul(rmix[i][k],a[k][j]);
            }
            t[i][j]=temp;
        }
    }

    for(int i=0;i<4;i++)
    {
        for(int j=0;j<4;j++)
        {
            a[i][j]=t[i][j];
        }
    }

}

void printmat(unsigned char a[4][4])
{
    cout<<endl;
    for(int i=0;i<4;i++)
    {

        for(int j=0;j<4;j++)
        {

             printf("%02X ",a[i][j]);
        }
        cout<<endl;
    }
    cout<<endl;
}

void aes_encrypt(unsigned char a[4][4])
{
    cout<<endl<<"AES Encryption"<<endl;
    printmat(a);
    addRoundKey(a,0);
    cout<<"Encryption Round 0"<<endl;
    printmat(a);

    for(int i=0;i<10;i++)
    {
        cout<<"Encryption Round "<<i+1<<endl;
        subsbox(a);
        cout<<"subed--\n";
        printmat(a);
        cout<<"shifted--\n";
        shiftrow(a);
        printmat(a);

        if(i!=9)
        {
            cout<<"mixcoulmn--\n";
            mixcolumns(a);
            printmat(a);
        }
        cout<<"roundkeyed--\n";
        addRoundKey(a,i+1);
        printmat(a);
    }

}

void aes_decrypt(unsigned char a[4][4])
{
    cout<<endl<<"AES Decrytion"<<endl;
    printmat(a);
    addRoundKey(a,10);
    cout<<"Decryption Round 0"<<endl;
    printmat(a);

    for(int i=9;i>=0;i--)
    {
        cout<<"Decryption Round "<<9-i+1<<endl;

        cout<<"invshifted--\n";
        invshiftrow(a);
        printmat(a);
        cout<<"invsubed--\n";
        invsubsbox(a);
        printmat(a);
        cout<<"roundkeyed--\n";
        addRoundKey(a,i);
        printmat(a);
        if(i!=0)
        {
            cout<<"invmixcoulmn--\n";
            invmixcolumns(a);
            printmat(a);
        }

    }

}

void rotword(unsigned char word[])
{
    unsigned char t[4];
    for(int i=0;i<4;i++)
    {
        t[i]=word[i];
    }
    for(int j=0;j<4;j++)
    {
        word[(4+j-1)%4]=t[j];
    }
    //return word;
}
void subword(unsigned char word[])
{

    for(int i=0;i<4;i++)
    {
       // unsigned char c=word[i];
        word[i]=sbox[word[i]];
        //printf("%02X ",word[i]);
        //printf("%02X",  (int)(*(unsigned unsigned char*)(&word[i])) );
    }


    //return word;
}
void printword(unsigned char word[])
{
     for(int j=0;j<4;j++)
    {
         printf("%02X ",word[j]);
    }
    printf("\n");
}
void keyexpansion(unsigned char k[],unsigned char ek[])
{
    unsigned char temp[4];
    for(int i=0;i<16;i++)
    {
        ek[i]=k[i];
    }
    for(int i=4;i<44;i++)
    {
        for(int j=0;j<4;j++)
        {
            temp[j]=ek[4*i-(4-j)];
        }


        if(i%4==0)
        {
            cout<<endl<<"Round "<<i/4<<endl;
           // printword(temp);
            rotword(temp);
            //printword(temp);
            subword(temp);
            //printword(temp);
           // printf("%02X ",Rcon[(i-1)/4]);
            temp[0]=temp[0]^Rcon[(i-1)/4];
           // printword(temp);
        }
        //cout<<endl;
        for(int j=0;j<4;j++)
        {
            ek[i*4+j]=ek[(i-4)*4+j]^temp[j];
            printf("%02X ",ek[i*4+j]);
        }

    }
}
void fillstate(unsigned char a[4][4],unsigned char text[])
{
    int k=0;
    for(int i=0;i<4;i++)
    {
        for(int j=0;j<4;j++)
        {

            a[j][i]=text[k];
            k++;
        }
    }
}

int main()
{
    //unsigned char x='a';
    //unsigned char y='b';
    //unsigned char* c;
    //unsigned unsigned char status = 'a';
   // printf("%02X ",gfmul(0x0e,0x02));
     string st = "5468617473206D79204B756E67204675";

    int length = st.length();
    int i;
    unsigned char buf = 0;
    int j=0;
    for(i = 0; i < length; i++){
            if(i % 2 != 0){
                   key[j]=(unsigned char)hex_to_ascii(buf, st[i]);
                    j++;
            }else{
                    buf = st[i];
            }
    }
    for(int i=0;i<j;i++)
     cout<<key[i];

    cout<<endl;
    for(int i=0;i<j;i++)
     printf("%02X ",key[i]);
    cout<<endl;
    keyexpansion(key,ek);

    cout<<endl;
    unsigned char a[4][4];
    unsigned char text[20]="Two One Nine Two ";
    fillstate(a,text);
    printmat(a);
    aes_encrypt(a);
    cout<<"=======cipher text==========";
    printmat(a);
    aes_decrypt(a);
    cout<<"========plain text==========";
    printmat(a);


   return 0;
}
