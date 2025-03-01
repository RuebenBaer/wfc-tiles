CXX = g++
CC = clang
CPPFLAGS =  \
	-Wall \
	-Werror \
	-Wpedantic \
	-pipe \
	-mthreads \
	-O2 \
	-std=gnu++11

INCLUDES =  \
	-I$(WXINCLUDE) \
	-I$(WXINCLUDESETUP)

DEFS =  \
	-D__WXMSW__ \
	-DwxUSE_UNICODE

LIBS = \
	-lkernel32 \
	-luser32 \
	-lgdi32 \
	-lcomdlg32 \
	-lwinspool \
	-lwinmm \
	-lshell32 \
	-lshlwapi \
	-lcomctl32 \
	-lole32 \
	-loleaut32 \
	-luuid \
	-lrpcrt4 \
	-ladvapi32 \
	-lversion \
	-lws2_32 \
	-lwininet \
	-loleacc \
	-luxtheme

include wxLib.lst

OBJDIR = obj
SRCDIR = src
BINDIR = bin
BIN = $(BINDIR)\wfc.exe
OBJ = \
	$(OBJDIR)\wfc_base.o \
	$(OBJDIR)\queue.o \
	$(OBJDIR)\wfc\wfc-core.o \
	$(OBJDIR)\resource.res
	

.PHONEY: clean all

all: $(OBJ)
	if not exist $(BINDIR) mkdir $(BINDIR)
	$(CXX) $(CPPFLAGS) $(DEFS) -L$(WXLIBPATH) -o $(BIN) $(OBJ) -s $(LIBS)

$(OBJDIR)\wfc_base.o: $(SRCDIR)\wfc_base.cpp $(SRCDIR)\wfc_base.h
	if not exist $(OBJDIR) mkdir $(OBJDIR)
	$(CXX) $(CPPFLAGS) $(DEFS) $(INCLUDES) -c $< -o $@
	
$(OBJDIR)\queue.o: $(SRCDIR)\queue.c
	if not exist $(OBJDIR) mkdir $(OBJDIR)
	$(CC) -c $< -o $@

$(OBJDIR)\wfc\wfc-core.o: $(SRCDIR)\wfc\wfc-core.c $(SRCDIR)\wfc\wfc-core.h
	if not exist $(OBJDIR)\wfc mkdir $(OBJDIR)\wfc
	$(CC) -c $< -o $@

$(OBJDIR)\resource.res: $(SRCDIR)\resource.rc
	if not exist $(OBJDIR) mkdir $(OBJDIR)
	windres.exe $(INCLUDES) -J rc -O coff -i $(SRCDIR)\resource.rc -o $(OBJDIR)\resource.res

clean:
	del /s /q $(OBJDIR)
	del /s /q $(BIN)