EXE_NAME   = ktb.exe
SRC_LIST   = ktb.cpp twininfo.cpp ktbtimer.c ktbhelp.c $(LIB_DIR)/tstring.cpp $(LIB_DIR)/tcollect.cpp
A_LIST     =
LIB_LIST   = ktbhook.lib

.INCLUDE : "makefile.inc"

ktb.obj : ktb.h

twininfo.obj : twininfo.h

ktbtimer.obj : ktbtimer.h

ktbhelp.obj : ktbhelp.h

$(LIB_DIR)/tstring.obj : $(INC_DIR)/ktype.h $(INC_DIR)/tstring.h

$(LIB_DIR)/tcollect.obj : $(INC_DIR)/tcollect.h

ktbhook.lib : ktbhook.dll ktbhook.def
    $(IMP) -o $@ ktbhook.def

ktbhook.dll : ktbhook.obj ktbhook.def
    $(CC) $(LFDLL) $&

ktbhook.obj : ktbhook.h
