#pacman -S $MINGW_PACKAGE_PREFIX-base-devel
#pacman -S $MINGW_PACKAGE_PREFIX-gcc
#pacman -S $MINGW_PACKAGE_PREFIX-toolchain
#pacman -S $MINGW_PACKAGE_PREFIX-libsystre
#pacman -S $MINGW_PACKAGE_PREFIX-libuuid

CXX=g++
CXXFLAGS=-fvtable-thunks
# -lregex -std=c++11 -lstdc++ -loleaut32 -lole32 -luuid 

OBJS=com.o re.o main.o

all: lnkedit

lnkedit: $(OBJS)
	$(CXX) -o lnkedit   $(OBJS)
# -loleaut32 -luuid -lole32 

.cpp.o:
	$(CXX) -c $(CXXFLAGSS) $<
