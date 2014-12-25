#include <iostream>
#include <fstream>
#include <ctime>
#include <cstdlib>

#include "shnorr.h"
#include "generationSimpleFile.h"
#include "hash.h"

class falseBigInt
{
public:
	char* Integer;
	bool Sign; //false - '+'; true - '-'
	int size;
};

char* getSimpleNum(std::ifstream &ifst)
{
	char *simple_num = new char[10];
	ifst>>simple_num;
	return simple_num;
}

char* intToChar(int a)
{
    char *num = new char[10];
    if(!a) return "0";
    int max_d = 0, k = a, j = 0, i = 0, p = 1;
    while(k)
    {
        max_d++;
        k /= 10;
    }
    do
    {
        for(j = 1; j < max_d; j++)
            p *= 10;
        k = a / p;
        a %= p;
        p = 1;
        num[i++] = itoa(k);
        max_d--;
    }while(max_d);
    //num = itoa(a, num, 10);
    num[i] = '\0';
    return num;
}

int charToInt(char *a)
{
    int num = 0;
    for(int i = 0; i < strlen(a); i++)
    {
        num *= 10;
        num += atoi(a[i]);
    }
    //num = atoi(a);
    return num;
}

int Space(std::ifstream &ifst)
{
    char c;
    int  amount = 0;
    while(!ifst.eof())
    {
        ifst.get(c);
        if(c == ' ')
            amount++;
    }
    return amount;
}

void GeneratedKey (char *p_fname)
{
    BigInt P(p_fname, false), one("1"), Fi = P - one;
    BigInt Q, G("2"), Y("1");
	//ðàçáèâàåì íà ìíîæåòåëè P-1
	int amount = 5761456, **degree, i, max = 0;
	std::ifstream ifst("simpleNum.txt");
	if(!ifst)
	{
		GenetateSimpleList();
		ifst.open("simpleNum.txt");
	}
	char *buf_ch = new char[10];
    BigInt A = BigInt(&Fi), B;
	degree = new int *[2];
	for(i = 0; i < 2; i ++)
		degree[i] = new int [amount];
	memset(degree[0], 0, 4 * amount);
	memset(degree[1], 0, 4 * amount);
	i = 0;
	bool end = true;
	while(end)
	{
		buf_ch = getSimpleNum(ifst);
        if((A % BigInt(buf_ch)) == BigInt("0"))
		{
            A /= BigInt(buf_ch);
			degree[0][i] = charToInt(buf_ch);
			degree[1][i]++;
			i = 0;
			ifst.seekg (0, ifst.beg);
		}
		else
			i++;
		if(A == one)
			end = false;
	}
	ifst.close();
	for (i = 0; i < amount; i++)
		if(max < degree[0][i])
			max = degree[0][i];
    Q = BigInt(intToChar(max));
	//G
    BigInt b;
	while (true)
	{
        b = (G ^ Q) % P ;
        if (b % P == one)
			break;
		G++;
	}
	srand(time(0));
	int w =  0;
	while(w < 2)
        w = rand()%max;
    BigInt W(intToChar(w));
	//Y = (G^W)
    BigInt DegreeNet[30], buf_bi("1"), deg = Fi - one;
	DegreeNet[0] = G;
	DegreeNet[0] %= P;
	for(int i = 1; i < 30; i++)
	{
		DegreeNet[i] = DegreeNet[i-1] * DegreeNet[i-1];
		DegreeNet[i] %= P;
	}
    BigInt degreeNum[30];
    degreeNum[0] = BigInt("1");
	for(int i = 1; i < 30; i++)
        degreeNum[i] = degreeNum[i-1] * BigInt("2");
    BigInt I("0");
	for(int j = 29; j >= 0;)
	{
		if(W >= I + degreeNum[j])
		{
			buf_bi *= DegreeNet[j];
			buf_bi %= P;
			I += degreeNum[j];
		}
		else
			j--;
	}
	//Y = (G^(P-2)) % P
	DegreeNet[0] = buf_bi;
	DegreeNet[0] %= P;
	for(int i = 1; i < 30; i++)
	{
		DegreeNet[i] = DegreeNet[i-1] * DegreeNet[i-1];
		DegreeNet[i] %= P;
	}
    I = BigInt("0");
	for(int j = 29; j >= 0;)
	{
		if(deg >= I + degreeNum[j])
		{
			Y *= DegreeNet[j];
			Y %= P;
			I += degreeNum[j];
		}
		else
			j--;
	}

	Q.TextWrite("q.txt");
	G.TextWrite("g.txt");
	Y.TextWrite("y.txt");
	W.TextWrite("w.txt");
}

void AuProtocol(char *p_fname,char *q_fname,char *g_fname,char *y_fname, char *w_fname)
{
	// pretreatment
    BigInt P(p_fname, false), Q(q_fname, false), G(g_fname, false), Y(y_fname, false);
	std::ifstream inq(q_fname);
	int q;
	inq >> q;
	inq.close();
	srand(time(0));
	int r =  0;
	while(r < 2)
		r = rand() % q;
    BigInt R(intToChar(r));
    BigInt X("1");
    BigInt DegreeNet[30];
	DegreeNet[0] = G;
	DegreeNet[0] %= P;
	for(int i = 1; i < 30; i++)
	{
		DegreeNet[i] = DegreeNet[i-1] * DegreeNet[i-1];
		DegreeNet[i] %= P;
	}
    BigInt degreeNum[30];
    degreeNum[0] = BigInt("1");
	for(int i = 1; i < 30; i++)
        degreeNum[i] = degreeNum[i-1] * BigInt("2");
    BigInt I("0");
	for(int j = 29; j >= 0;)
	{
		if(R >= I + degreeNum[j])
		{
			X *= DegreeNet[j];
			X %= P;
			I += degreeNum[j];
		}
		else
			j--;
	}
	//Send X opponent; Opponent send e.
	//E = random (1; 2^t - 1);
	srand(time(0));
	int e =  0;
	while(e < 2)
        e = rand();
    BigInt E(intToChar(e));
	// We calculate s;
	//S = (R+WE)mod Q;
    BigInt W(w_fname, false), S = E % Q;
	S *= W % Q;
	S %= Q;
	S += R % Q;
	S %= Q;
	//Send for accept
	// Z = (G^S)(Y^E) mod P;
    BigInt Z("1"), buf1("1");
	DegreeNet[0] = G;
	DegreeNet[0] %= P;
	for(int i = 1; i < 30; i++)
	{
		DegreeNet[i] = DegreeNet[i-1] * DegreeNet[i-1];
		DegreeNet[i] %= P;
	}
    I = BigInt("0");
	for(int j = 29; j >= 0;)
	{
		if(S >= I + degreeNum[j])
		{
			buf1 *= DegreeNet[j];
			buf1 %= P;
			I += degreeNum[j];
		}
		else
			j--;
	}
	DegreeNet[0] = Y;
	DegreeNet[0] %= P;
	for(int i = 1; i < 30; i++)
	{
		DegreeNet[i] = DegreeNet[i-1] * DegreeNet[i-1];
		DegreeNet[i] %= P;
	}
    I = BigInt("0");
	for(int j = 29; j >= 0;)
	{
		if(E >= I + degreeNum[j])
		{
			Z *= DegreeNet[j];
			Z %= P;
			I += degreeNum[j];
		}
		else
			j--;
	}
	Z *= buf1;
	Z %= P;
	if (X == Z)
		std::cout<<"All right\n";
	else
		std::cout<<"All wrong!\n";
}
