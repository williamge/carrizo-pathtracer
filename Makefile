CC = clang++
CFLAGS = -g
LIBS = -lm -L/usr/X11/lib -lpthread -lX11
INCLUDE = -I/usr/X11R6/include -Iinclude
OBJECTS = carrizo.o cObject.o rendModel.o vecmat.o
EXEC = carrizo


all: $(EXEC)
	
$(EXEC): $(OBJECTS)
	$(CC) -o $@ $^ $(LIBS) 
	
%.o: %.cpp
	$(CC) $(CFLAGS) -c $< $(INCLUDE)
	
clean:
	rm -f $(EXEC) $(OBJECTS)
	
