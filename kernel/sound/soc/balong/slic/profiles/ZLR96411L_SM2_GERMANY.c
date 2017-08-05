

#include "ZLR96411L_SM2_GERMANY.h"

/************** Device Profile **************/

/************** DC Profile **************/
/* ETSI DC FXS Parameters - 23mA Current Feed */
const VpProfileDataType DC_FXS_VE960_ABS100V_ETSI[] =
{
/* Profile Header ---------------------------------------------------------- */
   0x0C, 0x01, 0x00,       /* DC Profile for Le9662_A device */
   0x0B,                   /* Profile length = 11 + 4 = 15 bytes */
   0x02,                   /* Profile format version = 2 */
   0x03,                   /* MPI section length = 3 bytes */
/* Raw MPI Section --------------------------------------------------------- */
   0xC6, 0x95, 0xE5,       /* DC Feed Parameters: ir_overhead = 100 ohm; */
                           /* VOC = 51 V; LI = 50 ohm; ILA = 23 mA */
                           /* Battery Switch Offset Voltage = 4.02 V */
/* Formatted Parameters Section -------------------------------------------- */
   0x1A,                   /* Switch Hook Threshold = 9 mA */
                           /* Ground-Key Threshold = 18 mA */
   0x86,                   /* Switch Hook Debounce = 12 ms */
                           /* Ground-Key Debounce = 16 ms */
   0x58,                   /* Low-Power Switch Hook Hysteresis = 2 V */
                           /* Ground-Key Hysteresis = 6 mA */
                           /* Switch Hook Hysteresis = 2 mA */
   0x40,                   /* Low-Power Switch Hook Threshold = 18 V */
   0x00,                   /* R_OSP = 0 ohms */
   0x07                    /* R_ISP = 7 ohms */
};

/************** AC Profile **************/

/* AC FXS RF14 Germany 220R+820R//115nF Normal Coefficients */
const VpProfileDataType AC_FXS_RF14_DE[] =
{
  /* AC Profile */
 0xEB, 0x00, 0xDC, 0x4C, 0x01, 0x49, 0xCA, 0xEA, 0x98, 0xA2, 0xE1, 0x24,
 0xA2, 0x2A, 0xCB, 0xB3, 0xAA, 0xD3, 0xB3, 0x9A, 0x98, 0x92, 0x97, 0x9F,
 0x01, 0x8A, 0x2D, 0x01, 0x24, 0x20, 0x2C, 0xA8, 0xAB, 0x24, 0x22, 0x97,
 0xAF, 0x8F, 0x23, 0xBF, 0x88, 0x3B, 0xB0, 0x33, 0x46, 0x2A, 0x2E, 0x33,
 0x4F, 0xAE, 0xA5, 0xB3, 0xBE, 0x82, 0xA8, 0x71, 0x80, 0xA9, 0xF0, 0x50,
 0x00, 0x86, 0x53, 0x54, 0x32, 0xC4, 0x1A, 0xA1, 0x2B, 0x2A, 0xA3, 0xAA,
 0x4A, 0xA5, 0xBA, 0x60, 0x96, 0xD2, 0xA1, 0x00
};


/* AC FXS RF14 Germany 220R+820R//115nF Wideband Coefficients */
const VpProfileDataType AC_FXS_RF14_WB_DE[] =
{
  /* AC Profile */
 0x27, 0x00, 0xD7, 0x4C, 0x01, 0x49, 0xCA, 0xDB, 0x98, 0xBA, 0xA1, 0xBA,
 0x22, 0x2A, 0xF4, 0xAA, 0xAB, 0x22, 0x65, 0x9A, 0x01, 0x90, 0x01, 0x01,
 0x90, 0x8A, 0x2E, 0x01, 0xA3, 0x10, 0x2B, 0x29, 0x53, 0xB9, 0xBD, 0xB3,
 0x7B, 0xCC, 0xDB, 0xA6, 0x88, 0x2A, 0xB0, 0x2B, 0x9F, 0x2E, 0x3E, 0x22,
 0xBE, 0xC3, 0x26, 0x35, 0x2F, 0x82, 0xB8, 0x71, 0x80, 0xAB, 0xF0, 0x50,
 0x00, 0x86, 0xCA, 0x5A, 0xA2, 0xBA, 0x1F, 0x22, 0x3B, 0x2A, 0x23, 0x2B,
 0x3A, 0x24, 0x4B, 0x40, 0x96, 0x22, 0x21, 0x00
};

/************** Ringing Profile **************/

/************** Tone Profile **************/
// ������ Dial Tone profile & Dial Tone Cadence profile
const VpProfileDataType T_DL_DE[] =
{
   0x0D, 0x02, 0x00, 0x0F, 0x02, 0x0C, 0xD2, 0x00, 0x00, 0x00, 0x04, 0x89, 
   0x27, 0xD4, 0x00, 0x00, 0x00, 0x00, 0x01
};

// ������ Ringback Tone profile & Ringback Tone Cadence profile
const VpProfileDataType T_RB_DE[] =
{
   0x0D, 0x02, 0x00, 0x0F, 0x02, 0x0C, 0xD2, 0x00, 0x00, 0x00, 0x04, 0x89, 
   0x27, 0xD4, 0x00, 0x00, 0x00, 0x00, 0x01
};

// æ�� Busy Tone profile & Busy Tone Cadence profile
const VpProfileDataType T_BS_DE[] =
{
   0x0D, 0x02, 0x00, 0x0F, 0x02, 0x0C, 0xD2, 0x00, 0x00, 0x00, 0x04, 0x89, 
   0x27, 0xD4, 0x00, 0x00, 0x00, 0x00, 0x01
};

// ���еȴ��� Call waiting Tone profile & Call waiting Tone Cadence profile
const VpProfileDataType T_CW_DE[] =
{
   0x0D, 0x02, 0x00, 0x0F, 0x02, 0x0C, 0xD2, 0x00, 0x00, 0x00, 0x04, 0x89, 
   0x27, 0xD4, 0x00, 0x00, 0x00, 0x00, 0x01
};

// ������ Congestion Tone profile & Congestion Tone Cadence profile
const VpProfileDataType T_CG_DE[] =
{
   0x0D, 0x02, 0x00, 0x0F, 0x02, 0x0C, 0xD2, 0x00, 0x00, 0x00, 0x04, 0x89, 
   0x27, 0xD4, 0x00, 0x00, 0x00, 0x00, 0x01
};

// ������ Alarm Tone profile & Alarm Tone Cadence profile
const VpProfileDataType T_ALARM_DE[] =
{
   0x0D, 0x02, 0x00, 0x18, 0x02, 0x15, 0xD2, 0x00, 0x00, 0x00, 0x0E, 0xEF, 
   0x2C, 0xB0, 0x15, 0xF9, 0x2C, 0xB0, 0xD4, 0x1A, 0x22, 0x2C, 0xB0, 0x1B, 
   0xBC, 0x2C, 0xB0, 0x0F
};

// ҵ�������� Service Enable Tone profile & Service Enable Tone Cadence profile
const VpProfileDataType T_SVCEN_DE[] =
{
   0x0D, 0x02, 0x00, 0x0F, 0x02, 0x0C, 0xD2, 0x00, 0x00, 0x00, 0x04, 0x89, 
   0x27, 0xD4, 0x00, 0x00, 0x00, 0x00, 0x01
};

// ҵ������� Service Disable Tone profile & Service Disable Tone Cadence profile
const VpProfileDataType T_SVCDIS_DE[] =
{
   0x0D, 0x02, 0x00, 0x0F, 0x02, 0x0C, 0xD2, 0x00, 0x00, 0x00, 0x04, 0x89, 
   0x27, 0xD4, 0x00, 0x00, 0x00, 0x00, 0x01
};

// ������ Fault Tone profile & Fault Tone Candence profile
/* DE�ƺ�û��Fault Tone. ʹ��Busy Tone����. */

/************** Cadence Profile **************/
/* Germany Ringing Signal Cadence */
const VpProfileDataType CR_DE[] =
{
  /* Cadence Profile */
 0x00, 0x08, 0x00, 0x1E, 0x00, 0x00, 0x00, 0x1A, 0x01, 0x07, 0x20, 0xC8,
 0x01, 0x05, 0x20, 0xA0, 0x02, 0x01, 0x22, 0x80, 0x01, 0x07, 0x20, 0xC8,
 0x01, 0x00, 0x20, 0x28, 0x01, 0x05, 0x22, 0xF8, 0x46, 0x00
};

// ������ Dial Tone profile & Dial Tone Cadence profile
const VpProfileDataType CT_DL_DE[] =
{
   0x00, 0x03, 0x00, 0x08, 0x00, 0x00, 0x00, 0x04, 0x00, 0x81, 0x20, 0x00
};

// ������ Ringback Tone profile & Ringback Tone Cadence profile
const VpProfileDataType CT_RB_DE[] =
{
   0x00, 0x03, 0x00, 0x0E, 0x00, 0x00, 0x00, 0x0A, 0x00, 0x81, 0x20, 0xC8, 
   0x00, 0x80, 0x23, 0x20, 0x40, 0x00
};

// æ�� Busy Tone profile & Busy Tone Cadence profile
const VpProfileDataType CT_BS_DE[] =
{
   0x00, 0x03, 0x00, 0x0E, 0x00, 0x00, 0x00, 0x0A, 0x00, 0x81, 0x20, 0x60, 
   0x00, 0x80, 0x20, 0x60, 0x40, 0x00
};

// ���еȴ��� Call waiting Tone profile & Call waiting Tone Cadence profile
const VpProfileDataType CT_CW_DE[] =
{
   0x00, 0x03, 0x00, 0x16, 0x00, 0x00, 0x00, 0x12, 0x00, 0x81, 0x20, 0x28, 
   0x00, 0x80, 0x20, 0x28, 0x00, 0x81, 0x20, 0x28, 0x00, 0x80, 0x23, 0xE8, 
   0x40, 0x00
};

// ������ Congestion Tone profile & Congestion Tone Cadence profile
const VpProfileDataType CT_CG_DE[] =
{
   0x00, 0x03, 0x00, 0x0E, 0x00, 0x00, 0x00, 0x0A, 0x00, 0x81, 0x20, 0x30, 
   0x00, 0x80, 0x20, 0x30, 0x40, 0x00
};

// ������ Alarm Tone profile & Alarm Tone Cadence profile
const VpProfileDataType CT_ALARM_DE[] =
{
   0x00, 0x03, 0x00, 0x0E, 0x00, 0x00, 0x00, 0x0A, 0x00, 0x8F, 0x20, 0x14, 
   0x00, 0x80, 0x20, 0x14, 0x40, 0x00
};

// ҵ�������� Service Enable Tone profile & Service Enable Tone Cadence profile
const VpProfileDataType CT_SVCEN_DE[] =
{
   0x00, 0x03, 0x00, 0x0E, 0x00, 0x00, 0x00, 0x0A, 0x00, 0x81, 0x20, 0x8C, 
   0x00, 0x80, 0x20, 0x28, 0x40, 0x00
};

// ҵ������� Service Disable Tone profile & Service Disable Tone Cadence profile

const VpProfileDataType CT_SVCDIS_DE[] =
{
   0x00, 0x03, 0x00, 0x0E, 0x00, 0x00, 0x00, 0x0A, 0x00, 0x81, 0x20, 0x18, 
   0x00, 0x80, 0x20, 0x18, 0x40, 0x00
};
// ������ Fault Tone profile & Fault Tone Candence profile
/* DE�ƺ�û��Fault Tone. ʹ��Busy Tone����. */

/************** Caller ID Profile **************/

/* Germany Caller ID (Type 1 - On-Hook) - ETSI DTMF */
/*  �Ѿ���China�ж���. */

/* Germany Caller ID (Type 1 - On-Hook) - ETSI FSK */
const VpProfileDataType CID_TYPE1_DE[] =
{
  /* Space=2100Hz, Mark=1300Hz, Amp=-6.50dBm */
  /* Caller ID Profile */
  0x00, 0x05, 0x00, 0x20, 0x00, 0x00,
  0x09, 0xD4, /* MPI Length and Command */
  0x16, 0x66, 0x2A, 0x30, 0x0D, 0xDD, 0x2A, 0x30, /* MPI Data */
  0x00, 0x01, /* Checksum Computed by device/API */
  0x00, 0x10, /* Length of Elements Data */
  0x00, 0x06, 0x00, 0x64, /* Silence Interval for 100ms */
  0x00, 0x09, 0x00, 0xFA, /* Channel Seizure for 250ms */
  0x00, 0x0A, 0x00, 0x96, /* Mark Signal for 150ms */
  0x00, 0x0B, /* Message Data (FSK Format) */
  0x00, 0x0D  /* End of Transmission */
};

/* Germany Caller ID (Type 2 - Off-Hook) - ETSI FSK */
const VpProfileDataType CID_TYPE2_DE[] =
{
  /* Space=2100Hz, Mark=1300Hz, Amp=-6.50dBm */
  /* Caller ID Profile */
  0x00, 0x05, 0x00, 0x40, 0x00, 0x00,
  0x09, 0xD4, /* MPI Length and Command */
  0x16, 0x66, 0x2A, 0x30, 0x0D, 0xDD, 0x2A, 0x30, /* MPI Data */
  0x00, 0x01, /* Checksum Computed by device/API */
  0x00, 0x30, /* Length of Elements Data */
  0x00, 0x02, /* Mute the far end */
  0x00, 0x06, 0x00, 0x05, /* Silence Interval for 5ms */
  0x00, 0x04, /* Alert Tone */
  /* Caller ID Alert Tone (CAS) - Used in Many Country Caller ID Profiles */
  /* 2129.88 Hz, -8.00 dBm0, 2750.24 Hz, -8.00 dBm0 */
  0x09, 0xD4, 0x16, 0xB8, 0x23, 0x80, 0x1D, 0x56, 0x23, 0x80,
  0x00, 0x05, 0x00, 0x50, /* Alert Tone Part 2 - Tone+Cadence = 80ms */
  0x00, 0x08, 0x00, 0xA0, 0x00, 0xD0, 0x00, 0x00, /* Detect Tone A | D, Timeout = 160ms */
  0x00, 0x06, 0x00, 0x32, /* Silence Interval for 50ms */
  0x00, 0x0A, 0x00, 0x41, /* Mark Signal for 65ms */
  0x00, 0x0B, /* Message Data (FSK Format) */
  0x00, 0x06, 0x00, 0x05, /* Silence Interval for 5ms */
  0x00, 0x03, /* Unmute the far end */
  0x00, 0x0D  /* End of Transmission */
};

/************** Metering_Profile **************/

/* end of file ZLR96621L_SM2_GERMANY.c */