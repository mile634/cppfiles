#include<iostream>
#include"ADCinfo.h"
/*Ты остановился на написании функции для перетасовки байт*/
/*firstprjcheck(); */
using namespace std;

void generatenewdata()
{
	unsigned short *data = genInfo(1024);
	writeFile("E:/Games/generateddata.dat", data);
	delete[] data;
}

int main()
{	
	setlocale(LC_ALL, "Russian");
	generatenewdata();
	unsigned short *data = readData("E:/Games/generateddata.dat");
	unsigned short *copy = readData("E:/Games/generateddata.dat");
	//testprint(data);
	data = rebuild(data);
	unsigned char *coded = CodeInfo(data);
	unsigned char *coded2 = CodeInfo(data);
	writeCodeToFile("E:/Games/codeddata.dat", coded);
	delete[] coded;
	cout << "Try to change: E:/Games/codeddata.dat" << endl;
	system("pause");
	coded = readCodedData("E:/Games/codeddata.dat");
	
	vector<errorinfo> errorpos;
	unsigned char *decoded = DecodeInfo(coded,&errorpos);
	data = chartoshort(decoded);
	data = rerebuild(data);
	for (int i = 0; i < 1024; i+=8)
	{
		vector<errorinfo> tmpvector;
		for (errorinfo pos : errorpos)
		{
			if (i == pos.errorstart)
			{
				tmpvector.push_back(pos);					
			}
		}
		if (tmpvector.empty()) {
			for (int j = 0; j < 8; j++)
			{
				cout << endl << setw(4)<< i+j << ": Default info: " <<setw(6)<< copy[i + j] << " After decode: " << setw(6)<< data[i + j]<<" Confirmed!";
			}
		}
		else
		{
			int error = 0;
			for (errorinfo er : tmpvector)
			{
				error += pow(2, er.razrrev);
			}
			cout << "\nNext 8 values can be errored. possible error: " << error << "("<<(error/65535.0)*100<<"%)\n{";
			for (int j = 0; j < 8; j++)
			{
				cout << endl << setw(4) << i + j << ": Default info: " << setw(6) << copy[i + j] << " After decode: " << setw(6) << data[i + j];
			}
			cout << "\n}";
		}
	}
	system("pause");
	return 0;
}