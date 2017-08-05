
#include <linux/module.h>
#include <linux/delay.h>
#include <linux/string.h>
#include <linux/ctype.h>
#include <linux/kernel.h>
#include <linux/fs.h>

#include <product_nv_id.h>
#include <bsp_nvim.h>
#include <product_nv_def.h>
#include <linux/slab.h>

#include "wlan_at.h"
#include "wlan_at_api.h"
#include "wlan_utils.h"

#define WL_CONFIG_EXE_PATH   "/system/bin/bcm4352/exe/wl " 
#define WIFI_INSMOD_SH        "/system/bin/bcm4352/exe/wifi_poweron_4352_AT.sh"

/*2.4g adapter interface*/
#define IF_2G   "-i wl1"
/*5g adapter interface*/
#define IF_5G   "-i wl0"

#define RF_2G_0   (0)
#define RF_2G_1   (1)
/*5g��Ƶͨ·*/
#define RF_5G_0   (2)
#define RF_5G_1   (3)
/*MIMO*/
#define RF_2G_MIMO   (4)
#define RF_5G_MIMO   (5)

#define WL_CMD_BUF_LEN          128
#define WL_COUNTER_BUF_LEN      2048

#define WLAN_AT_2G_BAND    0
#define WLAN_AT_5G_BAND    1

#define WL_DEFAULT_POWER    (600) /* Ĭ�Ϲ��� */

#define WLAN_AT_MODE_BCM4352_SUPPORT    "1,2,3,4,5"        /*֧�ֵ�ģʽ(a/b/g/n/ac)*/    
             
#define WLAN_AT_BCM4352_BAND_SUPPORT    "0,1,2"            /*֧�ֵĴ���(20M/40M/80M/160M)*/

#define WLAN_AT_TSELRF_SUPPORT          "0,1,2,3"          /*֧�ֵ�������������*/

#define WLAN_AT_GROUP_MAX               3                  /*֧�ֵ������������*/
#define WLAN_AT_TYPE_MAX                2                  /*֧�ֻ�ȡ�������Ϣ����*/

/*WIFI���ʵ�������*/
#define WLAN_AT_POWER_MIN               (-15)
#define WLAN_AT_POWER_MAX               (30)

/*WiFi����ģʽ*/
#define AT_WIFI_MODE_ONLY_PA            0x00                /*WIFIֻ֧��PAģʽ*/
#define AT_WIFI_MODE_ONLY_NOPA          0x01                /*WIFIֻ֧��NO PAģʽ*/
#define AT_WIFI_MODE_PA_NOPA            0x02                /*WIFIͬʱ֧��PAģʽ��NO PAģʽ*/

#define WLAN_CHANNEL_2G_MIN             1                   /*2.4G�ŵ���Сֵ*/
#define WLAN_CHANNEL_5G_MIN             36                  /*5G�ŵ���Сֵ*/
#define WLAN_CHANNEL_2G_MAX             14                  /*2.4G�ŵ����*/
#define WLAN_CHANNEL_2G_MIDDLE          6
#define WLAN_CHANNEL_5G_MAX             165                 /*5G�ŵ����*/

#define WLAN_CHANNEL_5G_W52_START       36
#define WLAN_CHANNEL_5G_W52_END         48
#define WLAN_CHANNEL_5G_W53_START       52
#define WLAN_CHANNEL_5G_W53_END         64
#define WLAN_CHANNEL_5G_W57_START       149
#define WLAN_CHANNEL_5G_W57_END         161

#define WLAN_CHANNEL_5G_INTERVAL        4                     /*5G�ŵ����*/
#define WLAN_CHANNEL_5G_40M_INTERVAL    8                     /*5G 40M�ŵ����*/

#define WLAN_FREQ_2G_MAX                2484                  /*2.4G���Ƶ��*/

#define WLAN_FREQ_5G_W52_MIN            5180                  /*W52��СƵ��*/
#define WLAN_FREQ_5G_W53_MAX            5320                  /*W53���Ƶ��*/

#define WLAN_FREQ_5G_W52_40M_MIN        5190                  /*W52 40M��СƵ��*/
#define WLAN_FREQ_5G_W53_40M_MAX        5310                  /*W53 40M���Ƶ��*/

#define WLAN_FREQ_5G_W56_MIN            5500                  /*W56��СƵ��*/
#define WLAN_FREQ_5G_W56_MAX            5700                  /*W56���Ƶ��*/

#define WLAN_FREQ_5G_W56_40M_MIN        5510                  /*W56 40M��СƵ��*/
#define WLAN_FREQ_5G_W56_40M_MAX        5670                  /*W56 40M���Ƶ��*/

#define WLAN_FREQ_5G_W57_MIN            5745                  /*W57��СƵ��*/
#define WLAN_FREQ_5G_W57_MAX            5825                  /*W57���Ƶ��*/

#define WLAN_FREQ_5G_W57_40M_MIN        5755                  /*W57��СƵ��*/
#define WLAN_FREQ_5G_W57_40M_MAX        5795                  /*W57���Ƶ��*/

#define WIFI_CMD_MAX_SIZE               256                   /*cmd�ַ���256����*/
#define WIFI_CMD_8_SIZE                 8                     /*cmd�ַ���8����*/
#define HUNDRED                         100

#define DALEY_50_TIME     (50)
#define DALEY_100_TIME    (100)
#define DELAY_150_TIME    (150)
#define DALEY_500_TIME    (500)
#define DALEY_1000_TIME   (1000)
#define DALEY_5000_TIME   (5000)

#define AT_WIFI_N_RATE_NUM          (16)
#define AT_WIFI_AC20_RATE_NUM     (9)
#define AT_WIFI_AC40_RATE_NUM     (10) 
#define AT_WIFI_AC80_RATE_NUM     (10)

#define RMMOD_TIME 20000
#define INSMOD_TIME 180000

#define VOS_OK                   (0)
#define VOS_ERROR           (-1)
#define VOS_TRUE               1L
#define VOS_FALSE              0L
#define STR_LEN_MAX       128

#define WIFI_CHARAC_NAME_LEN    (31)                      /* WIFI ���������� */
#define  WIFI_WL_SW    ("WIFI_WL_REVERSE")
#define  WIFI_WL_KEEP  ("WIFI_WL_HOLD")

#define WLAN_AT_NUM_1    (1)
#define WLAN_AT_NUM_2    (2)
#define WLAN_AT_NUM_3    (3)
#define WLAN_AT_NUM_6    (6)
#define WLAN_AT_NUM_8    (8)
#define WLAN_AT_NUM_9    (9)
#define WLAN_AT_NUM_10   (10)
#define WLAN_AT_NUM_11   (11)
#define WLAN_AT_NUM_12   (12)
#define WLAN_AT_NUM_83   (83)

#define WLAN_AT_PA2GA0      "pa2ga0="
#define WLAN_AT_PA2GA1      "pa2ga1="
#define WLAN_AT_PA2GW0A0    "0:pa2gw0a0"
#define WLAN_AT_PA2GW1A0    "0:pa2gw1a0"
#define WLAN_AT_PA2GW2A0    "0:pa2gw2a0"
#define WLAN_AT_PA2GW0A1    "0:pa2gw0a1"
#define WLAN_AT_PA2GW1A1    "0:pa2gw1a1"
#define WLAN_AT_PA2GW2A1    "0:pa2gw2a1"

#define WLAN_AT_PA5GA0      "pa5ga0="
#define WLAN_AT_PA5GA1      "pa5ga1="
#define WLAN_AT_PA5GA2      "pa5ga2="
#define WLAN_AT_1PA5GA0     "1:pa5ga0"
#define WLAN_AT_1PA5GA1     "1:pa5ga1"
#define WLAN_AT_1PA5GA2     "1:pa5ga2"

#define INITIAL_PAVAR_NVM  "/var/pavars.nvm"

char    CMDValueStr[STR_LEN_MAX] = {0};
int      CMDRet = VOS_ERROR;

#define ANT_2G_NUM 2
#define ANT_5G_NUM 3
#define ANT_2G_CAL_NV_VALUE_BYTE_NUM 6
#define ANT_5G_CAL_NV_VALUE_BYTE_NUM 24
/******************************************************************************
��������:   g_ulWifiRF
��������:   ����Wifi��Ƶ���߲���
ȡֵ��Χ:   
���ú���:

ע������:   �ñ���Ϊ�ڲ�ҵ����Чʹ�õı��������ṩ���ⲿ�ӿ�ʹ��
******************************************************************************/ 
static unsigned int g_ulWifiRF = 0xffffffff;

/******************************************************************************
��������:   g_ulWifiMCSIndex
��������:   ����wifi��ǰ���ʶ�Ӧ��MCS index
ȡֵ��Χ:   
���ú���:

ע������:   �ñ���Ϊ�ڲ�ҵ����Чʹ�õı��������ṩ���ⲿ�ӿ�ʹ��
******************************************************************************/ 
static unsigned int g_ulWifiMCSIndex;

static unsigned int g_ul2GUP = 0;
static unsigned int g_ul5GUP = 0;

STATIC bool bl2GPavars = false;
STATIC bool bl5GPavars = false;

STATIC int32 Pa2GVarRecord[WLAN_AT_BUFFER_SIZE + 1] = {0};
STATIC int32 Pa5GVarRecord[WLAN_AT_BUFFER_SIZE + 1] = {0};

STATIC void Check_WIFI_CAL_NV(void);

WLAN_RETURN_TYPE prepare_wifi_driver_loaded(void);

/*��WiFiоƬ�·���������*/
#define WIFI_TEST_CMD(cmd) do{ \
    if (WLAN_SUCCESS != wlan_run_cmd(cmd)) \
    { \
        PLAT_WLAN_INFO("Run CMD Error!!\n"); \
        if(WLAN_DRV_NO_LOADED == prepare_wifi_driver_loaded()) \
        { \
            if (WLAN_SUCCESS != wlan_run_cmd(cmd)) \
            { \
                return AT_RETURN_FAILURE; \
            } \
        } \
        else \
        { \
            return AT_RETURN_FAILURE; \
        } \
    } \
}while(0)

#define WIFI_SHELL_CMD(cmd) do { \
            if (WLAN_SUCCESS != wlan_run_shell(cmd)) \
            { \
                PLAT_WLAN_INFO("Run shellcmd Error!!!"); \
                if(WLAN_DRV_NO_LOADED == prepare_wifi_driver_loaded()) \
                { \
                    if (WLAN_SUCCESS != wlan_run_shell(cmd)) \
                    { \
                        return AT_RETURN_FAILURE; \
                    } \
                } \
                else \
                { \
                    return AT_RETURN_FAILURE; \
                } \
            } \
        }while(0)


#define OSA_SNPRINTF(str, sz, fmt, ...) \
    {                                            \
        (void)snprintf(str, sz, fmt, ##__VA_ARGS__); \
        str[sz - 1] = '\0';                     \
    }

#ifndef STATIC
    #define STATIC static
#endif

/*��ָ����Լ��*/
#define ASSERT_NULL_POINTER(p, ret)  do { \
    if (NULL == (p)) \
    { \
        PLAT_WLAN_ERR("Exit ret = %d, on NULL Pointer %s", (int)ret, #p); \
        return ret; \
    } \
}while(0)


/*WiFiоƬʹ�ܶ��Լ��*/
#define ASSERT_WiFi_OFF(ret)                    \
if (AT_WIENABLE_OFF == g_wlan_at_data.wifiStatus) \
{                                               \
    WLAN_TRACE_INFO("Exit on WiFi OFF\n");        \
    return ret;                                 \
}

/*WiFiȫ�ֱ����ṹ�� */
typedef struct tagWlanATGlobal
{
    WLAN_AT_WIENABLE_TYPE   wifiStatus;    /*Ĭ�ϼ��ز���ģʽ*/
    WLAN_AT_WIMODE_TYPE     wifiMode;      /*wifiЭ��ģʽ*/
    WLAN_AT_WIBAND_TYPE     wifiBandwith;  /*wifiЭ����ʽ*/
    WLAN_AT_WIFREQ_TYPE     wifiBand;      /*wifi��ǰƵ��*/
    WLAN_AT_WIFREQ_STRU     wifiFreq;      /*wifiƵ����Ϣ*/
    uint32                  wifiRate;      /*wifi��������*/
    int32                   wifiPower;     /*wifi���书��*/
    WLAN_AT_FEATURE_TYPE    wifiTX;        /*wifi�����״̬*/
    WLAN_AT_WIRX_STRU       wifiRX;        /*wifi���ջ�״̬*/
    WLAN_AT_WIRPCKG_STRU    wifiPckg;      /*wifi�����*/  
    uint32                  wifiGroup;     /*wifi����ģʽ*/
}WLAN_AT_GLOBAL_ST;

typedef struct tagWlanATPacketREP
{
    unsigned int TotalRxPkts;
    unsigned int BadRxPkts;
    unsigned int UcastRxPkts;
    unsigned int McastRxPkts;
}WLAN_AT_PACK_REP_ST;

typedef struct
{
    uint32 brate;
    int8 rate_str[WIFI_CMD_8_SIZE];
}BRATE_ST;

/*��¼��ǰ��WiFiģʽ��������Ƶ�ʣ����ʵȲ���*/
STATIC WLAN_AT_GLOBAL_ST g_wlan_at_data = {AT_WIENABLE_TEST, AT_WIMODE_80211n, AT_WIBAND_20M, 0
         , {2412, 0}, 6500, 3175, AT_FEATURE_DISABLE, {AT_FEATURE_DISABLE, {0}, {0}}, {0, 0}, 0};/*����WiFi��Ĭ�ϲ���*/

STATIC unsigned int g_ulUcastWifiRxPkts;
STATIC unsigned int g_ulMcastWifiRxPkts;
STATIC unsigned int g_ulRxState = 0;

/*WiFi���ݰ�ͳ����Ϣ*/
STATIC WLAN_AT_PACK_REP_ST g_wifi_packet_report = {0};
STATIC WLAN_AT_PACK_REP_ST g_wifi_packet_new_rep = {0};/*ʵʱ��Ϣ*/

/*****************************************************************************
 ��������  : check_wifi_valid
 ��������  : �鿴wifiͨ·�Ƿ�ok
 �������  : NA
 �������  : NA
 �� �� ֵ  : 0 �ɹ�
             1 ʧ��
 ����˵��  : 
*****************************************************************************/
int32 check_wifi_valid()
{        
    WIFI_TEST_CMD(WL_CONFIG_EXE_PATH" ver");
    return AT_RETURN_SUCCESS;
}

/*****************************************************************************
 ��������  : check_wifi_driver_loaded
 ��������  : �鿴wifi�����Ƿ��Ѿ�����
 �������  : NA
 �������  : NA
 �� �� ֵ  : 0 �ɹ�
             1 ʧ��
 ����˵��  : 
*****************************************************************************/
WLAN_RETURN_TYPE prepare_wifi_driver_loaded(void)
{        
    int8 acCmdValueStr[WIFI_CMD_MAX_SIZE] = {0};
#if (FEATURE_ON == MBB_PCIE_CUST_REVERSE)
    char wl_reverse[WIFI_CHARAC_NAME_LEN + 1] = {WIFI_WL_SW};
#else
    char wl_reverse[WIFI_CHARAC_NAME_LEN + 1] = {WIFI_WL_KEEP};
#endif

    memset(acCmdValueStr, 0, sizeof(acCmdValueStr));

    OSA_SNPRINTF(acCmdValueStr,sizeof(acCmdValueStr), WL_CONFIG_EXE_PATH" ver");
    if (WLAN_SUCCESS != wlan_run_cmd(acCmdValueStr)) 
    { 
        PLAT_WLAN_INFO("Wi-Fi driver is not loaded, now start to loaded Wi-Fi driver!\n"); 

        memset(acCmdValueStr, 0, sizeof(acCmdValueStr));
        
        OSA_SNPRINTF(acCmdValueStr, sizeof(acCmdValueStr), "/bin/sh %s %s",WIFI_INSMOD_SH, wl_reverse);
        (void)wlan_run_cmd(acCmdValueStr);
        
        /*��鲢�����µ�wifiУ׼����*/
        Check_WIFI_CAL_NV();
        
        return WLAN_DRV_NO_LOADED;
    }

    return WLAN_SUCCESS;
}


STATIC unsigned int GetCmdRetValue(char * pValueStr)
{
    char        *pcRecPcktsStart        = NULL;
    char         acMonitorStrValue[WIFI_CMD_MAX_SIZE] = {0};
    unsigned int uiRetPcktsNumBuf       = 0;
    unsigned int uiIdPcktsCntValue      = 0;
    char filebuff[WL_COUNTER_BUF_LEN + 1] = {0};

    /* ��μ�� */
    ASSERT_NULL_POINTER(pValueStr, AT_RETURN_FAILURE);

    if(0 == wlan_read_file("/var/wifirxpkt", filebuff, sizeof(filebuff)))
    {
        PLAT_WLAN_ERR("read_from_file fail");
        return AT_RETURN_FAILURE;
    }

    pcRecPcktsStart = strstr(filebuff, pValueStr);

    if(NULL == pcRecPcktsStart)
    {
        return AT_RETURN_FAILURE;
    }    

    pcRecPcktsStart = pcRecPcktsStart + strlen(pValueStr)+ 1; /* 1:�ո�ռλ */
    
    while(' ' != *pcRecPcktsStart)
    {
        acMonitorStrValue[uiIdPcktsCntValue++] = *pcRecPcktsStart;
        pcRecPcktsStart++;
    }
    uiRetPcktsNumBuf = wlan_strtoi(acMonitorStrValue, NULL, WLAN_BASE10);
    return (uiRetPcktsNumBuf);
}


STATIC void wifi_get_rx_packet_report(unsigned int *ucastPkts, unsigned int *mcastPkts)
{

    char acCmdValueStr[WIFI_CMD_MAX_SIZE] = {0};
    
    if(NULL == ucastPkts || NULL == mcastPkts)
    {
        return;
    }

     if (WLAN_AT_2G_BAND == g_wlan_at_data.wifiBand)
    {
        OSA_SNPRINTF(acCmdValueStr,sizeof(acCmdValueStr), WL_CONFIG_EXE_PATH" %s counters > /var/wifirxpkt", IF_2G);
    }
    else if (WLAN_AT_5G_BAND == g_wlan_at_data.wifiBand)
    {
        OSA_SNPRINTF(acCmdValueStr,sizeof(acCmdValueStr), WL_CONFIG_EXE_PATH" %s counters > /var/wifirxpkt", IF_5G);
    }
    else
    {
        return;
    }   
 
    WIFI_SHELL_CMD(acCmdValueStr);

    *mcastPkts = GetCmdRetValue("pktengrxdmcast");
    *ucastPkts = GetCmdRetValue("pktengrxducast");
    PLAT_WLAN_INFO("*mcastPkts=%d, *ucastPkts=%d \n", *mcastPkts, *ucastPkts);
}

//////////////////////////////////////////////////////////////////////////
/*(1)^WIENABLE ����WiFiģ��ʹ�� */
//////////////////////////////////////////////////////////////////////////
/*****************************************************************************
 ��������  : int32 WlanATSetWifiEnable(WLAN_AT_WIENABLE_TYPE onoff)
 ��������  : ����wifi �������ģʽ������ģʽ���ر�wifi
 �������  :  0  �ر�
              1  ������ģʽ
              2  �򿪲���ģʽ
 �������  : NA
 �� �� ֵ  : 0 �ɹ�
             1 ʧ��
 ����˵��  : 
*****************************************************************************/
STATIC int32 ATSetWifiEnable(WLAN_AT_WIENABLE_TYPE onoff)
{
    WLAN_AT_RETURN_TYPE ret = AT_RETURN_SUCCESS;
    char wl_cmd[WL_CMD_BUF_LEN] = {0};

    ret = check_wifi_valid();
    if (AT_RETURN_SUCCESS != ret)
    {       
        PLAT_WLAN_INFO("check_wifi_valid failed!\n");
        return ret;
    }

    if (onoff == g_wlan_at_data.wifiStatus)
    {
        return AT_RETURN_SUCCESS;
    }
    
    switch (onoff)
    {
        case AT_WIENABLE_OFF:
            {                
                PLAT_WLAN_INFO("Set wifi to off mode\n");

                memset(wl_cmd, 0, WL_CMD_BUF_LEN);
                OSA_SNPRINTF(wl_cmd, WL_CMD_BUF_LEN, WL_CONFIG_EXE_PATH" %s down", IF_2G);
                WIFI_TEST_CMD(wl_cmd);

                memset(wl_cmd, 0, WL_CMD_BUF_LEN);
                OSA_SNPRINTF(wl_cmd, WL_CMD_BUF_LEN, WL_CONFIG_EXE_PATH" %s down", IF_5G);
                WIFI_TEST_CMD(wl_cmd);

                g_wlan_at_data.wifiStatus = AT_WIENABLE_OFF;
            }            
            break;
        case AT_WIENABLE_ON:
            {
                PLAT_WLAN_INFO("Set wifi to on mode\n");

                memset(wl_cmd, 0, WL_CMD_BUF_LEN);
                OSA_SNPRINTF(wl_cmd, WL_CMD_BUF_LEN, WL_CONFIG_EXE_PATH" %s down", IF_2G);
                WIFI_TEST_CMD(wl_cmd);

                memset(wl_cmd, 0, WL_CMD_BUF_LEN);
                OSA_SNPRINTF(wl_cmd, WL_CMD_BUF_LEN, WL_CONFIG_EXE_PATH" %s down", IF_5G);
                WIFI_TEST_CMD(wl_cmd);

                memset(wl_cmd, 0, WL_CMD_BUF_LEN);
                OSA_SNPRINTF(wl_cmd, WL_CMD_BUF_LEN, WL_CONFIG_EXE_PATH" %s up", IF_2G);
                WIFI_TEST_CMD(wl_cmd);

                memset(wl_cmd, 0, WL_CMD_BUF_LEN);
                OSA_SNPRINTF(wl_cmd, WL_CMD_BUF_LEN, WL_CONFIG_EXE_PATH" %s up", IF_5G);
                WIFI_TEST_CMD(wl_cmd);
                
                g_wlan_at_data.wifiStatus = AT_WIENABLE_ON;
            }
            break;
        case AT_WIENABLE_TEST:
            {                
                PLAT_WLAN_INFO("Set wifi to test mode\n");    
                
                memset(wl_cmd, 0, WL_CMD_BUF_LEN);
                OSA_SNPRINTF(wl_cmd, WL_CMD_BUF_LEN, WL_CONFIG_EXE_PATH" %s down", IF_2G);
                WIFI_TEST_CMD(wl_cmd);

                memset(wl_cmd, 0, WL_CMD_BUF_LEN);
                OSA_SNPRINTF(wl_cmd, WL_CMD_BUF_LEN, WL_CONFIG_EXE_PATH" %s down", IF_5G);
                WIFI_TEST_CMD(wl_cmd);

                memset(wl_cmd, 0, WL_CMD_BUF_LEN);
                OSA_SNPRINTF(wl_cmd, WL_CMD_BUF_LEN, WL_CONFIG_EXE_PATH" %s up", IF_2G);
                WIFI_TEST_CMD(wl_cmd);

                memset(wl_cmd, 0, WL_CMD_BUF_LEN);
                OSA_SNPRINTF(wl_cmd, WL_CMD_BUF_LEN, WL_CONFIG_EXE_PATH" %s up", IF_5G);
                WIFI_TEST_CMD(wl_cmd);
                
                g_wlan_at_data.wifiStatus = AT_WIENABLE_TEST;
            }         
            break;
        default: 
            ret = AT_RETURN_FAILURE;
            break;
    }   
    
    return ret;
}
/*****************************************************************************
 ��������  : WLAN_AT_WIENABLE_TYPE WlanATGetWifiEnable()
 ��������  : ��ȡ��ǰ��WiFiģ��ʹ��״̬
 �������  : NA
 �������  : NA
 �� �� ֵ  : 0  �ر�
             1  ����ģʽ(����ģʽ)
             2  ����ģʽ(������ģʽ)
 ����˵��  : 
*****************************************************************************/
STATIC WLAN_AT_WIENABLE_TYPE ATGetWifiEnable(void)
{    
    int32 ret = AT_RETURN_SUCCESS;
    
    PLAT_WLAN_INFO("%s in\n",__FUNCTION__);
    
    ret = check_wifi_valid();
    if (AT_RETURN_SUCCESS != ret)
    {       
        PLAT_WLAN_ERR("check_wifi_valid failed!\n");        
        return AT_WIENABLE_OFF;
    }
    
    PLAT_WLAN_INFO("%s in, wifiStatus = %d\n",__FUNCTION__,g_wlan_at_data.wifiStatus);
    return g_wlan_at_data.wifiStatus;
}

//////////////////////////////////////////////////////////////////////////
/*(2)^WIMODE ����WiFiģʽ���� Ŀǰ��Ϊ��ģʽ����*/
//////////////////////////////////////////////////////////////////////////
/*****************************************************************************
 ��������  : int32 WlanATSetWifiMode(WLAN_AT_WIMODE_TYPE mode)
 ��������  : ����WiFi AP֧�ֵ���ʽ
 �������  : 0,  CWģʽ
             1,  802.11a��ʽ
             2,  802.11b��ʽ
             3,  802.11g��ʽ
             4,  802.11n��ʽ
             5,  802.11ac��ʽ
 �������  : NA
 �� �� ֵ  : 0 �ɹ�
             1 ʧ��
 ����˵��  : 
*****************************************************************************/
STATIC int32 ATSetWifiMode(WLAN_AT_WIMODE_TYPE mode)
{
    PLAT_WLAN_INFO("WLAN_AT_WIMODE_TYPE:%d\n", mode);  

    if (mode > AT_WIMODE_80211ac)
    {
        return (AT_RETURN_FAILURE);
    }       

    g_wlan_at_data.wifiMode = mode;

    return (AT_RETURN_SUCCESS);
}

/*****************************************************************************
 ��������  : int32 WlanATGetWifiMode(WLAN_AT_BUFFER_STRU *strBuf)
 ��������  : ��ȡ��ǰWiFi֧�ֵ���ʽ
             ��ǰģʽ�����ַ�����ʽ����eg: 2
 �������  : NA
 �������  : NA
 �� �� ֵ  : NA
 ����˵��  : 
*****************************************************************************/
STATIC int32 ATGetWifiMode(WLAN_AT_BUFFER_STRU *strBuf)
{
    if (NULL == strBuf)
    {
        return (AT_RETURN_FAILURE);
    }

    OSA_SNPRINTF(strBuf->buf, WLAN_AT_BUFFER_SIZE, "%d", g_wlan_at_data.wifiMode);
    return (AT_RETURN_SUCCESS);
}

/*****************************************************************************
 ��������  : int32 WlanATGetWifiModeSupport(WLAN_AT_BUFFER_STRU *strBuf)
 ��������  : ��ȡWiFiоƬ֧�ֵ�����Э��ģʽ
             ֧�ֵ�����ģʽ�����ַ�����ʽ����eg: 2,3,4
 �������  : NA
 �������  : NA
 �� �� ֵ  : 0 �ɹ�
             1 ʧ��
 ����˵��  : 
*****************************************************************************/
STATIC int32 ATGetWifiModeSupport(WLAN_AT_BUFFER_STRU *strBuf)
{
    if (NULL == strBuf)
    {
        return (AT_RETURN_FAILURE);
    }

    OSA_SNPRINTF(strBuf->buf, WLAN_AT_BUFFER_SIZE, "%s", WLAN_AT_MODE_BCM4352_SUPPORT);
    
    return (AT_RETURN_SUCCESS);
}

//////////////////////////////////////////////////////////////////////////
/*(3)^WIBAND ����WiFi�������� */
//////////////////////////////////////////////////////////////////////////
/*****************************************************************************
 ��������  : int32 WlanATSetWifiBand(WLAN_AT_WIBAND_TYPE width)
 ��������  : ��������wifi����
 �������  : 0 20M
             1 40M
 �������  : NA
 �� �� ֵ  : 0 �ɹ�
             1 ʧ��
 ����˵��  : ֻ����nģʽ�²ſ�������40M����
*****************************************************************************/
STATIC int32 ATSetWifiBand(WLAN_AT_WIBAND_TYPE bandwith)
{
    int32 ret = AT_RETURN_SUCCESS;
    char wl_cmd[WL_CMD_BUF_LEN] = {0};
    
    PLAT_WLAN_INFO("%s:in,band = %d\n", __FUNCTION__, bandwith);
    
    switch(bandwith)
    {
        case AT_WIBAND_20M: 
            {
                g_wlan_at_data.wifiBandwith = AT_WIBAND_20M;
                break;
            }             
        case AT_WIBAND_40M:
            {                
                if(AT_WIMODE_80211n == g_wlan_at_data.wifiMode
                || AT_WIMODE_80211ac == g_wlan_at_data.wifiMode)
                {
                    g_wlan_at_data.wifiBandwith = AT_WIBAND_40M;                    
                }
                else
                {   
                    PLAT_WLAN_ERR("Error wifi mode,must in n/ac mode\n");
                    ret = AT_RETURN_FAILURE;
                }
                break;
            }
        case AT_WIBAND_80M:
            {                
                if(AT_WIMODE_80211ac == g_wlan_at_data.wifiMode)
                {
                    g_wlan_at_data.wifiBandwith = AT_WIBAND_80M;                    
                }
                else
                {   
                    PLAT_WLAN_ERR("Error wifi mode,must in ac mode\n");
                    ret = AT_RETURN_FAILURE;
                }
                break;
            }
        default:
            ret = AT_RETURN_FAILURE;
            break;
    }

    if (AT_RETURN_FAILURE == ret)
    {
        return ret;
    }

    if (WLAN_AT_2G_BAND == g_wlan_at_data.wifiBand)
    {
        memset(wl_cmd, 0, WL_CMD_BUF_LEN);
        OSA_SNPRINTF(wl_cmd, WL_CMD_BUF_LEN, WL_CONFIG_EXE_PATH" %s down", IF_2G);
        WIFI_TEST_CMD(wl_cmd);

        memset(wl_cmd, 0, WL_CMD_BUF_LEN);
        OSA_SNPRINTF(wl_cmd, WL_CMD_BUF_LEN, WL_CONFIG_EXE_PATH" %s mimo_txbw %d", 
            IF_2G, 2 * (bandwith + 1));
        WIFI_TEST_CMD(wl_cmd);
    }
    else
    {
         /* 5G���������ʵ�wl���������·�bandwidth */
    }

    return ret;
}

/*****************************************************************************
 ��������  : int32 WlanATGetWifiBand(WLAN_AT_BUFFER_STRU *strBuf)
 ��������  :��ȡ��ǰ�������� 
            ��ǰ���������ַ�����ʽ����eg: 0
 �������  : NA
 �������  : NA
 �� �� ֵ  : 0 �ɹ�
             1 ʧ��
 ����˵��  : 
*****************************************************************************/
STATIC int32 ATGetWifiBand(WLAN_AT_BUFFER_STRU *strBuf)
{
    if (NULL == strBuf)
    {
        return (AT_RETURN_FAILURE);
    }

    OSA_SNPRINTF(strBuf->buf, WLAN_AT_BUFFER_SIZE, "%d", g_wlan_at_data.wifiBandwith);
    return (AT_RETURN_SUCCESS);
}

/*****************************************************************************
 ��������  : int32 WlanATGetWifiBandSupport(WLAN_AT_BUFFER_STRU *strBuf)
 ��������  :��ȡWiFi֧�ֵĴ������� 
            ֧�ִ��������ַ�����ʽ����eg: 0,1
 �������  : NA
 �������  : NA
 �� �� ֵ  : 0 �ɹ�
             1 ʧ��
 ����˵��  : 
*****************************************************************************/
STATIC int32 ATGetWifiBandSupport(WLAN_AT_BUFFER_STRU *strBuf)
{
    if (NULL == strBuf)
    {
        return (AT_RETURN_FAILURE);
    }

    OSA_SNPRINTF(strBuf->buf, WLAN_AT_BUFFER_SIZE, "%s", WLAN_AT_BCM4352_BAND_SUPPORT);
    
    return (AT_RETURN_SUCCESS);
}

//////////////////////////////////////////////////////////////////////////
/*(4)^WIFREQ ����WiFiƵ�� */
//////////////////////////////////////////////////////////////////////////
/*****************************************************************************
 ��������  : int32 WlanATSet2GWifiFreq(WLAN_AT_WIFREQ_STRU *pFreq)
 ��������  : ����WiFiƵ��
 �������  : NA
 �������  : NA
 �� �� ֵ  : 0 �ɹ�
             1 ʧ��
 ����˵��  : 
*****************************************************************************/
STATIC int32 WlanATSet2GWifiFreq(WLAN_AT_WIFREQ_STRU *pFreq)
{
    int8   wl_cmd[WIFI_CMD_MAX_SIZE] = {0};
    int8   channeStr[WIFI_CMD_8_SIZE] = {0}; 
    uint16 ulWifiFreq = 0;
    uint16 i = 0;    
    int32  ret = AT_RETURN_SUCCESS;
    
    /* 2.4GƵ�㼯�� �ŵ�1-14 */
    const uint16   ausChannels[] = {2412,2417,2422,2427,2432,2437,2442,2447,2452,2457,2462,2467,2472,2484}; 
    
    /* 2.4G 40MƵ�㼯�� �����ŵ� 3-11 */
    const uint16   ausChannels_40M[] = {2422, 2427, 2432, 2437, 2442, 2447, 2452, 2457, 2462};
       
    
    if (NULL == pFreq)
    {
        return (AT_RETURN_FAILURE);
    }

    if (g_ulRxState)
    {      
        memset(wl_cmd, 0, WIFI_CMD_MAX_SIZE);
        OSA_SNPRINTF(wl_cmd, WIFI_CMD_MAX_SIZE, WL_CONFIG_EXE_PATH" %s down", IF_2G);
        WIFI_TEST_CMD(wl_cmd);   
        g_ulRxState = 0;
    }

    memset(wl_cmd, 0, WIFI_CMD_MAX_SIZE);
    OSA_SNPRINTF(wl_cmd, WIFI_CMD_MAX_SIZE, WL_CONFIG_EXE_PATH" %s band b", IF_2G);
    WIFI_TEST_CMD(wl_cmd);
    
    memset(wl_cmd, 0, WIFI_CMD_MAX_SIZE);
    OSA_SNPRINTF(wl_cmd, WIFI_CMD_MAX_SIZE, WL_CONFIG_EXE_PATH" %s country ALL", IF_2G);
    WIFI_TEST_CMD(wl_cmd);

    /*20M����*/
    if (AT_WIBAND_20M == g_wlan_at_data.wifiBandwith)
    {  
        if (pFreq->value <= WLAN_FREQ_2G_MAX)
        {
            for (i = 0; i < (sizeof(ausChannels) / sizeof(uint16)); i++)
            {
                if (pFreq->value == ausChannels[i])
                {
                    ulWifiFreq = (i + 1);
                    break;
                }
            }
        }
        else
        {        
            PLAT_WLAN_INFO("Error 20M wifiFreq parameters\n");      
            return AT_RETURN_FAILURE;
        }

        PLAT_WLAN_INFO("Target Channel = %d\n", ulWifiFreq);

        if (!(((WLAN_CHANNEL_2G_MIN <= ulWifiFreq) && (ulWifiFreq <= WLAN_CHANNEL_2G_MAX))))
        {
            PLAT_WLAN_INFO("Target Channel ERROR,ulWifiFreq = %u!\n", ulWifiFreq);
            return AT_RETURN_FAILURE;
        }
        
        memset(wl_cmd, 0, WIFI_CMD_MAX_SIZE);
        OSA_SNPRINTF(wl_cmd, WIFI_CMD_MAX_SIZE, WL_CONFIG_EXE_PATH" %s bw_cap 2g 1", IF_2G);
        WIFI_TEST_CMD(wl_cmd);
        
        memset(wl_cmd, 0, WIFI_CMD_MAX_SIZE);
        OSA_SNPRINTF(wl_cmd, WIFI_CMD_MAX_SIZE,WL_CONFIG_EXE_PATH" %s chanspec %d", IF_2G, ulWifiFreq);
        WIFI_TEST_CMD(wl_cmd);

    }
    else if(AT_WIBAND_40M == g_wlan_at_data.wifiBandwith)
    {
        if (pFreq->value <= WLAN_FREQ_2G_MAX)
        {
            for (i = 0; i < (sizeof(ausChannels_40M) / sizeof(uint16)); i++)
            {
                if (pFreq->value == ausChannels_40M[i])
                {
                    ulWifiFreq = (i + 1);
                    break;
                }
            }
        }       
        else
        {        
            PLAT_WLAN_INFO("Error 40M wifiFreq parameters\n");      
            return AT_RETURN_FAILURE;
        }
        
         /* �ӵ�3���ŵ���ʼ����Ҫ��2 */
        ulWifiFreq = ulWifiFreq + 2;
        PLAT_WLAN_INFO("Target Channel = %d\n", ulWifiFreq);
    
        if (!(((WLAN_CHANNEL_2G_MIN <= ulWifiFreq) && (WLAN_CHANNEL_2G_MAX >= ulWifiFreq))))
        {
            PLAT_WLAN_INFO("Target Channel ERROR!\n");
            return AT_RETURN_FAILURE;
        }
        
        /*40M�ŵ���U/L����*/
        memset(wl_cmd, 0, WIFI_CMD_MAX_SIZE);
        OSA_SNPRINTF(wl_cmd, WIFI_CMD_MAX_SIZE, WL_CONFIG_EXE_PATH" %s bw_cap 2g 3", IF_2G);
        WIFI_TEST_CMD(wl_cmd);
        
        memset(wl_cmd, 0, WIFI_CMD_MAX_SIZE);
        OSA_SNPRINTF(wl_cmd, WIFI_CMD_MAX_SIZE, WL_CONFIG_EXE_PATH" %s chanspec -c %d -b 2 -w 40 -s 1", IF_2G, ulWifiFreq);
        WIFI_TEST_CMD(wl_cmd);
    }

    /* ����ȫ�ֱ�����Ա���ѯ */
    g_wlan_at_data.wifiFreq.value = pFreq->value;
    g_wlan_at_data.wifiFreq.offset = pFreq->offset;
    return ret;

}

//////////////////////////////////////////////////////////////////////////
/*(4)^WIFREQ ����WiFiƵ�� */
//////////////////////////////////////////////////////////////////////////
/*****************************************************************************
 ��������  : int32 WlanATSet5GWifiFreq(WLAN_AT_WIFREQ_STRU *pFreq)
 ��������  : ����WiFiƵ��
 �������  : NA
 �������  : NA
 �� �� ֵ  : 0 �ɹ�
             1 ʧ��
 ����˵��  : 
*****************************************************************************/
STATIC int32 WlanATSet5GWifiFreq(WLAN_AT_WIFREQ_STRU *pFreq)
{

    int8 wl_cmd[WIFI_CMD_MAX_SIZE] = {0};
    int8 channeStr[WIFI_CMD_8_SIZE] = {0}; 
    uint16 ulWifiFreq = 0;
    uint16 i = 0;    

    /*5GƵ�㼯��*/
    const uint16 aulChannel036[] = {5180, 5200, 5220, 5240, 5260, 5280, 5300, 5320};  /*w52  w53*/
    const uint16 aulChannel100[] = {5500,5520,5540,5560,5580,5600,5620,5640,5660,5680,5700};  /* w56 */
    const uint16 aulChannel149[] = {5745,5765,5785,5805,5825}; /* w57 */

    /*5G 40MƵ�㼯��*/
    const uint16 aulChannel036_40M[] = {5190, 5230, 5270, 5310};
    const uint16 aulChannel100_40M[] = {5510, 5550, 5590, 5630, 5670}; 
    const uint16 aulChannel149_40M[] = {5755, 5795};

    /*5G 80MƵ�㼯��*/
    const uint16 aulChannelAC[] = {5210, 5290, 5530, 5610, 5775}; 

    /*5G 40M������Ҫ��u���ŵ�*/
    const uint16 channel_5g_40M_u[] = {40,48,56,64,104,112,120,128,136,153,161};    
    /*5G 40M������Ҫ��l���ŵ�*/
    const uint16 channel_5g_40M_l[] = {36,44,52,60,100,108,116,124,132,149,157};

    
    if (NULL == pFreq)
    {
        return (AT_RETURN_FAILURE);
    }

    if (g_ulRxState)
    {      
        memset(wl_cmd, 0, WIFI_CMD_MAX_SIZE);
        OSA_SNPRINTF(wl_cmd, WIFI_CMD_MAX_SIZE, WL_CONFIG_EXE_PATH" %s down", IF_5G);
        WIFI_TEST_CMD(wl_cmd);   
        g_ulRxState = 0;
    }

    /* 20M ����*/
    if (AT_WIBAND_20M == g_wlan_at_data.wifiBandwith)
    {
        if ((pFreq->value >= WLAN_FREQ_5G_W52_MIN) && (pFreq->value <= WLAN_FREQ_5G_W53_MAX))
        {
            for (i = 0; i < (sizeof(aulChannel036) / sizeof(uint16)); i++)
            {
                if (pFreq->value == aulChannel036[i])
                {
                    ulWifiFreq = (i * WLAN_CHANNEL_5G_INTERVAL + WLAN_CHANNEL_5G_MIN);
                    break;
                }
            }
        }
        else if ((pFreq->value >= WLAN_FREQ_5G_W56_MIN) && (pFreq->value <= WLAN_FREQ_5G_W56_MAX))
        {
            for (i = 0; i < (sizeof(aulChannel100) / sizeof(uint16)); i++)
            {
                if (pFreq->value == aulChannel100[i])
                {
                    ulWifiFreq = (i * WLAN_CHANNEL_5G_INTERVAL + HUNDRED);
                    break;
                }
            }

        }
	 else if ((pFreq->value >= WLAN_FREQ_5G_W57_MIN) && (pFreq->value <= WLAN_FREQ_5G_W57_MAX))
        {
            for (i = 0; i < (sizeof(aulChannel149) / sizeof(uint16)); i++)
            {
                if (pFreq->value == aulChannel149[i])
                {
                    ulWifiFreq = (i * WLAN_CHANNEL_5G_INTERVAL + WLAN_CHANNEL_5G_W57_START);
                    break;
                }
            }
        }
        else
        {        
            PLAT_WLAN_INFO("Error 20M wifiFreq parameters\n");      
            return AT_RETURN_FAILURE;
        }

	 PLAT_WLAN_INFO("Target Channel = %d\n", ulWifiFreq);
    
        if (! ((WLAN_CHANNEL_5G_MIN <= ulWifiFreq) && (WLAN_CHANNEL_5G_MAX >= ulWifiFreq)))
        {
            PLAT_WLAN_INFO("Target Channel ERROR,ulWifiFreq = %u!\n", ulWifiFreq);
            return AT_RETURN_FAILURE;
        }

	 memset(wl_cmd, 0, WIFI_CMD_MAX_SIZE);
        OSA_SNPRINTF(wl_cmd, WIFI_CMD_MAX_SIZE, WL_CONFIG_EXE_PATH" %s down", IF_5G);
        WIFI_TEST_CMD(wl_cmd);

        memset(wl_cmd, 0, WIFI_CMD_MAX_SIZE);
        OSA_SNPRINTF(wl_cmd, WIFI_CMD_MAX_SIZE, WL_CONFIG_EXE_PATH" %s phy_watchdog 0", IF_5G);
        WIFI_TEST_CMD(wl_cmd);

	 memset(wl_cmd, 0, WIFI_CMD_MAX_SIZE);
        OSA_SNPRINTF(wl_cmd, WIFI_CMD_MAX_SIZE, WL_CONFIG_EXE_PATH" %s band a", IF_5G);
        WIFI_TEST_CMD(wl_cmd);

        memset(wl_cmd, 0, WIFI_CMD_MAX_SIZE);
        OSA_SNPRINTF(wl_cmd, WIFI_CMD_MAX_SIZE, WL_CONFIG_EXE_PATH" %s mpc 0", IF_5G);
        WIFI_TEST_CMD(wl_cmd);

	 memset(wl_cmd, 0, WIFI_CMD_MAX_SIZE);
        OSA_SNPRINTF(wl_cmd, WIFI_CMD_MAX_SIZE, WL_CONFIG_EXE_PATH" %s country ALL", IF_5G);
        WIFI_TEST_CMD(wl_cmd);
		
        memset(wl_cmd, 0, WIFI_CMD_MAX_SIZE);
        OSA_SNPRINTF(wl_cmd, WIFI_CMD_MAX_SIZE, WL_CONFIG_EXE_PATH" %s scansuppress 1", IF_5G);
        WIFI_TEST_CMD(wl_cmd);
	
        memset(wl_cmd, 0, WIFI_CMD_MAX_SIZE);
        OSA_SNPRINTF(wl_cmd,  WIFI_CMD_MAX_SIZE, WL_CONFIG_EXE_PATH" %s chanspec %d", IF_5G, ulWifiFreq);
        WIFI_TEST_CMD(wl_cmd);

    }
    else if (AT_WIBAND_40M == g_wlan_at_data.wifiBandwith)
    {
        if ((pFreq->value >= WLAN_FREQ_5G_W52_40M_MIN) && (pFreq->value <= WLAN_FREQ_5G_W53_40M_MAX))
        {
            for (i = 0; i < (sizeof(aulChannel036_40M) / sizeof(uint16)); i++)
            {
                if (pFreq->value == aulChannel036_40M[i])
                {
                    ulWifiFreq = (i * WLAN_CHANNEL_5G_40M_INTERVAL + WLAN_CHANNEL_5G_MIN);
                    break;
                }
            }
        }        
        else if ((pFreq->value >= WLAN_FREQ_5G_W56_40M_MIN) && (pFreq->value <= WLAN_FREQ_5G_W56_40M_MAX))
        {
            for (i = 0; i < (sizeof(aulChannel100_40M) / sizeof(uint16)); i++)
            {
                if (pFreq->value == aulChannel100_40M[i])
                {
                    ulWifiFreq = (i * WLAN_CHANNEL_5G_40M_INTERVAL + HUNDRED);
                    break;
                }
            }

        }        
        else if ((pFreq->value >= WLAN_FREQ_5G_W57_40M_MIN) && (pFreq->value <= WLAN_FREQ_5G_W57_40M_MAX))
        {
            for (i = 0; i < (sizeof(aulChannel149_40M) / sizeof(uint16)); i++)
            {
                if (pFreq->value == aulChannel149_40M[i])
                {
                    ulWifiFreq = (i * WLAN_CHANNEL_5G_40M_INTERVAL + WLAN_CHANNEL_5G_W57_START);
                    break;
                }
            }
        }
        else
        {        
            PLAT_WLAN_INFO("Error 40M wifiFreq parameters\n");      
            return AT_RETURN_FAILURE;
        }

        PLAT_WLAN_INFO("Target Channel = %d\n", ulWifiFreq);
    
        /*40M�ŵ���U/L����*/
        if ((ulWifiFreq >= WLAN_CHANNEL_5G_MIN)
                && (ulWifiFreq <= WLAN_CHANNEL_5G_MAX))
        {            
            for (i = 0;i < (sizeof(channel_5g_40M_l) / sizeof(uint16));i++)
            {
                if(ulWifiFreq == channel_5g_40M_l[i])
                {
                    OSA_SNPRINTF(channeStr, sizeof(channeStr), "%dl", ulWifiFreq);                     
                    break;
                }                
            }

            if(i == (sizeof(channel_5g_40M_l) / sizeof(uint16)))
            {
                for (i = 0;i < (sizeof(channel_5g_40M_u) / sizeof(uint16));i++)
                {
                    if(ulWifiFreq == channel_5g_40M_u[i])
                    {
                        OSA_SNPRINTF(channeStr, sizeof(channeStr), "%du", ulWifiFreq);
                        break;
                    }                 
                } 
            }
            
            if(i == (sizeof(channel_5g_40M_u) / sizeof(uint16)))
            {
                OSA_SNPRINTF(channeStr, sizeof(channeStr), "%d", ulWifiFreq); 
            }
        }
        else
        {
            PLAT_WLAN_INFO("40M Channel Process ERROR!\n");
            return AT_RETURN_FAILURE;
        }       

        memset(wl_cmd, 0, WIFI_CMD_MAX_SIZE);
        OSA_SNPRINTF(wl_cmd, WIFI_CMD_MAX_SIZE, WL_CONFIG_EXE_PATH" %s down", IF_5G);
        WIFI_TEST_CMD(wl_cmd);

        memset(wl_cmd, 0, WIFI_CMD_MAX_SIZE);
        OSA_SNPRINTF(wl_cmd, WIFI_CMD_MAX_SIZE, WL_CONFIG_EXE_PATH" %s phy_watchdog 0", IF_5G);
        WIFI_TEST_CMD(wl_cmd);

	    memset(wl_cmd, 0, WIFI_CMD_MAX_SIZE);
        OSA_SNPRINTF(wl_cmd, WIFI_CMD_MAX_SIZE, WL_CONFIG_EXE_PATH" %s band a", IF_5G);
        WIFI_TEST_CMD(wl_cmd);

	    memset(wl_cmd, 0, WIFI_CMD_MAX_SIZE);
        OSA_SNPRINTF(wl_cmd, WIFI_CMD_MAX_SIZE, WL_CONFIG_EXE_PATH" %s mpc 0", IF_5G);
        WIFI_TEST_CMD(wl_cmd);

	    memset(wl_cmd, 0, WIFI_CMD_MAX_SIZE);
        OSA_SNPRINTF(wl_cmd, WIFI_CMD_MAX_SIZE, WL_CONFIG_EXE_PATH" %s country ALL", IF_5G);
        WIFI_TEST_CMD(wl_cmd);
		
        memset(wl_cmd, 0, WIFI_CMD_MAX_SIZE);
        OSA_SNPRINTF(wl_cmd, WIFI_CMD_MAX_SIZE, WL_CONFIG_EXE_PATH" %s scansuppress 1", IF_5G);
        WIFI_TEST_CMD(wl_cmd);

        memset(wl_cmd, 0, WIFI_CMD_MAX_SIZE);
        OSA_SNPRINTF(wl_cmd,  WIFI_CMD_MAX_SIZE, WL_CONFIG_EXE_PATH" %s chanspec %d/40", IF_5G, ulWifiFreq);
        WIFI_TEST_CMD(wl_cmd);
		
    }
    else if (AT_WIBAND_80M == g_wlan_at_data.wifiBandwith)
    {
        for (i = 0; i < (sizeof(aulChannelAC) / sizeof(uint16)); i++)
        {
            if (pFreq->value == aulChannelAC[i])
            {
                ulWifiFreq = aulChannelAC[i];
                break;
            }
        }

	    if (i == (sizeof(aulChannelAC) / sizeof(uint16)))
	    {
	        PLAT_WLAN_INFO("Error 80 wifiFreq parameters\n");      
            return AT_RETURN_FAILURE;
	    }

        if (5210 == ulWifiFreq)
        {
            ulWifiFreq = 36;
        }
        else if (5290 == ulWifiFreq)
        {
            ulWifiFreq = 64;
        }
        else if (5530 == ulWifiFreq)
        {
            ulWifiFreq = 100;
        }
        else if (5610)
        {
            ulWifiFreq = 124;
        }
        else
        {
            ulWifiFreq = 157;
        }

	    memset(wl_cmd, 0, WIFI_CMD_MAX_SIZE);
        OSA_SNPRINTF(wl_cmd, WIFI_CMD_MAX_SIZE, WL_CONFIG_EXE_PATH" %s down", IF_5G);
        WIFI_TEST_CMD(wl_cmd);

        memset(wl_cmd, 0, WIFI_CMD_MAX_SIZE);
        OSA_SNPRINTF(wl_cmd, WIFI_CMD_MAX_SIZE, WL_CONFIG_EXE_PATH" %s phy_watchdog 0", IF_5G);
        WIFI_TEST_CMD(wl_cmd);

	    memset(wl_cmd, 0, WIFI_CMD_MAX_SIZE);
        OSA_SNPRINTF(wl_cmd, WIFI_CMD_MAX_SIZE, WL_CONFIG_EXE_PATH" %s band a", IF_5G);
        WIFI_TEST_CMD(wl_cmd);

	    memset(wl_cmd, 0, WIFI_CMD_MAX_SIZE);
        OSA_SNPRINTF(wl_cmd, WIFI_CMD_MAX_SIZE, WL_CONFIG_EXE_PATH" %s mpc 0", IF_5G);
        WIFI_TEST_CMD(wl_cmd);

	    memset(wl_cmd, 0, WIFI_CMD_MAX_SIZE);
        OSA_SNPRINTF(wl_cmd, WIFI_CMD_MAX_SIZE, WL_CONFIG_EXE_PATH" %s country ALL", IF_5G);
        WIFI_TEST_CMD(wl_cmd);
		
        memset(wl_cmd, 0, WIFI_CMD_MAX_SIZE);
        OSA_SNPRINTF(wl_cmd, WIFI_CMD_MAX_SIZE, WL_CONFIG_EXE_PATH" %s scansuppress 1", IF_5G);
        WIFI_TEST_CMD(wl_cmd);	
	
        memset(wl_cmd, 0, WIFI_CMD_MAX_SIZE);
        OSA_SNPRINTF(wl_cmd,  WIFI_CMD_MAX_SIZE, WL_CONFIG_EXE_PATH" %s chanspec %d/80", IF_5G, ulWifiFreq);
        WIFI_TEST_CMD(wl_cmd);
		
    }
    else
    {
        PLAT_WLAN_INFO("Error bandwidth!\n");      
        return AT_RETURN_FAILURE;
    }
	
    /* ����ȫ�ֱ�����Ա���ѯ */
    g_wlan_at_data.wifiFreq.value = pFreq->value;
    g_wlan_at_data.wifiFreq.offset = pFreq->offset;
    return AT_RETURN_SUCCESS;

}

//////////////////////////////////////////////////////////////////////////
/*(4)^WIFREQ ����WiFiƵ�� */
//////////////////////////////////////////////////////////////////////////
/*****************************************************************************
 ��������  : int32 WlanATSetWifiFreq(WLAN_AT_WIFREQ_STRU *pFreq)
 ��������  : ����WiFiƵ��
 �������  : NA
 �������  : NA
 �� �� ֵ  : 0 �ɹ�
             1 ʧ��
 ����˵��  : 
*****************************************************************************/
STATIC int32 ATSetWifiFreq(WLAN_AT_WIFREQ_STRU *pFreq)
{
    if (NULL == pFreq)
    {
        return (AT_RETURN_FAILURE);
    }

    if (WLAN_AT_2G_BAND == g_wlan_at_data.wifiBand)
    {
        return WlanATSet2GWifiFreq(pFreq);
    }
    else if (WLAN_AT_5G_BAND == g_wlan_at_data.wifiBand)
    {
        return WlanATSet5GWifiFreq(pFreq);
    }
    else
    {
        return (AT_RETURN_FAILURE);
    }
}

/*****************************************************************************
 ��������  : int32 WlanATGetWifiFreq(WLAN_AT_WIFREQ_STRU *pFreq)
 ��������  : ��ȡWiFiƵ��
 �������  : NA
 �������  : NA
 �� �� ֵ  : 0 �ɹ�
             1 ʧ��
 ����˵��  : 
*****************************************************************************/
STATIC int32 ATGetWifiFreq(WLAN_AT_WIFREQ_STRU *pFreq)
{   
    ASSERT_NULL_POINTER(pFreq, AT_RETURN_FAILURE);
    
    memcpy(pFreq, &(g_wlan_at_data.wifiFreq), sizeof(WLAN_AT_WIFREQ_STRU));
      
    return (AT_RETURN_SUCCESS);
}

//////////////////////////////////////////////////////////////////////////
/*(5)^WIDATARATE ���úͲ�ѯ��ǰWiFi���ʼ�����
  WiFi���ʣ���λΪ0.01Mb/s��ȡֵ��ΧΪ0��65535 */
//////////////////////////////////////////////////////////////////////////
/*****************************************************************************
 ��������  : int32 WlanATSet2GWifiDataRate(uint32 rate)
 ��������  : ����WiFi��������
 �������  : NA
 �������  : NA
 �� �� ֵ  : 0 �ɹ�
             1 ʧ��
 ����˵��  : 
*****************************************************************************/
STATIC int32 WlanATSet2GWifiDataRate(uint32 rate)
{    
    int8    wl_cmd[WIFI_CMD_MAX_SIZE] = {0};
    uint32  ulWifiRate = rate / HUNDRED;
    uint32  ulNRate = 0;  
    
    const BRATE_ST wifi_brates_table[] = {{100, "1"}, {200, "2"}, {550, "5.5"}, {1100, "11"}};//b 
    const uint32 wifi_20m_nrates_table[] = {650, 1300, 1950, 2600, 3900, 5200, 5850, 6500};//n ht20
    const uint32 wifi_40m_nrates_table[] = {1350, 2700, 4050, 5400, 8100, 10800, 12150, 13500};//n ht40 
    
    #define WIFI_BRATES_TABLE_SIZE (sizeof(wifi_brates_table) / sizeof(BRATE_ST))
    #define WIFI_20M_NRATES_TABLE_SIZE (sizeof(wifi_20m_nrates_table) / sizeof(uint32))
    #define WIFI_40M_NRATES_TABLE_SIZE (sizeof(wifi_40m_nrates_table) / sizeof(uint32))
    
    PLAT_WLAN_INFO("WifiRate = %u\n", ulWifiRate);
    
    switch (g_wlan_at_data.wifiMode)
    {
        case AT_WIMODE_CW:
            PLAT_WLAN_INFO("AT_WIMODE_CW\n");
            return (AT_RETURN_FAILURE);

        case AT_WIMODE_80211b:
        case AT_WIMODE_80211g:
            OSA_SNPRINTF(wl_cmd, WIFI_CMD_MAX_SIZE, WL_CONFIG_EXE_PATH" %s nrate -r %d", IF_2G, ulWifiRate);
            WIFI_TEST_CMD(wl_cmd);
            break; 
            
        case AT_WIMODE_80211n:
            if (AT_WIBAND_20M == g_wlan_at_data.wifiBandwith)
            {
                /* WIFI 20M nģʽWL���������ֵΪ0~7����8�� */
                for (ulNRate = 0; ulNRate < WIFI_20M_NRATES_TABLE_SIZE; ulNRate++)
                {
                    if (wifi_20m_nrates_table[ulNRate] == rate)
                    {
                        PLAT_WLAN_INFO("20M NRate Index = %u\n", ulNRate);                        
                        break;
                    }
                }

                if (WIFI_20M_NRATES_TABLE_SIZE == ulNRate)
                {
                    PLAT_WLAN_INFO("20M NRate Error!\n");
                    return (AT_RETURN_FAILURE);
                }

                OSA_SNPRINTF(wl_cmd, WIFI_CMD_MAX_SIZE, WL_CONFIG_EXE_PATH" %s nrate -m %d", IF_2G, ulNRate);
                WIFI_TEST_CMD(wl_cmd);
            }
            else if (AT_WIBAND_40M == g_wlan_at_data.wifiBandwith)
            {
                for (ulNRate = 0; ulNRate < WIFI_40M_NRATES_TABLE_SIZE; ulNRate++)
                {
                    if (wifi_40m_nrates_table[ulNRate] == rate)
                    {
                        PLAT_WLAN_INFO("40M NRate Index = %u\n", ulNRate);
                        break;
                    }
                }

                if (WIFI_40M_NRATES_TABLE_SIZE == ulNRate)
                {
                    PLAT_WLAN_INFO("40M NRate Error!\n");
                    return (AT_RETURN_FAILURE);
                }    
                
                OSA_SNPRINTF(wl_cmd, WIFI_CMD_MAX_SIZE, WL_CONFIG_EXE_PATH" %s nrate -m %d -s 3", IF_2G, ulNRate);
                WIFI_TEST_CMD(wl_cmd);              
            }    
            break;
        default:
            return (AT_RETURN_FAILURE);            
    } 
    
    WIFI_TEST_CMD(wl_cmd);
    /*����ȫ�ֱ�����Ա���ѯ*/
    g_wlan_at_data.wifiRate = rate;
       
    return (AT_RETURN_SUCCESS);       
}


STATIC int32 AT_TransferWifiRate(uint32 ulInRate, uint32 *pulOutRate)
{
    uint8    ucIndex = 0;
    /* WIFI nģʽ AT^WIDATARATE���õ�����ֵ��WL��������ֵ�Ķ�Ӧ�� */
    uint32  aucAtWifiN20Rate_Table[] = {650, 1300, 1950, 2600, 3900, 5200, 5850, 6500, \
                                                               1300, 2600, 3900, 5200, 7800, 10400, 11700, 13000};

    uint32  aucAtWifiN40Rate_Table[] = {1350, 2700, 4050, 5400, 8100, 10800, 12150, 13500, \
                                                               2700, 5400, 8100, 10800, 16200, 21600, 24300, 27000};

    uint32  aucAtWifiAC20Rate_Table[] = {650, 1300, 1950, 2600, 3900, 5200, 5850, 6500,  7800};

    uint32  aucAtWifiAC40Rate_Table[] = {650, 1300, 1950, 2600, 3900, 5200, 5850, 6500,  16200, 18000};

    uint32  aucAtWifiAC80Rate_Table[] = {650, 1300, 1950, 2600, 3900, 5200, 5850, 6500,  35100, 39000};

    if(AT_WIBAND_20M == g_wlan_at_data.wifiBandwith)
    {

        if(RF_2G_0 == g_ulWifiRF || RF_2G_1 == g_ulWifiRF)
        {
            for (ucIndex = 0; ucIndex < AT_WIFI_N_RATE_NUM / 2; ucIndex++)
            {
                if (aucAtWifiN20Rate_Table[ucIndex] == ulInRate)
                {
                    *pulOutRate = ucIndex;
                    break;
                }
            }
			
            if (ucIndex >= AT_WIFI_N_RATE_NUM / 2)
            {
                return AT_RETURN_FAILURE;
            }
        }
	 else if(RF_5G_0 == g_ulWifiRF || RF_5G_1 == g_ulWifiRF)
        {
            for (ucIndex = 0; ucIndex < AT_WIFI_AC20_RATE_NUM; ucIndex++)
            {
                if (aucAtWifiAC20Rate_Table[ucIndex] == ulInRate)
                {
                    *pulOutRate = ucIndex;
                    break;
                }
            }
            if (ucIndex >= AT_WIFI_AC20_RATE_NUM)
            {
                return AT_RETURN_FAILURE;
            }
        }
        else if(RF_2G_MIMO == g_ulWifiRF || RF_5G_MIMO == g_ulWifiRF)
        {
            for (ucIndex = AT_WIFI_N_RATE_NUM / 2; ucIndex < AT_WIFI_N_RATE_NUM; ucIndex++)
            {
                if (aucAtWifiN20Rate_Table[ucIndex] == ulInRate)
                {
                    *pulOutRate = ucIndex;
                    break;
                }
            }
			
            if (ucIndex >= AT_WIFI_N_RATE_NUM)
            {
                return AT_RETURN_FAILURE;
            }
        }
    }
    else if(AT_WIBAND_40M == g_wlan_at_data.wifiBandwith)
    { 
        if(RF_2G_0 == g_ulWifiRF || RF_2G_1 == g_ulWifiRF)
        {
            for (ucIndex = 0; ucIndex < AT_WIFI_N_RATE_NUM / 2; ucIndex++)
            {
                if (aucAtWifiN40Rate_Table[ucIndex] == ulInRate)
                {
                    *pulOutRate = ucIndex;
                    break;
                }
            }
            if (ucIndex >= AT_WIFI_N_RATE_NUM / 2)
            {
                return AT_RETURN_FAILURE;
            }
        }
        else if(RF_5G_0 == g_ulWifiRF || RF_5G_1 == g_ulWifiRF)
        {
            if(AT_WIMODE_80211n == g_wlan_at_data.wifiMode)
            {
                for (ucIndex = 0; ucIndex < AT_WIFI_N_RATE_NUM ; ucIndex++)
                {
                    if (aucAtWifiN40Rate_Table[ucIndex] == ulInRate)
                    {
                        *pulOutRate = ucIndex;
                        break;
                    }
                }
                if (ucIndex >= AT_WIFI_N_RATE_NUM )
                {
                    return AT_RETURN_FAILURE;
                }
            }
            else
            {
                for (ucIndex = 0; ucIndex < AT_WIFI_AC40_RATE_NUM; ucIndex++)
                {
                    if (aucAtWifiAC40Rate_Table[ucIndex] == ulInRate)
                    {
                        *pulOutRate = ucIndex;
                        break;
                    }
                }
                if (ucIndex >= AT_WIFI_AC40_RATE_NUM)
                {
                    return AT_RETURN_FAILURE;
                }
            }
        }
        else if(RF_2G_MIMO == g_ulWifiRF || RF_5G_MIMO == g_ulWifiRF)
        {
            for (ucIndex = AT_WIFI_N_RATE_NUM / 2; ucIndex < AT_WIFI_N_RATE_NUM; ucIndex++)
            {
                if (aucAtWifiN40Rate_Table[ucIndex] == ulInRate)
                {
                    *pulOutRate = ucIndex;
                    break;
                }
            }
            if (ucIndex >= AT_WIFI_N_RATE_NUM)
            {
                return AT_RETURN_FAILURE;
            }
        }
    }
    else if(AT_WIBAND_80M == g_wlan_at_data.wifiBandwith)
    {
        if(RF_5G_0 == g_ulWifiRF || RF_5G_1 == g_ulWifiRF)
        {
            for (ucIndex = 0; ucIndex < AT_WIFI_AC80_RATE_NUM; ucIndex++)
            {
                if (aucAtWifiAC80Rate_Table[ucIndex] == ulInRate)
                {
                    *pulOutRate = ucIndex;
                    break;
                }
            }
			
            if (ucIndex >= AT_WIFI_AC80_RATE_NUM)
            {
                return AT_RETURN_FAILURE;
            }
        }
    }

    return AT_RETURN_SUCCESS;
}
//////////////////////////////////////////////////////////////////////////
/*(5)^WIDATARATE ���úͲ�ѯ��ǰWiFi���ʼ�����
  WiFi���ʣ���λΪ0.01Mb/s��ȡֵ��ΧΪ0��65535 */
//////////////////////////////////////////////////////////////////////////
/*****************************************************************************
 ��������  : int32 WlanATSet2GWifiDataRate(uint32 rate)
 ��������  : ����WiFi��������
 �������  : NA
 �������  : NA
 �� �� ֵ  : 0 �ɹ�
             1 ʧ��
 ����˵��  : 
*****************************************************************************/
STATIC int32 WlanATSet5GWifiDataRate(uint32 rate)
{
    uint8     wl_cmd[WIFI_CMD_MAX_SIZE] = {0};
    uint32   ulWifiRate = rate / HUNDRED;;
    uint32   ulNRate = 0;
    int32     ulRslt;
    int32     ret = 0;
    uint32   single_para = 0;
  
    if(WLAN_AT_5G_BAND == g_wlan_at_data.wifiBand)
    {
        switch(g_wlan_at_data.wifiMode)
        {
            case AT_WIMODE_CW:
	     {
                PLAT_WLAN_INFO("AT_WIMODE_CW\n");
                return (AT_RETURN_FAILURE);
            }
            case AT_WIMODE_80211a:
	     {
	         OSA_SNPRINTF(wl_cmd, WIFI_CMD_MAX_SIZE, WL_CONFIG_EXE_PATH" %s 5g_rate -r %d -b %d", IF_5G, 
		     	                                    ulWifiRate, (g_wlan_at_data.wifiBandwith+ 1)* 20);
	         WIFI_TEST_CMD(wl_cmd);
	         break;   
            }
            case AT_WIMODE_80211n:
            {
                ulRslt = AT_TransferWifiRate(rate, &ulNRate);
                if(AT_RETURN_SUCCESS == ulRslt)
                {
                    OSA_SNPRINTF(wl_cmd, WIFI_CMD_MAX_SIZE, WL_CONFIG_EXE_PATH" %s 5g_rate -h %d -b %d", IF_5G, 
                                                           ulNRate, (g_wlan_at_data.wifiBandwith + 1)* 20);
		      WIFI_TEST_CMD(wl_cmd);

		      g_ulWifiMCSIndex = ulNRate;
                }
                else
                {
                    PLAT_WLAN_INFO("5G NRate Error!\n");
                    return (AT_RETURN_FAILURE);
                }
	         break;
            }
            case AT_WIMODE_80211ac:
            {
                ulRslt = AT_TransferWifiRate(rate, &ulNRate);
                if(AT_RETURN_SUCCESS == ulRslt)
                {
                    if(AT_WIBAND_80M == g_wlan_at_data.wifiBandwith)
                    {
                        OSA_SNPRINTF(wl_cmd, WIFI_CMD_MAX_SIZE, WL_CONFIG_EXE_PATH" %s 5g_rate -v %d -s 1 -b 80", IF_5G, ulNRate);
                    }
                    else
                    {
                        OSA_SNPRINTF(wl_cmd, WIFI_CMD_MAX_SIZE, WL_CONFIG_EXE_PATH" %s 5g_rate -v %d -s 1 -b %d", IF_5G, 
                                                    ulNRate, (g_wlan_at_data.wifiBandwith + 1)* 20);
                    }
					
		            WIFI_TEST_CMD(wl_cmd);
                }
                else
                {
                    PLAT_WLAN_INFO("5G ACRate Error!\n");
                    return (AT_RETURN_FAILURE);
                }
	         break;
            }
            default:
	     {
	         PLAT_WLAN_INFO("5G Mode Error!\n");
                return (AT_RETURN_FAILURE);
            }
        }
    }
    else
    {
        PLAT_WLAN_INFO("Band Error!\n");
        return (AT_RETURN_FAILURE);
    }
       
    g_wlan_at_data.wifiRate = rate;

    return AT_RETURN_SUCCESS;
}
//////////////////////////////////////////////////////////////////////////
/*(5)^WIDATARATE ���úͲ�ѯ��ǰWiFi���ʼ�����
  WiFi���ʣ���λΪ0.01Mb/s��ȡֵ��ΧΪ0��65535 */
//////////////////////////////////////////////////////////////////////////
/*****************************************************************************
 ��������  : int32 WlanATSetWifiDataRate(uint32 rate)
 ��������  : ����WiFi��������
 �������  : NA
 �������  : NA
 �� �� ֵ  : 0 �ɹ�
             1 ʧ��
 ����˵��  : 
*****************************************************************************/
STATIC int32 ATSetWifiDataRate(uint32 rate)
{   

    if (WLAN_AT_2G_BAND == g_wlan_at_data.wifiBand)
    {
        return WlanATSet2GWifiDataRate(rate);
    }
    else if (WLAN_AT_5G_BAND == g_wlan_at_data.wifiBand)
    {
        return WlanATSet5GWifiDataRate(rate);
    }
    else
    {
        return (AT_RETURN_FAILURE);
    }
}
/*****************************************************************************
 ��������  : uint32 WlanATGetWifiDataRate()
 ��������  : ��ѯ��ǰWiFi��������
 �������  : NA
 �������  : NA
 �� �� ֵ  : wifi����
 ����˵��  : 
*****************************************************************************/
STATIC uint32 ATGetWifiDataRate(void)
{
    return g_wlan_at_data.wifiRate;
}

//////////////////////////////////////////////////////////////////////////
/*(6)^WIPOW ������WiFi���书�� 
   WiFi���书�ʣ���λΪ0.01dBm��ȡֵ��ΧΪ -32768��32767 */
//////////////////////////////////////////////////////////////////////////
/*****************************************************************************
 ��������  : int32 WlanATSetWifiPOW(int32 power_dBm_percent)
 ��������  : ����WiFi���书��
 �������  : NA
 �������  : NA
 �� �� ֵ  : 0 �ɹ�
             1 ʧ��
 ����˵��  : 
*****************************************************************************/
STATIC int32 ATSetWifiPOW(int32 power_dBm_percent)
{
    /*����ȫ�ֱ�����Ա���ѯ*/
    g_wlan_at_data.wifiPower = power_dBm_percent;
    return (AT_RETURN_SUCCESS);
}

/*****************************************************************************
 ��������  : int32 WlanATGetWifiPOW()
 ��������  : ��ȡWiFi��ǰ���书��
 �������  : NA
 �������  : NA
 �� �� ֵ  : NA
 ����˵��  : 
*****************************************************************************/
STATIC int32 ATGetWifiPOW(void)
{
    return g_wlan_at_data.wifiPower;
}

//////////////////////////////////////////////////////////////////////////
/*(7)^WITX ������WiFi��������� */
//////////////////////////////////////////////////////////////////////////
/*****************************************************************************
 ��������  : int32 WlanATSet2GWifiTX(WLAN_AT_FEATURE_TYPE onoff)
 ��������  : �򿪻�ر�wifi�����
 �������  : 0 �ر�
             1 ��
 �������  : NA
 �� �� ֵ  : 0 �ɹ�
             1 ʧ��
 ����˵��  : 
*****************************************************************************/
STATIC int32 WlanATSet2GWifiTX(WLAN_AT_FEATURE_TYPE onoff)
{
    int8   wl_cmd[WIFI_CMD_MAX_SIZE] = {0};
    int32 WifiPower = 0;

    if (AT_WIMODE_CW == g_wlan_at_data.wifiMode)
    {
        /* cw ���� */
    }
    else
    {
        if(AT_FEATURE_DISABLE == onoff)
        {
            memset(wl_cmd, 0, WIFI_CMD_MAX_SIZE);
            OSA_SNPRINTF(wl_cmd, WIFI_CMD_MAX_SIZE, WL_CONFIG_EXE_PATH" %s pkteng_stop tx", IF_2G);
            WIFI_TEST_CMD(wl_cmd);

            memset(wl_cmd, 0, WIFI_CMD_MAX_SIZE);
            OSA_SNPRINTF(wl_cmd, WIFI_CMD_MAX_SIZE, WL_CONFIG_EXE_PATH " %s down", IF_2G);
            WIFI_TEST_CMD(wl_cmd);

            memset(wl_cmd, 0, WIFI_CMD_MAX_SIZE);
            OSA_SNPRINTF(wl_cmd, WIFI_CMD_MAX_SIZE, WL_CONFIG_EXE_PATH " %s txchain 3", IF_2G);
            WIFI_TEST_CMD(wl_cmd);

            memset(wl_cmd, 0, WIFI_CMD_MAX_SIZE);
            OSA_SNPRINTF(wl_cmd, WIFI_CMD_MAX_SIZE, WL_CONFIG_EXE_PATH " %s up", IF_2G);
            WIFI_TEST_CMD(wl_cmd);

            memset(wl_cmd, 0, WIFI_CMD_MAX_SIZE);
            OSA_SNPRINTF(wl_cmd, WIFI_CMD_MAX_SIZE, WL_CONFIG_EXE_PATH " %s phy_watchdog 1", IF_2G);
            WIFI_TEST_CMD(wl_cmd);

        }
        else
        {
            memset(wl_cmd, 0, WIFI_CMD_MAX_SIZE);
            OSA_SNPRINTF(wl_cmd, WIFI_CMD_MAX_SIZE, WL_CONFIG_EXE_PATH" %s down", IF_2G);
            WIFI_TEST_CMD(wl_cmd);

            memset(wl_cmd, 0, WIFI_CMD_MAX_SIZE);
            OSA_SNPRINTF(wl_cmd, WIFI_CMD_MAX_SIZE, WL_CONFIG_EXE_PATH" %s txchain 3", IF_2G);
            WIFI_TEST_CMD(wl_cmd);

            memset(wl_cmd, 0, WIFI_CMD_MAX_SIZE);
            OSA_SNPRINTF(wl_cmd, WIFI_CMD_MAX_SIZE, WL_CONFIG_EXE_PATH" %s up", IF_2G);
            WIFI_TEST_CMD(wl_cmd);
            
            memset(wl_cmd, 0, WIFI_CMD_MAX_SIZE);
            OSA_SNPRINTF(wl_cmd, WIFI_CMD_MAX_SIZE, WL_CONFIG_EXE_PATH" %s ssid \"\"", IF_2G);
            WIFI_SHELL_CMD(wl_cmd);

            if(RF_2G_0 == g_ulWifiRF || RF_2G_1 == g_ulWifiRF)
            {
                memset(wl_cmd, 0, WIFI_CMD_MAX_SIZE);
                OSA_SNPRINTF(wl_cmd, WIFI_CMD_MAX_SIZE, WL_CONFIG_EXE_PATH" %s txant %d",  IF_2G,  g_ulWifiRF);
                WIFI_TEST_CMD(wl_cmd);

                memset(wl_cmd, 0, WIFI_CMD_MAX_SIZE);
                OSA_SNPRINTF(wl_cmd, WIFI_CMD_MAX_SIZE, WL_CONFIG_EXE_PATH" %s antdiv %d",  IF_2G,  g_ulWifiRF);
                WIFI_TEST_CMD(wl_cmd);
            }

            WifiPower = g_wlan_at_data.wifiPower / 100;

            if ((WLAN_AT_POWER_MIN <= WifiPower) && (WLAN_AT_POWER_MAX >= WifiPower))
            {
                memset(wl_cmd, 0, WIFI_CMD_MAX_SIZE);
                OSA_SNPRINTF(wl_cmd, WIFI_CMD_MAX_SIZE, WL_CONFIG_EXE_PATH" %s txpwr1 -o -q %d", IF_2G, WifiPower * 4);
                WIFI_TEST_CMD(wl_cmd);
            }
            else
            {
                PLAT_WLAN_INFO("2.4G invaild argument and will use power vaule in nv\n");
            }

            memset(wl_cmd, 0, WIFI_CMD_MAX_SIZE);
            OSA_SNPRINTF(wl_cmd, WIFI_CMD_MAX_SIZE, WL_CONFIG_EXE_PATH" %s phy_forcecal 1", IF_2G);
            WIFI_TEST_CMD(wl_cmd);

            memset(wl_cmd, 0, WIFI_CMD_MAX_SIZE);
            OSA_SNPRINTF(wl_cmd, WIFI_CMD_MAX_SIZE, WL_CONFIG_EXE_PATH" %s pkteng_start 00:90:4c:21:00:8e tx 100 1500 0", IF_2G);
            WIFI_TEST_CMD(wl_cmd);
        } 
    }

    /*����ȫ�ֱ�����ѱ���ѯ*/
    g_wlan_at_data.wifiTX = onoff;

    return (AT_RETURN_SUCCESS);

}

//////////////////////////////////////////////////////////////////////////
/*(7)^WITX ������WiFi��������� */
//////////////////////////////////////////////////////////////////////////
/*****************************************************************************
 ��������  : int32 WlanATSet5GWifiTX(WLAN_AT_FEATURE_TYPE onoff)
 ��������  : �򿪻�ر�wifi�����
 �������  : 0 �ر�
             1 ��
 �������  : NA
 �� �� ֵ  : 0 �ɹ�
             1 ʧ��
 ����˵��  : 
*****************************************************************************/
STATIC int32 WlanATSet5GWifiTX(WLAN_AT_FEATURE_TYPE onoff)
{
    int8   wl_cmd[WIFI_CMD_MAX_SIZE] = {0};
    int32 WifiPower = 0;
    
    if (AT_WIMODE_CW == g_wlan_at_data.wifiMode)
    {
        /* cw ���� */
    }
    else
    {
        if(AT_FEATURE_DISABLE == onoff)
        {
            memset(wl_cmd, 0, WIFI_CMD_MAX_SIZE);
            OSA_SNPRINTF(wl_cmd, WIFI_CMD_MAX_SIZE, WL_CONFIG_EXE_PATH" %s pkteng_stop tx", IF_5G);
            WIFI_TEST_CMD(wl_cmd);

            memset(wl_cmd, 0, WIFI_CMD_MAX_SIZE);
            OSA_SNPRINTF(wl_cmd, WIFI_CMD_MAX_SIZE, WL_CONFIG_EXE_PATH " %s down", IF_5G);
            WIFI_TEST_CMD(wl_cmd);

            memset(wl_cmd, 0, WIFI_CMD_MAX_SIZE);
            OSA_SNPRINTF(wl_cmd, WIFI_CMD_MAX_SIZE, WL_CONFIG_EXE_PATH " %s phy_watchdog 1", IF_5G);
            WIFI_TEST_CMD(wl_cmd);

            memset(wl_cmd, 0, WIFI_CMD_MAX_SIZE);
            OSA_SNPRINTF(wl_cmd, WIFI_CMD_MAX_SIZE, WL_CONFIG_EXE_PATH " %s up", IF_5G);
            WIFI_TEST_CMD(wl_cmd);

        }
        else
        {
            memset(wl_cmd, 0, WIFI_CMD_MAX_SIZE);
            OSA_SNPRINTF(wl_cmd, WIFI_CMD_MAX_SIZE, WL_CONFIG_EXE_PATH" %s down", IF_5G);
            WIFI_TEST_CMD(wl_cmd);

            memset(wl_cmd, 0, WIFI_CMD_MAX_SIZE);
            OSA_SNPRINTF(wl_cmd, WIFI_CMD_MAX_SIZE, WL_CONFIG_EXE_PATH" %s txchain 3", IF_5G);
            WIFI_TEST_CMD(wl_cmd);

            memset(wl_cmd, 0, WIFI_CMD_MAX_SIZE);
            OSA_SNPRINTF(wl_cmd, WIFI_CMD_MAX_SIZE, WL_CONFIG_EXE_PATH" %s rxchain 3", IF_5G);
            WIFI_TEST_CMD(wl_cmd);

            memset(wl_cmd, 0, WIFI_CMD_MAX_SIZE);
            OSA_SNPRINTF(wl_cmd, WIFI_CMD_MAX_SIZE, WL_CONFIG_EXE_PATH" %s up", IF_5G);
            WIFI_TEST_CMD(wl_cmd);

            if(AT_WIMODE_80211n == g_wlan_at_data.wifiMode
                      && AT_WIFI_N_RATE_NUM / 2 <= g_ulWifiMCSIndex)
            {
                /*MIMO*/
                memset(wl_cmd, 0, WIFI_CMD_MAX_SIZE);
                OSA_SNPRINTF(wl_cmd, WIFI_CMD_MAX_SIZE, WL_CONFIG_EXE_PATH" %s txcore -s 1 -c 3", IF_5G);
                WIFI_TEST_CMD(wl_cmd);
            }
            else if(AT_WIMODE_80211a ==  g_wlan_at_data.wifiMode)
            {
                memset(wl_cmd, 0, WIFI_CMD_MAX_SIZE);
                OSA_SNPRINTF(wl_cmd, WIFI_CMD_MAX_SIZE, WL_CONFIG_EXE_PATH" %s txcore -o %d", IF_5G,  g_ulWifiRF-1);
                WIFI_TEST_CMD(wl_cmd);
            }
            else
            {
                memset(wl_cmd, 0, WIFI_CMD_MAX_SIZE);
                OSA_SNPRINTF(wl_cmd, WIFI_CMD_MAX_SIZE, WL_CONFIG_EXE_PATH" %s txcore -s 1 -c %d", IF_5G,  g_ulWifiRF - 1);
                WIFI_TEST_CMD(wl_cmd);
            }

            memset(wl_cmd, 0, WIFI_CMD_MAX_SIZE);
            OSA_SNPRINTF(wl_cmd, WIFI_CMD_MAX_SIZE, WL_CONFIG_EXE_PATH" %s ssid \"\"", IF_5G);
            WIFI_SHELL_CMD(wl_cmd);

            WifiPower = g_wlan_at_data.wifiPower / 100;

            if ((WLAN_AT_POWER_MIN <= WifiPower) && (WLAN_AT_POWER_MAX >= WifiPower))
            {
                memset(wl_cmd, 0, WIFI_CMD_MAX_SIZE);
                OSA_SNPRINTF(wl_cmd, WIFI_CMD_MAX_SIZE, WL_CONFIG_EXE_PATH" %s txpwr1 -o -q %d", IF_5G, WifiPower * 4);
                WIFI_TEST_CMD(wl_cmd);
            }
            else
            {
                PLAT_WLAN_INFO("5G invaild argument and will use power vaule in nv\n");
            }

            memset(wl_cmd, 0, WIFI_CMD_MAX_SIZE);
            OSA_SNPRINTF(wl_cmd, WIFI_CMD_MAX_SIZE, WL_CONFIG_EXE_PATH" %s phy_forcecal 1", IF_5G);
            WIFI_TEST_CMD(wl_cmd);

            if(AT_WIMODE_80211ac ==  g_wlan_at_data.wifiMode &&
                (AT_WIBAND_40M == g_wlan_at_data.wifiBandwith || AT_WIBAND_80M == g_wlan_at_data.wifiBandwith))
            {
                memset(wl_cmd, 0, WIFI_CMD_MAX_SIZE);
                OSA_SNPRINTF(wl_cmd, WIFI_CMD_MAX_SIZE, WL_CONFIG_EXE_PATH" %s pkteng_start 00:90:4c:21:00:8e tx 30 1500 0", IF_5G);
                WIFI_TEST_CMD(wl_cmd);
            }
            else
            {
                memset(wl_cmd, 0, WIFI_CMD_MAX_SIZE);
                OSA_SNPRINTF(wl_cmd, WIFI_CMD_MAX_SIZE, WL_CONFIG_EXE_PATH" %s pkteng_start 00:90:4c:21:00:8e tx 100 1500 0", IF_5G);
                WIFI_TEST_CMD(wl_cmd);
            }
			
        } 
    }

    /*����ȫ�ֱ�����ѱ���ѯ*/
    g_wlan_at_data.wifiTX = onoff;

    return (AT_RETURN_SUCCESS);
}
//////////////////////////////////////////////////////////////////////////
/*(7)^WITX ������WiFi��������� */
//////////////////////////////////////////////////////////////////////////
/*****************************************************************************
 ��������  : int32 WlanATSetWifiTX(WLAN_AT_FEATURE_TYPE onoff)
 ��������  : �򿪻�ر�wifi�����
 �������  : 0 �ر�
             1 ��
 �������  : NA
 �� �� ֵ  : 0 �ɹ�
             1 ʧ��
 ����˵��  : 
*****************************************************************************/
STATIC int32 ATSetWifiTX(WLAN_AT_FEATURE_TYPE onoff)
{
    if (WLAN_AT_2G_BAND == g_wlan_at_data.wifiBand)
    {
        return WlanATSet2GWifiTX(onoff);
    }
    else if (WLAN_AT_5G_BAND == g_wlan_at_data.wifiBand)
    {
        return WlanATSet5GWifiTX(onoff);
    }
    else
    {
        return (AT_RETURN_FAILURE);
    }
}
/*****************************************************************************
 ��������  : WLAN_AT_FEATURE_TYPE WlanATGetWifiTX()
 ��������  : ��ѯ��ǰWiFi�����״̬��Ϣ
 �������  : NA
 �������  : NA
 �� �� ֵ  : 0 �رշ����
             1 �򿪷����
 ����˵��  : 
*****************************************************************************/
STATIC WLAN_AT_FEATURE_TYPE ATGetWifiTX(void)
{
    return g_wlan_at_data.wifiTX;
}

/*****************************************************************************
 ��������  : WLAN_AT_FEATURE_TYPE WlanATSetWifiRX()
 ��������  : ��ѯ��ǰWiFi�����״̬��Ϣ
 �������  : NA
 �������  : NA
 �� �� ֵ  : 0 �رշ����
             1 �򿪷����
 ����˵��  : 
*****************************************************************************/
//////////////////////////////////////////////////////////////////////////
/*(8)^WIRX ����WiFi���ջ����� */
//////////////////////////////////////////////////////////////////////////
STATIC WLAN_AT_FEATURE_TYPE WlanATSet2GWifiRX(WLAN_AT_WIRX_STRU *params)
{
    int8    wl_cmd[WIFI_CMD_MAX_SIZE] = {0};
    ASSERT_NULL_POINTER(params, AT_RETURN_FAILURE);
        
    switch (params->onoff)
    {
        case AT_FEATURE_DISABLE:
            memset(wl_cmd, 0, WIFI_CMD_MAX_SIZE);
            OSA_SNPRINTF(wl_cmd, WIFI_CMD_MAX_SIZE, WL_CONFIG_EXE_PATH" %s pkteng_stop rx", IF_2G);
            WIFI_TEST_CMD(wl_cmd);      
            
            memset(wl_cmd, 0, WIFI_CMD_MAX_SIZE);
            OSA_SNPRINTF(wl_cmd, WIFI_CMD_MAX_SIZE, WL_CONFIG_EXE_PATH" %s down", IF_2G);
            WIFI_TEST_CMD(wl_cmd);
            
            g_ulRxState = AT_FEATURE_DISABLE;
            break;
            
        case AT_FEATURE_ENABLE:
            if (0 == strncmp(params->src_mac, "", MAC_ADDRESS_LEN))
            {
                PLAT_WLAN_INFO("src mac is NULL\n");
                return (AT_RETURN_FAILURE);
            }
            PLAT_WLAN_INFO("src mac is %s\n",params->src_mac);
            
            memset(wl_cmd, 0, WL_CMD_BUF_LEN);
            OSA_SNPRINTF(wl_cmd, sizeof(wl_cmd), WL_CONFIG_EXE_PATH" %s rxchain %u", IF_2G, 
                (g_wlan_at_data.wifiGroup % 2 + 1));   /*����ģʽ*/
            WIFI_TEST_CMD(wl_cmd);
            
            memset(wl_cmd, 0, WL_CMD_BUF_LEN);
            OSA_SNPRINTF(wl_cmd, WL_CMD_BUF_LEN, WL_CONFIG_EXE_PATH" %s ssid \"\"", IF_2G);
            WIFI_SHELL_CMD(wl_cmd);
            
            memset(wl_cmd, 0, WL_CMD_BUF_LEN);
            OSA_SNPRINTF(wl_cmd, sizeof(wl_cmd), WL_CONFIG_EXE_PATH"%s up", IF_2G);
            WIFI_TEST_CMD(wl_cmd);
            
            memset(wl_cmd, 0, WL_CMD_BUF_LEN);
            OSA_SNPRINTF(wl_cmd, sizeof(wl_cmd), WL_CONFIG_EXE_PATH"%s phy_forcecal 1", IF_2G);
            WIFI_TEST_CMD(wl_cmd);

            msleep(DELAY_150_TIME);  /* ��У׼���հ�����ʱ150���룬��У׼��Ч */

            OSA_SNPRINTF(wl_cmd, sizeof(wl_cmd), WL_CONFIG_EXE_PATH" %s pkteng_start %s rx", IF_2G, params->src_mac);
            WIFI_TEST_CMD(wl_cmd);
            
            g_ulRxState = AT_FEATURE_ENABLE;           
            
            wifi_get_rx_packet_report(&g_ulUcastWifiRxPkts,  &g_ulMcastWifiRxPkts);
            break;
            
        default:
            return (AT_RETURN_FAILURE);
    }
    
    memcpy(&g_wlan_at_data.wifiRX, params, sizeof(WLAN_AT_WIRX_STRU));
        
    return (AT_RETURN_SUCCESS);
}

/*****************************************************************************
 ��������  : WLAN_AT_FEATURE_TYPE WlanATSet5GWifiRX()
 ��������  : ��ѯ��ǰWiFi�����״̬��Ϣ
 �������  : NA
 �������  : NA
 �� �� ֵ  : 0 �رշ����
             1 �򿪷����
 ����˵��  : 
*****************************************************************************/
//////////////////////////////////////////////////////////////////////////
/*(8)^WIRX ����WiFi���ջ����� */
//////////////////////////////////////////////////////////////////////////
STATIC WLAN_AT_FEATURE_TYPE WlanATSet5GWifiRX(WLAN_AT_WIRX_STRU *params)
{
    int8    wl_cmd[WIFI_CMD_MAX_SIZE] = {0};
    ASSERT_NULL_POINTER(params, AT_RETURN_FAILURE);
        
    switch (params->onoff)
    {
        case AT_FEATURE_DISABLE:
            memset(wl_cmd, 0, WIFI_CMD_MAX_SIZE);
            OSA_SNPRINTF(wl_cmd, WIFI_CMD_MAX_SIZE, WL_CONFIG_EXE_PATH" %s pkteng_stop rx", IF_5G);
            WIFI_TEST_CMD(wl_cmd);      
            
            memset(wl_cmd, 0, WIFI_CMD_MAX_SIZE);
            OSA_SNPRINTF(wl_cmd, WIFI_CMD_MAX_SIZE, WL_CONFIG_EXE_PATH" %s down", IF_5G);
            WIFI_TEST_CMD(wl_cmd);
            
            g_ulRxState = AT_FEATURE_DISABLE;
            break;
            
        case AT_FEATURE_ENABLE:
            if (0 == strncmp(params->src_mac, "", MAC_ADDRESS_LEN))
            {
                PLAT_WLAN_INFO("src mac is NULL\n");
                return (AT_RETURN_FAILURE);
            }
            PLAT_WLAN_INFO("src mac is %s\n",params->src_mac);

            memset(wl_cmd, 0, WL_CMD_BUF_LEN);
            OSA_SNPRINTF(wl_cmd, sizeof(wl_cmd), WL_CONFIG_EXE_PATH"%s phy_watchdog 0", IF_5G);
            WIFI_TEST_CMD(wl_cmd);
            
            memset(wl_cmd, 0, WL_CMD_BUF_LEN);
            OSA_SNPRINTF(wl_cmd, sizeof(wl_cmd), WL_CONFIG_EXE_PATH" %s rxchain %u", IF_5G, 
                (g_wlan_at_data.wifiGroup % 2 + 1));   /*����ģʽ*/
            WIFI_TEST_CMD(wl_cmd);
            
            memset(wl_cmd, 0, WL_CMD_BUF_LEN);
            OSA_SNPRINTF(wl_cmd, WL_CMD_BUF_LEN, WL_CONFIG_EXE_PATH" %s ssid \"\"", IF_5G);
            WIFI_SHELL_CMD(wl_cmd);
            
            memset(wl_cmd, 0, WL_CMD_BUF_LEN);
            OSA_SNPRINTF(wl_cmd, sizeof(wl_cmd), WL_CONFIG_EXE_PATH"%s up", IF_5G);
            WIFI_TEST_CMD(wl_cmd);
            
            memset(wl_cmd, 0, WL_CMD_BUF_LEN);
            OSA_SNPRINTF(wl_cmd, sizeof(wl_cmd), WL_CONFIG_EXE_PATH"%s phy_forcecal 1", IF_5G);
            WIFI_TEST_CMD(wl_cmd);

            msleep(DELAY_150_TIME);  /* ��У׼���հ�����ʱ150���룬��У׼��Ч */

            OSA_SNPRINTF(wl_cmd, sizeof(wl_cmd), WL_CONFIG_EXE_PATH" %s pkteng_start %s rx", IF_5G, params->src_mac);
            WIFI_TEST_CMD(wl_cmd);
            
            g_ulRxState = AT_FEATURE_ENABLE;           
            
            wifi_get_rx_packet_report(&g_ulUcastWifiRxPkts,  &g_ulMcastWifiRxPkts);
            break;
            
        default:
            return (AT_RETURN_FAILURE);
    }
    
    memcpy(&g_wlan_at_data.wifiRX, params, sizeof(WLAN_AT_WIRX_STRU));
        
    return (AT_RETURN_SUCCESS);
}

/*****************************************************************************
 ��������  : WLAN_AT_FEATURE_TYPE WlanATSetWifiRX()
 ��������  : ��ѯ��ǰWiFi�����״̬��Ϣ
 �������  : NA
 �������  : NA
 �� �� ֵ  : 0 �رշ����
             1 �򿪷����
 ����˵��  : 
*****************************************************************************/
//////////////////////////////////////////////////////////////////////////
/*(8)^WIRX ����WiFi���ջ����� */
//////////////////////////////////////////////////////////////////////////
STATIC WLAN_AT_FEATURE_TYPE ATSetWifiRX(WLAN_AT_WIRX_STRU *params)
{
    if (WLAN_AT_2G_BAND == g_wlan_at_data.wifiBand)
    {
        return WlanATSet2GWifiRX(params);
    }
    else if (WLAN_AT_5G_BAND == g_wlan_at_data.wifiBand)
    {
        return WlanATSet5GWifiRX(params);
    }
    else
    {
        return (AT_RETURN_FAILURE);
    }
    
}
/*****************************************************************************
 ��������  : int32 WlanATGetWifiRX(WLAN_AT_WIRX_STRU *params)
 ��������  : ��ȡwifi���ջ���״̬
 �������  : NA
 �������  : NA
 �� �� ֵ  : NA
 ����˵��  : 
*****************************************************************************/
STATIC int32 ATGetWifiRX(WLAN_AT_WIRX_STRU *params)
{
    ASSERT_NULL_POINTER(params, AT_RETURN_FAILURE);

    memcpy(params, &g_wlan_at_data.wifiRX, sizeof(WLAN_AT_WIRX_STRU));
    
    return (AT_RETURN_SUCCESS);
}

//////////////////////////////////////////////////////////////////////////
/*(9)^WIRPCKG ��ѯWiFi���ջ�����룬�ϱ����յ��İ�������*/
//////////////////////////////////////////////////////////////////////////
/*****************************************************************************
 ��������  : int32 WlanATSetWifiRPCKG(int32 flag)
 ��������  : ���Wifi����ͳ�ư�Ϊ��
 �������  : 0 ���wifiͳ�ư�
             ��0 ��Ч����
 �������  : NA
 �� �� ֵ  : 0 �ɹ�
             1 ʧ��
 ����˵��  : 
*****************************************************************************/
STATIC int32 ATSetWifiRPCKG(int32 flag)
{
    ASSERT_WiFi_OFF(AT_RETURN_FAILURE);

    if (0 != flag)
    {
        PLAT_WLAN_INFO("Exit on flag = %d\n", flag);
        return (AT_RETURN_FAILURE);
    }

    wifi_get_rx_packet_report(&g_ulUcastWifiRxPkts,  &g_ulMcastWifiRxPkts);
    
    return (AT_RETURN_SUCCESS);
}

/*****************************************************************************
 ��������  : int32 WlanATSetWifiRPCKG(int32 flag)
 ��������  : ��ѯWiFi���ջ�����룬�ϱ����յ��İ�������
 �������  : WLAN_AT_WIRPCKG_STRU *params
 �������  : uint16 good_result; //������յ��ĺð�����ȡֵ��ΧΪ0~65535
             uint16 bad_result;  //������յ��Ļ�������ȡֵ��ΧΪ0~65535
 �� �� ֵ  : NA
 ����˵��  : 
*****************************************************************************/
STATIC int32 ATGetWifiRPCKG(WLAN_AT_WIRPCKG_STRU *params)
{
    unsigned int ulUcastWifiRxPkts = 0;
    unsigned int ulMcastWifiRxPkts = 0;
    unsigned int ulWifiRxPkts = 0;
    unsigned int ulWifiMcastRxPkts = 0;

    int32 ret = AT_RETURN_SUCCESS; 
    ASSERT_NULL_POINTER(params, AT_RETURN_FAILURE);

    /* �жϽ��ջ��Ƿ�� */
    if(AT_FEATURE_DISABLE == g_wlan_at_data.wifiRX.onoff)
    {
        PLAT_WLAN_INFO("Not Rx Mode.\n");
        ret = AT_RETURN_FAILURE;
        return ret;
    }   
    
    wifi_get_rx_packet_report(&ulUcastWifiRxPkts,  &ulMcastWifiRxPkts);
    ulWifiRxPkts = (ulUcastWifiRxPkts - g_ulUcastWifiRxPkts);
    ulWifiMcastRxPkts = (ulMcastWifiRxPkts - g_ulMcastWifiRxPkts);
    if(ulWifiMcastRxPkts > ulWifiRxPkts)
    {
        ulWifiRxPkts = ulWifiMcastRxPkts;
    }
   
    params->good_result = ulWifiRxPkts;
    params->bad_result = 0;   
    
    PLAT_WLAN_INFO("Exit [good = %d, bad = %d]\n", params->good_result, params->bad_result);   
    
    return (AT_RETURN_SUCCESS);
}

//////////////////////////////////////////////////////////////////////////
/*(10)^WIINFO ��ѯWiFi�������Ϣ*/
//////////////////////////////////////////////////////////////////////////
#define SIZE_OF_INFOGROUP(group) (sizeof(group) / sizeof(WLAN_AT_WIINFO_MEMBER_STRU))
/*****************************************************************************
 ��������  : uint32 ATGetWifiInfo(WLAN_AT_WIINFO_STRU *params)
 ��������  : ��ѯWiFi�������Ϣ(�ڲ��ӿ�)
 �������  : NA
 �������  : NA
 �� �� ֵ  : NA
 ����˵��  : 
*****************************************************************************/
STATIC int32 ATGetWifiInfo(WLAN_AT_WIINFO_STRU *params)
{         
    static char sChannels24G[] = "3,7,11";
    static char sChannels5G[] = "36,40,44,48,52,56,60,64,100,104,108,112,116,120,124,128,132,136,140,149,153,157,161,165";   
    static char sChannels5G_ac4[] = "36,44,52,60,100,108,116,124,132,140,149,157";   
    static char sChannels5G_ac8[] = "36,52,100,116,132,149";   
    
    /*�ŵ���Ϣ*/
    static WLAN_AT_WIINFO_MEMBER_STRU sChannelGroup0[] = 
    {       
        {"b", sChannels24G},
        {"g", sChannels24G},
        {"n", sChannels24G},
        {"n4", sChannels24G}
    };
    static WLAN_AT_WIINFO_MEMBER_STRU sChannelGroup1[] = 
    {              
        {"b", sChannels24G},
        {"g", sChannels24G},
        {"n", sChannels24G},
        {"n4", sChannels24G}
    };
    static WLAN_AT_WIINFO_MEMBER_STRU sChannelGroup2[] = 
    {       
        {"a", sChannels5G},
        {"n", sChannels5G},
        {"n4", sChannels5G},
        {"ac", sChannels5G},
        {"ac4", sChannels5G_ac4},
        {"ac8", sChannels5G_ac8}
    };
    static WLAN_AT_WIINFO_MEMBER_STRU sChannelGroup3[] = 
    {               
        {"a", sChannels5G},
        {"n", sChannels5G},
        {"n4", sChannels5G},
        {"ac", sChannels5G},
        {"ac4", sChannels5G_ac4},
        {"ac8", sChannels5G_ac8}
    };
    /*������Ϣ*/
    static WLAN_AT_WIINFO_MEMBER_STRU sPowerGroup0[] =
    {      
        {"b", "160"},
        {"g", "160"},
        {"n", "160"},
		{"n4","160"}
    };
    static WLAN_AT_WIINFO_MEMBER_STRU sPowerGroup1[] =
    {            
        {"b", "160"},
        {"g", "160"},
        {"n", "160"},
		{"n4","160"}
    };
    static  WLAN_AT_WIINFO_MEMBER_STRU sPowerGroup2[] =
    {             
        {"a",   "170"},
        {"n",   "170"},
        {"n4",  "170"},
        {"ac",  "160"},
        {"ac4", "160"},
        {"ac8", "160"}
    };
    static WLAN_AT_WIINFO_MEMBER_STRU sPowerGroup3[] =
    {     
        {"a",   "170"},
        {"n",   "170"},
        {"n4",  "170"},
        {"ac",  "160"},
        {"ac4", "160"},
        {"ac8", "160"}
    };
    /*Ƶ����Ϣ:0��ʾ2.4G,1��ʾ5G*/  
    static WLAN_AT_WIINFO_MEMBER_STRU sFreqGroup0[] =
    {         
        {"b", "0"},
        {"g", "0"},
        {"n", "0"},
		{"n4", "0"}
    };
    static WLAN_AT_WIINFO_MEMBER_STRU sFreqGroup1[] =
    {               
        {"b", "0"},
        {"g", "0"},
        {"n", "0"},
		{"n4", "0"}
    };
    static WLAN_AT_WIINFO_MEMBER_STRU sFreqGroup2[] =
    {              
        {"a",   "1"},
        {"n",   "1"},
        {"n4",  "1"},
        {"ac",  "1"},
        {"ac4", "1"},
        {"ac8", "1"}    
    };
    static WLAN_AT_WIINFO_MEMBER_STRU sFreqGroup3[] =
    {         
        {"a",   "1"},
        {"n",   "1"},
        {"n4",  "1"},
        {"ac",  "1"},
        {"ac4", "1"},
        {"ac8", "1"}    
    };

    static WLAN_AT_WIINFO_GROUP_STRU sInfoGroup0[] =
    {
        {sChannelGroup0, SIZE_OF_INFOGROUP(sChannelGroup0)},
        {sPowerGroup0, SIZE_OF_INFOGROUP(sPowerGroup0)},
        {sFreqGroup0, SIZE_OF_INFOGROUP(sFreqGroup0)}      
    };   

    static WLAN_AT_WIINFO_GROUP_STRU sInfoGroup1[] =
    {
        {sChannelGroup1, SIZE_OF_INFOGROUP(sChannelGroup1)},
        {sPowerGroup1, SIZE_OF_INFOGROUP(sPowerGroup1)},
        {sFreqGroup1, SIZE_OF_INFOGROUP(sFreqGroup1)}
  
    };

    static WLAN_AT_WIINFO_GROUP_STRU sInfoGroup2[] =
    {
        {sChannelGroup2, SIZE_OF_INFOGROUP(sChannelGroup2)},
        {sPowerGroup2, SIZE_OF_INFOGROUP(sPowerGroup2)},
        {sFreqGroup2, SIZE_OF_INFOGROUP(sFreqGroup2)}
   
    };

    static WLAN_AT_WIINFO_GROUP_STRU sInfoGroup3[] =
    {
        {sChannelGroup3, SIZE_OF_INFOGROUP(sChannelGroup3)},
        {sPowerGroup3, SIZE_OF_INFOGROUP(sPowerGroup3)},
        {sFreqGroup3, SIZE_OF_INFOGROUP(sFreqGroup3)}
    };
    
    static WLAN_AT_WIINFO_GROUP_STRU *sTotalInfoGroups[] =
    {
        sInfoGroup0,
        sInfoGroup1,
        sInfoGroup2,
        sInfoGroup3  
    };
   
    char *strBuf = NULL;
    int32 idx = 0, iLen = 0, igroup = 0,itype = 0, iTmp = 0;
    WLAN_AT_WIINFO_GROUP_STRU *pstuInfoGrup = NULL;
    WLAN_AT_WIINFO_GROUP_STRU *pstuInfoType = NULL;
    
    ASSERT_NULL_POINTER(params, AT_RETURN_FAILURE);
    PLAT_WLAN_INFO("Enter ATGetWifiInfo [group=%d,type=%u]\n", params->member.group, params->type);

    igroup = (int32)params->member.group;
    if (WLAN_AT_GROUP_MAX < igroup)
    {
        return (AT_RETURN_FAILURE);
    }   

    itype = (int32)params->type;
    if (WLAN_AT_TYPE_MAX < itype)
    {
        return (AT_RETURN_FAILURE);
    }      
   
    strBuf = (char *)params->member.content;
    iLen = (int32)(sizeof(params->member.content) - 1);

    pstuInfoGrup = sTotalInfoGroups[igroup];

    pstuInfoType = &pstuInfoGrup[itype]; 
    for(idx = 0; idx < pstuInfoType->size; idx++)
    {
        if (NULL == pstuInfoType->member[idx].name
          || NULL == pstuInfoType->member[idx].value)
        {
            continue;
        }
    
        OSA_SNPRINTF(strBuf, iLen, "%s,%s"
                    , pstuInfoType->member[idx].name
                    , pstuInfoType->member[idx].value);
        
        iTmp = (int32)(strlen(strBuf) + 1);
        iLen -= iTmp;
        strBuf += iTmp;
        if (iLen <= 0)
        {
            return (AT_RETURN_FAILURE);
        }
    }   
   
    *strBuf = '\0';
    return (AT_RETURN_SUCCESS);
}

//////////////////////////////////////////////////////////////////////////
/*(11)^WIPLATFORM ��ѯWiFi����ƽ̨��Ӧ����Ϣ */
//////////////////////////////////////////////////////////////////////////
/*****************************************************************************
 ��������  : WLAN_AT_WIPLATFORM_TYPE WlanATGetWifiPlatform()
 ��������  : ��ѯWiFi����ƽ̨��Ӧ����Ϣ
 �������  : NA
 �������  : NA
 �� �� ֵ  : NA
 ����˵��  : 
*****************************************************************************/
STATIC WLAN_AT_WIPLATFORM_TYPE ATGetWifiPlatform(void)
{
    return (AT_WIPLATFORM_BROADCOM);
}


STATIC void  AT_InitWiFiBCM2G(void)
{
    int8 wl_cmd[WIFI_CMD_MAX_SIZE] = {0};
    
    memset(wl_cmd, 0, WIFI_CMD_MAX_SIZE);
    OSA_SNPRINTF(wl_cmd, WIFI_CMD_MAX_SIZE, WL_CONFIG_EXE_PATH" %s down", IF_2G);
    WIFI_TEST_CMD(wl_cmd);

    memset(wl_cmd, 0, WIFI_CMD_MAX_SIZE);
    OSA_SNPRINTF(wl_cmd, WIFI_CMD_MAX_SIZE, WL_CONFIG_EXE_PATH" %s country ALL", IF_2G);
    WIFI_TEST_CMD(wl_cmd);

    memset(wl_cmd, 0, WIFI_CMD_MAX_SIZE);
    OSA_SNPRINTF(wl_cmd, WIFI_CMD_MAX_SIZE, WL_CONFIG_EXE_PATH" %s mpc 0", IF_2G);
    WIFI_TEST_CMD(wl_cmd);

    memset(wl_cmd, 0, WIFI_CMD_MAX_SIZE);
    OSA_SNPRINTF(wl_cmd, WIFI_CMD_MAX_SIZE, WL_CONFIG_EXE_PATH" %s interference 0", IF_2G);
    WIFI_TEST_CMD(wl_cmd);

    memset(wl_cmd, 0, WIFI_CMD_MAX_SIZE);
    OSA_SNPRINTF(wl_cmd, WIFI_CMD_MAX_SIZE, WL_CONFIG_EXE_PATH" %s phy_watchdog 0", IF_2G);
    WIFI_TEST_CMD(wl_cmd);

    memset(wl_cmd, 0, WIFI_CMD_MAX_SIZE);
    OSA_SNPRINTF(wl_cmd, WIFI_CMD_MAX_SIZE, WL_CONFIG_EXE_PATH" %s scansuppress 1", IF_2G);
    WIFI_TEST_CMD(wl_cmd);

    memset(wl_cmd, 0, WIFI_CMD_MAX_SIZE);
    OSA_SNPRINTF(wl_cmd, WIFI_CMD_MAX_SIZE, WL_CONFIG_EXE_PATH" %s frameburst 0", IF_2G);
    WIFI_TEST_CMD(wl_cmd);

    memset(wl_cmd, 0, WIFI_CMD_MAX_SIZE);
    OSA_SNPRINTF(wl_cmd, WIFI_CMD_MAX_SIZE, WL_CONFIG_EXE_PATH" %s ampdu 0", IF_2G);
    WIFI_TEST_CMD(wl_cmd);

    memset(wl_cmd, 0, WIFI_CMD_MAX_SIZE);
    OSA_SNPRINTF(wl_cmd, WIFI_CMD_MAX_SIZE, WL_CONFIG_EXE_PATH" %s mimo_bw_cap 1", IF_2G);
    WIFI_TEST_CMD(wl_cmd);

    memset(wl_cmd, 0, WIFI_CMD_MAX_SIZE);
    OSA_SNPRINTF(wl_cmd, WIFI_CMD_MAX_SIZE, WL_CONFIG_EXE_PATH" %s bi 65535", IF_2G);
    WIFI_TEST_CMD(wl_cmd);

    memset(wl_cmd, 0, WIFI_CMD_MAX_SIZE);
    OSA_SNPRINTF(wl_cmd, WIFI_CMD_MAX_SIZE, WL_CONFIG_EXE_PATH" %s up", IF_2G);
    WIFI_TEST_CMD(wl_cmd);

    if (0 == g_ul2GUP)
    {
        memset(wl_cmd, 0, WIFI_CMD_MAX_SIZE);
        OSA_SNPRINTF(wl_cmd, WIFI_CMD_MAX_SIZE, "/system/bin/ifconfig wl1 up");
        WIFI_SHELL_CMD(wl_cmd);

        g_ul2GUP = 1;
    }
}


STATIC void  AT_InitWiFiBCM5G(void)
{
    int8 wl_cmd[WIFI_CMD_MAX_SIZE] = {0};
        
    memset(wl_cmd, 0, WIFI_CMD_MAX_SIZE);
    OSA_SNPRINTF(wl_cmd, WIFI_CMD_MAX_SIZE, WL_CONFIG_EXE_PATH" %s down", IF_5G);
    WIFI_TEST_CMD(wl_cmd);

    memset(wl_cmd, 0, WIFI_CMD_MAX_SIZE);
    OSA_SNPRINTF(wl_cmd, WIFI_CMD_MAX_SIZE, WL_CONFIG_EXE_PATH" %s mpc 0", IF_5G);
    WIFI_TEST_CMD(wl_cmd);

    memset(wl_cmd, 0, WIFI_CMD_MAX_SIZE);
    OSA_SNPRINTF(wl_cmd, WIFI_CMD_MAX_SIZE, WL_CONFIG_EXE_PATH" %s country ALL", IF_5G);
    WIFI_TEST_CMD(wl_cmd);

    memset(wl_cmd, 0, WIFI_CMD_MAX_SIZE);
    OSA_SNPRINTF(wl_cmd, WIFI_CMD_MAX_SIZE, WL_CONFIG_EXE_PATH" %s wsec 0", IF_5G);
    WIFI_TEST_CMD(wl_cmd);

    memset(wl_cmd, 0, WIFI_CMD_MAX_SIZE);
    OSA_SNPRINTF(wl_cmd, WIFI_CMD_MAX_SIZE, WL_CONFIG_EXE_PATH" %s stbc_tx 0", IF_5G);
    WIFI_TEST_CMD(wl_cmd);

    memset(wl_cmd, 0, WIFI_CMD_MAX_SIZE);
    OSA_SNPRINTF(wl_cmd, WIFI_CMD_MAX_SIZE, WL_CONFIG_EXE_PATH" %s stbc_rx 1", IF_5G);
    WIFI_TEST_CMD(wl_cmd);

    memset(wl_cmd, 0, WIFI_CMD_MAX_SIZE);
    OSA_SNPRINTF(wl_cmd, WIFI_CMD_MAX_SIZE, WL_CONFIG_EXE_PATH" %s scansuppress 1", IF_5G);
    WIFI_TEST_CMD(wl_cmd);

    memset(wl_cmd, 0, WIFI_CMD_MAX_SIZE);
    OSA_SNPRINTF(wl_cmd, WIFI_CMD_MAX_SIZE, WL_CONFIG_EXE_PATH" %s txbf 0", IF_5G);
    WIFI_TEST_CMD(wl_cmd);

    memset(wl_cmd, 0, WIFI_CMD_MAX_SIZE);
    OSA_SNPRINTF(wl_cmd, WIFI_CMD_MAX_SIZE, WL_CONFIG_EXE_PATH" %s spect 0", IF_5G);
    WIFI_TEST_CMD(wl_cmd);

    memset(wl_cmd, 0, WIFI_CMD_MAX_SIZE);
    OSA_SNPRINTF(wl_cmd, WIFI_CMD_MAX_SIZE, WL_CONFIG_EXE_PATH" %s bw_cap 5g 7", IF_5G);
    WIFI_TEST_CMD(wl_cmd);

    memset(wl_cmd, 0, WL_CMD_BUF_LEN);
    OSA_SNPRINTF(wl_cmd, WIFI_CMD_MAX_SIZE, WL_CONFIG_EXE_PATH" %s mbss 0", IF_5G);
    WIFI_TEST_CMD(wl_cmd);

    memset(wl_cmd, 0, WIFI_CMD_MAX_SIZE);
    OSA_SNPRINTF(wl_cmd, WIFI_CMD_MAX_SIZE, WL_CONFIG_EXE_PATH" %s frameburst 1", IF_5G);
    WIFI_TEST_CMD(wl_cmd);

    memset(wl_cmd, 0, WIFI_CMD_MAX_SIZE);
    OSA_SNPRINTF(wl_cmd, WIFI_CMD_MAX_SIZE, WL_CONFIG_EXE_PATH" %s ampdu 1", IF_5G);
    WIFI_TEST_CMD(wl_cmd);

    memset(wl_cmd, 0, WIFI_CMD_MAX_SIZE);
    OSA_SNPRINTF(wl_cmd, WIFI_CMD_MAX_SIZE, WL_CONFIG_EXE_PATH" %s bi 65535", IF_5G);
    WIFI_TEST_CMD(wl_cmd);

    memset(wl_cmd, 0, WIFI_CMD_MAX_SIZE);
    OSA_SNPRINTF(wl_cmd, WIFI_CMD_MAX_SIZE, WL_CONFIG_EXE_PATH" %s up", IF_5G);
    WIFI_TEST_CMD(wl_cmd);

    if (0 == g_ul5GUP)
    {
        memset(wl_cmd, 0, WIFI_CMD_MAX_SIZE);
        OSA_SNPRINTF(wl_cmd, WIFI_CMD_MAX_SIZE, "/system/bin/ifconfig wl0 up");
        WIFI_SHELL_CMD(wl_cmd);

        g_ul5GUP = 1;
    }
}

//////////////////////////////////////////////////////////////////////////
/*(12)^TSELRF ��ѯ���õ����WiFi��Ƶͨ·*/
//////////////////////////////////////////////////////////////////////////
/*****************************************************************************
 ��������  : int32 WlanATSetTSELRF(uint32 group)
 ��������  : �������ߣ��Ƕ�ͨ·��0
 �������  : NA
 �������  : NA
 �� �� ֵ  : NA
 ����˵��  : 
*****************************************************************************/
STATIC int32 ATGetTSELRF(void) /* ��ȡ���� */
{
    return g_wlan_at_data.wifiGroup;
}

//////////////////////////////////////////////////////////////////////////
/*(12)^TSELRF ��ѯ���õ����WiFi��Ƶͨ·*/
//////////////////////////////////////////////////////////////////////////
/*****************************************************************************
 ��������  : int32 WlanATSetTSELRF(uint32 group)
 ��������  : �������ߣ��Ƕ�ͨ·��0
 �������  : NA
 �������  : NA
 �� �� ֵ  : NA
 ����˵��  : 
*****************************************************************************/
STATIC int32 ATGetTSELRFSupport(WLAN_AT_BUFFER_STRU *strBuf)
{
    ASSERT_NULL_POINTER(strBuf, AT_RETURN_FAILURE);

    OSA_SNPRINTF(strBuf->buf, sizeof(strBuf->buf), "%s", WLAN_AT_TSELRF_SUPPORT);
    return (AT_RETURN_SUCCESS);
}

//////////////////////////////////////////////////////////////////////////
/*(12)^TSELRF ��ѯ���õ����WiFi��Ƶͨ·*/
//////////////////////////////////////////////////////////////////////////
/*****************************************************************************
 ��������  : int32 WlanATSetTSELRF(uint32 group)
 ��������  : �������ߣ��Ƕ�ͨ·��0
 �������  : NA
 �������  : NA
 �� �� ֵ  : NA
 ����˵��  : 
*****************************************************************************/
STATIC int32 ATSetTSELRF(uint32 group)
{
    int8 acCmd[WIFI_CMD_MAX_SIZE] = {0};
     
     if(WLAN_AT_GROUP_MAX < group)
     {
         return AT_RETURN_FAILURE;
     }
     
     g_wlan_at_data.wifiGroup = group;
     g_ulWifiRF = group;

     if (g_wlan_at_data.wifiGroup == RF_2G_0 || g_wlan_at_data.wifiGroup == RF_2G_1
      || g_wlan_at_data.wifiGroup == RF_2G_MIMO)
     {
         g_wlan_at_data.wifiBand = WLAN_AT_2G_BAND;
     }
     else if (g_wlan_at_data.wifiGroup == RF_5G_0 || g_wlan_at_data.wifiGroup == RF_5G_1
      || g_wlan_at_data.wifiGroup == RF_5G_MIMO)
     {
         g_wlan_at_data.wifiBand = WLAN_AT_5G_BAND;
     }
     else
     {
         PLAT_WLAN_INFO("Set incorrect TSELRF group %d\n", g_ulWifiRF);
         return AT_RETURN_FAILURE;
     }
    
     PLAT_WLAN_INFO("[%s]:Enter,group = %u\n", __FUNCTION__, group); 
    
     if ( g_wlan_at_data.wifiBand == WLAN_AT_2G_BAND )
     {
         AT_InitWiFiBCM2G();
     }
     else
     {
         AT_InitWiFiBCM5G();
     }
     
     return (AT_RETURN_SUCCESS);

}

//////////////////////////////////////////////////////////////////////////
/*(13)^WiPARANGE���á���ȡWiFi PA���������*/
//////////////////////////////////////////////////////////////////////////
/*****************************************************************************
 ��������  : int wifi_set_pa_mode(int wifiPaMode)
 ��������  : ����WiFi PA���������
 �������  : ����ģʽ
 �������  : NA
 �� �� ֵ  : NA
 ����˵��  : Ŀǰ��ֻ֧��NO PAģʽ
*****************************************************************************/
STATIC int wifi_set_pa_mode(int wifiPaMode)
{
    if (AT_WIFI_MODE_ONLY_PA == wifiPaMode)
    {
        return AT_RETURN_SUCCESS;
    }
    else
    {
        return AT_RETURN_FAILURE;
    }
}

/*****************************************************************************
 ��������  : int wifi_set_pa_mode(int wifiPaMode)
 ��������  : ��ȡ֧�ֵ�WiFi PA���������
 �������  : NA
 �������  : NA
 �� �� ֵ  : NA
 ����˵��  : Ŀǰ��ֻ֧��NO PAģʽ
*****************************************************************************/
STATIC int wifi_get_pa_mode(void)
{
    return AT_WIFI_MODE_ONLY_PA;
}
/*****************************************************************************
 ��������  : int32 WlanATSetWifiParange(WLAN_AT_WiPARANGE_TYPE pa_type)
 ��������  : ����WiFi PA���������
 �������  : NA
 �������  : NA
 �� �� ֵ  : NA
 ����˵��  : 
*****************************************************************************/
STATIC int32 ATSetWifiParange(WLAN_AT_WiPARANGE_TYPE pa_type)
{
    int32 ret = AT_RETURN_FAILURE;
    switch (pa_type)
    {
        case AT_WiPARANGE_LOW:
            ret = wifi_set_pa_mode(AT_WIFI_MODE_ONLY_NOPA);
            break;
        case AT_WiPARANGE_HIGH:
            ret = wifi_set_pa_mode(AT_WIFI_MODE_ONLY_PA);
            break;
        default:
            break;
    }
    return ret;
}

/*****************************************************************************
 ��������  : WLAN_AT_WiPARANGE_TYPE WlanATGetWifiParange()
 ��������  : ��ȡWiFi PA���������
 �������  : NA
 �������  : NA
 �� �� ֵ  : NA
 ����˵��  : 
*****************************************************************************/
STATIC WLAN_AT_WiPARANGE_TYPE ATGetWifiParange(void)
{
    WLAN_AT_WiPARANGE_TYPE lWifiPAMode = AT_WiPARANGE_BUTT;
    switch (wifi_get_pa_mode())
    {
        case AT_WIFI_MODE_ONLY_NOPA:
            lWifiPAMode = AT_WiPARANGE_LOW;
            break;
        case AT_WIFI_MODE_ONLY_PA:
            lWifiPAMode = AT_WiPARANGE_HIGH;
            break;
        default:
            break;
    }
    return lWifiPAMode;
}

/*****************************************************************************
 ��������  : int32 WlanATGetWifiParangeSupport(WLAN_AT_BUFFER_STRU *strBuf)
 ��������  : ֧�ֵ�paģʽ���У����ַ�����ʽ����eg: l,h
 �������  : NA
 �������  : NA
 �� �� ֵ  : NA
 ����˵��  : 
*****************************************************************************/
STATIC int32 ATGetWifiParangeSupport(WLAN_AT_BUFFER_STRU *strBuf)
{
    if (NULL == strBuf)
    {
        return (AT_RETURN_FAILURE);
    }
    switch (wifi_get_pa_mode())
    {
        case AT_WIFI_MODE_ONLY_NOPA:
            OSA_SNPRINTF(strBuf->buf, sizeof(strBuf->buf), "%c", AT_WiPARANGE_LOW);
            break;
        case AT_WIFI_MODE_ONLY_PA:
            OSA_SNPRINTF(strBuf->buf, sizeof(strBuf->buf), "%c", AT_WiPARANGE_HIGH);
            break;
        case AT_WIFI_MODE_PA_NOPA:
            OSA_SNPRINTF(strBuf->buf, sizeof(strBuf->buf), "%c, %c"
                                                 , AT_WiPARANGE_LOW ,AT_WiPARANGE_HIGH);
            break;
        default:
            return (AT_RETURN_FAILURE);
    }
    
    return (AT_RETURN_SUCCESS);
}

/*****************************************************************************
 ��������  : int32 WlanATGetWifi2GPavars(WLAN_AT_BUFFER_STRU *strBuf)
 ��������  : 2.4G��Ƶ������ѯ��װ����
 �������  : NA
 �������  : NA
 �� �� ֵ  : NA
 ����˵��  : 
*****************************************************************************/
STATIC int32 ATGetWifi2GPavars(WLAN_AT_BUFFER_STRU *strBuf)
{
    int8 wl_cmd[WIFI_CMD_MAX_SIZE] = {0};
    int8 filebuff[WL_COUNTER_BUF_LEN + 1] = {0};
    int8 foramtdata2g[2][8] ={"pa2ga0=","pa2ga1="};
    int8 realdata2g[6][9]   ={"pa2gw0a0","pa2gw1a0","pa2gw2a0","pa2gw0a1","pa2gw1a1","pa2gw2a1"};
    int32 i,j;
    char *temp = NULL;
    if (NULL == strBuf)
    {
        return (AT_RETURN_FAILURE);
    }
           
    memset(wl_cmd, 0, WIFI_CMD_MAX_SIZE);
    OSA_SNPRINTF(wl_cmd, WIFI_CMD_MAX_SIZE, WL_CONFIG_EXE_PATH" %s pavars > /var/tmpfile", IF_2G);
    WIFI_SHELL_CMD(wl_cmd);
 
    /*pavars����ִ�н��*/
    if(0 == wlan_read_file("/var/tmpfile", filebuff, sizeof(filebuff)))
    {
        PLAT_WLAN_ERR("read_from_file fail");
        return AT_RETURN_FAILURE;
    }

    /*��װ2.4g����ֵ����5g��ʾ��ʽһ��*/
    for ( i = 0, j=0 ; i < 2 ; i++ )
    {
        strcat(strBuf->buf, foramtdata2g[i]);

        temp = strstr(filebuff, realdata2g[j++]);
        temp = temp + 9;
        strncat(strBuf->buf, temp, 6);
        strcat(strBuf->buf, ",");

        temp = strstr(filebuff, realdata2g[j++]);
        temp = temp + 9;
        strncat(strBuf->buf, temp, 6);
        strcat(strBuf->buf, ",");

        temp =strstr(filebuff, realdata2g[j++]);
        temp = temp + 9;
        strncat(strBuf->buf, temp, 6);
        strcat(strBuf->buf, "\n");
    }
    
    return (AT_RETURN_SUCCESS);
}

/*****************************************************************************
 ��������  : int32 ATGetInitWifi2GPavars(WLAN_AT_BUFFER_STRU *strBuf)
 ��������  : 2.4G��Ƶ������ѯ��װ����
 �������  : WLAN_AT_BUFFER_STRU *strBuf AT������ַ���
 �������  : NA
 �� �� ֵ  : NA
 ����˵��  : 
*****************************************************************************/
STATIC int32 ATGetInitWifi2GPavars(WLAN_AT_BUFFER_STRU *strBuf)
{
    int8 filebuff[WL_COUNTER_BUF_LEN + 1] = {0};
    int8 foramtdata2g[WLAN_AT_NUM_2][WLAN_AT_NUM_8] = {WLAN_AT_PA2GA0, WLAN_AT_PA2GA1};
    int8 realdata2g[WLAN_AT_NUM_6][WLAN_AT_NUM_12]  = {WLAN_AT_PA2GW0A0, WLAN_AT_PA2GW1A0, WLAN_AT_PA2GW2A0, WLAN_AT_PA2GW0A1, WLAN_AT_PA2GW1A1, WLAN_AT_PA2GW2A1};
    int32 i,j;
    char *temp = NULL;

    if (NULL == strBuf)
    {
        return (AT_RETURN_FAILURE);
    }

    if (true == bl2GPavars)
    {
        memcpy(strBuf->buf, Pa2GVarRecord, WLAN_AT_BUFFER_SIZE);
        return AT_RETURN_SUCCESS;
    }
    
    /* ��ȡnvm�еĳ�ʼֵ */
    if(0 == wlan_read_file(INITIAL_PAVAR_NVM, filebuff, sizeof(filebuff)))
    {
        PLAT_WLAN_ERR("read_from_file fail");
        return AT_RETURN_FAILURE;
    }

    /* ��װ2.4g����ֵ����5g��ʾ��ʽһ�� */
    for ( i = 0, j=0 ; i < WLAN_AT_NUM_2 ; i++ )
    {
        strcat(strBuf->buf, foramtdata2g[i]);

        temp = strstr(filebuff, realdata2g[j++]);
        temp = temp + WLAN_AT_NUM_11;
        strncat(strBuf->buf, temp, WLAN_AT_NUM_6);
        strncat(strBuf->buf, ",", WLAN_AT_NUM_1);

        temp = strstr(filebuff, realdata2g[j++]);
        temp = temp + WLAN_AT_NUM_11;
        strncat(strBuf->buf, temp, WLAN_AT_NUM_6);
        strncat(strBuf->buf, ",", WLAN_AT_NUM_1);

        temp =strstr(filebuff, realdata2g[j++]);
        temp = temp + WLAN_AT_NUM_11;
        strncat(strBuf->buf, temp, WLAN_AT_NUM_6);
        strncat(strBuf->buf, "\n", WLAN_AT_NUM_1);
    }

    strBuf->buf[WLAN_AT_BUFFER_SIZE-1] = '\0';
    bl2GPavars = true;
    memcpy(Pa2GVarRecord, strBuf->buf, WLAN_AT_BUFFER_SIZE);
    
    return (AT_RETURN_SUCCESS);
}

/*****************************************************************************
 ��������  : int32 WlanATSetWifi2GPavars(WLAN_AT_PAVARS2G_STRU *strBuf)
 ��������  : 2.4G��Ƶ�������÷�װ����
 �������  : NA
 �������  : NA
 �� �� ֵ  : NA
 ����˵��  : 
*****************************************************************************/
STATIC int32 ATSetWifi2GPavars(WLAN_AT_PAVARS2G_STRU *strBuf)
{
    int8 wl_cmd[WIFI_CMD_MAX_SIZE] = {0};
    int8 wl_temp[WIFI_CMD_MAX_SIZE] = {0};
    int32 i;
    if (NULL == strBuf)
    {
        return (AT_RETURN_FAILURE);
    }
           
    memset(wl_cmd, 0, WIFI_CMD_MAX_SIZE);
    OSA_SNPRINTF(wl_cmd, WIFI_CMD_MAX_SIZE, WL_CONFIG_EXE_PATH"%s down", IF_2G);
    WIFI_SHELL_CMD(wl_cmd);

    memset(wl_cmd, 0, WIFI_CMD_MAX_SIZE);
    OSA_SNPRINTF(wl_cmd, WIFI_CMD_MAX_SIZE, WL_CONFIG_EXE_PATH"%s pavars ", IF_2G);
    for(i =0 ; i<3; i++)
    {
        OSA_SNPRINTF(wl_temp, WIFI_CMD_MAX_SIZE, "pa2gw%da%d=%s ", i, strBuf->ant_index, strBuf->data[i]);
        strcat(wl_cmd, wl_temp);   
    }
    WIFI_SHELL_CMD(wl_cmd);

    memset(wl_cmd, 0, WIFI_CMD_MAX_SIZE);
    OSA_SNPRINTF(wl_cmd, WIFI_CMD_MAX_SIZE, WL_CONFIG_EXE_PATH" %s up", IF_2G);
    WIFI_SHELL_CMD(wl_cmd);

    return (AT_RETURN_SUCCESS);
}
/*****************************************************************************
 ��������  : int32 WlanATGetWifi5GPavars(WLAN_AT_BUFFER_STRU *strBuf)
 ��������  : 5G��Ƶ������ѯ��װ����
 �������  : NA
 �������  : NA
 �� �� ֵ  : NA
 ����˵��  : 
*****************************************************************************/
STATIC int32 ATGetWifi5GPavars(WLAN_AT_BUFFER_STRU *strBuf)
{
    int8 wl_cmd[WIFI_CMD_MAX_SIZE] = {0};
    int8 filebuff[WL_COUNTER_BUF_LEN + 1] = {0};
    int8* temp = NULL;
    if (NULL == strBuf)
    {
        return (AT_RETURN_FAILURE);
    }
           
    memset(wl_cmd, 0, WIFI_CMD_MAX_SIZE);
    OSA_SNPRINTF(wl_cmd, WIFI_CMD_MAX_SIZE, WL_CONFIG_EXE_PATH" %s pavars > /var/tmpfile", IF_5G);
    WIFI_SHELL_CMD(wl_cmd);
 
    /*pavars����ִ�н��*/
    if(0 == wlan_read_file("/var/tmpfile", filebuff, sizeof(filebuff)))
    {
        PLAT_WLAN_ERR("read_from_file fail");
        return AT_RETURN_FAILURE;
    }

    /*��ȡ��Ҫ��5g����*/
    temp = strstr(filebuff,"pa5ga0") ;
    if(WLAN_AT_BUFFER_SIZE > strlen(temp))
    {
        strncpy(strBuf->buf, temp, strlen(temp));
    }
    else
    {
        PLAT_WLAN_ERR("temp len is larger than WLAN_AT_BUFFER_SIZE(512)");
        return AT_RETURN_FAILURE;
    }

    return (AT_RETURN_SUCCESS);
}
/*****************************************************************************
 ��������  : int32 ATGetInitWifi5GPavars(WLAN_AT_BUFFER_STRU *strBuf)
 ��������  : 5G��Ƶ������ѯ��װ����
 �������  : WLAN_AT_BUFFER_STRU *strBuf AT������ַ���
 �������  : NA
 �� �� ֵ  : NA
 ����˵��  : 
*****************************************************************************/
STATIC int32 ATGetInitWifi5GPavars(WLAN_AT_BUFFER_STRU *strBuf)
{
    int8 filebuff[WL_COUNTER_BUF_LEN + 1] = {0};
    int8 foramtdata5g[WLAN_AT_NUM_3][WLAN_AT_NUM_10] = {WLAN_AT_PA5GA0, WLAN_AT_PA5GA1, WLAN_AT_PA5GA2};
    int8 realdata5g[WLAN_AT_NUM_3][WLAN_AT_NUM_10]   = {WLAN_AT_1PA5GA0, WLAN_AT_1PA5GA1, WLAN_AT_1PA5GA2};
    int32 i,j;
    char *temp = NULL;

    if (NULL == strBuf)
    {
        return (AT_RETURN_FAILURE);
    }

    if (true == bl5GPavars)
    {
        memcpy(strBuf->buf, Pa5GVarRecord, WLAN_AT_BUFFER_SIZE);
        return AT_RETURN_SUCCESS;
    }
    
    /* ��ȡnvm�еĳ�ʼֵ */
    if(0 == wlan_read_file(INITIAL_PAVAR_NVM, filebuff, sizeof(filebuff)))
    {
        PLAT_WLAN_ERR("read_from_file fail");
        return AT_RETURN_FAILURE;
    }

    /* ��װ5g����ֵ */
    for ( i = 0, j=0 ; i < WLAN_AT_NUM_3 ; i++ )
    {
        strncat(strBuf->buf, foramtdata5g[i], strlen(foramtdata5g[i]));

        temp = strstr(filebuff, realdata5g[j++]);
        temp = temp + WLAN_AT_NUM_9;
        strncat(strBuf->buf, temp, WLAN_AT_NUM_83);
        strncat(strBuf->buf, "\n", WLAN_AT_NUM_1);
    }

    strBuf->buf[WLAN_AT_BUFFER_SIZE-1] = '\0';
    bl5GPavars = true;
    memcpy(Pa5GVarRecord, strBuf->buf, WLAN_AT_BUFFER_SIZE);
   
    return AT_RETURN_SUCCESS;
}
/*****************************************************************************
 ��������  : int32 WlanATSetWifi5GPavars(WLAN_AT_PAVARS5G_STRU *strBuf)
 ��������  : 5G��Ƶ�������÷�װ����
 �������  : NA
 �������  : NA
 �� �� ֵ  : NA
 ����˵��  : 
*****************************************************************************/
STATIC int32 ATSetWifi5GPavars(WLAN_AT_PAVARS5G_STRU *strBuf)
{
    int8 wl_cmd[WIFI_CMD_MAX_SIZE] = {0};
    int8 wl_temp[WIFI_CMD_MAX_SIZE] = {0};
    int32 i;
    if (NULL == strBuf)
    {
        return (AT_RETURN_FAILURE);
    }
           
    memset(wl_cmd, 0, WIFI_CMD_MAX_SIZE);
    OSA_SNPRINTF(wl_cmd, WIFI_CMD_MAX_SIZE, WL_CONFIG_EXE_PATH"%s down", IF_5G);
    WIFI_SHELL_CMD(wl_cmd);

    memset(wl_cmd, 0, WIFI_CMD_MAX_SIZE);
    OSA_SNPRINTF(wl_cmd, WIFI_CMD_MAX_SIZE, WL_CONFIG_EXE_PATH"%s pavars pa5ga%d=", IF_5G, strBuf->ant_index);
    for(i =0 ; i<12; i++)
    {
        OSA_SNPRINTF(wl_temp, WIFI_CMD_MAX_SIZE, "%s,", strBuf->data[i]);
        strcat(wl_cmd, wl_temp);
    }
    wl_cmd[strlen(wl_cmd)]='\0';
    WIFI_SHELL_CMD(wl_cmd);
 
    memset(wl_cmd, 0, WIFI_CMD_MAX_SIZE);
    OSA_SNPRINTF(wl_cmd, WIFI_CMD_MAX_SIZE, WL_CONFIG_EXE_PATH" %s up", IF_5G);
    WIFI_SHELL_CMD(wl_cmd);

    return (AT_RETURN_SUCCESS);
}

    
/*****************************************************************************
 ��������  : int32 GET_VALUE_FROM_NV(viod)
 ��������  : ��ȡwifi��nv(50579,50580)�е�ֵ
 �������  : NA
 �������  : NA
 �� �� ֵ  : NA
 ����˵��  : 
*****************************************************************************/
STATIC int32 GET_VALUE_FROM_NV(unsigned short type, unsigned int len,char *buf)
{
    unsigned short nv_id = type;  
    /* ��ȡnv���� */
    if (NV_OK != NV_Read(nv_id, buf, len))
    {
        PLAT_WLAN_ERR("Error, nv read Failed! nv_id = %d, len = %d", nv_id, len);
        return WLAN_PARA_ERR;
    }   
    return WLAN_SUCCESS;
}
    
/*****************************************************************************
 ��������  : int32 Check_WIFI_CAL_NV(viod)
 ��������  : ���wifi��nv(50579,50580)�е�ֵ
 �������  : NA
 �������  : NA
 �� �� ֵ  : NA
 ����˵��  : 
*****************************************************************************/
STATIC void Check_WIFI_CAL_NV(void)
{
    int8 wl_cmd[WIFI_CMD_MAX_SIZE] = {0};
    int8 temp[WIFI_CMD_MAX_SIZE]={0};
    unsigned short nv_id_2g = NV_ID_WIFI_2G_RFCAL;
    unsigned short nv_id_5g = NV_ID_WIFI_5G_RFCAL; 
    unsigned short nv_wifi_2g_len = 0;
    unsigned short nv_wifi_5g_len = 0;
    char buf_2g[WLAN_AT_BUFFER_SIZE] = {0};
    char buf_5g[WLAN_AT_BUFFER_SIZE] =  {0};
    int i,j;
    
    nv_wifi_2g_len = sizeof(NV_WIFI_2G_RFCAL_STRU);
    nv_wifi_5g_len = sizeof(NV_WIFI_5G_RFCAL_STRU);

    if(WLAN_PARA_ERR == GET_VALUE_FROM_NV(nv_id_2g, nv_wifi_2g_len, buf_2g)
       &&WLAN_PARA_ERR == GET_VALUE_FROM_NV(nv_id_5g, nv_wifi_5g_len, buf_5g))
    {
        return WLAN_PARA_ERR ;
    }
    
    if(WLAN_SUCCESS == GET_VALUE_FROM_NV(nv_id_2g, nv_wifi_2g_len, buf_2g))
    {
         /*2.4g����У׼����*/
        for(i =0; i< ANT_2G_NUM;i++)
        {   
            if(0 != *(buf_2g+i*ANT_2G_CAL_NV_VALUE_BYTE_NUM))
            {
                memset(wl_cmd, 0, WIFI_CMD_MAX_SIZE);
                OSA_SNPRINTF(wl_cmd, WIFI_CMD_MAX_SIZE, WL_CONFIG_EXE_PATH" %s pavars ", IF_2G);
                for(j = 0; j < ANT_2G_CAL_NV_VALUE_BYTE_NUM/2; j++)
                {
                    memset(temp, 0, sizeof(temp));
                    OSA_SNPRINTF(temp, WIFI_CMD_MAX_SIZE, "pa2gw%da%d=0x%02x%02x ", j, i,*(buf_2g + i*ANT_2G_CAL_NV_VALUE_BYTE_NUM +j*2), \
                        *(buf_2g +i*ANT_2G_CAL_NV_VALUE_BYTE_NUM+j*2+1));
                    strcat(wl_cmd, temp);
                }
                WIFI_SHELL_CMD(wl_cmd);
            }
        }
    }   
    /*5gУ׼��������*/
    if(WLAN_SUCCESS == GET_VALUE_FROM_NV(nv_id_5g, nv_wifi_5g_len, buf_5g))
    {
         /*����У׼����*/
        for(i =0; i< ANT_5G_NUM;i++)
        {   
            if(0 != *(buf_5g + i*ANT_5G_CAL_NV_VALUE_BYTE_NUM ))
            {
                memset(wl_cmd, 0, WIFI_CMD_MAX_SIZE);
                OSA_SNPRINTF(wl_cmd, WIFI_CMD_MAX_SIZE, WL_CONFIG_EXE_PATH" %s pavars pa5ga%d=", IF_5G, i);
                for(j = 0; j < ANT_5G_CAL_NV_VALUE_BYTE_NUM/2; j++)
                {
                    memset(temp, 0, sizeof(temp));
                    OSA_SNPRINTF(temp, WIFI_CMD_MAX_SIZE, "0x%02x%02x,", *(buf_5g +i*ANT_5G_CAL_NV_VALUE_BYTE_NUM + j*2), \
                        *(buf_5g + i*ANT_5G_CAL_NV_VALUE_BYTE_NUM + j*2 + 1));
                    strcat(wl_cmd, temp);
                }
                WIFI_SHELL_CMD(wl_cmd);
            }
        }  
    }
    return WLAN_SUCCESS;
}

WLAN_CHIP_OPS bcm4352_ops = 
{
    .WlanATSetWifiEnable = ATSetWifiEnable,
    .WlanATGetWifiEnable = ATGetWifiEnable,
    
    .WlanATSetWifiMode   = ATSetWifiMode,
    .WlanATGetWifiMode   = ATGetWifiMode,
    .WlanATGetWifiModeSupport = ATGetWifiModeSupport,

    .WlanATSetWifiBand = ATSetWifiBand,
    .WlanATGetWifiBand = ATGetWifiBand,
    .WlanATGetWifiBandSupport = ATGetWifiBandSupport,

    .WlanATSetWifiFreq = ATSetWifiFreq,
    .WlanATGetWifiFreq = ATGetWifiFreq,

    .WlanATSetWifiDataRate = ATSetWifiDataRate,
    .WlanATGetWifiDataRate = ATGetWifiDataRate,

    .WlanATSetWifiPOW = ATSetWifiPOW,
    .WlanATGetWifiPOW = ATGetWifiPOW,

    .WlanATSetWifiTX = ATSetWifiTX,
    .WlanATGetWifiTX = ATGetWifiTX,

    .WlanATSetWifiRX = ATSetWifiRX,
    .WlanATGetWifiRX = ATGetWifiRX,

    .WlanATSetWifiRPCKG = ATSetWifiRPCKG,
    .WlanATGetWifiRPCKG = ATGetWifiRPCKG,
    
    .WlanATGetWifiInfo = ATGetWifiInfo,
    
    .WlanATGetWifiPlatform = ATGetWifiPlatform,
    
    .WlanATGetTSELRF = ATGetTSELRF,
    .WlanATSetTSELRF = ATSetTSELRF,
    .WlanATGetTSELRFSupport = ATGetTSELRFSupport,

    .WlanATSetWifiParange = ATSetWifiParange,
    .WlanATGetWifiParange = ATGetWifiParange,
    .WlanATGetWifiParangeSupport = ATGetWifiParangeSupport,
    
    .WlanATGetWifiCalTemp = NULL,
    .WlanATSetWifiCalTemp = NULL,
    .WlanATSetWifiCalData = NULL,
    .WlanATGetWifiCalData = NULL,
    .WlanATSetWifiCal = NULL,
    .WlanATGetWifiCal = NULL,
    .WlanATGetWifiCalSupport = NULL,
    .WlanATSetWifiCalFreq = NULL,
    .WlanATGetWifiCalFreq = NULL,
    .WlanATSetWifiCalPOW = NULL,
    .WlanATGetWifiCalPOW = NULL,
    .WlanATSetWifi2GPavars = ATSetWifi2GPavars,
    .WlanATGetWifi2GPavars = ATGetWifi2GPavars,
    .WlanATSetWifi5GPavars = ATSetWifi5GPavars,
    .WlanATGetWifi5GPavars = ATGetWifi5GPavars,
    .WlanATGetInitWifi2GPavars = ATGetInitWifi2GPavars,
    .WlanATGetInitWifi5GPavars = ATGetInitWifi5GPavars,
};
