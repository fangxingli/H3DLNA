srcs=main.c mainMenu.c add_a_friend.c image.c settings.c friendlist.c  chat_window.c friend_setting.c common_data_handler.c dbus_intf.c animate_effect.c window_status_machine.c btmsgproc.c dialing.c bubble_tips.c login_status_machine.c call_status_machine.c

CFLAGS=-O2 -Wall -g -Wfatal-errors -std=gnu99 -Wno-unused-result -I../h3intf
LDFLAGS=-Wl,-E

mods=clutter-1.0 mx-1.0 dbus-glib-1 clutter-imcontext-0.1
pkgpath=/home/jinglun/os/h3F/others/clutter/out/lib/pkgconfig

CFLAGS+=$(shell PKG_CONFIG_PATH=$(pkgpath) pkg-config $(mods) --cflags)

LDFLAGS+=$(shell PKG_CONFIG_PATH=$(pkgpath) pkg-config $(mods) --libs)
LDFLAGS+= -L/usr/lib -lng-toolkit

objs=$(patsubst %.c,%.o,$(srcs))

all: main

main: $(objs)
	gcc $^ $(LDFLAGS) -o $@

depend:
	makedepend -- $(CFLAGS) -- $(srcs)

install: main
	mkdir -p /hdisk/data/apps/app0004/bin
	cp -f main /hdisk/data/apps/app0004/bin/h3im_main
	mkdir -p /hdisk/data/apps/app0004/share/images
	cp -f default.css /hdisk/data/apps/app0004/share
	cp -rf images /hdisk/data/apps/app0004/share

clean:
	rm -f *.o
	rm -f main

# DO NOT DELETE THIS LINE -- make depend depends on it.


