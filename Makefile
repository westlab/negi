#$Id: Makefile,v 5.36 2012-08-22 06:20:41 sin Exp $
#
#------------------------------------------------------
# Makefile for C/C++ Program
#------------------------------------------------------
# Target: a.out
# Authour: H. Watanabe
#------------------------------------------------------

TARGET=negi

#------------------------------------------------------
# Default Parameters
#------------------------------------------------------

CC=g++
#OPT=-Wall -O9
#OPT=-Wall -g -ggdb -pg -O9 -fno-inline
#OPT=-Wall -m32
#OPT=-Wall -g -ggdb -O0 -fno-inline
#OPT=-Wall -g -ggdb -pg -fpermissive -Wunused-but-set-variable
OPT=-Wall -g
INC=
#LIB=-lpqxx -lpcap -pthread -lboost_thread -lz
LIB=-lpqxx -lpcap -lz
#LIB= -pthread -lpcap -lz

#------------------------------------------------------
# Compile Option
#------------------------------------------------------

-include makefile.opt

#------------------------------------------------------
# Definition
#------------------------------------------------------

.SUFFIXES:.C .H .c .o .h

#---
# Source Files
#---

SRC=$(shell ls *.C)
HED=$(shell ls *.H)
OBJ=$(SRC:.C=.o)

#------------------------------------------------------
# rules
#------------------------------------------------------

all: $(TARGET)
$(TARGET): $(OBJ)
	$(CC) $(OPT) -o $(TARGET) $(OBJ) $(LIB)

.c.o:
	$(CC) $(OPT) -c $< $(INC)
.cc.o:
	$(CC) $(OPT) -c $< $(INC)

.C.o:
	$(CC) $(OPT) -c $< $(INC)

dep:
	g++ -MM -MG $(SRC) >makefile.depend

clean:
	rm -f $(TARGET) $(TARGET).exe
	rm -f *.o *.obj
	rm -f *~ *.~*

grind:
	time valgrind --tool=callgrind ./srim config/pcap.conf

tar:
	tar cvzf $(TARGET).tar.gz *.cc *.h makefile

trunc:
	psql -h localhost -Utest testdb -c "truncate table save_stream; truncate table save_result; select setval('save_result_id_seq',1);select setval('save_data_id_seq',1);"

debug:
	make trunc
	make
	./srim srim.conf.debug

tag:
	ctags $(SRC) $(HED)

#--------------------------------------------------
-include makefile.depend

# DO NOT DELETE
