/****************************************************************************
* Copyright (C), 1994-2008, Routon Electronic Co., Ltd.
* �ļ����� intfDef.h
* ���ݼ��������ö���ͷ�ļ�
*                               1��ͨ�ñ������Ͷ���
*                               2��ͨ��ȫ�ֱ�������
* �ļ���ʷ��
* �汾�� 		���� 			���� 		˵��
*

* 01a 			2008-09-22 		������	�������ļ�
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
* �ṹ��Ҫ���ý������뷽ʽ��1�ֽڶ��룩ʱ������������궨��
*/
#ifndef PACKED
	#ifdef __GNUC__
		#define PACKED __attribute__((__packed__))
	#else
		#define PACKED
	#endif
#endif

#define S_OK                            0	/* ���������� */
#define S_FAILED                        -1	/* �������ؼ� */

#define S_SRC_NOT_EXSIT                 -2	/* Դ�ļ������� */
#define S_DEST_CREATE_ERR               -3	/* ����Ŀ���ļ�ʧ�� */
#define S_READ_ERR               		-4	/* ���ļ�ʧ�� */
#define S_WRITE_ERR             		-5	/* д�ļ�ʧ�� */
#define S_NO_FILE             			-6	/* д�ļ�ʧ�� */

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
* ��Ϣ�ӿڶ���
* ��Ϣ���ͺͽ���ʱ��ע����Ϣ�ı���ͳ������ͣ�
*
*/
/* ȫ����Ϣ��*/
#define MAX_MSG_BYTES   			(1024 * 40)
#define MAX_MSG_DATA_BYTES  	(MAX_MSG_BYTES - 4)

typedef struct _RtnMsg
{
	UINT16 msg_code;    						/* ��Ϣ���� */
	UINT16 msg_len;         					/* ��Ϣ���ݣ�pData���ĳ��ȣ�����С�˸�ʽ*/
	CHAR   msg_data[MAX_MSG_DATA_BYTES];				/* ��Ϣ���ݣ��ɾ�����Ϣ���� */
}RtnMsg;

/*��Ļ����*/
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
* �����ļ�·��buf�ĳ���ֵ,��ģ�鶼��Ҫ��̬�����ڴ����ڴ���ļ�·������
* ͳһʹ�������ֽڳ��ȶ���
*/
#define 	MAX_PATH_LEN		255		 //���������ַ������ֽ���
#define	PATH_BUFF_LEN		(MAX_PATH_LEN+1) //��������buf


/* ��ģ����Ϣ���뷶Χ���� */
#define	MAX_MSGS_MODULE		100 								/*ÿ��ģ��������Ϣ��*/
#define MSG_BASE_ALL			0										/*������Ϣ*/

enum
{
	MSG_BASE_MAIN = MSG_BASE_ALL + MAX_MSGS_MODULE,				/* ����ģ�� */
	MSG_BASE_DESK = MSG_BASE_MAIN + MAX_MSGS_MODULE,			/* ����ģ�� */
	MSG_BASE_AVCORE = MSG_BASE_DESK + MAX_MSGS_MODULE,		/* ����Ƶ����ģ�� */
	MSG_BASE_MOVIE = MSG_BASE_AVCORE + MAX_MSGS_MODULE,		/* ��Ƶ������ģ�� */
	MSG_BASE_MUSIC = MSG_BASE_MOVIE + MAX_MSGS_MODULE,		/* ��Ƶ������ģ�� */
	MSG_BASE_PHOTO = MSG_BASE_MUSIC + MAX_MSGS_MODULE,		/* ͼƬ���ģ�� */
	MSG_BASE_BTMAIN = MSG_BASE_PHOTO + MAX_MSGS_MODULE,		/* BT����ģ�� */
	MSG_BASE_XUNLEI = MSG_BASE_BTMAIN + MAX_MSGS_MODULE,	/* Ѹ�׽���ģ�� */
	MSG_BASE_UPDATE = MSG_BASE_XUNLEI + MAX_MSGS_MODULE,		/* ��������ģ�� */
	MSG_BASE_IM = MSG_BASE_UPDATE + MAX_MSGS_MODULE,			/* IMģ�� */
	MSG_BASE_EXPLORER = MSG_BASE_IM + MAX_MSGS_MODULE,		/* �����ģ�� */
	MSG_BASE_RSS = MSG_BASE_EXPLORER + MAX_MSGS_MODULE,		/* RSSģ�� */
	MSG_BASE_BTKB = MSG_BASE_RSS + MAX_MSGS_MODULE,				/* ����������ģ��  */
	MSG_BASE_PWRM = MSG_BASE_BTKB + MAX_MSGS_MODULE,		/* ��Դ����ģ�� */
	MSG_BASE_WIFI = MSG_BASE_PWRM + MAX_MSGS_MODULE,		/* wifiģ�� */
	MSG_BASE_MNTN = MSG_BASE_WIFI + MAX_MSGS_MODULE,		/* ά��ģ�� */
	MSG_BASE_TEST = MSG_BASE_MNTN + MAX_MSGS_MODULE,		/* ��������ģ�� */
	MSG_BASE_SWITCH = MSG_BASE_TEST + MAX_MSGS_MODULE,		/* �л�ģ�� */
	MSG_BASE_HINT = MSG_BASE_SWITCH + MAX_MSGS_MODULE,		/* ��ʾ��Ϣģ�� */
  	MSG_BASE_KB   = MSG_BASE_HINT + MAX_MSGS_MODULE,                /* ����������ģ�� zhp 2009-4-10*/
	MSG_BASE_POWER   = MSG_BASE_KB + MAX_MSGS_MODULE,                /* ��Դ����ģ�� zhp 2009-4-10*/
	MSG_BASE_WEATHER = MSG_BASE_POWER + MAX_MSGS_MODULE,
	MSG_BASE_ADS	= MSG_BASE_WEATHER + MAX_MSGS_MODULE,
	MSG_BASE_LANSVR	= MSG_BASE_ADS + MAX_MSGS_MODULE,		/* �����̨����ģ�� zhp 2009-7-27*/
	MSG_BASE_SRCMANAGER = MSG_BASE_LANSVR + MAX_MSGS_MODULE,
	MSG_BASE_ADSLSVR = MSG_BASE_SRCMANAGER + MAX_MSGS_MODULE,		/* �����̨����ģ�� zhp 2009-7-28*/
	MSG_BASE_IPRADIO = MSG_BASE_ADSLSVR + MAX_MSGS_MODULE,      /* ����㲥ģ�� */
	MSG_BASE_CERTIFY = MSG_BASE_IPRADIO + MAX_MSGS_MODULE,      /* ����㲥ģ�� */
	MSG_BASE_ANDROID = MSG_BASE_CERTIFY + MAX_MSGS_MODULE,      /* androidģ�� */
 	MSG_BASE_IME = MSG_BASE_ANDROID + MAX_MSGS_MODULE,			/* ime module */
 	MSG_BASE_CSSM = MSG_BASE_IME + MAX_MSGS_MODULE,				/* CsSM module */
 	MSG_BASE_SUBMENU = MSG_BASE_CSSM + MAX_MSGS_MODULE,			/* submenu module */
	MSG_BASE_ACCOUNT = MSG_BASE_SUBMENU + MAX_MSGS_MODULE,		/* Account module */
 	MSG_BASE_WEBTV = MSG_BASE_ACCOUNT + MAX_MSGS_MODULE,			/* ������Ƶģ��*/
 	MSG_BASE_DTV = MSG_BASE_WEBTV + MAX_MSGS_MODULE,			/* ���ֵ���ģ��*/
 	MSG_BASE_AGENT = MSG_BASE_DTV + MAX_MSGS_MODULE,			/*��¿agentģ��*/
	MSG_BASE_PLAYLIST = MSG_BASE_AGENT + MAX_MSGS_MODULE,			/*���ż�¼ģ��*/
	MSG_BASE_WIDGET = MSG_BASE_PLAYLIST + MAX_MSGS_MODULE,			/*����Ԥ��ģ��*/
	MSG_BASE_SIM = MSG_BASE_WIDGET + MAX_MSGS_MODULE,			/* speech input method module */
	MSG_BASE_WASUSVR = MSG_BASE_SIM + MAX_MSGS_MODULE,				/* ������Ϣ*/
	MSG_BASE_PAY = MSG_BASE_WASUSVR + MAX_MSGS_MODULE,				/* ֧����Ϣ*/
	MSG_BASE_INTEL = MSG_BASE_PAY + MAX_MSGS_MODULE,
	MSG_MAX = 0x8000
};

/*������Ϣ��ʼ���ӿڵ�ģ���ַ�������xmsg.h�Ľӿں���XMsgInit��Ӧ��*/
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
#define     	MODULE_ID_KB                		"h3.KB"        /*����������ģ�� zhp 2009-4-10*/
#define 	MODULE_ID_ETMAIN			"h3.ETMain"
#define 	MODULE_ID_PROCESS			"h3.Process"	//���������ⲿ�����˳� wanjie 2009-04-23
#define 	MODULE_ID_UPDATEMAIN		"h3.Updatemain"	//����ģ�����ģ��
#define 	MODULE_ID_UPDATESVR			"h3.Updatesvr"	//����ģ���̨����ģ��
#define 	MODULE_ID_WEATHERMAIN 		"h3.Wmain"
#define 	MODULE_ID_WEATHERDOWN 		"h3.Wdown"
#define 	MODULE_ID_ADS 				"h3.Ads"
#define 	MODULE_ID_LANSVR 			"h3.Lansvr"	/* �����̨����ģ�� zhp 2009-7-27*/
#define 	MODULE_ID_SRCMANAGER		"h3.srcmanager"
#define 	MODULE_ID_ADSLSVR 			"h3.Adslsvr"	/* ADSL��̨����ģ�� zhp 2009-7-28*/
#define     MODULE_ID_IPRADIO           "h3.Radio"      /* ����㲥ģ�� */
#define	MODULE_ID_WEBTV					"h3.Webtv"		/* pengtao 2009-09-04 ����������Ƶ��̨ģ����Ϣid */
#define     MODULE_ID_BTAGENT           "h3.BTAgent"
#define 	MODULE_ID_CERTIFY			"h3.Certify"	/* h3��֤ģ�� */
#define 	MODULE_ID_ANDROID			"h3.Android"	/* androidģ�� */
#define 	MODULE_ID_IME				"h3.ime"		/* imeģ�� */
#define		MODULE_ID_DIGITAL			"h3.digital"	/* digitalģ�� */
#define		MODULE_ID_DTVUI				"h3.dtvui"		/* digital����ģ�� */
#define		MODULE_ID_DAMEON			"h3.Dameon"		/* ��̨���ģ�� */
#define		MODULE_ID_CSSM				"h3.CsSM"		/* CsSMģ�� */
#define 	MODULE_ID_SEARCH_RES		"h3.SearchRes"	/* �ؼ�������ģ�� */
#define		MODULE_ID_SUBMENU			"h3.Submenu" 	/* ��Ϸ��app�б������ģ�� */
#define		MODULE_ID_ACCOUNT			"h3.Account"	/* �˻�����ģ�� */
#define		MODULE_ID_PLAYLIST			"h3.Playlist"	/* ���ż�¼ģ�� */
#define 	MODULE_ID_NVD				"h3.nvd"		/* nvd */
#define 	MODULE_ID_WEATHERWIDGET		"h3.WeatherWidget"		/* ����Ԥ��ģ�� */
/* #define 	MODULE_ID_PAY3				"h3.PayThird"		 ������֧��ģ�� */
#define 	MODULE_ID_PAY				"h3.Pay"		/* ֧��ģ�� */
#define 	MODULE_ID_SIM				"h3.SIM"		/* speech input method module */
#define 	MODULE_ID_WASUSVR			"h3.WaSuSvr"	/* ���� �������� */
#define 	MODULE_ID_WASU				"h3.Wasu"		/* ����Ӧ��ģ�� */
#define   MODULE_ID_INTELUI                 "h3.intelUI"
#define   MODULE_ID_INTELCMDSEND      "h3.intlCmdSend"
#define 	MODULE_ID_TOPSCREEN			"h3.TopScreen"
#define	MODULE_ID_NOTICE				"h3.Notice"


/*
* ģ��������ļ�·������
*/
#define 	RC_PATH					"/hdisk/rc/style"			//

//ȱʡ�Ľ����������ļ������ģ��û������Ҫ����Լ���ȱʡ���ļ�
#define 	RC_DEFAULT_FILE			RC_PATH"/defaultrc"
#define 	RC_MNTN_FILE			RC_PATH"/mntnrc"	//ά������ļ�
#define 	RC_MLIST_FILE			RC_PATH"/mlistrc"
#define 	RC_LIBCSSM_FILE			RC_PATH"/libcssmrc"
#define 	RC_PROGRESSBAR_FILE		RC_PATH"/processbarrc"
#define 	RC_MOVIE_FILE			RC_PATH"/movierc"

/*�ֿ��ļ�·�����壬��Ҫ��ʾ���ֵ�ģ��ע������ֿ��ļ�*/
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

/*����ͼ���ļ�·������*/
#define SKIN_PATH				"/hdisk/rc/pics"		/* ͼƬ·�� */
#define SKIN_MNTN_PATH			SKIN_PATH"/mntn"	/*	����ģ��Ľ��汳��ͼƬ��ͼ��*/
#define SKIN_COMM_PATH			SKIN_PATH"/common"  /* 	����ͼ������ļ�Ŀ¼ */
#define SKIN_MOVIE_PATH			SKIN_PATH"/movie"  	/* 	movieͼ������ļ�Ŀ¼ */
#define SKIN_DESKTOP_PATH		SKIN_PATH"/desktop"  	/* 	����ͼ������ļ�Ŀ¼ */
#define SKIN_ROOT_BG			SKIN_PATH"/desktop/root_bg.png"  	/* 	����ͼ������ļ�Ŀ¼ */
#define SKIN_IPRADIO_PATH               SKIN_PATH"/ipradio"     /* ����㲥�����ļ�Ŀ¼ */
#define SKIN_DTV_PATH			SKIN_PATH"/dtv"	/*���ֵ��ӽ����ļ�Ŀ¼*/
#define STAGE_BACKGROUND_PATH	SKIN_COMM_PATH"/stage_background.pvr"

/* desktop Macro define */
#define DESKTOP_PROG_NAME		"desktop"
#define DESKTOP_PROG_VERSION	DESKTOP_PROG_NAME"1.0.0.0 (2009-03-10)"
#define DESKTOP_NAME			"Desktop"

#define DESKTOP_INI_FILE		"/hdisk/etc/desktop/desktop.ini"
#define DESKTOP_MENU_INI_FILE	"/hdisk/etc/desktop/desktop_style.ini"
#define RC_DESKTOP_FILE			"/hdisk/rc/style/desktoprc"

#define MAXNUM_BGIMG	4	/* ���汳��ͼƬ���� */
#define DESKTOP_BG_IMG 	"/hdisk/rc/pics/desktop/background-0.png"

/*�����ļ�·������*/
#define 	CONFIG_INI_PATH				"/hdisk/etc"		//

/*  MNTN��� */
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

/*  MOVIE��� */
#define 	MOVIE_INI_PATH				CONFIG_INI_PATH"/movie"
#define 	MOVIE_INI_FILE				MOVIE_INI_PATH"/movie.ini"

/*  ACCOUNT��� */
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

/* ��Ļ�ֱ��ʵ����Ͷ��� */
enum
{
	MOVIE_PLAY_NONE = 0,		/* δ���� */
	MOVIE_PLAY_720P,		/* 720P */
	MOVIE_PLAY_1080P,		/* 1080P */
	MOVIE_PLAY_DEFAULT
};
/*�汾��Ϣ*/
#define OS_VERSION 	"/version"

/*ϵͳ������*/
#define SYSTEM_DATA 	"/hdisk/"
/*�û�������*/
#define USER_DATA 		SYSTEM_DATA"data/"
/*�û�������*/
#define BACK_DATA 		SYSTEM_DATA"backup/"

/* NET PC PATH */
#define NET_PC_DIR 		"/hdisk/data/NET_PC/"

#define USB_DIR 		"/hdisk/data/USB/"

#if 1
/* USB A��mountĿ¼ */
#define USB_A_DIR 		"/hdisk/data/USBA/"
#define USB_A_DIR_A 	"/hdisk/data/USBA/A/"
#define USB_A_DIR_B		"/hdisk/data/USBA/B/"
#define USB_A_DIR_C 	"/hdisk/data/USBA/C/"
#define USB_A_DIR_D 	"/hdisk/data/USBA/D/"
#define USB_A_DIR_E 	"/hdisk/data/USBA/E/"
#define USB_A_DIR_F		"/hdisk/data/USBA/F/"

/* USB B��mountĿ¼ */
#define USB_B_DIR 		"/hdisk/data/USBB/"
#define USB_B_DIR_A 	"/hdisk/data/USBB/A/"
#define USB_B_DIR_B		"/hdisk/data/USBB/B/"
#define USB_B_DIR_C 	"/hdisk/data/USBB/C/"
#define USB_B_DIR_D 	"/hdisk/data/USBB/D/"
#define USB_B_DIR_E 	"/hdisk/data/USBB/E/"
#define USB_B_DIR_F		"/hdisk/data/USBB/F/"
#endif

/* gphotoʶ������Ŀ¼ */
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


/*wifi���*/
#define 	WIFI_INI_PATH				CONFIG_INI_PATH"/wifi"		//wifi�����ļ�
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

/*��Ļ��ɫ����*/
enum
{
	COLOR_YELLOW=0,
	COLOR_BLUE,
	COLOR_RED,
	COLOR_ORANGE,
	COLOR_GREEN,
	COLOR_GRAY
};

/* �ն�������� */
#define MAXNUM_LANG				(3)	/* ������������� */

/* ������İ����ļ��� */
#define HELP_TXT_PATH				"/hdisk/etc/help"
#define HELP_MAIN					HELP_TXT_PATH"/main.txt"		//������
#define HELP_VIDEO					HELP_TXT_PATH"/video.txt"		//��Ƶ�˵�����
#define HELP_INTERNET				HELP_TXT_PATH"/internet.txt"		//�������˵�����
#define HELP_SYSTEM			        HELP_TXT_PATH"/system.txt"	//ϵͳ����
#define HELP_GAME					HELP_TXT_PATH"/game.txt"		//��ϷͼƬ����
#define HELP_PROGRAM 				HELP_TXT_PATH"/program.txt"		//Ӧ�ó���ͼƬ����

#define HELP_VIDEO_LIST 			HELP_TXT_PATH"/video-list.txt"	//��Ƶ-�����б�˵�����
#define HELP_VIDEO_LIST_TYPE 		HELP_TXT_PATH"/video-list-type.txt"		//��Ƶ-�����б�-ӰƬ�⡢�������ء��Զ������-�ļ��б����
#define HELP_VIDEO_LIST_EX 			HELP_TXT_PATH"/video-list-external.txt"	//��Ƶ-�����б�-�ⲿ�豸������
#define HELP_VIDEO_LIST_EX_FILELIST HELP_TXT_PATH"/video-list-external-fileList.txt"	//��Ƶ-�����б�-�ⲿ�豸-�ļ��б����
#define HELP_VIDEO_PLOT				HELP_TXT_PATH"/video-plot.txt"		//��Ƶ-������
#define HELP_VIDEO_COPY				HELP_TXT_PATH"/video-copy.txt"		//��Ƶ-���ƽ���
#define HELP_VIDEO_ONLIME 			HELP_TXT_PATH"/video-online.txt"		//��Ƶ-���߲��Ž���
#define HELP_RADIO					HELP_TXT_PATH"/radio.txt"		//����㲥����
#define HELP_ALBUM					HELP_TXT_PATH"/album.txt"		//���˵�����
#define HELP_ALBUM_ALL				HELP_TXT_PATH"/album-all.txt"	//���-ȫ������
#define HELP_ALBUM_EX 				HELP_TXT_PATH"/album-external.txt"//���-�ⲿ�豸����
#define HELP_ALBUM_EX_FILELIST 		HELP_TXT_PATH"/album-external-fileList.txt"//���-�ⲿ�豸-�ļ��б����
#define HELP_ALBUM_COPY				HELP_TXT_PATH"/album-copy.txt"		//���-���ƽ���
#define HELP_DTV					HELP_TXT_PATH"/dtv.txt"			//���ֵ��Ӳ˵�����

#define HELP_VIDEO_DOWN_STATE 		HELP_TXT_PATH"/video-download-state.txt"//��Ƶ-����״̬����
#define HELP_VIDEO_DOWN_STATE_STAT	HELP_TXT_PATH"/video-download-state-stat.txt"//��Ƶ-����״̬-ͳ�ƽ���
#define HELP_VIDEO_DOWN_STATE_BT 	HELP_TXT_PATH"/video-download-state-bt.txt"	//��Ƶ-����״̬-�������ӽ���
#define HELP_VIDEO_DOWN_STATE_SET 	HELP_TXT_PATH"/video-download-state-set.txt"	//��Ƶ-����״̬-���ý���
#define HELP_DISK					HELP_TXT_PATH"/disk.txt"		//��Դ�������
#define HELP_DISK_KEY				HELP_TXT_PATH"/disk-key.txt"		//��Դ�������
#define HELP_SETTING				HELP_TXT_PATH"/setting.txt"		//���ò˵�����
#define HELP_SETTING_SYSTEM 		HELP_TXT_PATH"/setting-system.txt"	//����-ϵͳ���ý���
#define HELP_SETTING_LAN			HELP_TXT_PATH"/setting-LAN.txt"		//����-LAN���ý���
#define HELP_SETTING_WIFI			HELP_TXT_PATH"/setting-WiFi.txt"	//����-WIFI���ý���
#define HELP_SETTING_UPDATE 		HELP_TXT_PATH"/setting-update.txt"	//����-�����������
#define HELP_SETTING_UPDATE_DETAIL	HELP_TXT_PATH"/setting-update-detail.txt"	//����-��������������
#define HELP_SETTING_INFO			HELP_TXT_PATH"/setting-info.txt"	//����-��Ϣ����
#define HELP_SETTING_REG			HELP_TXT_PATH"/setting-reg.txt"	//����-ע�����
#define HELP_SETTING_GAME			HELP_TXT_PATH"/setting-game.txt"//����-��Ϸע�����
/*���Զ���*/
enum
{
	SIMPLECHINESE = 0,
	TRADITIOANLCHINESE,
	ENGLISH,

	LANGUAGE_MAX
};

/*����״̬*/
enum
{
	STATUS_DISCONNECT = 0,
	STATUS_CONNECTED,
	STATUS_CONNECTING
};

/*�ֱ��ʱ�־*/
enum
{
	TVMODE_720P = 0,
	TVMODE_1080P,
};

/*board chip��־*/
enum
{
	INTEL_CE3100 = 0,
	INTEL_CE4110_NOR,
	INTEL_CE4110_NAND,
	INTEL_CE4130_NAND,
	TRIDENT,
};

/*cpu type��־*/
enum
{
	H3 = 0,
	H3D,
	H3E,
	H3F,
	H4D,
	H4F,
};

/*harddisk type��־*/
enum
{
	HAVE_HARDDISK = 0,
	NO_HARDDISK,
};

//�豸����
#define LM95235					"/dev/lm95235"
#define SLE4442					"/dev/sle4442"

//Ӧ�ó����˳�ʱ�ĵȴ�ʱ��
#define APP_QUIT_WAIT_TIME 0//200000

#ifdef __cplusplus
}
#endif

#endif /* _INTF_DEF_H */
/* END */
