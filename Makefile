STD		 = -std=c++0x
CXXFLAGS = -Wall -Wextra
OPT    	 = -O3
ARCH	 = -m64 -mfpmath=sse

.PHONY: all clean
.DEFAULT_GOAL = all

SRCS := simd_test.cpp
OBJS := $(patsubst %.cpp, %.o, $(SRCS))
EXEC := test

sse: CXXFLAGS += -DCMK_USE_SSE2 -msse4
sse: all

avx: CXXFLAGS += -DCMK_USE_AVX -mavx
avx: all

debug: SANITIZER	:= address
debug: LDFLAGS		:= -fuse-ld=gold
debug: OPT 	   := -O0 -g -fno-omit-frame-pointer -fsanitize=$(SANITIZER)
debug: LNFLAGS := -fsanitize=$(SANITIZER)
debug: all

all: $(EXEC)

%.o : %.cpp Makefile
	@$(CXX) $(STD) $(CXXFLAGS) $(OPT) $(ARCH) $(OMP) -c -o $@ $<

$(EXEC) : $(OBJS)
	@$(CXX) -o $@ $< $(LNFLAGS) $(OMP) $(LDFLAGS)

clean:
	@ rm -f $(OBJS) $(EXEC)
