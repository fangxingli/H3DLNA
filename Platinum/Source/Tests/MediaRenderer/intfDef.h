/****************************************************************************
* Copyright (C), 1994-2008, Routon Electronic Co., Ltd.
* 文件名： intfDef.h
* 内容简述：公用定义头文件
*                               1、通用变量类型定义
*                               2、通用全局变量定义
* 文件历史：
* 版本号 		日期 			作者 		说明
*

* 01a 			2008-09-22 		王雅文	创建该文件
*/
#ifndef _INTF_DEF_H
#define _INTF_DEF_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <jpeglib.h>
/*
* 结构需要采用紧缩对齐方式（1字节对齐）时，请采用下述宏定义
*/
#ifndef PACKED
	#ifdef __GNUC__
		#define PACKED __attribute__((__packed__))
	#else
		#define PACKED
	#endif
#endif

#define S_OK                            0	/* 函数返回真 */
#define S_FAILED                        -1	/* 函数返回假 */

#define S_SRC_NOT_EXSIT                 -2	/* 源文件不存在 */
#define S_DEST_CREATE_ERR               -3	/* 创建目的文件失败 */
#define S_READ_ERR               		-4	/* 读文件失败 */
#define S_WRITE_ERR             		-5	/* 写文件失败 */
#define S_NO_FILE             			-6	/* 写文件失败 */

#define K_SIZE  1000

#define CONST		const
#define MULTY_USB       1

/* signed */
typedef int8_t          INT1;
typedef int16_t         INT2;
typedef int32_t         INT4;

/* unsigned */
typedef uint8_t         UINT1;
typedef uint16_t        UINT2;
typedef uint32_t        UINT4;

/* Typedef declaration */
/* signed
typedef signed char			INT8;
typedef signed short		INT16;
typedef signed long			INT32;*/
typedef signed long long	INT64;

/* unsigned */
#if 0
typedef unsigned char		UINT8;
typedef unsigned short		UINT16;
#endif
typedef unsigned long		UINT32;
typedef unsigned long long	UINT64;

/* other */
typedef void		        VOID;
typedef char		        CHAR;
typedef int			 		INT;
typedef unsigned int	 	UINT;
typedef short		        SHORT;
typedef float		        FLOAT;
typedef double		 		DOUBLE;

#define MAKEWORD(l, h)                  ((UINT2)(((UINT1)(l)) | ((UINT2)((UINT1)(h))) << 8))
#define MAKELONG(l, h)                  ((UINT4)(((UINT2)(l)) | ((UINT4)((UINT2)(h))) << 16))
#define LOWORD(l)                       ((UINT2)(l))
#define HIWORD(l)                       ((UINT2)(((UINT4)(l) >> 16) & 0xffff))
#define LOBYTE(w)                       ((UINT1)(w))
#define HIBYTE(w)                       ((UINT1)(((UINT2)(w) >> 8) & 0xff))
#define LOUINT(I64)                     ((UINT4)(I64))
#define HIUINT(I64)                     ((UINT4)(((int64_t)(I64) >> 32) & 0xffffffff))
#define SIZEOF_TABLE(t)                 (sizeof (t) / sizeof ((t)[0]))


typedef struct _rect
{
	int x,y;
	int w,h;
} RECT;

/*
* 消息接口定义
* 消息发送和接收时请注意消息的编码和长度类型，
*
*/
/* 全局消息体*/
#define MAX_MSG_BYTES   			(1024 * 40)
#define MAX_MSG_DATA_BYTES  	(MAX_MSG_BYTES - 4)

typedef struct _RtnMsg
{
	UINT16 msg_code;    						/* 消息编码 */
	UINT16 msg_len;         					/* 消息内容（pData）的长度，采用小端格式*/
	CHAR   msg_data[MAX_MSG_DATA_BYTES];				/* 消息内容，由具体消息定义 */
}RtnMsg;

/*屏幕参数*/
//	#define 	SCREEN_W				(1280)
//	#define 	SCREEN_H				(720)
extern int GetScreenHeight(void);
#define 	SCREEN_W				(GetScreenWidth())
#define 	SCREEN_H				(GetScreenHeight())
#define 	SIZE_SCALER(l)			((l) * GetScreenHeight() / 720)
#define 	IS_SCREEN_1080P			(GetScreenHeight() > 1000)
#define 	AD_SCREEN_W				(200)
#define 	AD_SCREEN_H				(720)

#define 	FONT "Microsoft YaHei"

/*
* 定义文件路径buf的长度值,各模块都需要静态分配内存用于存放文件路径，请
* 统一使用以下字节长度定义
*/
#define 	MAX_PATH_LEN		255		 //用于限制字符串的字节数
#define	PATH_BUFF_LEN		(MAX_PATH_LEN+1) //用于声明buf


/* 各模块消息代码范围定义 */
#define	MAX_MSGS_MODULE		100 								/*每个模块最多的消息数*/
#define MSG_BASE_ALL			0										/*公共消息*/

enum
{
	MSG_BASE_MAIN = MSG_BASE_ALL + MAX_MSGS_MODULE,				/* 主控模块 */
	MSG_BASE_DESK = MSG_BASE_MAIN + MAX_MSGS_MODULE,			/* 桌面模块 */
	MSG_BASE_AVCORE = MSG_BASE_DESK + MAX_MSGS_MODULE,		/* 音视频播放模块 */
	MSG_BASE_MOVIE = MSG_BASE_AVCORE + MAX_MSGS_MODULE,		/* 视频播放器模块 */
	MSG_BASE_MUSIC = MSG_BASE_MOVIE + MAX_MSGS_MODULE,		/* 音频播放器模块 */
	MSG_BASE_PHOTO = MSG_BASE_MUSIC + MAX_MSGS_MODULE,		/* 图片浏览模块 */
	MSG_BASE_BTMAIN = MSG_BASE_PHOTO + MAX_MSGS_MODULE,		/* BT管理模块 */
	MSG_BASE_XUNLEI = MSG_BASE_BTMAIN + MAX_MSGS_MODULE,	/* 迅雷界面模块 */
	MSG_BASE_UPDATE = MSG_BASE_XUNLEI + MAX_MSGS_MODULE,		/* 程序升级模块 */
	MSG_BASE_IM = MSG_BASE_UPDATE + MAX_MSGS_MODULE,			/* IM模块 */
	MSG_BASE_EXPLORER = MSG_BASE_IM + MAX_MSGS_MODULE,		/* 浏览器模块 */
	MSG_BASE_RSS = MSG_BASE_EXPLORER + MAX_MSGS_MODULE,		/* RSS模块 */
	MSG_BASE_BTKB = MSG_BASE_RSS + MAX_MSGS_MODULE,				/* 蓝牙控制器模块  */
	MSG_BASE_PWRM = MSG_BASE_BTKB + MAX_MSGS_MODULE,		/* 电源管理模块 */
	MSG_BASE_WIFI = MSG_BASE_PWRM + MAX_MSGS_MODULE,		/* wifi模块 */
	MSG_BASE_MNTN = MSG_BASE_WIFI + MAX_MSGS_MODULE,		/* 维护模块 */
	MSG_BASE_TEST = MSG_BASE_MNTN + MAX_MSGS_MODULE,		/* 生产测试模块 */
	MSG_BASE_SWITCH = MSG_BASE_TEST + MAX_MSGS_MODULE,		/* 切换模块 */
	MSG_BASE_HINT = MSG_BASE_SWITCH + MAX_MSGS_MODULE,		/* 提示信息模块 */
  	MSG_BASE_KB   = MSG_BASE_HINT + MAX_MSGS_MODULE,                /* 蓝牙控制器模块 zhp 2009-4-10*/
	MSG_BASE_POWER   = MSG_BASE_KB + MAX_MSGS_MODULE,                /* 电源管理模块 zhp 2009-4-10*/
	MSG_BASE_WEATHER = MSG_BASE_POWER + MAX_MSGS_MODULE,
	MSG_BASE_ADS	= MSG_BASE_WEATHER + MAX_MSGS_MODULE,
	MSG_BASE_LANSVR	= MSG_BASE_ADS + MAX_MSGS_MODULE,		/* 网络后台连接模块 zhp 2009-7-27*/
	MSG_BASE_SRCMANAGER = MSG_BASE_LANSVR + MAX_MSGS_MODULE,
	MSG_BASE_ADSLSVR = MSG_BASE_SRCMANAGER + MAX_MSGS_MODULE,		/* 网络后台连接模块 zhp 2009-7-28*/
	MSG_BASE_IPRADIO = MSG_BASE_ADSLSVR + MAX_MSGS_MODULE,      /* 网络广播模块 */
	MSG_BASE_CERTIFY = MSG_BASE_IPRADIO + MAX_MSGS_MODULE,      /* 网络广播模块 */
	MSG_BASE_ANDROID = MSG_BASE_CERTIFY + MAX_MSGS_MODULE,      /* android模块 */
 	MSG_BASE_IME = MSG_BASE_ANDROID + MAX_MSGS_MODULE,			/* ime module */
 	MSG_BASE_CSSM = MSG_BASE_IME + MAX_MSGS_MODULE,				/* CsSM module */
 	MSG_BASE_SUBMENU = MSG_BASE_CSSM + MAX_MSGS_MODULE,			/* submenu module */
	MSG_BASE_ACCOUNT = MSG_BASE_SUBMENU + MAX_MSGS_MODULE,		/* Account module */
 	MSG_BASE_WEBTV = MSG_BASE_ACCOUNT + MAX_MSGS_MODULE,			/* 在线视频模块*/
 	MSG_BASE_DTV = MSG_BASE_WEBTV + MAX_MSGS_MODULE,			/* 数字电视模块*/
 	MSG_BASE_AGENT = MSG_BASE_DTV + MAX_MSGS_MODULE,			/*电驴agent模块*/
	MSG_BASE_PLAYLIST = MSG_BASE_AGENT + MAX_MSGS_MODULE,			/*播放记录模块*/
	MSG_BASE_WIDGET = MSG_BASE_PLAYLIST + MAX_MSGS_MODULE,			/*天气预报模块*/
	MSG_BASE_SIM = MSG_BASE_WIDGET + MAX_MSGS_MODULE,			/* speech input method module */
	MSG_BASE_WASUSVR = MSG_BASE_SIM + MAX_MSGS_MODULE,				/* 华数消息*/
	MSG_BASE_PAY = MSG_BASE_WASUSVR + MAX_MSGS_MODULE,				/* 支付消息*/
	MSG_BASE_INTEL = MSG_BASE_PAY + MAX_MSGS_MODULE,
	MSG_MAX = 0x8000
};

/*定义消息初始化接口的模块字符串，在xmsg.h的接口函数XMsgInit中应用*/
#define		MODULE_ID_MAIN				"h3.Main"
#define		MODULE_ID_DESKTOP			"h3.Desktop"
#define		MODULE_ID_MOVIE				"h3.Movie"
#define		MODULE_ID_MUSIC				"h3.Music"
#define		MODULE_ID_PHOTO				"h3.Photo"


#define		MODULE_ID_BTGUI				"h3.BTGUI"

#define     MODULE_ID_EDONKEY           "h3.EDonkey"
#define     MODULE_ID_BTAGENT           "h3.BTAgent"
#define     MODULE_ID_DLTOOL			"h3.dltool"
#define     MODULE_ID_ETMAIN_AGENT      MODULE_ID_BTAGENT
#define     MODULE_ID_BTGUI_AGENT       MODULE_ID_BTAGENT

#define		MODULE_ID_IM				"h3im"
#define		MODULE_ID_EXPLORER			"h3.Explore"
#define		MODULE_ID_RSS				"h3.RSS"
#define		MODULE_ID_BTKB				"h3.BTController"
#define		MODULE_ID_PWRM				"h3.Power"
#define		MODULE_ID_USB				"h3.Usb"
#define		MODULE_ID_WIFI				"h3.Wifi"
#define		MODULE_ID_MNTN				"h3.Mntn"
#define		MODULE_ID_TEST				"h3.Test"
#define		MODULE_ID_SWITCH			"h3.Switch"
#define		MODULE_ID_HINT				"h3.Hint"
#define 	MODULE_ID_XLSVC				"h3.svcxunlei"
#define 	MODULE_ID_XLGUI				"h3.guixunlei"
#define		MODULE_ID_PLAY				"h3.Play"
#define		MODULE_ID_IPTV				"h3.IPTV"
#define 	MODULE_ID_PLAYER			"h3.sg_player"//wyw 2009-4-9
#define     	MODULE_ID_KB                		"h3.KB"        /*蓝牙控制器模块 zhp 2009-4-10*/
#define 	MODULE_ID_ETMAIN			"h3.ETMain"
#define 	MODULE_ID_PROCESS			"h3.Process"	//处理其他外部程序退出 wanjie 2009-04-23
#define 	MODULE_ID_UPDATEMAIN		"h3.Updatemain"	//升级模块界面模块
#define 	MODULE_ID_UPDATESVR			"h3.Updatesvr"	//升级模块后台服务模块
#define 	MODULE_ID_WEATHERMAIN 		"h3.Wmain"
#define 	MODULE_ID_WEATHERDOWN 		"h3.Wdown"
#define 	MODULE_ID_ADS 				"h3.Ads"
#define 	MODULE_ID_LANSVR 			"h3.Lansvr"	/* 网络后台连接模块 zhp 2009-7-27*/
#define 	MODULE_ID_SRCMANAGER		"h3.srcmanager"
#define 	MODULE_ID_ADSLSVR 			"h3.Adslsvr"	/* ADSL后台拨号模块 zhp 2009-7-28*/
#define     MODULE_ID_IPRADIO           "h3.Radio"      /* 网络广播模块 */
#define	MODULE_ID_WEBTV					"h3.Webtv"		/* pengtao 2009-09-04 增加在线视频后台模块消息id */
#define     MODULE_ID_BTAGENT           "h3.BTAgent"
#define 	MODULE_ID_CERTIFY			"h3.Certify"	/* h3认证模块 */
#define 	MODULE_ID_ANDROID			"h3.Android"	/* android模块 */
#define 	MODULE_ID_IME				"h3.ime"		/* ime模块 */
#define		MODULE_ID_DIGITAL			"h3.digital"	/* digital模块 */
#define		MODULE_ID_DTVUI				"h3.dtvui"		/* digital界面模块 */
#define		MODULE_ID_DAMEON			"h3.Dameon"		/* 后台监控模块 */
#define		MODULE_ID_CSSM				"h3.CsSM"		/* CsSM模块 */
#define 	MODULE_ID_SEARCH_RES		"h3.SearchRes"	/* 关键字搜索模块 */
#define		MODULE_ID_SUBMENU			"h3.Submenu" 	/* 游戏和app列表和启动模块 */
#define		MODULE_ID_ACCOUNT			"h3.Account"	/* 账户管理模块 */
#define		MODULE_ID_PLAYLIST			"h3.Playlist"	/* 播放记录模块 */
#define 	MODULE_ID_NVD				"h3.nvd"		/* nvd */
#define 	MODULE_ID_WEATHERWIDGET		"h3.WeatherWidget"		/* 天气预报模块 */
/* #define 	MODULE_ID_PAY3				"h3.PayThird"		 第三方支付模块 */
#define 	MODULE_ID_PAY				"h3.Pay"		/* 支付模块 */
#define 	MODULE_ID_SIM				"h3.SIM"		/* speech input method module */
#define 	MODULE_ID_WASUSVR			"h3.WaSuSvr"	/* 华数 下载数据 */
#define 	MODULE_ID_WASU				"h3.Wasu"		/* 华数应用模块 */
#define   MODULE_ID_INTELUI                 "h3.intelUI"
#define   MODULE_ID_INTELCMDSEND      "h3.intlCmdSend"
#define 	MODULE_ID_TOPSCREEN			"h3.TopScreen"
#define	MODULE_ID_NOTICE				"h3.Notice"


/*
* 模块界面风格文件路径定义
*/
#define 	RC_PATH					"/hdisk/rc/style"			//

//缺省的界面风格配置文件，如果模块没有特殊要求可以加载缺省的文件
#define 	RC_DEFAULT_FILE			RC_PATH"/defaultrc"
#define 	RC_MNTN_FILE			RC_PATH"/mntnrc"	//维护风格文件
#define 	RC_MLIST_FILE			RC_PATH"/mlistrc"
#define 	RC_LIBCSSM_FILE			RC_PATH"/libcssmrc"
#define 	RC_PROGRESSBAR_FILE		RC_PATH"/processbarrc"
#define 	RC_MOVIE_FILE			RC_PATH"/movierc"

/*字库文件路径定义，需要显示文字的模块注意加载字库文件*/
#define FONT_PATH				"/hdisk/rc/fonts"
#define FONT12_NAME				"*-12-*"
#define FONT13_NAME				"*-13-*"
#define FONT14_NAME				"*-14-*"
#define FONT15_NAME				"*-15-*"
#define FONT16_NAME				"*-16-*"
#define FONT17_NAME				"*-17-*"
#define FONT18_NAME				"*-18-*"
#define FONT19_NAME				"*-19-*"
#define FONT20_NAME				"*-20-*"
#define FONT21_NAME				"*-21-*"
#define FONT22_NAME				"*-22-*"
#define FONT23_NAME				"*-23-*"
#define FONT24_NAME				"*-24-*"
#define FONT25_NAME				"*-25-*"
#define FONT26_NAME				"*-26-*"
#define FONT27_NAME				"*-27-*"
#define FONT28_NAME				"*-28-*"
#define FONT29_NAME				"*-29-*"
#define FONT30_NAME				"*-30-*"
#define FONT32_NAME				"*-32-*"
#define FONT36_NAME				"*-36-*"
#define FONT40_NAME				"*-40-*"
#define FONT48_NAME				"*-48-*"
#define FONT72_NAME				"*-72-*"

/*界面图标文件路径定义*/
#define SKIN_PATH				"/hdisk/rc/pics"		/* 图片路径 */
#define SKIN_MNTN_PATH			SKIN_PATH"/mntn"	/*	设置模块的界面背景图片或图标*/
#define SKIN_COMM_PATH			SKIN_PATH"/common"  /* 	公用图标界面文件目录 */
#define SKIN_MOVIE_PATH			SKIN_PATH"/movie"  	/* 	movie图标界面文件目录 */
#define SKIN_DESKTOP_PATH		SKIN_PATH"/desktop"  	/* 	桌面图标界面文件目录 */
#define SKIN_ROOT_BG			SKIN_PATH"/desktop/root_bg.png"  	/* 	桌面图标界面文件目录 */
#define SKIN_IPRADIO_PATH               SKIN_PATH"/ipradio"     /* 网络广播界面文件目录 */
#define SKIN_DTV_PATH			SKIN_PATH"/dtv"	/*数字电视界面文件目录*/
#define STAGE_BACKGROUND_PATH	SKIN_COMM_PATH"/stage_background.pvr"

/* desktop Macro define */
#define DESKTOP_PROG_NAME		"desktop"
#define DESKTOP_PROG_VERSION	DESKTOP_PROG_NAME"1.0.0.0 (2009-03-10)"
#define DESKTOP_NAME			"Desktop"

#define DESKTOP_INI_FILE		"/hdisk/etc/desktop/desktop.ini"
#define DESKTOP_MENU_INI_FILE	"/hdisk/etc/desktop/desktop_style.ini"
#define RC_DESKTOP_FILE			"/hdisk/rc/style/desktoprc"

#define MAXNUM_BGIMG	4	/* 桌面背景图片总数 */
#define DESKTOP_BG_IMG 	"/hdisk/rc/pics/desktop/background-0.png"

/*配置文件路径定义*/
#define 	CONFIG_INI_PATH				"/hdisk/etc"		//

/*  MNTN相关 */
#define 	MNTN_INI_PATH			CONFIG_INI_PATH"/mntn"
#define 	MNTN_INI_FILE			MNTN_INI_PATH"/mntn.ini"
#define 	ANDROID_MNTN_INI_FILE		"/hdisk/android/mntn.ini"
#define 	MNTN_INI_CITYFILE 		MNTN_INI_PATH"/city.xml"

#define 	MNTN_INI_SYSTEM_SECT		"Setup"
#define 	MNTN_INI_LANG_KEY		"Language"
#define     MNTN_INT_UP_MUSIC_KEY       "BootAudio"
#define 	MNTN_INI_BRIGHTNESS_KEY		"Brightness"
#define 	MNTN_INI_CONTRAST_KEY		"Contrast"
#define 	MNTN_INI_SUBTITLCOLOR_KEY	"SubtitleColor"
#define 	MNTN_INI_BACKGROUND_KEY		"BackGround"
#define 	MNTN_INI_POWERON_KEY		"PowerOn"
#define 	MNTN_INI_POWEROFF_KEY		"PowerOff"
#define 	MNTN_INI_SUBTITLE_KEY		"Subtitle"
#define 	MNTN_INI_SUBLANG_KEY		"SubLang"
#define 	MNTN_INI_SUBSIZE_KEY		"SubSize"
#define 	MNTN_INI_VIDEOOUT_KEY		"VideoOutput"
#define 	MNTN_INI_HDMIAUDIO_KEY		"HDMIAudio"
#define 	MNTN_INI_SPDIFAUDIO_KEY		"SPDIFAudio"
#define     MNTN_INI_SPDIFAUDIOOUTPUT_KEY		"SPDIFAudioOutput"
#define 	MNTN_INI_AUDIOTYPE_KEY    "AUDIOType"
#define 	MNTN_INI_POWERAMPLIFIER_KEY "POWERamplifier"
#define 	MNTN_INI_UPDATEMODE_KEY		"UpdateMode"
#define 	MNTN_INI_SCREENSAVER_KEY	"ScreenSaver"
#define 	MNTN_INI_DOWNLOADSPEED_KEY	"DownloadSpeed"
#define     MNTN_INI_DOWNLOADSPEED_MAX  "DownloadMax"
#define 	MNTN_INI_UDISKNUM_KEY	        "UDiskNum"

//time off
#define 	MNTN_INI_TIME_OFF_SECT     "TimeOff"
#define     MNTN_INI_TIME_OFF_KEY      "time"
#define		MNTN_INI_TIME_SETTING_KEY  "setting"
#define		MNTN_INI_TIME_WEEK_KEY	   "week"
#define     MNTN_INI_TIME_MON_KEY      "mon"
#define     MNTN_INI_TIME_TUES_KEY      "tues"
#define     MNTN_INI_TIME_WED_KEY      "wed"
#define     MNTN_INI_TIME_THUR_KEY      "thur"
#define     MNTN_INI_TIME_FRI_KEY      "fri"
#define     MNTN_INI_TIME_SAT_KEY      "sat"
#define     MNTN_INI_TIME_SUN_KEY      "sun"

//user Lock
#define     MNTN_INI_LOCK_SECT          "Lock"
#define     MNTN_INI_LOCK_ENABLE        "enable"


#define 	MNTN_INI_UDISK_SECT    		"Udisk"   //added by sj
#define 	MNTN_INI_UDISK_UDISK		"udisk"
#define 	MNTN_INI_UDISK_PARTITION	"partition" //added by sj

#define 	MNTN_INI_NETWORK_SECT		"Network"
#define 	MNTN_INI_NETWORKENABLE		"Enable"
#define 	MNTN_INI_NETWORKTYPE		"Type"
#define 	MNTN_INI_PPPOEUSER		"PPPOEUser"
#define 	MNTN_INI_PPPOEPASS		"PPPOEPass"
#define 	MNTN_INI_IP			"IP"
#define 	MNTN_INI_MASK			"Mask"
#define 	MNTN_INI_GATEWAY		"Gateway"
#define 	MNTN_INI_DNS1			"DNS1"
#define 	MNTN_INI_DNS2			"DNS2"
#define 	MNTN_INI_HOST			"Host"
#define 	MNTN_INI_DNSSET			"DNSSet"
#define 	MNTN_INI_DDNS1			"DDNS1"
#define 	MNTN_INI_DDNS2			"DDNS2"

#define		MNTN_INI_WIFI_SECT		"WiFi"
#define 	MNTN_INI_START			"Start"

#define		MNTN_INI_BLUETOOTH_SECT		"BlueTooth"
#define 	MNTN_INI_USER			"User"
#define 	MNTN_INI_PASS			"Pass"

#define 	MNTN_INI_ABOUT_SECT		"About"
#define 	MNTN_INI_APP_VERSION		"AppVersion"
#define 	MNTN_INI_PLAYER_VERSION		"PlayerVersion"
#define 	MNTN_INI_BROWSER_VERSION	"BrowserVersion"
#define		MNTN_INI_MCU_VERSION		"McuVersion"
#define 	MNTN_INI_UUID			"UUID"

#define 	MNTN_INI_WEATHER_SECT		"Weather"
#define 	MNTN_INI_PROVINCE		"Province"
#define 	MNTN_INI_CITY			"City"
#define 	MNTN_INI_CITY_NO		"CityNo"

#define 	MNTN_INI_PRODUCT_SECT		"Product"
#define 	MNTN_INI_DISPLAY		"display"
#define 	MNTN_INI_MODE_NUM		"mode_num"
#define 	MNTN_INI_MODE_TOTAL		"mode_total"

#define 	MNTN_INI_MODE_SECT		"mode*"
#define		MNTN_INI_REFRESH		"refresh"
#define		MNTN_INI_INTERLACED		"interlaced"
#define		MNTN_INI_HEIGHT			"height"
#define		MNTN_INI_WIDTH			"width"

/*  MOVIE相关 */
#define 	MOVIE_INI_PATH				CONFIG_INI_PATH"/movie"
#define 	MOVIE_INI_FILE				MOVIE_INI_PATH"/movie.ini"

/*  ACCOUNT相关 */
#define 	ACCOUNT_INI_PATH			CONFIG_INI_PATH"/account"
#define 	ACCOUNT_INI_FILE			ACCOUNT_INI_PATH"/account.ini"
#define 	ACCOUNT_INI_INFO_SECT		"Info"
#define 	ACCOUNT_INI_ID				"account-id"
#define 	ACCOUNT_INI_BALANCE			"balance"
#define 	ACCOUNT_INI_NICKNAME		"nickname"
#define 	ACCOUNT_INI_MOBILE			"mobile"
#define 	ACCOUNT_INI_EMAIL			"email"
#define 	ACCOUNT_INI_PWD				"password"

#define 	ACCOUNT_INI_STATE_SECT		"State"
#define 	ACCOUNT_INI_FIRST_USE		"first-use"

#define	ACCOUNT_INI_URL_SECT			"urls"
#define   ACCOUNT_INI_COOKIE_FILE 		"cookie"
#define 	ACCOUNT_INI_QUERY				"query"
#define   ACCOUNT_INI_INFO_MOD			"info"
#define 	ACCOUNT_INI_PWD_MOD			"pwd"
#define 	ACCOUNT_INI_RECHARGE			"recharge"
#define 	ACCOUNT_INI_CHANGE 			"change"

/* 屏幕分辨率的类型定义 */
enum
{
	MOVIE_PLAY_NONE = 0,		/* 未播放 */
	MOVIE_PLAY_720P,		/* 720P */
	MOVIE_PLAY_1080P,		/* 1080P */
	MOVIE_PLAY_DEFAULT
};
/*版本信息*/
#define OS_VERSION 	"/version"

/*系统数据区*/
#define SYSTEM_DATA 	"/hdisk/"
/*用户数据区*/
#define USER_DATA 		SYSTEM_DATA"data/"
/*用户数据区*/
#define BACK_DATA 		SYSTEM_DATA"backup/"

/* NET PC PATH */
#define NET_PC_DIR 		"/hdisk/data/NET_PC/"

#define USB_DIR 		"/hdisk/data/USB/"

#if 1
/* USB A的mount目录 */
#define USB_A_DIR 		"/hdisk/data/USBA/"
#define USB_A_DIR_A 	"/hdisk/data/USBA/A/"
#define USB_A_DIR_B		"/hdisk/data/USBA/B/"
#define USB_A_DIR_C 	"/hdisk/data/USBA/C/"
#define USB_A_DIR_D 	"/hdisk/data/USBA/D/"
#define USB_A_DIR_E 	"/hdisk/data/USBA/E/"
#define USB_A_DIR_F		"/hdisk/data/USBA/F/"

/* USB B的mount目录 */
#define USB_B_DIR 		"/hdisk/data/USBB/"
#define USB_B_DIR_A 	"/hdisk/data/USBB/A/"
#define USB_B_DIR_B		"/hdisk/data/USBB/B/"
#define USB_B_DIR_C 	"/hdisk/data/USBB/C/"
#define USB_B_DIR_D 	"/hdisk/data/USBB/D/"
#define USB_B_DIR_E 	"/hdisk/data/USBB/E/"
#define USB_B_DIR_F		"/hdisk/data/USBB/F/"
#endif

/* gphoto识别的相机目录 */
#define CAMERA_DIR 		"/hdisk/data/CAMERA/"

#define USB_A_ICON 		"/hdisk/rc/pics/common/usba.png"
#define USB_B_ICON 		"/hdisk/rc/pics/common/usbb.png"
#define USB_A_ROOT_ICON	"/hdisk/rc/pics/common/usba-root.png"
#define USB_B_ROOT_ICON	"/hdisk/rc/pics/common/usbb-root.png"
#define USB_A_SUB_ICON	"/hdisk/rc/pics/common/usba-sub.png"
#define USB_B_SUB_ICON	"/hdisk/rc/pics/common/usbb-sub.png"
#define MOVIE_ICON 		"/hdisk/rc/pics/common/movie_icon.png"
#define PIC_ICON 		"/hdisk/rc/pics/common/picture_icon.png"
#define DEFAULT_ICON 	"/hdisk/rc/pics/common/default_icon.png"
#define FOLD_ICON 		"/hdisk/rc/pics/common/fold.png"
#define BACK_ICON 		"/hdisk/rc/pics/common/back.png"
#define HARDDISK_ICON 	"/hdisk/rc/pics/common/harddisk.png"
#define NET_ICON 		"/hdisk/rc/pics/common/net.png"
#define APP_ICON 		"/hdisk/rc/pics/common/app_icon.png"
#define GAME_ICON 		"/hdisk/rc/pics/common/game_icon.png"


/*wifi相关*/
#define 	WIFI_INI_PATH				CONFIG_INI_PATH"/wifi"		//wifi配置文件
#define 	WIFI_INI_FILE				WIFI_INI_PATH"/wifi.ini"
#define 	WIFI_INI_SETUP				"Setup"
#define 	WIFI_INI_AUTOCONNECT		"Autoconnect"
#define 	WIFI_INI_ESSID				"Essid"
#define 	WIFI_INI_DNS				"Dns"
#define 	WIFI_INI_DNS2				"Dns2"
#define 	WIFI_INI_GATEWAY			"Gateway"
#define 	WIFI_INI_NETMASK			"Netmask"
#define 	WIFI_INI_IP					"Ip"
#define 	WIFI_INI_DHCP				"Dhcp"
#define 	WIFI_INI_KEY				"Key"
#define 	WIFI_INI_ENCRYPTION			"Encryption"
#define		WIFI_INI_MAXRATE			"Maxrate"
#define 	WIFI_INI_MASK				"Mask"
#define 	WIFI_INI_HOST				"Host"
#define 	WIFI_INI_DNSSET				"DNSSet"
#define 	WIFI_INI_DDNS				"DDns"
#define 	WIFI_INI_DDNS2				"DDns2"
#define 	WIFI_INI_PPPOEUSER		"PPPOEUser"
#define 	WIFI_INI_PPPOEPASS		"PPPOEPass"

/*字幕颜色定义*/
enum
{
	COLOR_YELLOW=0,
	COLOR_BLUE,
	COLOR_RED,
	COLOR_ORANGE,
	COLOR_GREEN,
	COLOR_GRAY
};

/* 终端语言相关 */
#define MAXNUM_LANG				(3)	/* 语言种类的数量 */

/* 各界面的帮助文件名 */
#define HELP_TXT_PATH				"/hdisk/etc/help"
#define HELP_MAIN					HELP_TXT_PATH"/main.txt"		//主界面
#define HELP_VIDEO					HELP_TXT_PATH"/video.txt"		//视频菜单界面
#define HELP_INTERNET				HELP_TXT_PATH"/internet.txt"		//互联网菜单界面
#define HELP_SYSTEM			        HELP_TXT_PATH"/system.txt"	//系统界面
#define HELP_GAME					HELP_TXT_PATH"/game.txt"		//游戏图片界面
#define HELP_PROGRAM 				HELP_TXT_PATH"/program.txt"		//应用程序图片界面

#define HELP_VIDEO_LIST 			HELP_TXT_PATH"/video-list.txt"	//视频-播放列表菜单界面
#define HELP_VIDEO_LIST_TYPE 		HELP_TXT_PATH"/video-list-type.txt"		//视频-播放列表-影片库、最新下载、自定义类别-文件列表界面
#define HELP_VIDEO_LIST_EX 			HELP_TXT_PATH"/video-list-external.txt"	//视频-播放列表-外部设备类别界面
#define HELP_VIDEO_LIST_EX_FILELIST HELP_TXT_PATH"/video-list-external-fileList.txt"	//视频-播放列表-外部设备-文件列表界面
#define HELP_VIDEO_PLOT				HELP_TXT_PATH"/video-plot.txt"		//视频-简介界面
#define HELP_VIDEO_COPY				HELP_TXT_PATH"/video-copy.txt"		//视频-复制界面
#define HELP_VIDEO_ONLIME 			HELP_TXT_PATH"/video-online.txt"		//视频-在线播放界面
#define HELP_RADIO					HELP_TXT_PATH"/radio.txt"		//网络广播界面
#define HELP_ALBUM					HELP_TXT_PATH"/album.txt"		//相册菜单界面
#define HELP_ALBUM_ALL				HELP_TXT_PATH"/album-all.txt"	//相册-全部界面
#define HELP_ALBUM_EX 				HELP_TXT_PATH"/album-external.txt"//相册-外部设备界面
#define HELP_ALBUM_EX_FILELIST 		HELP_TXT_PATH"/album-external-fileList.txt"//相册-外部设备-文件列表界面
#define HELP_ALBUM_COPY				HELP_TXT_PATH"/album-copy.txt"		//相册-复制界面
#define HELP_DTV					HELP_TXT_PATH"/dtv.txt"			//数字电视菜单界面

#define HELP_VIDEO_DOWN_STATE 		HELP_TXT_PATH"/video-download-state.txt"//视频-下载状态界面
#define HELP_VIDEO_DOWN_STATE_STAT	HELP_TXT_PATH"/video-download-state-stat.txt"//视频-下载状态-统计界面
#define HELP_VIDEO_DOWN_STATE_BT 	HELP_TXT_PATH"/video-download-state-bt.txt"	//视频-下载状态-导入种子界面
#define HELP_VIDEO_DOWN_STATE_SET 	HELP_TXT_PATH"/video-download-state-set.txt"	//视频-下载状态-设置界面
#define HELP_DISK					HELP_TXT_PATH"/disk.txt"		//资源管理界面
#define HELP_DISK_KEY				HELP_TXT_PATH"/disk-key.txt"		//资源管理界面
#define HELP_SETTING				HELP_TXT_PATH"/setting.txt"		//设置菜单界面
#define HELP_SETTING_SYSTEM 		HELP_TXT_PATH"/setting-system.txt"	//设置-系统设置界面
#define HELP_SETTING_LAN			HELP_TXT_PATH"/setting-LAN.txt"		//设置-LAN设置界面
#define HELP_SETTING_WIFI			HELP_TXT_PATH"/setting-WiFi.txt"	//设置-WIFI设置界面
#define HELP_SETTING_UPDATE 		HELP_TXT_PATH"/setting-update.txt"	//设置-软件升级界面
#define HELP_SETTING_UPDATE_DETAIL	HELP_TXT_PATH"/setting-update-detail.txt"	//设置-软件升级详情界面
#define HELP_SETTING_INFO			HELP_TXT_PATH"/setting-info.txt"	//设置-信息界面
#define HELP_SETTING_REG			HELP_TXT_PATH"/setting-reg.txt"	//设置-注册界面
#define HELP_SETTING_GAME			HELP_TXT_PATH"/setting-game.txt"//设置-游戏注册界面
/*语言定义*/
enum
{
	SIMPLECHINESE = 0,
	TRADITIOANLCHINESE,
	ENGLISH,

	LANGUAGE_MAX
};

/*连接状态*/
enum
{
	STATUS_DISCONNECT = 0,
	STATUS_CONNECTED,
	STATUS_CONNECTING
};

/*分辨率标志*/
enum
{
	TVMODE_720P = 0,
	TVMODE_1080P,
};

/*board chip标志*/
enum
{
	INTEL_CE3100 = 0,
	INTEL_CE4110_NOR,
	INTEL_CE4110_NAND,
	INTEL_CE4130_NAND,
	TRIDENT,
};

/*cpu type标志*/
enum
{
	H3 = 0,
	H3D,
	H3E,
	H3F,
	H4D,
	H4F,
};

/*harddisk type标志*/
enum
{
	HAVE_HARDDISK = 0,
	NO_HARDDISK,
};

//设备定义
#define LM95235					"/dev/lm95235"
#define SLE4442					"/dev/sle4442"

//应用程序退出时的等待时间
#define APP_QUIT_WAIT_TIME 0//200000

#ifdef __cplusplus
}
#endif

#endif /* _INTF_DEF_H */
/* END */
