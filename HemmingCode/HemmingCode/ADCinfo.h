#pragma once
#include<iostream>
#include<stdio.h>
#include<iomanip>
#include<bitset>
#include<vector>
#include<math.h>
#include"myfunc.h"
/*функции для генерации исходной информации*/
unsigned short *genInfo(unsigned short);
int writeFile(const char*, unsigned short*);
/*конец*/
struct errorinfo {
	int globalbytepos;
	int razr;
	int razrrev;
	int errorstart;
	errorinfo(int gb) {
		globalbytepos = gb;
		razr = globalbytepos / 128;
		razrrev = 15 - razr;
		errorstart = (globalbytepos % 128) * 8;
	}
};
int writeCodeToFile(const char *path, unsigned char *buf);
void printInfo(unsigned short*);
void printInfo(unsigned char*);
void testprint(unsigned short *buf);
unsigned short *readData(const char*);
unsigned char *readCodedData(const char *path);
unsigned char *readCode(const char*);
void setbit(unsigned short &num, unsigned short pos);
bool bitsetted(unsigned short &num, unsigned short pos);
unsigned short *rebuild(unsigned short*);
unsigned short *rerebuild(unsigned short *data);
unsigned short *chartoshort(unsigned char *from);
unsigned char *CodeInfo(unsigned short *data);
unsigned char *DecodeInfo(unsigned char *data, std::vector<errorinfo> *errors);
char get_d_char(unsigned short num);
unsigned short checkbits2(unsigned short num);
