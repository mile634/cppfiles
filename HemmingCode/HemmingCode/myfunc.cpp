#include"myfunc.h"
unsigned int getNum(std::string str)
{
	unsigned int rezult = 0, size = str.length() - 1;
	for (int i = 0; i <= size; i++)
		rezult = (rezult | ((str[i] == '1') ? 1 : 0)) << 1;
	rezult >>= 1;
	return rezult;
}
unsigned short calcOnes(unsigned short num)
{
	unsigned short rez = 0;
	for (short i = 0; i < sizeof(short) * 8; i++)
	{
		rez = ((num >> i) % 2 == 1) ? rez + 1 : rez;
	}
	return rez;
}
unsigned short checkbits(unsigned short num)
{
	unsigned short rezult = 0;
	if (calcOnes(num&getNum("1010101010")) % 2 != ((num >> 11) % 2)) rezult += 1;
	if (calcOnes(num&getNum("1001100110")) % 2 != ((num >> 10) % 2)) rezult += 2;
	if (calcOnes(num&getNum("11100001")) % 2 != ((num >> 8) % 2)) rezult += 4;
	if (calcOnes(num&getNum("1111")) % 2 != ((num >> 4) % 2)) rezult += 8;
	return rezult + 4;
}
char get_d_char(short num) /*получает символ из 12 бит*/
{
	num &= getNum("1111111111");
	short tmp = num & 15;
	num = ((num >> 5) << 4) | tmp;
	tmp = num & getNum("1111111");
	num = ((num >> 8) << 7) | tmp;
	return (char)num;
}
std::string Code(std::string str)
{
	std::ostringstream output;
	bool even = true;
	unsigned short buf = 0;
	for (unsigned char a : str)
	{
		buf |= a;
		insertBits(buf);
		calcBits(buf);
		if (even) {
			unsigned char symb = buf >> 4;
			output << symb;
			buf = (buf & 15) << 10;
			even = false;
		}
		else {
			unsigned char symb1 = buf >> 8;
			unsigned char symb2 = buf & 255;
			output << symb1 << symb2;
			buf = 0;
			even = true;
		}
	}
	if (!even)
	{
		unsigned char tocka = '.';
		unsigned short tochka = tocka;
		insertBits(tochka);
		calcBits(tochka);
		buf = (buf << 2) | tochka;
		unsigned char symb1 = buf >> 8;
		unsigned char symb2 = buf & 255;
		output << symb1 << symb2;
	}
	return output.str();
}
std::string Decode(std::string str, bool &error) {
	std::ostringstream out;
	short rtype = 1;
	unsigned short buf = 0;
	unsigned short tmp = 0;
	for (unsigned char a : str)
	{
		if (rtype == 1)
		{
			buf = 0;
			buf |= a;
			buf <<= 8;
			rtype = 2;
		}
		else if (rtype == 2) {
			buf |= a;
			tmp = buf & 15;
			buf >>= 4;
			unsigned short check = checkbits(buf);
			if (check == 4) out << get_d_char(buf);
			else {
				out << "{error}";
				error = true;
			}
			rtype = 3;
		}
		else if (rtype == 3)
		{
			buf = (tmp << 8);
			buf = buf | a;
			if (checkbits(buf) == 4) out << get_d_char(buf);
			else {
				out << "{error}";
				error = true;
			}
			rtype = 1;
		}
	}
	return out.str();
}
std::string fix(std::string str)
{
	std::ostringstream out;
	short rtype = 1;
	unsigned short buf = 0;
	unsigned short tmp = 0;
	for (unsigned char a : str)
	{
		if (rtype == 1)
		{
			buf = 0;
			buf |= a;
			buf <<= 8;
			rtype = 2;
		}
		else if (rtype == 2) {
			buf |= a;
			tmp = buf & 15;
			buf >>= 4;
			unsigned short check = checkbits(buf);
			if (check == 4) out << get_d_char(buf);
			else {
				invertBit(buf, check);
				out << get_d_char(buf);
			}
			rtype = 3;
		}
		else if (rtype == 3)
		{
			buf = (tmp << 8);
			buf = buf | a;
			unsigned short check = checkbits(buf);
			if (check == 4) out << get_d_char(buf);
			else {
				invertBit(buf, check);
				out << get_d_char(buf);
			}
			rtype = 1;
		}
	}
	return out.str();
}
std::string readFile(const std::string& fileName) {
	std::ifstream f(fileName);
	std::stringstream ss;
	ss << f.rdbuf();
	return ss.str();
}
void calcBits(unsigned short &num)
{
	int tmp = num;
	if (calcOnes(num&getNum("1010101010")) % 2 == 1) tmp |= getNum("100000000000");
	if (calcOnes(num&getNum("1001100110")) % 2 == 1)tmp |= getNum("10000000000");
	if (calcOnes(num&getNum("11100001")) % 2 == 1)tmp |= getNum("100000000");
	if (calcOnes(num&getNum("1111")) % 2 == 1)tmp |= getNum("10000");
	num = tmp;
}
void insertBits(unsigned short &num)
{
	unsigned short tmp = num & getNum("1111");
	num = ((num >> 4) << 5) | tmp;
	tmp = num & getNum("11111111");
	num = ((num >> 8) << 9) | tmp;
}
void invertBit(unsigned short &num, unsigned short pos)
{

	unsigned int mask = (1 << 16 - pos);
	num ^= mask;
}
void writeFile(std::string path, std::string text)
{
	std::ofstream out(path);
	out << text;
	out.close();
}
void writeFileBin(std::string path, std::string text)
{
	std::ostringstream outstr;
	for (unsigned char a : text)
	{
		for (int i = 7; i >= 0; i--)
		{
			outstr << (a >> i) % 2;
		}
	}
	std::ofstream out(path);
	out << outstr.str();
	out.close();
}
void firstprjcheck()
{
	bool ok = false;
	std::string path = "C:\\Games\\output.dat";
	std::string binpath = "C:\\Games\\output2.dat";
	std::string text = "Секретная info 322\nThe tempe\nOleg lalka\nraturDiana drinking\ne metro lemon is 271K\nneed more info";
	std::cout << "Text (" << text.length() << ") chars: \n{\n" << text << "\n}\n";
	std::string codetext = Code(text);
	std::cout << "Coded text: (" << codetext.length() << ") chars: " << codetext << std::endl;
	try {
		writeFile(path, codetext);
		writeFileBin(binpath, codetext);
		std::cout << "Text writen: " << path << ". Now you can try to change file.\n";
		std::cout << "Binary data writen: " << binpath << ". Now you can try to change file.\n";
	}
	catch (...) { std::cout << "write error" << std::endl; }
	system("pause");
	try
	{
		text = readFile("C:\\Games\\output.dat");
		std::cout << "\nText readed (" << text.length() << ") chars from " <<
			path << ": " << text << "\nDecoding:\n{\n" << Decode(text, ok) << "\n}\n";
	}
	catch (...) { std::cout << "read error\n"; }
	if (ok) std::cout << "Trying to fix:\n{\n" << fix(text) << "\n}\n";
}