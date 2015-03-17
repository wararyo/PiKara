hello: main.c
	gcc -o pikara main.c libshapes.o oglinit.o -I/opt/vc/include/ -I/opt/vc/include/interface/vcos/pthreads -lbcm_host -L/opt/vc/lib/ -lGLESv2 -ljpeg -lEGL
