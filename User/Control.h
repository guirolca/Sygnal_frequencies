#ifndef CONTRL_H
#define CONTRL_H

void LCS(float speedF, float error, float time);
void shift_up_fcn(void);
void shift_dn_fcn(void);
void autoShift (uint16_t RPM, float frontSpeed);

#endif
