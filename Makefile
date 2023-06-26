CC := g++
CFLAGS := -g -std=c++17 -Wall -pedantic -Wno-long-long -Werror -Wextra 

SDLCFLAGS := `sdl2-config --cflags` 
MEMFLAG = -fsanitize=address
LIBS :=  `sdl2-config --libs` -lSDL2 -lSDL2main -lSDL2_image -lSDL2_ttf 

TARGET:= game
SRCS := $(wildcard src/*.cpp)
HDRS := $(wildcard src/*.h)
OBJS = $(SRCS:src/%.cpp=build/%.o)

TESTS := $(wildcard examples/*.cpp)
TEST_TARGETS = $(TESTS:examples/%.test.cpp=debug/%.test)

all:$(TARGET)

compile:$(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o yoshinao $^ $(LIBS)

build/%.o: src/%.cpp
	mkdir -p $(@D)
	$(CC) $(CFLAGS) $(SDLCFLAGS) -c -o $@ $<

run:
	./yoshinao

clean:
	rm -rf build $(TARGET) dependencies.make yoshinao doc dependencies.test.make debug RecordTest.rec

doc:
	doxygen Doxyfile
	mv doc/html/* doc/

debug/%.test: examples/%.test.cpp $(filter-out build/main.o,$(OBJS))
	mkdir -p $(@D)
	$(CC) $(CFLAGS) $(SDLCFLAGS) $(MEMFLAG) $^ $(LIBS) -I /src $(filter-out build/main.o,$(OBJECTS)) -o $@ 

.PHONY: test_all

test_all: $(TEST_TARGETS)
	for TEST in debug/*.test; do ./$$TEST  && echo $${TEST} ok || echo $${TEST} failed!; done

.PHONY: all compile run clean doc 


dependencies.make:
	$(CC) -MM $(CFLAGS) $(SDLCFLAGS) src/*.cpp | sed 's/^\([a-zA-Z][^.]*.o\)/build\/\1/g' > $@
-include dependencies.make

dependencies.test.make:
	$(CC) -MM $(CFLAGS) -I src examples/*.cpp | sed 's/^\([a-zA-Z][^.]*.o\)/debug\/\1/g' > $@