#CXX 	   	 = clang++
CXX 	   	 = g++
#CXX			 = icc
STD			 = -std=c++0x
CXXFLAGS   	 = -Wall -Wextra #-Wshadow -Wnarrowing #-Wodr
OPT 	   	 = -O3
#ARCH		 = -xhost=native
ARCH		 = -m64 -mfpmath=sse -march=native #-flto
#OMP		:= -fopenmp

.PHONY: all clean
.DEFAULT_GOAL = all

SRCS := test.cpp
OBJS := $(patsubst %.cpp, %.o, $(SRCS))
EXEC := test

debug: SANITIZER	:= address
debug: LDFLAGS		:= -fuse-ld=gold
debug: OPT 	   := -O0 -g -fno-omit-frame-pointer -fsanitize=$(SANITIZER)
debug: LNFLAGS := -fsanitize=$(SANITIZER)
debug: all

all: $(EXEC)

%.o : %.cpp Makefile
	$(CXX) $(STD) $(CXXFLAGS) $(OPT) $(ARCH) $(OMP) -c -o $@ $<

$(EXEC) : $(OBJS)
	$(CXX) -o $@ $< $(LNFLAGS) $(OMP) $(LDFLAGS)

clean:
	@ rm -f $(OBJS) $(EXEC)
