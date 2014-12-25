#include <fstream>
#include <iostream>
#include "shnorr.h"


int main(int argc, char *argv[])
{
    int menu;
	char *a_P = new char[24];
	//std::cout<<"Input file name P for A: \n";
	//std::cin>>a_P;
	//GeneratedKey(a_P);
	GeneratedKey("p.txt");
	AuProtocol("p.txt","q.txt","g.txt","y.txt","w.txt");
    std::cin>>menu;
	return 0;
}

/*int main(int argc, char *argv[])
{
	int menu = 0;
	char *a_P = new char[24];
	if (argc == 2)
	{
		if(!strcmp("-h", argv[1]) || !strcmp("-H", argv[1]) || !strcmp("-help", argv[1]))
		{
            std::cout<<"This program implements Shnoor scheme authentication mode\nExample of using:\n rsa.exe file_P_for_A file_Q_for_A file_E_for_A file_P_for_B file_Q_for_B file_E_for_B";
			return 1;
		}
		else
		{
			strcpy(a_P, argv[1]);
            GeneratedKey(a_P);
            AuProtocol(a_P,"q.txt","g.txt","y.txt","w.txt");
        }
	}
	else
	{
		std::cout<<"Not enough input data\n";
	}
	std::cin>>menu;
	return 0;
}
*/
