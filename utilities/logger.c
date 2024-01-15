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

static const char severity[][] = { "[Trace]",
                                   "[Debug]",
                                   "[Info]",
                                   "[Warn]",
                                   "[Error]",
                                   "[Fatal]", }

static LOG_logger_t logger;                                //!< logger instance


LOG_errMessage_t LOG_addString(char *s, LOG_level_t level)
{

    uint16_t len = 0;
    while((*s != 0) && (len< LOGGER_MSG_LEN))
    {
        len++;
        s++;
    } 
    
    if (len==0)
        return LOG_EMPTY_STR;
    
    if((logger.len != 0) && (UARTx.getTxSize() > logger.len))       // aux buffer non-empty and enough space in tx buffer?
    {
        UARTx.writeBuffer(logger.loggerBuff, logger.len);           // send aux buffer content    
        logger.len = 0;
    }
    
    if(UARTx.getTxSize() >= len)                                    // if there's still enough room remaining send the rest
        UARTx.writeBuffer((uint8_t *)s, len);
    else if((logger.len + len) < LOGGER_BUFF_LEN)                    // else copy our input to aux buffer
    {
        Copy((uint8_t *)s, logger.loggerBuff, len);                       
        logger.len += len;
    }
    else
        return LOG_FAILED_ADD;                              // if everything fails 
        
    return LOG_OK;
}

LOG_errMessage_t LOG_addChar(int8_t data)
{
    uint8_t *s;
    s = char2ascii(data);
    return LOG_addString((char*) s);
}

LOG_errMessage_t LOG_addShort(int16_t data)
{
    uint8_t *s;
    s = short2ascii(data);
    return LOG_addString((char*) s);
}

LOG_errMessage_t LOG_addInt(int32_t data)
{
    uint8_t *s;
    s = int2ascii(data);
    return LOG_addString((char*) s);
}

LOG_errMessage_t LOG_addUInt(uint32_t data)
{
    uint8_t *s;
    s = uint2ascii(data);
    return LOG_addString((char*)s);
}

