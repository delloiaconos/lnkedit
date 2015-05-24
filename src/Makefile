CXX=gcc
CXXFLAGS=-fvtable-thunks
OBJS=com.o re.o main.o

all: lnkedit

lnkedit: $(OBJS)
	$(CXX) -o lnkedit $(OBJS)

.cpp.o:
	$(CXX) -c $(CXXFLAGSS) $<
