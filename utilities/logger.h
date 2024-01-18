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
#include <stddef.h>

#ifndef LOGGER_BUFF_LEN
#define LOGGER_BUFF_LEN 200
#endif


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
    LOG_EMPTY_STR,
    LOG_INVALID_LEVEL
            
}LOG_errMessage_t;

typedef struct{

    char buff[2][LOGGER_BUFF_LEN];
    uint16_t len[2];
    char head;
    uint8_t lvl;
    
    /* Specific peripheral functions */

}LOG_logger_t;

void LOG_setLevel(LOG_level_t level);
char* LOG_getBuff(void);
char* LOG_getPosition(void);
uint16_t LOG_getLen(void);
uint16_t LOG_getRemain(void);
void LOG_switchBuff(void);

LOG_errMessage_t LOG_addLogInt(const char* s, int i, LOG_level_t level);
LOG_errMessage_t LOG_addLogFloat(const char* s, float f, LOG_level_t level);
LOG_errMessage_t LOG_addLog(const char* s, LOG_level_t level);
LOG_errMessage_t LOG_addString(char *s);
LOG_errMessage_t LOG_addChar(int8_t data);
LOG_errMessage_t LOG_addShort(int16_t data);
LOG_errMessage_t LOG_addInt(int32_t data);
LOG_errMessage_t LOG_addFloat(float data);
LOG_errMessage_t LOG_addUInt(uint32_t data);


#ifdef	__cplusplus
}
#endif

#endif	/* LOGGER_ */

