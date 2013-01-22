EXE_NAME   = ktb.exe
SRC_LIST   = ktb.cpp twininfo.cpp ktbtimer.c ktbhelp.c $(LIB_DIR)/tstring.cpp $(LIB_DIR)/tcollect.cpp
A_LIST     = ktbhook.a
LIB_LIST   = ktbhook.lib

.INCLUDE : "makefile.inc"

ktb.o : ktb.h

twininfo.o : twininfo.h

ktbtimer.o : ktbtimer.h

ktbhelp.o  : ktbhelp.h

$(LIB_DIR)/tstring.o : $(INC_DIR)/ktype.h $(INC_DIR)/tstring.h

$(LIB_DIR)/tcollect.o : $(INC_DIR)/tcollect.h

ktbhook.a   : ktbhook.dll ktbhook.def

ktbhook.lib : ktbhook.dll ktbhook.def
    $(IMP) -o $@ ktbhook.def

ktbhook.dll : ktbhook.c ktbhook.def
    $(CC) $(LFDLL) $&

ktbhook.c : ktbhook.h

