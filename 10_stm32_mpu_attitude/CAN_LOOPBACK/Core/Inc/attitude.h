#ifndef __ATTITUDE_H
#define __ATTITUDE_H

void attitude_init(void);            /* gyro bias calib: keep board STILL ~1s */
void attitude_update_100ms(void);    /* call every 100ms slot */
int  attitude_get_pitch10(void);     /* pitch x10 (deg) */
int  attitude_get_roll10(void);      /* roll  x10 (deg) */

#endif
