#ifndef _relay_H_
#define _relay_H_

#include <stdint.h>

typedef enum {NO=1,NC=0}RelayStatus;
typedef enum {jiawen = 0,huanshui=1,shuixunhuan=2,chongyang=3}ClassRelay;

void RelayConfig(void);
void ChangeRelayStatus(ClassRelay index,RelayStatus s);


#endif //_relay_H_
