/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * Copyright 2019 NXP
 * Copyright 2020-2021 Variscite Ltd.
 */

#ifndef __IMX8MP_VAR_DART_H
#define __IMX8MP_VAR_DART_H

#include <linux/sizes.h>
#include <asm/arch/imx-regs.h>

#include "imx_env.h"

#define CONFIG_SPL_MAX_SIZE				(152 * 1024)
#define CONFIG_SYS_MONITOR_LEN				(512 * 1024)
#define CONFIG_SYS_MMCSD_RAW_MODE_U_BOOT_USE_SECTOR
#define CONFIG_SYS_MMCSD_RAW_MODE_U_BOOT_SECTOR	0x300
#define CONFIG_SYS_MMCSD_FS_BOOT_PARTITION		1
#define CONFIG_SYS_UBOOT_BASE				(QSPI0_AMBA_BASE + CONFIG_SYS_MMCSD_RAW_MODE_U_BOOT_SECTOR * 512)

#ifdef CONFIG_SPL_BUILD
#define CONFIG_SPL_STACK				0x187FF0
#define CONFIG_SPL_BSS_START_ADDR			0x0095e000
#define CONFIG_SPL_BSS_MAX_SIZE				0x2000	/* 8 KB */
#define CONFIG_SYS_SPL_MALLOC_START			0x42200000
#define CONFIG_SYS_SPL_MALLOC_SIZE			SZ_512K	/* 512 KB */

#define CONFIG_MALLOC_F_ADDR				0x184000 /* malloc f used before GD_FLG_FULL_MALLOC_INIT set */

#define CONFIG_SPL_ABORT_ON_RAW_IMAGE

#define CONFIG_POWER
#define CONFIG_POWER_I2C
#define CONFIG_POWER_PCA9450

#define CONFIG_SYS_I2C

#endif

#define CONFIG_CMD_READ
#define CONFIG_SERIAL_TAG
#define CONFIG_FASTBOOT_USB_DEV		0

#define CONFIG_REMAKE_ELF

/* ENET Config */
#if defined(CONFIG_CMD_NET)
#define CONFIG_ETHPRIME			"eth0" /* Set eqos to primary since we use its MDIO */

#define CONFIG_FEC_XCV_TYPE		RGMII
#define FEC_QUIRK_ENET_MAC

#ifdef CONFIG_DWC_ETH_QOS
#define CONFIG_SYS_NONCACHED_MEMORY	(1 * SZ_1M)
#endif

#define PHY_ANEG_TIMEOUT		20000

#endif

#define CONFIG_MFG_ENV_SETTINGS \
	CONFIG_MFG_ENV_SETTINGS_DEFAULT \
	"initrd_addr=0x43800000\0" \
	"initrd_high=0xffffffffffffffff\0" \
	"emmc_dev=2\0"\
	"sd_dev=1\0" \

/* Initial environment variables */
#define CONFIG_EXTRA_ENV_SETTINGS		\
	CONFIG_MFG_ENV_SETTINGS \
	"bootdir=/boot\0"	\
	"script=boot.scr\0" \
	"image=Image.gz\0" \
	"img_addr=0x42000000\0" \
	"splashimage=0x50000000\0" \
	"console=ttymxc0,115200\0" \
	"fdt_addr=0x43000000\0"			\
	"fdt_high=0xffffffffffffffff\0"		\
	"boot_fit=no\0" \
	"fdt_file=undefined\0" \
	"ip_dyn=yes\0" \
	"initrd_addr=0x43800000\0"		\
	"initrd_high=0xffffffffffffffff\0" \
	"mmcdev="__stringify(CONFIG_SYS_MMC_ENV_DEV)"\0" \
	"mmcblk=1\0" \
	"mmcpart=1\0" \
	"mmcautodetect=yes\0" \
	"m7_addr=0x7e0000\0" \
	"m7_bin=hello_world.bin\0" \
	"use_m7=no\0" \
	"loadm7bin=load mmc ${mmcdev}:${mmcpart} ${loadaddr} ${bootdir}/${m7_bin} && " \
	"cp.b ${loadaddr} ${m7_addr} ${filesize}\0" \
	"runm7bin=" \
		"if test ${m7_addr} = 0x7e0000; then " \
			"echo Booting M7 from TCM; " \
		"else " \
			"echo Booting M7 from DRAM; " \
			"dcache flush; " \
		"fi; " \
		"bootaux ${m7_addr};\0" \
	"optargs=setenv bootargs ${bootargs} ${kernelargs};\0" \
	"mmcargs=setenv bootargs console=${console} " \
		"root=/dev/mmcblk${mmcblk}p${mmcpart} rootwait rw ${cma_size}\0 " \
	"loadbootscript=load mmc ${mmcdev}:${mmcpart} ${loadaddr} ${bootdir}/${script};\0" \
	"bootscript=echo Running bootscript from mmc ...; " \
		"source\0" \
	"loadimage=load mmc ${mmcdev}:${mmcpart} ${img_addr} ${bootdir}/${image};" \
		"unzip ${img_addr} ${loadaddr}\0" \
	"findfdt=" \
		"if test $fdt_file = undefined; then " \
			"if test $board_name = VAR-SOM-MX8M-PLUS; then " \
				"setenv fdt_file imx8mp-var-som-symphony.dtb; " \
			"else " \
				"if test $dart_carrier_rev = legacy; then " \
					"setenv fdt_file imx8mp-var-dart-dt8mcustomboard-legacy.dtb;" \
				"else " \
					"setenv fdt_file imx8mp-var-dart-dt8mcustomboard.dtb;" \
				"fi; " \
			"fi; " \
		"fi; \0" \
	"loadfdt=run findfdt; " \
		"echo fdt_file=${fdt_file}; " \
		"load mmc ${mmcdev}:${mmcpart} ${fdt_addr} ${bootdir}/${fdt_file}\0" \
	"ramsize_check="\
		"if test $sdram_size -le 512; then " \
			"setenv cma_size cma=320M; " \
		"else " \
			"if test $sdram_size -le 1024; then " \
				"setenv cma_size cma=640M; " \
			"else " \
				"setenv cma_size cma=960M; " \
			"fi; " \
		"fi;\0" \
	"mmcboot=echo Booting from mmc ...; " \
		"run mmcargs; " \
		"run optargs; " \
		"if test ${boot_fit} = yes || test ${boot_fit} = try; then " \
			"bootm ${loadaddr}; " \
		"else " \
			"if run loadfdt; then " \
				"booti ${loadaddr} - ${fdt_addr}; " \
			"else " \
				"echo WARN: Cannot load the DT; " \
			"fi; " \
		"fi;\0" \
	"netargs=setenv bootargs console=${console} " \
		"root=/dev/nfs ${cma_size} " \
		"ip=dhcp nfsroot=${serverip}:${nfsroot},v3,tcp ${cma_size}\0" \
	"netboot=echo Booting from net ...; " \
		"run netargs;  " \
		"run optargs;  " \
		"if test ${ip_dyn} = yes; then " \
			"setenv get_cmd dhcp; " \
		"else " \
			"setenv get_cmd tftp; " \
		"fi; " \
		"${get_cmd} ${img_addr} ${image}; unzip ${img_addr} ${loadaddr}; " \
		"if test ${boot_fit} = yes || test ${boot_fit} = try; then " \
			"bootm ${loadaddr}; " \
		"else " \
			"run findfdt; " \
			"echo fdt_file=${fdt_file}; " \
			"if ${get_cmd} ${fdt_addr} ${fdt_file}; then " \
				"booti ${loadaddr} - ${fdt_addr}; " \
			"else " \
				"echo WARN: Cannot load the DT; " \
			"fi; " \
		"fi;\0"

#define CONFIG_BOOTCOMMAND \
	"run ramsize_check; " \
	"mmc dev ${mmcdev}; " \
	"if mmc rescan; then " \
		   "if test ${use_m7} = yes && run loadm7bin; then " \
			"run runm7bin; " \
		   "fi; " \
		   "if run loadbootscript; then " \
			   "run bootscript; " \
		   "else " \
			   "if run loadimage; then " \
				"run mmcboot; " \
			   "else " \
				"run netboot; " \
			   "fi; " \
		   "fi; " \
	   "fi;"

/* Link Definitions */
#define CONFIG_LOADADDR			0x40480000

#define CONFIG_SYS_LOAD_ADDR		CONFIG_LOADADDR

#define CONFIG_SYS_INIT_RAM_ADDR	0x40000000
#define CONFIG_SYS_INIT_RAM_SIZE	0x80000
#define CONFIG_SYS_INIT_SP_OFFSET \
	(CONFIG_SYS_INIT_RAM_SIZE - GENERATED_GBL_DATA_SIZE)
#define CONFIG_SYS_INIT_SP_ADDR \
	(CONFIG_SYS_INIT_RAM_ADDR + CONFIG_SYS_INIT_SP_OFFSET)

#define CONFIG_ENV_OVERWRITE

#define CONFIG_SYS_MMC_ENV_DEV		1   /* USDHC2 */

/* Size of malloc() pool */
#define CONFIG_SYS_MALLOC_LEN		SZ_32M

/* DDR configs */
#define CONFIG_SYS_SDRAM_BASE		0x40000000
#define PHYS_SDRAM			0x40000000
#define DEFAULT_SDRAM_SIZE		(512 * SZ_1M)

#define CONFIG_SYS_MEMTEST_START	PHYS_SDRAM
#define CONFIG_SYS_MEMTEST_END		(CONFIG_SYS_MEMTEST_START + \
					(DEFAULT_SDRAM_SIZE >> 1))

#define CONFIG_MXC_UART_BASE		UART1_BASE_ADDR

/* Monitor Command Prompt */
#define CONFIG_SYS_PROMPT_HUSH_PS2	"> "
#define CONFIG_SYS_CBSIZE		2048
#define CONFIG_SYS_MAXARGS		64
#define CONFIG_SYS_BARGSIZE		CONFIG_SYS_CBSIZE
#define CONFIG_SYS_PBSIZE		(CONFIG_SYS_CBSIZE + \
					sizeof(CONFIG_SYS_PROMPT) + 16)

#define CONFIG_IMX_BOOTAUX
#define CONFIG_FSL_USDHC

#define CONFIG_SYS_FSL_USDHC_NUM	2
#define CONFIG_SYS_FSL_ESDHC_ADDR	0

#define CONFIG_SYS_MMC_IMG_LOAD_PART	1


#define CONFIG_SYS_I2C_SPEED		100000

/* USB configs */
#ifndef CONFIG_SPL_BUILD
#define CONFIG_CMD_USB
#define CONFIG_USB_STORAGE

#define CONFIG_CMD_USB_MASS_STORAGE
#define CONFIG_USB_GADGET_MASS_STORAGE
#define CONFIG_USB_FUNCTION_MASS_STORAGE
#endif

#define CONFIG_USB_MAX_CONTROLLER_COUNT		2
#define CONFIG_USBD_HS
#define CONFIG_USB_GADGET_VBUS_DRAW		2

#ifdef CONFIG_DM_VIDEO
#define CONFIG_VIDEO_LOGO
#define CONFIG_SPLASH_SCREEN
#define CONFIG_SPLASH_SCREEN_ALIGN
#define CONFIG_CMD_BMP
#define CONFIG_BMP_16BPP
#define CONFIG_BMP_24BPP
#define CONFIG_BMP_32BPP
#define CONFIG_VIDEO_BMP_RLE8
#define CONFIG_VIDEO_BMP_LOGO
#endif

/* EEPROM configs */
#define VAR_EEPROM_DRAM_START	(PHYS_SDRAM + (DEFAULT_SDRAM_SIZE >> 1))

#endif