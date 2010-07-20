CC=g++
INSTDIR=$(DESTDIR)/usr/bin
CFLAGS=-Wall -g -DI_AM_A_SUCKER
STRINGLIB=bytevector.o stringlist.o

ccc: headers ccc.o
	$(CC) $(STRINGLIB) texmaker.o ccc.o -o cdcover $(CFLAGS)

ccc.o: ccc.cc texmaker.o stringlist.o bytevector.o 
	$(CC) -c ccc.cc $(CFLAGS)

texmaker.o: texmaker.h texmaker.cc stringlist.o bytevector.o
	$(CC) -c texmaker.cc $(CFLAGS)

# Fix this to just run text2h when the source files change...
headers: text2h
	./text2h ccchelp <ccchelp.txt >ccchelp.h 
	./text2h cccpara <cccpara.txt >cccpara.h
	./text2h cccpara2 <cccpara2.txt >cccpara2.h
	./text2h reshead <reshead.tex >reshead.h
	./text2h resback <resback.tex >resback.h
	./text2h resfront <resfront.tex >resfront.h

text2h: text2h.o bytevector.o stringlist.o 
	$(CC) $(STRINGLIB) text2h.o -o text2h $(CFLAGS)

text2h.o: text2h.cc
	$(CC) -c text2h.cc $(CFLAGS)

bytevector.o: bytevector.h bytevector.cc
	${CC} -c bytevector.cc $(CFLAGS)

stringlist.o: bytevector.o stringlist.h stringlist.cc
	$(CC) -c stringlist.cc $(CFLAGS)

clean:
	rm -f *.o cdcover text2h
	rm -f ccchelp.h cccpara.h cccpara2.h reshead.h resback.h resfront.h

install: ccc
	cp cdcover $(INSTDIR)

