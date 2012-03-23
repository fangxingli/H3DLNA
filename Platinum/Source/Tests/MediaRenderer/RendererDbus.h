#include <stdio.h>
#include <stdlib.h>
#include <dbus/dbus-glib.h>

class RendererDbus
{
public:
	RendererDbus();
	~RendererDbus();
	int DbusInvoke(DBusGProxy *proxy, const char *method, GError **error, GType e_type, ...);	
	int DbusInit();

private: 
	DBusGProxy *s_bus_proxy;
	DBusGConnection *s_bus;
};

RendererDbus::RendererDbus(){
	s_bus_proxy = NULL;
	s_bus = NULL;
}

int RendererDbus::DbusInvoke(DBusGProxy *proxy, const char *method, GError **error, GType e_type, ...){
	va_list args;

	va_start(args, e_type);
	if ( !dbus_g_proxy_call(proxy, method, error, e_type, args) ){
		printf("Error occur in invoking %s\n", method);
		return -1;	
	}
	va_end(args);
    	return 0;
}

int RendererDbus::DbusInit(){
	GError *error = NULL;
	DBusGProxy *bus_proxy = NULL;

	s_bus = dbus_g_bus_get(DBUS_BUS_SESSION, &error); 
	if (!s_bus) {
		printf("Couldn't connect to system bus\n");
		return -1;
	}
	bus_proxy = dbus_g_proxy_new_for_name(s_bus, "com.routon.h3.Webtv", "/com/routon", "com.routon");//Service, Path, Interface

	s_bus_proxy = bus_proxy;

	return 0;
}

#if 0
int main()
{
	return 0;
}
#endif
