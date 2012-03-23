/*
 * H3 DLNA Renderer
 * Author  	: LiFangxing
 * Version 	: 1.0 
 * Date    	: 2011.10.21
 * Description	: Play, Pause, Stop, Set Volume, Seek, Get position and others action have finished using process invode
 *
 * Version 	: 1.1
 * Date	   	: 2011.11.04
 * Description	: Use Dbus for PIC instead of process invode
 *
 * Version 	: 1.2
 * Date	   	: 2011.11.09
 * Description	: Communicating with h3.Desktop added
 *
 * Version 	: 1.3
 * Date	   	: 2011.11.17
 * Description	: everything is all right for Tencent video except mute
 */
#include "MediaRendererTest.h"

#ifdef ROUTON

/*
 *  Semaphore handler and process
 */ 
int MessageProcess(char *sender, unsigned code, size_t len, const char *data)
{
        char tmp[64]="\0";
        switch(code){
                case MSG_ROUTON_GET_SCRUB_RET:
                        strcpy(tmp, "MSG_ROUTON_GET_SCRUB_RET");
			h3_renderer->m_Duration = ((float *)data)[0];
			h3_renderer->m_Position = ((float *)data)[1];
                        break;
                case MSG_ROUTON_MPLAYER_IS_RUNNING_RET:
                        strcpy(tmp, "MSG_ROUTON_MPLAYER_IS_RUNNING_RET");
			h3_renderer->m_RunningRet = ((int *)data)[0];
                        break;
                case MSG_ROUTON_IS_PLAYING_RET:
                        strcpy(tmp, "MSG_ROUTON_IS_PLAYING_RET");
			h3_renderer->m_PlayingRet = ((int *)data)[0];
                        break;
		case MSG_DESK_START_APPLE_PLAY_RESULT:
                        strcpy(tmp, "MSG_DESK_START_APPLE_PLAY_RESULT");
                        h3_renderer->m_WebtvOnRet = ((int *)data)[0];
			break;
		case MSG_DESK_STOP_APPLE_PLAY_RESULT:
			h3_renderer->m_PlayingRet = 0;
			h3_renderer->m_RunningRet = 0;
			h3_renderer->m_WebtvOnRet = 1;
			h3_renderer->m_Duration = 0.0;
			h3_renderer->m_Position = 0.0;
			h3_renderer->m_Volume   =   0;
			h3_renderer->m_Mute     =   0;
                        strcpy(tmp, "MSG_DESK_STOP_APPLE_PLAY_RESULT");
			break;
                default:
                        break;    
        }   
        printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Sender: (%d)%s code: %s data: %d len: %d\n", code, sender, tmp, *(int *)data, len);
	h3_renderer->m_MessageReceiveFlag = true;
	
        return 0;
}

/*
 * OnSet*: Server=>Renderer=>Player action->GetArgumentValue ( ArgumentDirction: In  )
 * OnGet*: Player=>Renderer=>Server action->SetArugmentValue ( ArgumentDirction: Out  )
 */
PLT_ROUTON_H3::PLT_ROUTON_H3(
		const char * friendly_name, 
		bool show_ip, 
		const char *uuid, 
		unsigned int port, 
		bool port_rebind) : 
	PLT_MediaRenderer(friendly_name, show_ip, uuid, port, port_rebind)
{
	m_MessageReceiveFlag = false;
	m_PlayingRet = 0;
	m_RunningRet = 0;
	m_WebtvOnRet = 1;
	m_Duration = 0.0;
	m_Position = 0.0;
	m_Volume   =   0;
	m_Mute	   =   0;
}
	
	// Connection Manager
	// NPT_Result OnGetCurrentConnectionInfo(PLT_ActionReference &action) { }

	// AVTransport
#if 1
NPT_Result PLT_ROUTON_H3::OnGetMediaInfo(PLT_ActionReference& action)
{
	PLT_Service* serviceAVT;
	NPT_String current_uri, metadata;
	char current_duration[8] = "\0";
	Float2Time(m_Duration, current_duration);
	
	NPT_CHECK_WARNING(FindServiceByType("urn:schemas-upnp-org:service:AVTransport:1", serviceAVT));
	serviceAVT->GetStateVariableValue("AVTransportURI", current_uri);
	serviceAVT->GetStateVariableValue("AVTransportURIMetaData", metadata);
	printf(")))))))))))))))(((((((((((((%s %s\n %s \n", current_duration, current_uri.GetChars(), metadata.GetChars());

	NPT_CHECK_WARNING(action->SetArgumentValue("NrTracks", "1"));
	NPT_CHECK_WARNING(action->SetArgumentValue("MediaDuration", current_duration));
	NPT_CHECK_WARNING(action->SetArgumentValue("CurrentURI", current_uri));
	NPT_CHECK_WARNING(action->SetArgumentValue("CurrentURIMetaData", metadata));
	NPT_CHECK_WARNING(action->SetArgumentValue("NextURI", "NOT_IMPLEMENTED"));
	NPT_CHECK_WARNING(action->SetArgumentValue("NextURIMetaData", "NOT_IMPLEMENTED"));
	NPT_CHECK_WARNING(action->SetArgumentValue("PlayMedium", "NONE"));
	NPT_CHECK_WARNING(action->SetArgumentValue("RecordMedium", "NOT_IMPLEMENTED"));
	NPT_CHECK_WARNING(action->SetArgumentValue("WriteStatus", "NOT_IMPLEMENTED"));
	return NPT_SUCCESS;  
}
#endif

NPT_Result PLT_ROUTON_H3::OnGetTransportInfo(PLT_ActionReference& action)
{
	PLT_Service* serviceAVT;
	NPT_String current_state, current_status;


	NPT_CHECK_WARNING(FindServiceByType("urn:schemas-upnp-org:service:AVTransport:1", serviceAVT));

	serviceAVT->GetStateVariableValue("TransportState", current_state);
	NPT_CHECK_WARNING(action->SetArgumentValue("CurrentTransportState", current_state));

	serviceAVT->GetStateVariableValue("TransportStatus", current_status);
	NPT_CHECK_WARNING(action->SetArgumentValue("CurrentTransportStatus", current_status));

	NPT_CHECK_WARNING(action->SetArgumentValue("CurrentSpeed", "1"));

	return NPT_SUCCESS;  
}

NPT_Result PLT_ROUTON_H3::OnGetPositionInfo(PLT_ActionReference& action)
{  
	char duration_s[8]="\0", position_s[8]="\0";
	NPT_String uri, metadata, current_state;
	PLT_Service* serviceAVT;

	if( m_WebtvOnRet == 0){
		if( m_RunningRet ){
			SendAndWaitMessage(MSG_ROUTON_GET_SCRUB, MODULE_ID_WEBTV);
		}
	}
		
	// if mplayer is not open, then the variables will be default
	Float2Time(m_Duration, duration_s);
	Float2Time(m_Position, position_s);

	NPT_CHECK_WARNING(FindServiceByType("urn:schemas-upnp-org:service:AVTransport:1", serviceAVT));
	serviceAVT->GetStateVariableValue("AVTransportURI", uri);
	serviceAVT->GetStateVariableValue("AVTransportURIMetaData", metadata);

	serviceAVT->GetStateVariableValue("TransportState", current_state);
	//printf("$$$$$$$$$$$$$$$$$$AVTransportURI: %s, AVTransportURIMetaData: %s$$$$$$$$$$$$$$$$$$$$$$$$$$\n", uri.GetChars(), metadata.GetChars());

	printf("^^^^^^^^^^^^^^^^^^^^^^^^^^^^^State: %s, Duration: %f(%s), position: %f(%s)\n",current_state.GetChars(), m_Duration , duration_s, m_Position, position_s);
	NPT_CHECK_WARNING(action->SetArgumentValue("Track", "1"));
	NPT_CHECK_WARNING(action->SetArgumentValue("TrackDuration", duration_s));
	serviceAVT->SetStateVariable("CurrentTrackDuration", duration_s);
	NPT_CHECK_WARNING(action->SetArgumentValue("TrackMetaData", metadata));
	NPT_CHECK_WARNING(action->SetArgumentValue("TrackURI", uri));
	NPT_CHECK_WARNING(action->SetArgumentValue("RelTime", position_s));//one media relative time
	NPT_CHECK_WARNING(action->SetArgumentValue("AbsTime", position_s));//all media time 
	NPT_CHECK_WARNING(action->SetArgumentValue("RelCount", "26333"));//Not implemented
	NPT_CHECK_WARNING(action->SetArgumentValue("AbsCount", "26333"));

	return NPT_SUCCESS;  
}

NPT_Result PLT_ROUTON_H3::OnSetAVTransportURI(PLT_ActionReference& action)
{
	// default implementation is using state variable
	char data[512] = "\0";
	NPT_String uri;
	NPT_CHECK_WARNING(action->GetArgumentValue("CurrentURI", uri));

	NPT_String metadata;
	NPT_CHECK_WARNING(action->GetArgumentValue("CurrentURIMetaData", metadata));

	PLT_Service* serviceAVT;
	NPT_CHECK_WARNING(FindServiceByType("urn:schemas-upnp-org:service:AVTransport:1", serviceAVT));

	/*
	 * update service state variables
	 * NOTE: the next SetState is necessary, because GetPositionInfo will get them
 	 */
	serviceAVT->SetStateVariable("AVTransportURI", uri);
	serviceAVT->SetStateVariable("AVTransportURIMetaData", metadata);
	printf("URI: %s, Length:%d\n", uri.GetChars(), strlen(uri.GetChars()));

	if( m_WebtvOnRet == true ){// 1: webtv is not on
		while(m_WebtvOnRet == true){
			tnMessageSend_sync(MODULE_ID_DESKTOP, MSG_DESK_START_APPLE_PLAY, 0, NULL);
		}
	}
	//NOTE: here must sleep some second(millisecond), or else the message sending to webtv will fail, and I don't why
	sleep(1);
	
	printf("After Send MSG to Desktop\n");
	if(m_RunningRet == false){
		tnMessageSend(MODULE_ID_WEBTV, MSG_ROUTON_MPLAYER_START, 0, NULL);// open the mplayer

		// add play list
		((int *)data)[0] = 1;
		((int *)data)[1] = uri.GetLength();
		sprintf(data+8, "%s", uri.GetChars());
		tnMessageSend(MODULE_ID_WEBTV, MSG_ROUTON_ADD_TO_PLAYLIST, uri.GetLength()+8, data);
		while( m_Duration == 0.0 ){
			SendAndWaitMessage(MSG_ROUTON_GET_SCRUB, MODULE_ID_WEBTV);
		}

		tnMessageSend(MODULE_ID_WEBTV, MSG_ROUTON_MPLAYER_IS_RUNNING, 0, NULL);
	}//make sure that the player is running
	printf("After Send MSG to Webtv and mplayer\n");

	serviceAVT->SetStateVariable("NumberOfTracks", "1");
	serviceAVT->SetStateVariable("TransportState", "PLAYING");
	serviceAVT->SetStateVariable("TransportStatus", "OK");

	return NPT_SUCCESS;
}

NPT_Result PLT_ROUTON_H3::OnNext(PLT_ActionReference &action){ return NPT_SUCCESS; }

NPT_Result PLT_ROUTON_H3::OnPause(PLT_ActionReference &action)
{
	SendAndWaitMessage(MSG_ROUTON_MPLAYER_IS_RUNNING, MODULE_ID_WEBTV);
	if( m_RunningRet == true )
		tnMessageSend(MODULE_ID_WEBTV, MSG_ROUTON_PAUSE, 0, NULL);
	else
		return NPT_SUCCESS;  

	// update service state variables
	PLT_Service* serviceAVT;
	NPT_CHECK_WARNING(FindServiceByType("urn:schemas-upnp-org:service:AVTransport:1", serviceAVT));
	serviceAVT->SetStateVariable("TransportState", "PAUSED_PLAYBACK");
	serviceAVT->SetStateVariable("TransportStatus", "OK");

	return NPT_SUCCESS;  
}

/*
 *  GetProtocolInfo->Stop->GetVolume->GetMute->GetTransportInfo->Stop->SetAVTransportURI->Play
 */
NPT_Result PLT_ROUTON_H3::OnPlay(PLT_ActionReference &action)
{ 
	NPT_String current_state, uri;
	PLT_Service* serviceAVT;

	/*
	 * Both webtv and mplayer states check are locate in OnSetAVTransportURI
	 * So below is useless
 	 */	
#if 0
	while( m_RunningRet == false){
		tnMessageSend(MODULE_ID_WEBTV, MSG_ROUTON_MPLAYER_START, 0, NULL);
		SendAndWaitMessage(MSG_ROUTON_MPLAYER_IS_RUNNING, MODULE_ID_WEBTV);
	}
#endif
	// check the current state 
	NPT_CHECK_WARNING(FindServiceByType("urn:schemas-upnp-org:service:AVTransport:1", serviceAVT));
	serviceAVT->GetStateVariableValue("AVTransportURI", uri);
	serviceAVT->GetStateVariableValue("TransportState", current_state);
	
	if( !strcmp(current_state.GetChars(), "PLAYING") || m_RunningRet == 0 ){
		printf("PlayingRet == 0\n");
		return NPT_SUCCESS;
	}
	else if( !strcmp(current_state.GetChars(), "PAUSED_PLAYBACK") ){
		tnMessageSend(MODULE_ID_WEBTV, MSG_ROUTON_PLAY, 0, NULL);	
		serviceAVT->SetStateVariable("TransportState", "PLAYING");
		serviceAVT->SetStateVariable("TransportStatus", "OK");
		return NPT_SUCCESS;
	}else if( !strcmp(current_state.GetChars(), "STOPPED") ){
		tnMessageSend(MODULE_ID_WEBTV, MSG_ROUTON_PLAY, 0, NULL);	
		serviceAVT->SetStateVariable("TransportState", "PLAYING");
		serviceAVT->SetStateVariable("TransportStatus", "OK");
		return NPT_SUCCESS;

/*
 		((int *)data)[0] = 1;
		((int *)data)[1] = uri.GetLength();
		sprintf(data+8, "%s", uri.GetChars());

		tnMessageSend(MODULE_ID_WEBTV, MSG_ROUTON_ADD_TO_PLAYLIST, uri.GetLength()+8, data);	
		serviceAVT->SetStateVariable("TransportState", "PLAYING");
		serviceAVT->SetStateVariable("TransportStatus", "OK");
		return NPT_SUCCESS;  
*/
	}	
	return NPT_SUCCESS;  
}

NPT_Result PLT_ROUTON_H3::OnPrevious(PLT_ActionReference &action){ return NPT_SUCCESS;  }

NPT_Result PLT_ROUTON_H3::OnSeek(PLT_ActionReference &action)
{
	int h,m,s;
	char data[sizeof(int)] = "\0";
	// update service state variables
	PLT_Service* serviceAVT;

	NPT_CHECK_WARNING(FindServiceByType("urn:schemas-upnp-org:service:AVTransport:1", serviceAVT));
	serviceAVT->SetStateVariable("TransportState", "TRANSITIONING");
	serviceAVT->SetStateVariable("TransportStatus", "OK");

	NPT_String unit;
	NPT_CHECK_WARNING(action->GetArgumentValue("Unit", unit));
	NPT_String target;
	NPT_CHECK_WARNING(action->GetArgumentValue("Target", target));
	printf("$$$$$$$$$$$$$$$$$$Unit: %s, target: %s$$$$$$$$$$$$$$$$$$$$$$$$$$\n", unit.GetChars(), target.GetChars());

	sscanf(target.GetChars(), "%d:%d:%d", &h, &m, &s);
	SendAndWaitMessage(MSG_ROUTON_MPLAYER_IS_RUNNING, MODULE_ID_WEBTV);
	if( m_RunningRet == true ){
		*(int *)data = h*3600+m*60+s;
		tnMessageSend(MODULE_ID_WEBTV, MSG_ROUTON_SET_SCRUB, sizeof(int), data);
		tnMessageSend(MODULE_ID_WEBTV, MSG_ROUTON_PLAY , 0, NULL);
	}

	// update service state variables
	NPT_CHECK_WARNING(FindServiceByType("urn:schemas-upnp-org:service:AVTransport:1", serviceAVT));
	serviceAVT->SetStateVariable("TransportState", "PLAYING");//stateBeforeSeek.GetChars());
	serviceAVT->SetStateVariable("TransportStatus", "OK");

	return NPT_SUCCESS;  
}

/*
 * 
 */
NPT_Result PLT_ROUTON_H3::OnStop(PLT_ActionReference &action)
{
	if( m_WebtvOnRet == 0 ){
		SendAndWaitMessage(MSG_ROUTON_MPLAYER_IS_RUNNING, MODULE_ID_WEBTV);
		if( m_RunningRet == true ){
			printf("in MSG_ROUTON_STOP\n");
			tnMessageSend(MODULE_ID_WEBTV, MSG_ROUTON_STOP, 0, NULL);
			printf("in MSG_ROUTON_ESC\n");
			tnMessageSend(MODULE_ID_WEBTV, MSG_ROUTON_ESC, 0, NULL);
		}
	}

	// update service state variables
	PLT_Service* serviceAVT;
	NPT_String current_state;
	NPT_CHECK_WARNING(FindServiceByType("urn:schemas-upnp-org:service:AVTransport:1", serviceAVT));
	// if current state is NO_MEDIA_PRESENT, then do nothing
	serviceAVT->GetStateVariableValue("TransportState", current_state);
	if( !strcmp(current_state.GetChars(), "NO_MEDIA_PRESENT") )
		return NPT_SUCCESS;  

	// the other states
	serviceAVT->SetStateVariable("TransportState", "NO_MEDIA_PRESENT");
	serviceAVT->SetStateVariable("TransportStatus", "OK");
	
	// set the mplayer, webtv to the default value
	return NPT_SUCCESS;  
}

//NPT_Result OnSetPlayMode(PLT_ActionReference &action){ return NPT_SUCCESS;  }

// Rendering Control
#if 1
NPT_Result PLT_ROUTON_H3::OnGetVolume(PLT_ActionReference &action)
{ 
	/*
	 * Here should get the mplayer volume and return to Controller
	 * We have not the Get Volume Message
	 */
	NPT_CHECK_WARNING(action->SetArgumentValue("CurrentVolume", "1"));

	return NPT_SUCCESS;  
}
NPT_Result PLT_ROUTON_H3::OnGetMute(PLT_ActionReference &action)
{ 
	/*
	 * Here should get the mplayer mute and return to Controller
	 */
	if( m_Mute == 0 )
		NPT_CHECK_WARNING(action->SetArgumentValue("CurrentMute", "0"));
	else
		NPT_CHECK_WARNING(action->SetArgumentValue("CurrentMute", "1"));

	return NPT_SUCCESS;  
}
#endif
NPT_Result PLT_ROUTON_H3::OnSetVolume(PLT_ActionReference &action)
{
	char data[sizeof(int)] = "\0";
	NPT_String volume;
	NPT_CHECK_WARNING(action->GetArgumentValue("DesiredVolume", volume));
	printf("Volume: %s\n", volume.GetChars());

	*(int *)data = atoi(volume.GetChars());
	m_Volume = *(int *)data;
	SendAndWaitMessage(MSG_ROUTON_MPLAYER_IS_RUNNING, MODULE_ID_WEBTV);
	if( m_RunningRet == true )
		tnMessageSend(MODULE_ID_WEBTV, MSG_ROUTON_SET_VOLUME, sizeof(int), data);	
	return NPT_SUCCESS;  
}

NPT_Result PLT_ROUTON_H3::OnSetVolumeDB(PLT_ActionReference &action){ return NPT_SUCCESS;  }
NPT_Result PLT_ROUTON_H3::OnGetVolumeDBRange(PLT_ActionReference &action){ return NPT_SUCCESS;  }
NPT_Result PLT_ROUTON_H3::OnSetMute(PLT_ActionReference &action)
{ 
	// maybe with error
	char data[sizeof(int)] = "\0";
	PLT_Service* serviceAVT;
	NPT_String mute, volume;
	NPT_CHECK_WARNING(FindServiceByType("urn:schemas-upnp-org:service:AVTransport:1", serviceAVT));
	serviceAVT->GetStateVariableValue("Volume", volume);
	printf("$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ CurrentVolume: (%d)%s $$$$$$$$$$$$$$$$$$$$$$$$$$$\n", m_Volume, volume.GetChars());
	
	NPT_CHECK_WARNING(action->GetArgumentValue("DesiredMute", mute));
	SendAndWaitMessage(MSG_ROUTON_MPLAYER_IS_RUNNING, MODULE_ID_WEBTV);
	if( m_RunningRet == true ){
		if( !strcmp(mute.GetChars(), "0") ){// Current State: unMute
			*(int *)data = m_Volume;
			m_Mute = 0;
		}else{	//Current State: Mute
			*(int *)data = 0;//atoi(volume.GetChars());
			m_Mute = 1;
		}
		tnMessageSend(MODULE_ID_WEBTV, MSG_ROUTON_SET_VOLUME, sizeof(int), data);	
	}

	return NPT_SUCCESS;  
}

// By Lee

PLT_ROUTON_H3::~PLT_ROUTON_H3(){}
#endif

/*----------------------------------------------------------------------
|   globals
+---------------------------------------------------------------------*/
struct Options {
    const char* friendly_name;
} Options;

/*----------------------------------------------------------------------
|   PrintUsageAndExit
+---------------------------------------------------------------------*/
static void
PrintUsageAndExit(char** args)
{
    fprintf(stderr, "usage: %s [-f <friendly_name>]\n", args[0]);
    fprintf(stderr, "-f : optional upnp server friendly name\n");
    fprintf(stderr, "<path> : local path to serve\n");
    exit(1);
}

/*----------------------------------------------------------------------
|   ParseCommandLine
+---------------------------------------------------------------------*/
static void
ParseCommandLine(char** args)
{
    const char* arg;
    char**      tmp = args+1;

    /* default values */
    Options.friendly_name = NULL;

    while ((arg = *tmp++)) {
        if (!strcmp(arg, "-f")) {
            Options.friendly_name = *tmp++;
        } else {
            fprintf(stderr, "ERROR: too many arguments\n");
            PrintUsageAndExit(args);
        }
    }
}

/*----------------------------------------------------------------------
|   main
+---------------------------------------------------------------------*/
int
main(int /* argc */, char** argv)
{   
    PLT_UPnP upnp;

    /* parse command line */
    ParseCommandLine(argv);

    tnLoopInit(0);
    tnMessageInit("airplay", MessageProcess);

#ifdef ROUTON
	h3_renderer = new PLT_ROUTON_H3(Options.friendly_name?Options.friendly_name:"Platinum Media Renderer",
                              false,
                              "e6572b54-f3c7-2d91-2fb5-b757f2537e21");
	PLT_DeviceHostReference device(h3_renderer);
#endif
    upnp.AddDevice(device);
    bool added = true;

    upnp.Start();

    tnLoopRun();
#if 1
    char buf[256];
    while (gets(buf)) {
        if (*buf == 'q')
            break;

        if (*buf == 's') {
            if (added) {
                upnp.RemoveDevice(device);
            } else {
                upnp.AddDevice(device);
            }
            added = !added;
        }
    }
#endif

    upnp.Stop();

    return 0;
}
