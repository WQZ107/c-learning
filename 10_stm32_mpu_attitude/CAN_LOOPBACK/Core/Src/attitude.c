/**
 * attitude.c - complementary-filter attitude estimation (Block L, 2026-10)
 * Fusion (100ms period): angle = 0.98*(angle + gyro_rate*dt) + 0.02*accel_angle
 *   gyro: fast & clean in motion, but drifts (bias removed by init average)
 *   accel: absolute reference, but noisy while moving -> only 2% weight
 * Axis mapping (DEFAULT: module z up, x to board front):
 *   pitch rate = gy, roll rate = gx.  Verify by live test:
 *   direction reversed -> negate that gyro term; nothing moves -> swap gx/gy.
 * Units: pitch/roll in degrees, exported x10 as int.
 */
#include "attitude.h"
#include "mpu6050.h"
#include <math.h>

#define GYRO_SENS   131.0f   /* LSB/(deg/s) @ +-250dps */
#define ALPHA_NUM   95       /* complementary weight: 95% gyro (was 98: 5s converge too slow) */
#define ALPHA_DEN   100
#define DT_S        0.1f     /* 100ms slot */

/* sign/axis trim -- set by live test 2026-10-04 (tilt forward: P should go NEGATIVE;
   tilt left: R should go NEGATIVE, per aviation convention) */
#define PITCH_ACC_SIGN   (-1.0f)  /* accel branch pulled P positive on forward tilt: negate */
#define ROLL_ACC_SIGN    (-1.0f)  /* left tilt gave R=+38, convention wants negative: negate */
#define PITCH_GYRO_SIGN  (+1.0f)  /* forward tilt drove P negative: correct */
#define ROLL_GYRO_SIGN   (-1.0f)  /* left tilt drove R positive: negate */

static float pitch, roll;        /* fused angles, degrees */
static float g_off[3];           /* gyro bias, captured at init */

void attitude_init(void)
{
    float s0 = 0.0f, s1 = 0.0f, s2 = 0.0f;
    int16_t gx, gy, gz;
    int i;

    for (i = 0; i < 100; i++) {              /* board MUST be still during boot */
        MPU6050_ReadGyro(&gx, &gy, &gz);
        s0 += gx;  s1 += gy;  s2 += gz;
        HAL_Delay(10);
    }
    g_off[0] = s0 / 100.0f;
    g_off[1] = s1 / 100.0f;
    g_off[2] = s2 / 100.0f;
    pitch = 0.0f;
    roll  = 0.0f;
}

void attitude_update_100ms(void)
{
    int16_t ax, ay, az, gx, gy, gz;
    float axg, ayg, azg, pitch_acc, roll_acc;
    float gx_dps, gy_dps;

    MPU6050_ReadAccel(&ax, &ay, &az);
    MPU6050_ReadGyro(&gx, &gy, &gz);

    /* accel branch: absolute angles (clean at rest, dirty in motion) */
    axg = ax / 16384.0f;  ayg = ay / 16384.0f;  azg = az / 16384.0f;
    pitch_acc = PITCH_ACC_SIGN * atan2f(-axg, sqrtf(ayg * ayg + azg * azg)) * 57.2958f;
    roll_acc  = ROLL_ACC_SIGN  * atan2f(ayg, azg) * 57.2958f;

    /* gyro branch: rates (deg/s), bias removed */
    gx_dps = (gx - g_off[0]) / GYRO_SENS;
    gy_dps = (gy - g_off[1]) / GYRO_SENS;

    /* complementary fusion -- axis mapping verified by live test 2026-10-04:
       forward tilt -> gy branch (P negative, OK); left tilt -> gx branch (sign trimmed). */
    pitch = (ALPHA_NUM * (pitch + PITCH_GYRO_SIGN * gy_dps * DT_S)
             + (ALPHA_DEN - ALPHA_NUM) * pitch_acc) / ALPHA_DEN;
    roll  = (ALPHA_NUM * (roll  + ROLL_GYRO_SIGN * gx_dps * DT_S)
             + (ALPHA_DEN - ALPHA_NUM) * roll_acc) / ALPHA_DEN;
}

int attitude_get_pitch10(void) { return (int)(pitch * 10.0f); }
int attitude_get_roll10(void)  { return (int)(roll  * 10.0f); }
