/******************************************************************************
* intfMntn.h - MNTNģ���ⲿ�ӿ�����ͷ�ļ�
*
* Copyright 1994-2008 Routon Electronic Co.,Ltd.
*
* modification history:
* -----------------------------------------------
* �汾  ����      ����      ˵��
* -----------------------------------------------
* 01a, 2009-01-15, tongjia, 	Create
******************************************************************************/

#ifndef _INTF_WEBTV_H
#define _INTF_WEBTV_H

#ifdef __cplusplus
extern "C" {
#endif   /*__cplusplus */

/* Include */
#include "intfDef.h"

/* Macro define */


/* ά��ģ����Ϣ���� */
enum
{
	MSG_ONLINE_ENTER_PLAY=MSG_BASE_WEBTV,				/* �������߲����� */
	MSG_ONLINE_EXIT_PLAY,				/* �˳����߲����� */
	MSG_ONLINE_SEND_DATA,
	MSG_LIBWEBTV_QUIT_LAST,

// FOR THE USE OF DLNA AND AIRPLAY
	MSG_ROUTON_MPLAYER_START,
	MSG_ROUTON_PAUSE,
	MSG_ROUTON_PLAY,
	MSG_ROUTON_STOP,
	MSG_ROUTON_ADD_TO_PLAYLIST,
	MSG_ROUTON_SET_SCRUB,
	MSG_ROUTON_GET_SCRUB,
	MSG_ROUTON_GET_SCRUB_RET,
	MSG_ROUTON_MPLAYER_IS_RUNNING,
	MSG_ROUTON_MPLAYER_IS_RUNNING_RET,
	MSG_ROUTON_IS_PLAYING,
	MSG_ROUTON_IS_PLAYING_RET,
	MSG_ROUTON_SET_VOLUME,
	MSG_ROUTON_ESC,
	MSG_ROUTON_AIRPLAYER_START,

	
};



/* Typedef declaration */

/* Global declaration */

/* Function declaration */

/* Static Variable define */


#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif	/* _INTF_WEBTV_H */
