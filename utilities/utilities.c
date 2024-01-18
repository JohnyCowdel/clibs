/*
 * utilities.c
 *
 *  Created on: Jan 7, 2024
 *      Author: jajtn
 */

#include "utilities.h"

#define ASCII_FIELD_LENGTH	13
#define POW_TABLE_LEN       10
#define POW_TABLE_END       (POW_TABLE_LEN - 1)

#define ASCII_PRECISION 	1000
#define ASCII_FIELD_END		ASCII_FIELD_LENGTH - 1

static char asciiFloat[ASCII_FIELD_LENGTH];
static char *fptr = &asciiFloat[ASCII_FIELD_END-1];
static const char *fhead = &asciiFloat[1];
static const char *ftail = &asciiFloat[ASCII_FIELD_END - 1];


static char asciiField[ASCII_FIELD_LENGTH] = { [ASCII_FIELD_END] = '\0'};
static char *ptr = &asciiField[ASCII_FIELD_END-1];
static const char *head = &asciiField[1];
static const char *tail = &asciiField[ASCII_FIELD_END - 1];
static char sign;

static const int pow10[POW_TABLE_LEN] = {
    1,
    10,
    100,
    1000,
    10000,
    100000,
    1000000,
    10000000,
    100000000,
    1000000000
};


char *memSet(char symbol, char *dest, uint8_t len)
{

	while(len--)
	{
		*dest++ = symbol;
	}

	*dest = '\0';

	return dest;
}


char *copyString(char *source, char *dest)
{
	if(source == NULL || dest == NULL)
		return source;

	while( (*source) != '\0')
		*dest++ = *source++;

	*dest = '\0';

	return dest;
}

char *copyChar(char *source, char *dest, uint8_t length)
{
	while(length--)
	{
		*dest++ = *source++;
	}

	return dest;
}


int	signInt(int i)
{
	return ((i>0)-(i<0));
}

int signShort(short i)
{
	return ((i>0)-(i<0));
}

int signChar(char i)
{
	return ((i>0)-(i<0));
}


int signFloat(float i)
{
	return ((i>0.0f)-(i<0.0f));
}




char* int2ascii(int32_t i)
{
	ptr = tail;

	sign = signInt(i);
	i = i*sign;

	while((i>0) && (ptr>head))
	{
		*(ptr--) = '0' + (i % 10);
		i /= 10;
	}

	* ptr = ((sign>=0)*'+') + ((sign<0)*'-');

	return ptr;
}

char* short2ascii(int16_t i)
{
	return int2ascii((int32_t) i);

}

char* char2ascii(int8_t i)
{
	return int2ascii((int32_t) i);
}

char* uint2ascii(uint32_t i)
{
	ptr = tail;

	while((i>0) && (ptr>head))
	{
		*(ptr--) = '0' + (i % 10);
		i /= 10;
	}
	*ptr = '+';
	return ptr;
}

char* ushort2ascii(uint16_t i)
{
	return uint2ascii((uint32_t) i);
}

char* uchar2ascii(uint8_t i)
{
	return uint2ascii((uint32_t) i);
}

char* float2ascii(float fn)
{

	int noChars, idx;
	int wholeNum = (int)fn; // using native int float to int instruction

	sign = signFloat(fn);
	ptr = int2ascii(wholeNum);
	*ptr = (sign >= 0) * '+' + (sign < 0) * '-';

	/* whole number part */
	fptr = copyString(ptr, asciiFloat);

	/* decimal part */
	fn -= wholeNum;
	noChars = (ftail - fptr);
	idx  = (noChars > POW_TABLE_END)* POW_TABLE_END;
	idx += !idx * noChars;
	fn *= pow10[idx];		//use as much free character space as possible
	*(fptr++) = '.';
	ptr = int2ascii((int) fn);
	noChars = idx - (tail - ptr);
	ptr++;

	/* leading zeros */
	fptr = memSet('0', fptr, noChars);
	fptr = copyString(ptr, fptr);


	return asciiFloat;
}

