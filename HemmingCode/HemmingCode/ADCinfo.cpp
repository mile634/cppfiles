#include"ADCinfo.h"
#include"myfunc.h"
#define INFO_SIZE 1024
#define INFOCHARS_SIZE 2048
#define CODE_SIZE 3072
/*функции для генерации исходной информации*/
unsigned short *genInfo(unsigned short size) {
	{
		unsigned short *buf = new unsigned short[size];
		for (int i = 0; i < size; i++)
			*(buf + i) = rand() % 65000;
		return buf;
	}
}
int writeFile(const char *path, unsigned short *buf)
{
	FILE *f;
	errno_t ok = fopen_s(&f, path, "wb");
	if (ok) { std::cout << "Error when open: " << ok; return 0; }
	fwrite(buf, sizeof(unsigned short), INFO_SIZE, f);
	fclose(f);
	return 1;
}
/*конец*/
int writeCodeToFile(const char *path, unsigned char *buf)
{
	FILE *f;
	errno_t ok = fopen_s(&f, path, "wb");
	if (ok) { std::cout << "Error when open: " << ok; return 0; }
	fwrite(buf, sizeof(unsigned char), CODE_SIZE, f);
	fclose(f);
	return 1;
}
void printInfo(unsigned short *buf)
{
	for (int i = 0; i < INFO_SIZE; i++) std::cout << std::setw(6) << *(buf + i);
}
void printInfo(unsigned char *buf)
{
	for (int i = 0; i < INFOCHARS_SIZE; i++) std::cout << std::setw(1) << *(buf + i);
}
void printDecodedInfo(unsigned char *buf)
{
	for (int i = 0; i < INFOCHARS_SIZE; i++) std::cout << *(buf + i);
}
void testprint(unsigned short *buf)
{
	for (int i = 0; i < INFO_SIZE; i++)
	{
		std::bitset<sizeof(short) * 8> num(buf[i]);
		std::cout <<i<<" short: "<< num << std::endl;
	}
}
unsigned short *readData(const char *path)
{
	unsigned short *buf = new unsigned short[INFO_SIZE];
	FILE *f;
	errno_t ok = fopen_s(&f, path, "rb");
	if (ok) { std::cout << "Error openread: " << ok; return NULL; }
	fread_s(buf, sizeof(unsigned short)*INFO_SIZE, sizeof(unsigned short), INFO_SIZE, f);
	fclose(f);
	return buf;
}
unsigned char *readCodedData(const char *path)
{
	unsigned char *buf = new unsigned char[CODE_SIZE];
	FILE *f;
	errno_t ok = fopen_s(&f, path, "rb");
	if (ok) { std::cout << "Error openread: " << ok; return NULL; }
	fread_s(buf, sizeof(unsigned char)*CODE_SIZE, sizeof(unsigned char), CODE_SIZE, f);
	fclose(f);
	return buf;
}
unsigned char *readCode(const char *path)
{
	unsigned char *buf = new unsigned char[CODE_SIZE];
	FILE *f;
	errno_t ok = fopen_s(&f, path, "rb");
	if (ok) { std::cout << "Error openread: " << ok; return NULL; }
	fread_s(buf, sizeof(unsigned char)*CODE_SIZE, sizeof(unsigned char), CODE_SIZE, f);
	fclose(f);
	return buf;
}
void setbit(unsigned short &num, unsigned short pos)
{
	num |= (1 << pos);
}
bool bitsetted(unsigned short &num, unsigned short pos)
{
	return ((num >> pos) % 2);
}
unsigned short *rebuild(unsigned short *data) {
	unsigned short *buf = new unsigned short[INFO_SIZE];
	for (int i = 0; i < INFO_SIZE; i++) *(buf + i) = 0;
	int bitcounter = 15, shortcounter = 0;
	for (int k = 15; k >= 0; k--)
		for (int i = 0; i < INFO_SIZE; i++)
		{
			if (bitsetted(*(data+i), k)) {
				setbit(buf[shortcounter], bitcounter);
			}
			bitcounter--;
			if (bitcounter == -1) { bitcounter = 15; shortcounter++; }
		}
	return buf;
}


unsigned short *rerebuild(unsigned short *data) {
	unsigned short *buf = new unsigned short[INFO_SIZE];
	for (int i = 0; i < INFO_SIZE; i++) *(buf + i) = 0;
	for(int i = 0;i<16;i++)
	{
		int bufi = 0;
		for (int k = 0; k < 64; k++)
		{
			unsigned short block = data[i*64+k];
			
			for (int j = 15; j >= 0; j--)
			{
				if (bitsetted(block, j)) setbit(buf[bufi], 15-i);	
				//unsigned short tmp = buf[bufi];
				bufi++;
			}
			
		}
	}
	return buf;
}
unsigned short *chartoshort(unsigned char *from) {
	unsigned short *info = new unsigned short[INFO_SIZE];
	for (int i = 0; i < INFO_SIZE; i++)
	{
		info[i] = ((unsigned short)from[i * 2] << 8) | from[i * 2 + 1];
	}
	return info;
}
unsigned char *CodeInfo(unsigned short *data)
{
	unsigned char *bufer = new unsigned char[CODE_SIZE];
	int counter = 0;
	for (int i = 0; i < INFO_SIZE; i++)
	{
		unsigned short buf = 0;

		unsigned char first = data[i] >> 8;			
		buf |= first;
		insertBits(buf);
		calcBits(buf);		
		unsigned char symb = buf >> 4;
		bufer[counter++] = symb;
		buf = (buf & 15) << 10;		
		unsigned char second = data[i]&255;
		buf |= second;
		insertBits(buf);
		calcBits(buf);
		unsigned char symb1 = buf >> 8;
		unsigned char symb2 = buf & 255;
		bufer[counter++] = symb1;
		bufer[counter++] = symb2;
	}
	return bufer;
}

unsigned char *DecodeInfo(unsigned char *data, std::vector<errorinfo> *errors) {
	unsigned char *rezult = new unsigned char[INFOCHARS_SIZE];
	int rezulti = 0;
	unsigned short buf = 0;
	unsigned short tmp = 0;
	for (int i = 0; i < CODE_SIZE; i+=3)
	{

		buf = 0;
		buf |= data[i];
		buf <<= 8;

		buf |= data[i + 1];
		tmp = buf & 15;
		buf >>= 4;
		rezult[rezulti] = get_d_char(buf);
		if (checkbits2(buf) != 4) errors->push_back(errorinfo(rezulti));

		rezulti++;
		buf = (tmp << 8);
		buf = buf | data[i + 2];
		rezult[rezulti] = get_d_char(buf);
		if (checkbits2(buf) != 4) errors->push_back(errorinfo(rezulti));
		rezulti++;
		}
	return rezult;
	}	
unsigned short checkbits2(unsigned short num)
{
	unsigned short rezult = 0;
	if (calcOnes(num&getNum("1010101010")) % 2 != ((num >> 11) % 2)) rezult += 1;
	if (calcOnes(num&getNum("1001100110")) % 2 != ((num >> 10) % 2)) rezult += 2;
	if (calcOnes(num&getNum("11100001")) % 2 != ((num >> 8) % 2)) rezult += 4;
	if (calcOnes(num&getNum("1111")) % 2 != ((num >> 4) % 2)) rezult += 8;
	return rezult + 4;
}
char get_d_char(unsigned short num) /*получает символ из 12 бит*/
{
	num &= getNum("1111111111");
	short tmp = num & 15;
	num = ((num >> 5) << 4) | tmp;
	tmp = num & getNum("1111111");
	num = ((num >> 8) << 7) | tmp;
	return (char)num;
}