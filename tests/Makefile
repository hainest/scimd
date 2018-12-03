cxx_std   := -std=c++11
cxx_flags := -Wall -Wextra -Wsign-compare -Wsign-conversion -Wnarrowing $($(cxx_flags))
opt       := -O3
arch      := -m64 -mfpmath=sse

quiet := $(if $(filter $(VERBOSE),1),,@)

target := test

.PHONY: all clean
.DEFAULT_GOAL = $(target)

fma: arch += -mfma
fma: $(target)

sse: arch += -msse4.2
sse: $(target)

avx: arch += -mavx
avx: $(target)

avx512: arch += $(if $(findstring icc, $(CXX)), -xMIC-AVX512, -march=knl)
avx512: $(target)

scalar: defines := -DSCIMD_DISABLE_AVX -DSCIMD_DISABLE_SSE -DSCIMD_DISABLE_AVX512 $(CPPFLAGS)
scalar: arch += -march=x86-64
scalar: $(target)

# Force a rebuild if the compiler flags have changed
.PHONY: force
settings: force
	$(quiet) flags="$(cxx_std) $(cxx_flags) $(opt) $(arch) $(defines)" && \
		echo "$$flags" | cmp -s - $@ || echo "$$flags" > $@

all: $(target)

# The driver doesn't need to know about the architecture used
# This ensures it's not rebuilt on arch changes
driver_$(CXX).o : driver.cpp Makefile
	$(quiet) $(CXX) $(cxx_std) -I.. $(defines) $(cxx_flags) $(opt) -c -o $@ $<

%.o : %.cpp Makefile settings
	$(quiet) $(CXX) $(cxx_std) -I.. $(defines) $(cxx_flags) $(opt) $(arch) -c -o $@ $<

$(target) : test.o driver_$(CXX).o
	$(quiet) $(CXX) -o $@ $^

clean:
	$(quiet) rm -f test.o $(target)

dist-clean: clean
	$(quiet) rm -f driver*.o

dump:
	$(quiet) objdump -D -j .text -M intel $(target) | c++filt > $(target)_$(CXX).asm
