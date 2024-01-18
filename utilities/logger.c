/*! 
 * 	\file 	   logger.c
 *  \brief     source file for logger
 *  \details   Logger is build as a wrapper around uart drivers
 *             to provide serial console functionality
 *  \author    jajtn
 *  \version   1.000
 *  \date      27. zá?í 2023, 14:42
 */


#include "logger.h"
#include "utilities.h"

static const char *severity[] =     {"[Trace]: ",
                                     "[Debug]: ",
                                     "[Info]: ",
                                     "[Warn]: ",
                                     "[Error]: ",
                                     "[Fatal]: ", };

static LOG_logger_t logger = {.lvl = 0, .head = 0 };                                //!< logger instance

void LOG_setLevel(LOG_level_t level)    {logger.lvl = level;}
char* LOG_getBuff(void) { return logger.buff[logger.head]; }
char* LOG_getPosition(void) { return  &logger.buff[logger.head][logger.len[logger.head]]; }
uint16_t LOG_getLen(void) { return (logger.len[logger.head]); }
uint16_t LOG_getRemain(void) { return (LOGGER_BUFF_LEN - logger.len[logger.head]); }

void LOG_switchBuff(void) 
{
    logger.len[logger.head] = 0;
    logger.head = (~logger.head) & 0x01; 
}

LOG_errMessage_t LOG_addLogInt(const char* s, int i, LOG_level_t level)
{

    if (level < logger.lvl)
        return LOG_INVALID_LEVEL;
    LOG_addString(severity[level]);
    LOG_addString(s);
    LOG_addInt(i);
    return LOG_addString("\n\0");
}


LOG_errMessage_t LOG_addLogFloat(const char* s, float f, LOG_level_t level)
{
    if (level <  logger.lvl)
        return LOG_INVALID_LEVEL;
    LOG_addString(severity[level]);
    LOG_addString(s);
    LOG_addFloat(f);
    return LOG_addString("\n\0");
}

LOG_errMessage_t LOG_addLog(const char* s, LOG_level_t level)
{
    if(level < logger.lvl)
        return LOG_INVALID_LEVEL;
    LOG_addString(severity[level]);
    LOG_addString(s);
    return LOG_addString("\n\0");
}

LOG_errMessage_t LOG_addString(char *s)
{
    char* str = s;
    uint16_t len = 0;
    while((*str != 0) && (len < LOGGER_MSG_LEN))
    {
        len++;
        str++;
    }
      
    if (len==0)
        return LOG_EMPTY_STR;
     
    if (len < LOG_getRemain())
    {
        copyChar(s, LOG_getPosition(),len);
        logger.len[logger.head] += len;
    }
    else
        return LOG_FAILED_ADD;

    return LOG_OK;
}

LOG_errMessage_t LOG_addChar(int8_t data)
{
    char *s;
    s = char2ascii(data);
    return LOG_addString( s);
}

LOG_errMessage_t LOG_addShort(int16_t data)
{
    char *s;
    s = short2ascii(data);
    return LOG_addString(s);
}

LOG_errMessage_t LOG_addInt(int32_t data)
{
    char *s;
    s = int2ascii(data);
    return LOG_addString(s);
}

LOG_errMessage_t LOG_addFloat(float data)
{
    char *s;
    s = float2ascii(data);
    return LOG_addString(s);
}

LOG_errMessage_t LOG_addUInt(uint32_t data)
{
    char *s;
    s = uint2ascii(data);
    return LOG_addString(s);
}

