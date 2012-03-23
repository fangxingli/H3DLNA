/*****************************************************************
|
|   Platinum - HTTP Server
|
| Copyright (c) 2004-2010, Plutinosoft, LLC.
| All rights reserved.
| http://www.plutinosoft.com
|
| This program is free software; you can redistribute it and/or
| modify it under the terms of the GNU General Public License
| as published by the Free Software Foundation; either version 2
| of the License, or (at your option) any later version.
|
| OEMs, ISVs, VARs and other distributors that combine and 
| distribute commercially licensed software with Platinum software
| and do not wish to distribute the source code for the commercially
| licensed software under version 2, or (at your option) any later
| version, of the GNU General Public License (the "GPL") must enter
| into a commercial license agreement with Plutinosoft, LLC.
| 
| This program is distributed in the hope that it will be useful,
| but WITHOUT ANY WARRANTY; without even the implied warranty of
| MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
| GNU General Public License for more details.
|
| You should have received a copy of the GNU General Public License
| along with this program; see the file LICENSE.txt. If not, write to
| the Free Software Foundation, Inc., 
| 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
| http://www.gnu.org/licenses/gpl-2.0.html
|
****************************************************************/

/*----------------------------------------------------------------------
|   includes
+---------------------------------------------------------------------*/
#include "PltTaskManager.h"
#include "PltHttpServer.h"
#include "PltHttp.h"
#include "PltVersion.h"
#include "PltUPnPHelper.h"

NPT_SET_LOCAL_LOGGER("platinum.core.http.server")

/*----------------------------------------------------------------------
|   PLT_HttpServer::PLT_HttpServer
+---------------------------------------------------------------------*/
PLT_HttpServer::PLT_HttpServer(NPT_IpAddress address,
                               unsigned int  port,
                               bool          allow_random_port_on_bind_failure,   /* = false */
                               NPT_Cardinal  max_clients,                         /* = 0 */
                               bool          reuse_address) :                     /* = false */
    m_TaskManager(new PLT_TaskManager(max_clients)),
    m_Address(address),
    m_Port(port),
    m_AllowRandomPortOnBindFailure(allow_random_port_on_bind_failure),
    m_ReuseAddress(reuse_address),
    m_HttpListenTask(NULL)
{
}

/*----------------------------------------------------------------------
|   PLT_HttpServer::~PLT_HttpServer
+---------------------------------------------------------------------*/
PLT_HttpServer::~PLT_HttpServer()
{ 
    Stop();
    delete m_TaskManager;
    m_RequestHandlers.Apply(NPT_ObjectDeleter<NPT_HttpRequestHandler>());
}

/*----------------------------------------------------------------------
|   PLT_HttpServer::Start
+---------------------------------------------------------------------*/
NPT_Result
PLT_HttpServer::Start()
{
    NPT_Result res = NPT_FAILURE;
    
    // if we're given a port for our http server, try it
    if (m_Port) {
        res = SetListenPort(m_Port, m_ReuseAddress);
        // return right away if failed and not allowed to try again randomly
        if (NPT_FAILED(res) && !m_AllowRandomPortOnBindFailure) {
            NPT_CHECK_SEVERE(res);
        }
    }
    
    // try random port now
    if (!m_Port || NPT_FAILED(res)) {
        // randomly try a port for our http server
        int retries = 100;
        do {    
            int random = NPT_System::GetRandomInteger();
            int port = (unsigned short)(50000 + (random % 15000));
            if (NPT_SUCCEEDED(SetListenPort(port, m_ReuseAddress))) {
                break;
            }
        } while (--retries > 0);

        if (retries == 0) NPT_CHECK_SEVERE(NPT_FAILURE);
    }

    m_Port = m_BoundPort;
    
    // start a task to listen
    m_HttpListenTask = new PLT_HttpListenTask(this, &m_Socket, false);
    m_TaskManager->StartTask(m_HttpListenTask, NULL, false);

    NPT_SocketInfo info;
    m_Socket.GetInfo(info);
    NPT_LOG_INFO_2("HttpServer listening on %s:%d", 
        (const char*)info.local_address.GetIpAddress().ToString(), 
        m_Port);
    return NPT_SUCCESS;
}

/*----------------------------------------------------------------------
|   PLT_HttpServer::Stop
+---------------------------------------------------------------------*/
NPT_Result
PLT_HttpServer::Stop()
{
    if (m_HttpListenTask) {
        m_HttpListenTask->Kill();
        m_HttpListenTask = NULL;
    }

    // stop all other pending tasks 
    m_TaskManager->StopAllTasks();
    return NPT_SUCCESS;
}

/*----------------------------------------------------------------------
|   PLT_HttpServer::AddRequestHandler
+---------------------------------------------------------------------*/
NPT_Result 
PLT_HttpServer::AddRequestHandler(NPT_HttpRequestHandler* handler, 
                                  const char*             path, 
                                  bool                    include_children  /* = false */)
{
    m_RequestHandlers.Add(handler);
    return NPT_HttpServer::AddRequestHandler(handler, path, include_children);
}

/*----------------------------------------------------------------------
|   PLT_HttpServer::SetupResponse
+---------------------------------------------------------------------*/
NPT_Result 
PLT_HttpServer::SetupResponse(NPT_HttpRequest&              request, 
                              const NPT_HttpRequestContext& context,
                              NPT_HttpResponse&             response) 
{
    NPT_LOG_INFO_3("Received %s request from %s for \"%s\"", 
        (const char*) request.GetMethod(),
        (const char*) context.GetRemoteAddress().ToString(),
        (const char*) request.GetUrl().GetPath());
    PLT_LOG_HTTP_MESSAGE(NPT_LOG_LEVEL_FINER, &request);

    NPT_List<NPT_HttpRequestHandler*> handlers = FindRequestHandlers(request);
    if (handlers.GetItemCount() == 0) return NPT_ERROR_NO_SUCH_ITEM;
    
    PLT_UPnPMessageHelper::SetServer(response, NPT_HttpServer::m_ServerHeader, false);

    // ask the handler to setup the response
    return (*handlers.GetFirstItem())->SetupResponse(request, context, response);
}

/*----------------------------------------------------------------------
|   PLT_HttpServer::GetMimeType
+---------------------------------------------------------------------*/
const char* 
PLT_HttpServer::GetMimeType(const NPT_String& filename)
{
    int last_dot = filename.ReverseFind('.');
    if (last_dot > 0) {
        NPT_String extension = filename.GetChars()+last_dot+1;
        extension.MakeLowercase();

        for (unsigned int i=0; i<NPT_ARRAY_SIZE(NPT_HttpFileRequestHandler_DefaultFileTypeMap); i++) {
            if (extension == NPT_HttpFileRequestHandler_DefaultFileTypeMap[i].extension) {
                return NPT_HttpFileRequestHandler_DefaultFileTypeMap[i].mime_type;
            }
        }
    }

    return "application/octet-stream";
}

/*----------------------------------------------------------------------
|   PLT_HttpServer::ServeFile
+---------------------------------------------------------------------*/
NPT_Result 
PLT_HttpServer::ServeFile(const NPT_HttpRequest& request,
                          NPT_HttpResponse&      response,
                          NPT_String             file_path) 
{
    NPT_LargeSize            total_len;
    NPT_InputStreamReference stream;
    NPT_File                 file(file_path);
    NPT_FileInfo             file_info;

    // prevent hackers from accessing files outside of our root
    if ((file_path.Find("/..") >= 0) || (file_path.Find("\\..") >= 0) ||
        NPT_FAILED(NPT_File::GetInfo(file_path, &file_info))) {
        return NPT_ERROR_NO_SUCH_ITEM;
    }
    
    // set date
    NPT_TimeStamp now;
    NPT_System::GetCurrentTimeStamp(now);
    response.GetHeaders().SetHeader("Date", 
        NPT_DateTime(now).ToString(NPT_DateTime::FORMAT_RFC_1123), true);
    
    // extract range request info
    NPT_Position start, end;
    PLT_HttpHelper::GetRange(request, start, end);
    
    // handle potential 304 only if ranges not set
    NPT_DateTime  date;
    NPT_TimeStamp timestamp;
    if (NPT_SUCCEEDED(PLT_UPnPMessageHelper::GetIfModifiedSince((NPT_HttpMessage&)request, date)) &&
        start == (NPT_Position)-1 && end == (NPT_Position)-1) {
        date.ToTimeStamp(timestamp);
        
        NPT_LOG_INFO_5("File %s timestamps: request=%d (%s) vs file=%d (%s)", 
            (const char*)request.GetUrl().GetPath(),
            (NPT_UInt32)timestamp.ToSeconds(),
            (const char*)date.ToString(),
            (NPT_UInt32)file_info.m_ModificationTime,
            (const char*)NPT_DateTime(file_info.m_ModificationTime).ToString());
                       
        if (timestamp >= file_info.m_ModificationTime) {
            // it's a match
            NPT_LOG_FINE_1("Returning 304 for %s", request.GetUrl().GetPath().GetChars());
            response.SetStatus(304, "Not Modified", NPT_HTTP_PROTOCOL_1_1);
            return NPT_SUCCESS;
        }
    }

    // open file
    if (NPT_FAILED(file.Open(NPT_FILE_OPEN_MODE_READ)) || 
        NPT_FAILED(file.GetInputStream(stream))        ||
        stream.IsNull()								   ||
        NPT_FAILED(stream->GetSize(total_len))) {
        return NPT_ERROR_NO_SUCH_ITEM;
    }

    // get entity
    NPT_HttpEntity* entity = response.GetEntity();
    NPT_CHECK_POINTER_FATAL(entity);

    // set the content type if we can
    entity->SetContentType(GetMimeType(file_path));
    
    // set Last-Modified and Cache-Control headers
    if (file_info.m_ModificationTime) {
        NPT_DateTime last_modified = NPT_DateTime(file_info.m_ModificationTime);
        response.GetHeaders().SetHeader("Last-Modified", last_modified.ToString(NPT_DateTime::FORMAT_RFC_1123), true);
        response.GetHeaders().SetHeader("Cache-Control", "max-age=0,must-revalidate", true);
    }

    // see if it was a byte range request
    if (start != (NPT_Position)-1 || end != (NPT_Position)-1) {
        NPT_Position start_offset = (NPT_Position)-1, end_offset = total_len-1, len;
        if (start == (NPT_Position)-1 && end != (NPT_Position)-1) {
            // we are asked for the last N=end bytes
            // adjust start according to total length
            if (end >= total_len) {
                start_offset = 0;
            } else {
                start_offset = total_len-end;
            }
        } else if (start != (NPT_Position)-1) {
            start_offset = start;
            if (end != (NPT_Position)-1) {
                if (end < start) {
                    // if the end is specified but incorrect
                    // set the end_offset in order to generate a bad response
                    end_offset = (NPT_Position)-1;
                } else if (end - start < total_len) {
                    end_offset = end;
                }
            }
        }

        // in case the range request was invalid or we can't seek then respond appropriately
        if (start_offset == (NPT_Position)-1 || end_offset == (NPT_Position)-1 || 
            start_offset > end_offset || NPT_FAILED(stream->Seek(start_offset))) {
            response.SetStatus(416, "Requested range not satisfiable");
        } else {
            len = end_offset - start_offset + 1;
            response.SetStatus(206, "Partial Content");
            PLT_HttpHelper::SetContentRange(response, start_offset, end_offset, total_len);

            entity->SetInputStream(stream, false);
            entity->SetContentLength(len);
        }
    } else {
        entity->SetInputStream(stream, true);
    }
    
    return NPT_SUCCESS;
}
