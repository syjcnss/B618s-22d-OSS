

#ifndef __ATSETPARACMD_H__
#define __ATSETPARACMD_H__

/*****************************************************************************
  1 ����ͷ�ļ�����
*****************************************************************************/
#include "AtParse.h"
#include "ATCmdProc.h"
#include "product_config.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#if (FEATURE_ON == MBB_COMMON)

#pragma pack(4)

/*****************************************************************************
  2 �궨��
*****************************************************************************/

/*****************************************************************************
  3 ö�ٶ���
*****************************************************************************/


/*****************************************************************************
  4 ȫ�ֱ�������
*****************************************************************************/


/*****************************************************************************
  5 ��Ϣͷ����
*****************************************************************************/


/*****************************************************************************
  6 ��Ϣ����
*****************************************************************************/


/*****************************************************************************
  7 STRUCT����
*****************************************************************************/


/*****************************************************************************
  8 UNION����
*****************************************************************************/


/*****************************************************************************
  9 OTHERS����
*****************************************************************************/


/*****************************************************************************
  10 ��������
*****************************************************************************/
/*VOS_UINT32 At_WriteWifiNVToDefault(
    AT_CUSTOMIZE_ITEM_DFLT_ENUM_UINT8   enCustomizeItem
);*/

/*VOS_UINT32 At_WriteSimLockNVToDefault(
    AT_CUSTOMIZE_ITEM_DFLT_ENUM_UINT8   enCustomizeItem
);*/

VOS_UINT32 AT_UpdateImei(
    MODEM_ID_ENUM_UINT16                enModemId,
    VOS_UINT8                           aucImei[],
    VOS_UINT16                          usImeiLength
);

VOS_UINT32 AT_UpdateSvn(
    MODEM_ID_ENUM_UINT16                enModemId,
    VOS_UINT8                           aucSvn[],
    VOS_UINT16                          usSvnLength
);

#if (VOS_OS_VER == VOS_WIN32)
#pragma pack()
#else
#pragma pack(0)
#endif


#endif

#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif

#endif /* end of __ATSETPARACMD_H__ */
