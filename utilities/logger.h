/*! 
 * 	\file 	   logger.h
 *  \brief     header file
 *  \details   details
 *  \author    jajtn
 *  \version   1.000
 *  \date      27. zá?í 2023, 14:20
 */

#ifndef LOGGER_H
#define	LOGGER_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <stdint.h>



#ifndef LOGGER_MSG_LEN
#define LOGGER_MSG_LEN 50
#endif

typedef enum {

    LOG_TRACE,
    LOG_DEBUG,
    LOG_INFO,
    LOG_WARN,
    LOG_ERROR,
    LOG_FATAL

}LOG_level_t;

typedef enum{
    
    LOG_OK = 0,
    LOG_FAILED_ADD,
    LOG_FAILED_FULL,
    LOG_FAILED_INIT,
    LOG_EMPTY_STR
            
}LOG_errMessage_t;

typedef struct{

    uint8_t loggerBuff[LOGGER_BUFF_LEN];
    uint16_t len;
    /* Specific peripheral functions */

}LOG_logger_t;


LOG_errMessage_t LOG_addString(char *s);
LOG_errMessage_t LOG_addChar(int8_t data);
LOG_errMessage_t LOG_addShort(int16_t data);
LOG_errMessage_t LOG_addInt(int32_t data);
LOG_errMessage_t LOG_addUInt(uint32_t data);








#ifdef	__cplusplus
}
#endif

#endif	/* LOGGER_ */

