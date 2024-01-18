/*
 * utilities.h
 *
 *  Created on: Jan 7, 2024
 *      Author: jajtn
 */

#ifndef UTILITIES_H_
#define UTILITIES_H_

#ifdef	__cplusplus
extern "C" {
#endif

#include <stdint.h>


char *copyString(char *source, char *dest);
char *copyChar(char *source, char *dest, uint8_t length);
char *memSet(char source, char *dest, uint8_t len);

int	signLong(long i);
int	signInt(int i);
int signShort(short i);
int signChar(char i);
int signFloat(float i);

char* int2ascii(int32_t i);
char* short2ascii(int16_t i);
char* char2ascii(int8_t i);
char* uint2ascii(uint32_t i);
char* ushort2ascii(uint16_t i);
char* uchar2ascii(uint8_t i);

char* float2ascii(float f);


#ifdef	__cplusplus
}
#endif

#endif /* UTILITIES_H_ */
