#ifndef PLANCK_H
#define PLANCK_H

#ifdef SUBPROJECT_rev3
    #include "rev3.h"
#endif
#ifdef SUBPROJECT_rev4
    #include "rev4.h"
#endif

#include "quantum.h"

#define PLANCK_MIT(                                                                \
                                                                                   \
    /* spatial positions */                                                        \
    k00,k01,k02,k03,k04,k05,k06,k07,k08,k09,     k0A,k0B,k0C,k0D,k0E,k0F,k0G,k0H,  \
    k10,k11,k12,k13,k14,k15,k16,k17,k18,k19,     k1A,k1B,k1C,k1D,k1E,k1F,k1G,k1H,  \
    k20,k21,k22,k23,k24,k25,k26,k27,k28,k29,     k2A,k2B,k2C,k2D,k2E,k2F,k2G,      \
    k30,k31,k32,k33,k34,k35,k36,k37,k38,k39,     k3A,k3B,k3C,k3D,k3E,k3F,k3G,k3H,  \
    k40,k41,k42,k43,k44,k45,k46,k47,k48,         k4A,k4B,    k4D,k4E,k4F,k4G,      \
    k50,k51,k52,        k55,                     k5A,k5B,k5C,k5D,    k5F,k5G,k5H   \
    )                                                                              \
                                                                                   \
   /* matrix positions */                                                          \
   {                                                                                                  \
    { k00, k01, k02, k03, k04, k05, k06, k07, k08, k09, k0A, k0B, k0C, k0D, k0E, k0F, k0G, k0H },     \
    { k10, k11, k12, k13, k14, k15, k16, k17, k18, k19, k1A, k1B, k1C, k1D, k1E, k1F, k1G, k1H },     \
    { k20, k21, k22, k23, k24, k25, k26, k27, k28, k29, k2A, k2B, k2C, k2D, k2E, k2F, k2G,KC_N0 },     \
    { k30, k31, k32, k33, k34, k35, k36, k37, k38, k39, k3A, k3B, k3C, k3D, k3E, k3F, k3G, k3H },     \
    { k40, k41, k42, k43, k44, k45, k46, k47, k48, KC_NO, k4A, k4B,KC_NO,k4D, k4E, k4F, k4G,KC_NO },   \
    { k50, k51, k52,KC_NO,KC_NO,k55,KC_NO,KC_NO,KC_NO,KC_NO,k5A,k5B,k5C, k5D,KC_NO,k5F, k5G, k5H } \
   }

#define PLANCK_GRID( \
                                                                                   \
    /* spatial positions */                                                        \
    k00,k01,k02,k03,k04,k05,k06,k07,k08,k09, k3C,k0A,k0B,k0C,k0D,k0E,k0F,k0G,k0H,  \
    k10,k11,k12,k13,k14,k15,k16,k17,k18,k19, k1C,k1A,k1B,    k1D,k1E,k1F,k1G,k1H,  \
    k20,k21,k22,k23,k24,k25,k26,k27,k28,k29,     k2A,k2B,k2C,k2D,k2E,k2F,k2G,      \
    k30,k31,k32,k33,k34,k35,k36,k37,k38,k39,     k3A,k3B,    k3D,k3E,k3F,k3G,k3H,  \
    k40,k41,k42,k43,k44,k45,k46,k47,k48,         k4A,k4B,    k4D,k4E,k4F,k4G,      \
    k50,k51,k52,        k55,                     k5A,k5B,k5C,k5D,    k5F,k5G,k5H   \
    )                                                                              \
                                                                                   \
   /* matrix positions */                                                          \
   {                                                                                                  \
    { k0A, k0B, k0C, k0D, k0E, k0F, k0G, k0H, k00, k01, k02, k03, k04, k05, k06, k07, k08, k09 },     \
    { k1A, k1B, k1C, k1D, k1E, k1F, k1G, k1H, k10, k11, k12, k13, k14, k15, k16, k17, k18, k19 },     \
    { k2A, k2B, k2C, k2D, k2E, k2F, k2G,KC_NO,k20, k21, k22, k23, k24, k25, k26, k27, k28, k29 },     \
    { k3A, k3B, k3C, k3D, k3E, k3F, k3G, k3H, k30, k31, k32, k33, k34, k35, k36, k37, k38, k39 },     \
    { k4A, k4B,KC_NO,k4D, k4E, k4F, k4G,KC_NO,k40, k41, k42, k43, k44, k45, k46, k47, k48, KC_NO },   \
    { k5A, k5B, k5C, k5D,KC_NO,k5F, k5G, k5H, k50, k51, k52,KC_NO,KC_NO,k55,KC_NO,KC_NO,KC_NO,KC_NO } \
   }

#endif
