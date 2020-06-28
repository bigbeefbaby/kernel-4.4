/* Copyright Statement:
 * Reversed by zoggn@HakonTI
 *****************************************************************************/

#ifndef BUILD_LK
#include <linux/string.h>
#include <linux/kernel.h>
#endif
#include "lcm_drv.h"

#ifdef BUILD_LK
#include <platform/mt_gpio.h>
#include <string.h>
#elif defined(BUILD_UBOOT)
#include <asm/arch/mt_gpio.h>
#else
//#include <mach/mt_gpio.h>
#endif


// ---------------------------------------------------------------------------
//  Local Constants
// ---------------------------------------------------------------------------

#define FRAME_WIDTH  									(720)
#define FRAME_HEIGHT 									(1280)

#define REGFLAG_DELAY               (0xFFFE)
#define REGFLAG_END_OF_TABLE        (0xFFFF) // END OF REGISTERS MARKER


#define LCM_DSI_CMD_MODE								0

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

// ---------------------------------------------------------------------------
//  Local Variables
// ---------------------------------------------------------------------------

static LCM_UTIL_FUNCS lcm_util = {0};


#define SET_RESET_PIN(v)    								(lcm_util.set_reset_pin((v)))


#define UDELAY(n) 									(lcm_util.udelay(n))
#define MDELAY(n) 									(lcm_util.mdelay(n))



// ---------------------------------------------------------------------------
//  Local Functions
// ---------------------------------------------------------------------------
#define dsi_set_cmdq_V3(para_tbl,size,force_update)        lcm_util.dsi_set_cmdq_V3(para_tbl,size,force_update)
#define dsi_set_cmdq_V2(cmd, count, ppara, force_update)    lcm_util.dsi_set_cmdq_V2(cmd, count, ppara, force_update)
#define dsi_set_cmdq(pdata, queue_size, force_update)       lcm_util.dsi_set_cmdq(pdata, queue_size, force_update)
#define wrtie_cmd(cmd)                                      lcm_util.dsi_write_cmd(cmd)
#define write_regs(addr, pdata, byte_nums)                  lcm_util.dsi_write_regs(addr, pdata, byte_nums)
#define read_reg(cmd)											lcm_util.dsi_dcs_read_lcm_reg(cmd)
#define read_reg_v2(cmd, buffer, buffer_size)               lcm_util.dsi_dcs_read_lcm_reg_v2(cmd, buffer, buffer_size)
#define set_gpio_lcd_enp(cmd) 								lcm_util.set_gpio_lcd_enp_bias(cmd)
#define set_gpio_lcd_enn(cmd) 								lcm_util.set_gpio_lcd_enn_bias(cmd)

#define   LCM_DSI_CMD_MODE							0
struct LCM_setting_table {
	unsigned int cmd;
	unsigned char count;
	unsigned char para_list[64];
};


static struct LCM_setting_table lcm_initialization_setting[] = {
    { 0xF0, 0x05, {0x55, 0xAA, 0x52, 0x08, 0x00}},
    { 0xC0, 0x08, {0xC7, 0x00, 0x00, 0x00, 0x1E, 0x10, 0x60, 0xE5}},
    { 0xC1, 0x08, {0xC0, 0x01, 0x00, 0x00, 0x1D, 0x00, 0xF0, 0xC8}},
    { 0xC2, 0x08, {0xC0, 0x02, 0x00, 0x00, 0x1D, 0x2A, 0xA0, 0x9F}},
    { 0xC3, 0x08, {0xC0, 0x02, 0x00, 0x00, 0x1E, 0x2A, 0xA0, 0x9F}},
    { 0xC4, 0x08, {0xC0, 0x02, 0x00, 0x00, 0x1D, 0x10, 0x80, 0xB8}},
    { 0xC5, 0x08, {0xC0, 0x02, 0x00, 0x00, 0x1E, 0x10, 0xA0, 0xB8}},
    { 0xC6, 0x08, {0xC7, 0x00, 0x02, 0x00, 0x1E, 0x10, 0xA0, 0xEC}},
    { 0xC7, 0x08, {0xC7, 0x00, 0x00, 0x00, 0x1F, 0x10, 0x60, 0xE5}},
    { 0xC8, 0x01, {0xFF}},
    { 0xB0, 0x05, {0x00, 0x08, 0x0C, 0x14, 0x14}},
    { 0xBA, 0x01, {0x20}},
    { 0xBB, 0x07, {0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55}},
    { 0xF0, 0x05, {0x55, 0xAA, 0x52, 0x08, 0x02}},
    { 0xE1, 0x01, {0x00}},
    { 0xCA, 0x01, {0x04}},
    { 0xE2, 0x01, {0x0A}},
    { 0xE3, 0x01, {0x00}},
    { 0xE7, 0x01, {0x00}},
    { 0xED, 0x08, {0x48, 0x00, 0xE0, 0x13, 0x08, 0x00, 0x92, 0x08}},
    { 0xFD, 0x06, {0x00, 0x08, 0x1C, 0x00, 0x00, 0x01}},
    { 0xC3, 0x0B, {0x11, 0x24, 0x04, 0x0A, 0x01, 0x04, 0x00, 0x1C, 0x10, 0xF0, 0x00}},
    { 0xEA, 0x05, {0x7F, 0x20, 0x00, 0x00, 0x00}},
    { 0xF0, 0x05, {0x55, 0xAA, 0x52, 0x08, 0x01}},
    { 0xB0, 0x03, {0x01, 0x01, 0x01}},
    { 0xB1, 0x03, {0x05, 0x05, 0x05}},
    { 0xB2, 0x03, {0xD0, 0xD0, 0xD0}},
    { 0xB4, 0x03, {0x37, 0x37, 0x37}},
    { 0xB5, 0x03, {0x05, 0x05, 0x05}},
    { 0xB6, 0x03, {0x54, 0x54, 0x54}},
    { 0xB7, 0x03, {0x24, 0x24, 0x24}},
    { 0xB8, 0x03, {0x24, 0x24, 0x24}},
    { 0xB9, 0x03, {0x14, 0x14, 0x14}},
    { 0xBA, 0x03, {0x14, 0x14, 0x14}},
    { 0xBC, 0x03, {0x00, 0xF8, 0xB2}},
    { 0xBE, 0x03, {0x23, 0x00, 0x70}},
    { 0xCA, 0x01, {0x80}},
    { 0xCB, 0x0C, {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
    { 0xCC, 0x0C, {0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F}},
    { 0xF0, 0x05, {0x55, 0xAA, 0x52, 0x08, 0x03}},
    { 0xF1, 0x06, {0x10, 0x00, 0x00, 0x00, 0x01, 0x30}},
    { 0xF6, 0x01, {0x0A}},
    { 0xF0, 0x05, {0x55, 0xAA, 0x52, 0x08, 0x05}},
    { 0xC0, 0x07, {0x06, 0x02, 0x02, 0x22, 0x00, 0x00, 0x01}},
    { 0x35, 0x01, {0x00}},
    { 0x36, 0x01, {0x02}},
    { 0x51, 0x01, {0x00}},
    { 0x11, 0x01, {0x00}},
    { REGFLAG_DELAY, 100,{0x00}},
    { 0x29, 0x01,{0x00}},
    { REGFLAG_END_OF_TABLE,0x00,{0x00}}
};

static struct LCM_setting_table lcm_backlight_level_setting[] = {
	{0x51, 1, {0xFF}},
	//{REGFLAG_END_OF_TABLE, 0x00, {0x00}}
};

static void push_table(struct LCM_setting_table *table, unsigned int count, unsigned char force_update)
{
	unsigned int i;

	for (i = 0; i < count; i++) {

		unsigned cmd;
		cmd = table[i].cmd;

		switch (cmd) {

		case REGFLAG_DELAY:
			MDELAY(table[i].count);
			break;

		case REGFLAG_END_OF_TABLE:
			break;

		default:
			dsi_set_cmdq_V2(cmd, table[i].count, table[i].para_list, force_update);
		}
	}

}


// ---------------------------------------------------------------------------
//  LCM Driver Implementations
// ---------------------------------------------------------------------------

static void lcm_set_util_funcs(const LCM_UTIL_FUNCS * util)
{
	memcpy(&lcm_util, util, sizeof(LCM_UTIL_FUNCS));
}


static void lcm_get_params(LCM_PARAMS * params)
{
	memset(params, 0, sizeof(LCM_PARAMS));

  params->type = 2;
  params->dsi.data_format.format = 2;
  params->dsi.intermediat_buffer_num = 2;
  params->dsi.PS = 2;
  params->dsi.mode = 1;
  params->dsi.LANE_NUM = 4;
  params->dsi.word_count = 2160;
  params->dsi.vertical_backporch = 20;
  params->dsi.vertical_frontporch = 20;
  params->dsi.horizontal_backporch = 20;
  params->dsi.horizontal_frontporch = 20;
  params->dsi.ssc_disable = 1;
  params->dsi.clk_lp_per_line_enable = 1;
  params->dsi.esd_check_enable = 0;
  params->dsi.customization_esd_check_enable = 0;
  params->dsi.lcm_esd_check_table[0].count = 1;
  params->width = 720;
  params->height = 1280;
  params->dsi.packet_size = 256;
  params->dsi.vertical_sync_active = 10;
  params->dsi.vertical_active_line = 1280;
  params->dsi.horizontal_sync_active = 8;
  params->dsi.horizontal_active_pixel = 720;
  params->dsi.PLL_CLOCK = 220;
  params->dsi.lcm_esd_check_table[0].cmd = 10;
  params->dsi.lcm_esd_check_table[0].para_list[0] = 156;

}

static void lcm_init(void)
{
	SET_RESET_PIN(0);
	MDELAY(10);
	set_gpio_lcd_enp(1);
	MDELAY(10);
    set_gpio_lcd_enn(1);
	MDELAY(10);
	SET_RESET_PIN(1);
	MDELAY(20);
	push_table(lcm_initialization_setting,sizeof(lcm_initialization_setting) /sizeof(struct LCM_setting_table), 1);

#if defined(BUILD_LK)
	printf("-----------------init------------------");
#else
	printk("-----------------init------------------");
#endif
}

static void lcm_suspend(void)
{
#if !defined(BUILD_LK)
	unsigned int data_array[16];

	data_array[0] = 0x00280500;
	dsi_set_cmdq(data_array, 1, 1);
	MDELAY(50);
	data_array[0] = 0x00100500;
	dsi_set_cmdq(data_array, 1, 1);
	MDELAY(50);

	SET_RESET_PIN(0);
	MDELAY(10);
    set_gpio_lcd_enp(0); // im not sure about this one
#endif
}


static void lcm_resume(void)
{   
	lcm_init();
}


static unsigned int lcm_compare_id(void)
{
	return 1;
}

static void lcm_setbacklight(unsigned int level)
{
	unsigned int default_level = 50;
	unsigned int mapped_level = 0;
	printk("HCT_lcm_setbacklight level = %d\n", level);

	if(level > 255)
		level = 255;

	if(level >0)
		mapped_level = default_level+(level)*(255-default_level)/(255);
	else
		mapped_level=0;

	lcm_backlight_level_setting[0].para_list[0] = mapped_level;
	MDELAY(16);
	push_table(lcm_backlight_level_setting, sizeof(lcm_backlight_level_setting) / sizeof(struct LCM_setting_table), 1);
}

// ---------------------------------------------------------------------------
//  Get LCM Driver Hooks
// ---------------------------------------------------------------------------
LCM_DRIVER hct_rm67120_dsi_vdo_hd_gvo_lcm_drv = 
{
	.name			= "hct_rm67120_dsi_vdo_hd_gvo",
	.set_util_funcs = lcm_set_util_funcs,
	.get_params     = lcm_get_params,
	.init           = lcm_init,
	.suspend        = lcm_suspend,
	.resume         = lcm_resume,	
	.compare_id     = lcm_compare_id,	
	.set_backlight    = lcm_setbacklight,  
};
