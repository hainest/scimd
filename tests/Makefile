STD		 = -std=c++11
CXXFLAGS = -Wall -Wextra -Werror -Wsign-compare -Wsign-conversion -Wnarrowing
OPT    	 = -O3
ARCH	 = -m64 -mfpmath=sse

.PHONY: all clean
.DEFAULT_GOAL = all

SRCS := test.cpp
OBJS := $(patsubst %.cpp, %.o, $(SRCS))
EXEC := test

fma: ARCH += -mfma
fma: all

sse: ARCH += -msse4.2
sse: all

avx: ARCH += -mavx
avx: all

knl: ARCH += $(if $(findstring icc, $(CXX)), -xMIC-AVX512, -march=knl)
knl: all

scalar: ARCH += -DCMK_SIMD_DISABLE_AVX -DCMK_SIMD_DISABLE_SSE -DCMK_SIMD_DISABLE_KNL
scalar: all

debug: SANITIZER	:= address
debug: LDFLAGS		:= -fuse-ld=gold
debug: OPT 	   := -O0 -g -fno-omit-frame-pointer -fsanitize=$(SANITIZER)
debug: LNFLAGS := -fsanitize=$(SANITIZER)
debug: all

all: $(EXEC)

%.o : %.cpp Makefile
	@ $(CXX) $(STD) -I.. $(CXXFLAGS) $(OPT) $(ARCH) $(OMP) -c -o $@ $<

$(EXEC) : $(OBJS)
	@ $(CXX) -o $@ $< $(LNFLAGS) $(OMP) $(LDFLAGS)

.PHONY: example
example:
	$(CXX) $(CXXFLAGS) $(STD) $(ARCH) $(OPT) -c -o example.o example.cpp
	objdump -D -M intel example.o | c++filt > example.asm

clean:
	@ rm -f $(OBJS) $(EXEC)

dump:
	@ objdump -D -j .text -M intel $(EXEC) | c++filt > $(EXEC).asm
