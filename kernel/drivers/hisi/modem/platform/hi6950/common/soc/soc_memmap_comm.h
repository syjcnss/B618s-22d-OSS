#ifndef __SOC_MEMMAP_COMM_H__
#define __SOC_MEMMAP_COMM_H__ 
#ifdef __cplusplus
extern "C" {
#endif
#include "product_config.h"
#ifndef SZ_3M
#define SZ_3M (0x00300000)
#endif
#ifdef __KERNEL__
#include <asm-generic/sizes.h>
#else
#ifndef SZ_512
#define SZ_512 (0x00000200)
#endif
#ifndef SZ_1K
#define SZ_1K (0x00000400)
#endif
#ifndef SZ_2K
#define SZ_2K (0x00000800)
#endif
#ifndef SZ_4K
#define SZ_4K (0x00001000)
#endif
#ifndef SZ_8K
#define SZ_8K (0x00002000)
#endif
#ifndef SZ_16K
#define SZ_16K (0x00004000)
#endif
#ifndef SZ_32K
#define SZ_32K (0x00008000)
#endif
#ifndef SZ_64K
#define SZ_64K (0x00010000)
#endif
#ifndef SZ_128K
#define SZ_128K (0x00020000)
#endif
#ifndef SZ_256K
#define SZ_256K (0x00040000)
#endif
#ifndef SZ_512K
#define SZ_512K (0x00080000)
#endif
#ifndef SZ_1M
#define SZ_1M (0x00100000)
#endif
#ifndef SZ_2M
#define SZ_2M (0x00200000)
#endif
#ifndef SZ_4M
#define SZ_4M (0x00400000)
#endif
#ifndef SZ_8M
#define SZ_8M (0x00800000)
#endif
#ifndef SZ_16M
#define SZ_16M (0x01000000)
#endif
#ifndef SZ_32M
#define SZ_32M (0x02000000)
#endif
#ifndef SZ_64M
#define SZ_64M (0x04000000)
#endif
#ifndef SZ_128M
#define SZ_128M (0x08000000)
#endif
#ifndef SZ_256M
#define SZ_256M (0x10000000)
#endif
#ifndef SZ_512M
#define SZ_512M (0x20000000)
#endif
#endif
#define HI_APP_GIC_BASE_ADDR 0x3FE0000
#define HI_APP_GIC_SIZE SZ_1M
#define HI_MDM_GIC_BASE_ADDR 0x80100000
#define HI_MDM_GIC_SIZE SZ_1M
#define HI_M3TCM0_MEM_ADDR 0x10000000
#define HI_M3TCM0_MEM_SIZE SZ_64K
#define HI_M3TCM1_MEM_ADDR 0x10010000
#define HI_M3TCM1_MEM_SIZE SZ_64K
#define HI_DSP_SUBSYSTEM_BASE_ADDR 0x20000000
#define HI_DSP_SUBSYSTEM_SIZE SZ_128M
#define HI_PCIE0_DATA_BASE_ADDR 0x98000000
#define HI_PCIE0_DATA_SIZE SZ_256M
#define HI_PCIE0_CFG_BASE_ADDR 0xA8000000
#define HI_PCIE0_CFG_SIZE SZ_64K
#define HI_PCIE1_DATA_BASE_ADDR 0xA8010000
#define HI_PCIE1_DATA_SIZE SZ_256M
#define HI_PCIE1_CFG_BASE_ADDR 0xB8010000
#define HI_PCIE1_CFG_SIZE SZ_64K
#define HI_SRAM_MEM_BASE_ADDR (DRV_SRAM_ADDR)
#define HI_SRAM_MEM_SIZE (DRV_SRAM_SIZE)
#define HI_APPA9_L2_REGBASE_ADDR 0x3FF00000
#define HI_APPA9_L2_REG_SIZE SZ_1M
#define HI_MDMA9_L2_REGBASE_ADDR 0x80000000
#define HI_MDMA9_L2_REG_SIZE SZ_1M
#define HI_CS_SYS_REGBASE_ADDR 0x90400000
#define HI_CS_SYS_REG_SIZE SZ_1M
#define HI_PCIE0_PHY_BASE_ADDR 0x90080000
#define HI_PCIE0_PHY_SIZE SZ_256K
#define HI_PCIE1_PHY_BASE_ADDR 0x900C0000
#define HI_PCIE1_PHY_SIZE SZ_256K
#define HI_SYSCTRL_AO_BASE_ADDR 0x20000000
#define HI_SYSCTRL_AO_REG_SIZE SZ_4K
#define HI_SYSCTRL_AO_REG_BASE_ADDR HI_SYSCTRL_AO_BASE_ADDR
#define HI_AO_CRG_REG_BASE_ADDR HI_SYSCTRL_AO_BASE_ADDR
#define HI_SYSSC_AO_BASE_ADDR HI_SYSCTRL_AO_BASE_ADDR
#define HI_BBP_SYSTIME_BASE_ADDR HI_SYSCTRL_AO_REG_BASE_ADDR
#define HI_BBP_SYSTIME_SIZE HI_SYSCTRL_AO_REG_SIZE
#define HI_PD_CRG_BASE_ADDR 0x90000000
#define HI_SYSCTRL_PD_REG_SIZE SZ_4K
#define HI_SYSCTRL_PD_REG_BASE_ADDR HI_PD_CRG_BASE_ADDR
#define HI_SYSSC_PD_BASE_ADDR HI_PD_CRG_BASE_ADDR
#define HI_SYSCTRL_PCIE_REG_BASE_ADDR HI_PD_CRG_BASE_ADDR
#define HI_SYSCTRL_PCIE_REG_SIZE SZ_4K
#define HI_MODEM_SC_BASE_ADDR 0x80200000
#define HI_MODEM_SC_SIZE SZ_4K
#define HI_SYSCTRL_MDM_REG_BASE_ADDR HI_MODEM_SC_BASE_ADDR
#define HI_SYSSC_MDM_BASE_ADDR HI_MODEM_SC_BASE_ADDR
#define HI_WDT_BASE_ADDR 0x20001000
#define HI_WDT_REG_SIZE SZ_4K
#define HI_WDT_MDM_BASE_ADDR 0x80201000
#define HI_WDT_MDM_REG_SIZE SZ_4K
#define HI_TIMER_REG_SIZE SZ_4K
#define HI_RTC_REGBASE_ADDR 0x20005000
#define HI_RTC_REG_SIZE SZ_4K
#define HI_GPIO0_REGBASE_ADDR 0x2003F000
#define HI_GPIO0_REG_SIZE SZ_4K
#define HI_GPIO1_REGBASE_ADDR 0x20040000
#define HI_GPIO1_REG_SIZE SZ_4K
#define HI_GPIO2_REGBASE_ADDR 0x9002B000
#define HI_GPIO2_REG_SIZE SZ_4K
#define HI_GPIO3_REGBASE_ADDR 0x9002C000
#define HI_GPIO3_REG_SIZE SZ_4K
#define HI_GPIO4_REGBASE_ADDR 0x9002D000
#define HI_GPIO4_REG_SIZE SZ_4K
#define HI_GPIO5_REGBASE_ADDR 0x9002E000
#define HI_GPIO5_REG_SIZE SZ_4K
#define HI_GPIO6_REGBASE_ADDR 0x9002F000
#define HI_GPIO6_REG_SIZE SZ_4K
#define HI_GPIO7_REGBASE_ADDR 0x90030000
#define HI_GPIO7_REG_SIZE SZ_4K
#define HI_GPIO8_REGBASE_ADDR 0x90031000
#define HI_GPIO8_REG_SIZE SZ_4K
#define HI_GPIO9_REGBASE_ADDR 0x90032000
#define HI_GPIO9_REG_SIZE SZ_4K
#define HI_GPIO10_REGBASE_ADDR 0x90033000
#define HI_GPIO10_REG_SIZE SZ_4K
#define HI_GPIO11_REGBASE_ADDR 0x90034000
#define HI_GPIO11_REG_SIZE SZ_4K
#define HI_GPIO12_REGBASE_ADDR 0x90035000
#define HI_GPIO12_REG_SIZE SZ_4K
#define HI_UART0_REGBASE_ADDR 0x90024000
#define HI_UART0_REG_SIZE SZ_4K
#define HI_UART1_REGBASE_ADDR 0x90025000
#define HI_UART1_REG_SIZE SZ_4K
#define HI_UART2_REGBASE_ADDR 0x90026000
#define HI_UART2_REG_SIZE SZ_4K
#define HI_UART_MDM_REGBASE_ADDR 0x80203000
#define HI_UART_MDM_REG_SIZE SZ_4K
#define HI_SPI_MST0_REGBASE_ADDR 0x9001E000
#define HI_SPI_MST0_REG_SIZE SZ_4K
#define HI_SPI_MST1_REGBASE_ADDR 0x9001F000
#define HI_SPI_MST1_REG_SIZE SZ_4K
#define HI_SCI0_REGBASE_ADDR 0x20010000
#define HI_SCI0_REG_SIZE SZ_4K
#define HI_TSENSOR_REGBASE_ADDR 0x20012000
#define HI_TSENSOR_REG_SIZE SZ_4K
#define HI_TSENSOR_BASE_ADDR HI_TSENSOR_REGBASE_ADDR
#define HI_BC_CTRL_REGBASE_ADDR 0x20013000
#define HI_BC_CTRL_REG_SIZE SZ_4K
#define HI_PMUSSI0_REGBASE_ADDR 0x9001A000
#define HI_PMUSSI0_REG_SIZE SZ_4K
#define HI_IOS_AO_REGBASE_ADDR 0x20002000
#define HI_IOS_AO_REG_SIZE SZ_4K
#define HI_IOS_PD_REGBASE_ADDR 0x90002000
#define HI_IOS_PD_REG_SIZE SZ_4K
#define HI_I2C0_REGBASE_ADDR 0x90022000
#define HI_I2C0_REG_SIZE SZ_4K
#define HI_I2C1_REGBASE_ADDR 0x90023000
#define HI_I2C1_REG_SIZE SZ_4K
#define HI_MDDRC_REGBASE_ADDR 0x90040000
#define HI_MDDRC_REG_SIZE (SZ_8K*7)
#define HI_EFUSE_REGBASE_ADDR 0x20007000
#define HI_EFUSE_REG_SIZE SZ_4K
#define HI_EFUSE_BASE_ADDR HI_EFUSE_REGBASE_ADDR
#define HI_EDMA_REGBASE_ADDR 0x80204000
#define HI_EDMA_REG_SIZE SZ_4K
#define HI_EDMA_CH4_REGBASE_ADDR 0x90003000
#define HI_EDMA_CH4_REG_SIZE SZ_4K
#define HI_ASHB_REGBASE_ADDR 0x90005000
#define HI_ASHB_REG_SIZE SZ_4K
#define HI_ASHB_M3_MST_BASE_ADDR 0x80202000
#define HI_ASHB_M3_MST_REG_SIZE SZ_4K
#if 1
#define HI_BBP_APB_REGBASE_ADDR 0x9002C000
#define HI_BBP_APB_REG_SIZE SZ_8K
#endif
#define HI_PWM0_REGBASE_ADDR 0x90029000
#define HI_PWM0_REG_SIZE SZ_4K
#define HI_PWM1_REGBASE_ADDR 0x9002A000
#define HI_PWM1_REG_SIZE SZ_4K
#define HI_UICC_REGBASE_ADDR 0x80400000
#define HI_UICC_REG_SIZE SZ_256K
#define HI_EMI_REGBASE_ADDR 0x9102c000
#define HI_EMI_REG_SIZE SZ_4K
#define HI_EMIMEM_REGBASE_ADDR 0x9102d000
#define HI_EMIMEM_REG_SIZE SZ_4K
#define HI_NANDC_REGBASE_ADDR 0x9102B000
#define HI_NANDC_REG_SIZE SZ_4K
#define HI_IPCM_REGBASE_ADDR 0x9101E000
#define HI_IPCM_REG_SIZE SZ_4K
#define HI_LTESIO_REGBASE_ADDR 0x91033000
#define HI_LTESIO_REG_SIZE SZ_4K
#define HI_HSUART_REGBASE_ADDR 0x9102E000
#define HI_HSUART_REG_SIZE SZ_4K
#define HI_SOCP_REGBASE_ADDR 0x91000000
#define HI_SOCP_REG_SIZE SZ_4K
#define HI_CIPHER_BASE_ADDR 0x80452000
#define HI_CIPHER_REG_SIZE SZ_4K
#define HI_IPF_REGBASE_ADDR 0x91003000
#define HI_IPF_REG_SIZE SZ_4K
#define HI_PSAM_BASE_ADDR 0x91040000
#define HI_PSAM_REG_SIZE SZ_64K
#define HI_SPE_BASE_ADDR 0x91004000
#define HI_SPE_REG_SIZE SZ_16K
#define HI_CICOM0_REGBASE_ADDR 0x80440000
#define HI_CICOM0_REG_SIZE SZ_4K
#define HI_CICOM1_REGBASE_ADDR 0x80441000
#define HI_CICOM1_REG_SIZE SZ_4K
#define HI_HDLC_REGBASE_ADDR 0x91001000
#define HI_HDLC_REG_SIZE SZ_4K
#define HI_UPACC_BASE_ADDR 0x80453000
#define HI_UPACC_REG_SIZE SZ_4K
#define HI_BOOTROM_REGBASE_ADDR 0x200F0000
#define HI_BOOTROM_REG_SIZE SZ_64K
#define HI_USB3_REGBASE_ADDR 0x91100000
#define HI_USB3_REG_SIZE SZ_1M
#define HI_NAND_MEM_BUFFER_ADDR 0x93000000
#define HI_NAND_MEM_BUFFER_SIZE SZ_4M
#define HI_AMON_SOC_REGBASE_ADDR 0x96000000
#define HI_AMON_SOC_REG_SIZE SZ_16K
#define HI_AMON_CPUFAST_REGBASE_ADDR 0x80458000
#define HI_AMON_CPUFAST_REG_SIZE SZ_16K
#define HI_DSP0DMEM0_BASE_ADDR 0x82700000
#define HI_DSP0DMEM0_SIZE SZ_512K
#define HI_DSP0DMEM1_BASE_ADDR 0x82780000
#define HI_DSP0DMEM1_SIZE SZ_512K
#define HI_DSP0IMEM0_BASE_ADDR 0x82800000
#define HI_DSP0IMEM0_SIZE SZ_512K
#define HI_DSP0IMEM1_BASE_ADDR 0x82880000
#define HI_DSP0IMEM1_SIZE SZ_512K
#define HI_HIFI2DMEM0_BASE_ADDR 0x8FE00000
#define HI_HIFI2DMEM0_SIZE SZ_32K
#define HI_HIFI2DMEM1_BASE_ADDR 0x8FE08000
#define HI_HIFI2DMEM1_SIZE SZ_32K
#define HI_HIFI2IMEM0_BASE_ADDR 0x8FE10000
#define HI_HIFI2IMEM0_SIZE SZ_4K
#if 1
#define HI_HIFI2IMEM1_BASE_ADDR 0x49760000
#define HI_HIFI2IMEM1_SIZE SZ_128K
#endif
#define HI_CORESIGHT_STM_BASE_ADDR 0x90401000
#define HI_CORESIGHT_STM_SIZE SZ_4K
#define HI_CORESIGHT_AP_ETF_BASE_ADDR 0x904C2000
#define HI_CORESIGHT_AP_ETF_SIZE SZ_4K
#define HI_CORESIGHT_CP_ETF_BASE_ADDR 0x90483000
#define HI_CORESIGHT_CP_ETF_SIZE SZ_4K
#define HI_CORESIGHT_MDM_ETF_BASE_ADDR 0x90402000
#define HI_CORESIGHT_MDM_ETF_SIZE SZ_4K
#define HI_CORESIGHT_CP_FUNNEL_BASE_ADDR 0x90481000
#define HI_CORESIGHT_CP_FUNNEL_SIZE SZ_4K
#define HI_CORESIGHT_MDM_FUNNEL_BASE_ADDR 0x90404000
#define HI_CORESIGHT_MDM_FUNNEL_SIZE SZ_4K
#define HI_CORESIGHT_ETR_BASE_ADDR 0x90406000
#define HI_CORESIGHT_ETR_SIZE SZ_4K
#define HI_CORESIGHT_PTM0_BASE_ADDR 0x904FC000
#define HI_CORESIGHT_PTM0_SIZE SZ_4K
#define HI_CORESIGHT_PTM1_BASE_ADDR 0x904BC000
#define HI_CORESIGHT_PTM1_SIZE SZ_4K
#define HI_SHARED_DDR_BASE_ADDR (DDR_SHARED_MEM_ADDR)
#define HI_SHARED_DDR_SIZE (DDR_SHARED_MEM_SIZE)
#define HI_HKADCSSI_REGBASE_ADDR (0x9001C000)
#if 1
#define HI_SYSSC_GU_BASE_ADDR 0x27050000
#define HI_SYSSC_GU_BASE_SIZE SZ_4K
#define HI_ZSP_AHB_REG_BASE_ADDR 0xFFFFFFFF
#define HI_ZSP_DTCM_REG_BASE_ADDR 0xFFFFFFFF
#define HI_BBP_SRC_BASE_ADDR (0x81000000)
#define HI_BBP_SRC_SIZE SZ_1M
#define HI_BBP_DMA_BASE_ADDR (0x81200000)
#define HI_BBP_DMA_SIZE SZ_1M
#define HI_BBP_DBG_BASE_ADDR (0x81300000)
#define HI_BBP_DBG_SIZE SZ_1M
#define HI_BBP_INT_BASE_ADDR (0x81700000)
#define HI_BBP_INT_SIZE SZ_4K
#define HI_BBP_TSTU_BASE_ADDR (0x81d00000)
#define HI_BBP_TSTU_SIZE SZ_8K
#define HI_GBBP_REG_BASE_ADDR (0x81800000)
#define HI_GBBP_REG_SIZE SZ_512K
#define HI_GBBP1_REG_BASE_ADDR (0x0)
#define HI_GBBP1_REG_SIZE SZ_512K
#define HI_WBBP_REG_BASE_ADDR (0x81900000)
#define HI_WBBP_REG_REG_SIZE SZ_1M
#define HI_BBP_CDMA_BASE_ADDR (0x819E0000)
#define HI_BBP_GSDR_BASE_ADDR (0x81840000)
#define HI_BBP_CTU_BASE_ADDR (0x81f80000)
#define HI_CTU_SIZE SZ_32K
#if defined(BSP_CONFIG_SFT)
#define HI_BBP_LTEDRX_BASE_ADDR (0x81890000)
#define HI_BBP_TDSDRX_BASE_ADDR (0x81891400)
#define HI_BBP_COMM_ON_BASE_ADDR (0x81890000)
#define HI_GBBP_DRX_REG_BASE_ADDR (0x81890800)
#define HI_GBBP1_DRX_REG_BASE_ADDR (0x0)
#define HI_WBBP_DRX_REG_BASE_ADDR (0x81890000)
#define HI_BBP_CDMA_ON_BASE_ADDR (0x81890000)
#define HI_BBP_GLB_ON_BASE_ADDR (0x81890000)
#else
#define HI_BBP_LTEDRX_BASE_ADDR (0x20008000)
#define HI_BBP_TDSDRX_BASE_ADDR (0x20009400)
#define HI_BBP_COMM_ON_BASE_ADDR (0x20008000)
#define HI_GBBP_DRX_REG_BASE_ADDR (0x20008800)
#define HI_GBBP1_DRX_REG_BASE_ADDR (0x0)
#define HI_WBBP_DRX_REG_BASE_ADDR (0x20008000)
#define HI_BBP_CDMA_ON_BASE_ADDR (0x20008000)
#define HI_BBP_GLB_ON_BASE_ADDR (0x20008000)
#endif
#define HI_CTU_BASE_ADDR HI_BBP_CTU_BASE_ADDR
#define SOC_BBP_TDS_BASE_ADDR HI_BBP_TSTU_BASE_ADDR
#define HI_BBPMASTER_REG_BASE_ADDR HI_WBBP_REG_BASE_ADDR
#define HI_BBP_SYSTIME_BASE_ADDR HI_SYSCTRL_AO_REG_BASE_ADDR
#define HI_BBP_SYSTIME_SIZE HI_SYSCTRL_AO_REG_SIZE
#endif
#ifdef __cplusplus
}
#endif
#endif