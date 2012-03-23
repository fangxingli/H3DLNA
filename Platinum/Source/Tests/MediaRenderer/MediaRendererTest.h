/*----------------------------------------------------------------------
|   includes
+---------------------------------------------------------------------*/
#include "PltUPnP.h"
#include "PltMediaRenderer.h"

#include <stdlib.h>

#define ROUTON

#ifdef ROUTON
#include <unistd.h>
#include "xcmsg.h"
#include "intfWebtv.h"
NPT_SET_LOCAL_LOGGER("platinum.media.renderer")
#define Float2Time(__f, __buf) sprintf(__buf, "%d:%.2d:%.2d", ((int)__f)/3600, ((int)__f)%3600/60, ((int)__f)%60)
#define SendAndWaitMessage(__m, __module_id) do{     							\
                                	tnMessageSend(__module_id, (__m), 0, NULL);         \
                                	while( m_MessageReceiveFlag != true );                  \
                                	m_MessageReceiveFlag = false;                           \
                      		}while(0)
#endif

enum{
        MSG_DESK_START_APPLE_PLAY = 237,
        MSG_DESK_START_APPLE_PLAY_RESULT = 238,
        MSG_DESK_STOP_APPLE_PLAY = 239,
        MSG_DESK_STOP_APPLE_PLAY_RESULT = 240 
};

#ifdef ROUTON
/*
 * OnSet*: Server=>Renderer=>Player action->GetArgumentValue ( ArgumentDirction: In  )
 * OnGet*: Player=>Renderer=>Server action->SetArugmentValue ( ArgumentDirction: Out  )
 */

class PLT_ROUTON_H3 : public PLT_MediaRenderer
{
public:
	PLT_ROUTON_H3(const char * friendly_name, bool show_ip=false, const char *uuid=NULL, unsigned int port=0, bool port_rebind=false); 
	
	~PLT_ROUTON_H3();
	// Connection Manager
	// NPT_Result OnGetCurrentConnectionInfo(PLT_ActionReference &action) { }

	// AVTransport
	NPT_Result OnGetPositionInfo(PLT_ActionReference& action);

	NPT_Result OnGetMediaInfo(PLT_ActionReference& action);

	NPT_Result OnGetTransportInfo(PLT_ActionReference& action);

	NPT_Result OnSetAVTransportURI(PLT_ActionReference& action);

	NPT_Result OnNext(PLT_ActionReference &action);

	NPT_Result OnPause(PLT_ActionReference &action);
	/*
	 *  GetProtocolInfo->Stop->GetVolume->GetMute->GetTransportInfo->Stop->SetAVTransportURI->Play
	 */
	NPT_Result OnPlay(PLT_ActionReference &action);

	NPT_Result OnPrevious(PLT_ActionReference &action);

	NPT_Result OnSeek(PLT_ActionReference &action);
	/*
	 * 
	 */
	NPT_Result OnStop(PLT_ActionReference &action);
	//NPT_Result OnSetPlayMode(PLT_ActionReference &action){ return NPT_SUCCESS;  }
	
	// Rendering Control
#if 1
	NPT_Result OnGetVolume(PLT_ActionReference &action);
	NPT_Result OnGetMute(PLT_ActionReference &action);
#endif
	NPT_Result OnSetVolume(PLT_ActionReference &action);
	NPT_Result OnSetVolumeDB(PLT_ActionReference &action);
	NPT_Result OnGetVolumeDBRange(PLT_ActionReference &action);
	NPT_Result OnSetMute(PLT_ActionReference &action);

	bool m_MessageReceiveFlag;
	float m_Duration;
	float m_Position;
	int m_RunningRet;// 1: is Running or 0
	int m_PlayingRet;// 1: is Playing or 0	
	int m_WebtvOnRet;// 0: is On	  or 1
	int m_Volume;
	int m_Mute;// 0: not mute	  or 1
private:
	// It assume that these is only one MediaRenderer object from begin to end, so no need the ##static## key word
		
};
class PLT_ROUTON_H3 *h3_renderer = NULL;
#endif
