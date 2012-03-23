/*
 * a wrapper for dbus, with the tnMessage interface
 * Author: Pan, Shi Zhu
 */

#ifndef __DBUS_MSG_H
#define __DBUS_MSG_H

#include <stdlib.h>
#include <dbus/dbus-glib.h>

#ifdef __cplusplus
extern "C" {
#ifndef __cplusplus
};
#endif
#endif

static GMainLoop *s_mainloop = NULL;

int tnLoopInit(int need_multi_thread)
{
        g_type_init();
        s_mainloop = g_main_loop_new(NULL, FALSE);
   
        if (need_multi_thread) {
                dbus_g_thread_init();
        }   
        return 0;
}

int tnLoopRun(void)
{
        if (s_mainloop != NULL)
                g_main_loop_run(s_mainloop);
        return 0;
}

/* 模块名称 sender 和 target 最大长度为 64-1 */
#define MAX_MODULE_NAME_LEN 64

/* 此处 data 必须声明为 const */
typedef int (*MessageReceiveFunc)(char *sender, unsigned code, size_t len, const char *data);

/* 成功返回0，如果需要多线程使用，请先调用 dbus_g_thread_init(); */
int tnMessageInit(const char *name, MessageReceiveFunc receive_pfunc); 

/* 同步调用，返回被调用方法的返回值，被调用方法执行完毕后才返回 */
int tnMessageSend_sync(const char *target, unsigned code, size_t len, const char *data); 

/* 异步调用，立即返回，成功返回 0 */
int tnMessageSend(const char *target, unsigned code, size_t len, const char *data); 

/* 检查特定的模块是否已经注册（一般意味着他们是否启动） */
// >0 module exists in memory
// =0 module does not exists in memory
// <0 error
int tnModuleExists(const char *name);

#ifdef __cplusplus
#ifndef __cplusplus
{
#endif
};
#endif

#endif
