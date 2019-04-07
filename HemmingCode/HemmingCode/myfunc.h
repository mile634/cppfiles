#pragma once
#include<string>
#include<iostream>
#include<vector>
#include<sstream>
#include<fstream>
unsigned int getNum(std::string);
unsigned short calcOnes(unsigned short);
unsigned short checkbits(unsigned short);
char get_d_char(short); /*получает символ из 12 бит*/
std::string Code(std::string);
std::string Decode(std::string, bool&);
std::string fix(std::string);
std::string readFile(const std::string&);
void calcBits(unsigned short &num);
void insertBits(unsigned short &);
void invertBit(unsigned short&, unsigned short pos);
void writeFile(std::string , std::string);
void writeFileBin(std::string, std::string);/*не исп*/
void firstprjcheck();