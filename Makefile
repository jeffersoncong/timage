all:gm_test
.PHONY:all

gm_test: lua_timage.o
	gcc -o gm_test lua_timage.o gm_timage.c `GraphicsMagick-config --cppflags --ldflags --libs` `pkg-config --libs --cflags lua5.1`

lua_timage.o:
	gcc -c lua_timage.c `pkg-config --cflags lua5.1`

#lua_test:
#	gcc -o lua_test lua_timage.c `pkg-config --libs --cflags lua5.1`

clean:
	rm -f gm_test lua_test *.o
