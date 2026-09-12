#include "PascalStringUtil.h"
#include <string>

using namespace std;

void PascalAppend(Str255 str1, const Str255 str2)
{
	for (int i = str1[0] + 1; i <= str1[0] + str2[0]; i++)
		str1[i] = str2[i - str1[0]];
	str1[0] += str2[0];
}

void PascalCopy(const Str255 str1, Str255 str2)
{
	for (int i = 1; i <= str1[0]; i++)
		str2[i] = str1[i];
		
	str2[0] = str1[0];
}

bool PascalStringCompare(const Str255 str1, const Str255 str2)
{
	if (str1[0] != str2[0])
		return false;
	for (int i = 1; i <= str1[0]; i++)
		if (str1[i] != str2[i])
			return false;
	return true;
}

void CtoPascal(const char* cs, Str255 str)
{
	int i = 0;
	while (i < 255 && cs[i] != '\0')
	{
		str[i + 1] = cs[i];
		i++;
	}
	
	str[0] = i;
}

void PascalToC(const Str255 str, char* cs)
{
	for (int i = 0; i < str[0]; i++)
		cs[i] = str[i + 1];
	cs[str[0]] = '\0';
}

void PascalToString(const Str255 str1, string& str2)
{
	str2.clear();
	for (int i = 0; i < str1[0]; i++)
		str2 += str1[i + 1];
}

void FloatToPascal(long double value, int numDecimalPlaces, Str255 str)
{
	Str255 str1;
	
	str[0] = 0;
	
	if (value < 1)
		PascalAppend(str, "\p0.");
	else
	{
		NumToString(value, str1);
		PascalAppend(str, str1);
		PascalAppend(str, "\p.");
	}
	
	int decimalPlace = 0;
	for (int j = 10; j < 1000000000; j *= 10)
	{
		if (++decimalPlace > numDecimalPlaces)
			break;
		NumToString(((int)(value * j)) % 10, str1);
		PascalAppend(str, str1);
	}
}
