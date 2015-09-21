# program executable name 
TARGET = AssA
# compiler flags 
CFLAGS = -g -Wall -D_GNU_SOURCE
# linker flags 
LDFLAGS = -g -lcomedi -lrt -pthread -lm
# list of sources 
SOURCES = $(shell find -name "*.c") 
# default rule, to compile everything 
all: $(TARGET) 
# define object files 
OBJECTS = $(SOURCES:.c=.o) 
# link programs 
$(TARGET): $(OBJECTS) 
	gcc -o $@ $^ $(LDFLAGS)
# compile 
%.o : %.c 
	gcc $(CFLAGS) -c -o $@ $< 
# cleaning 
clean: 
	rm -f $(TARGET) $(OBJECTS)
