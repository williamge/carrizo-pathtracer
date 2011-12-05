CC = g++
CFLAGS = -g
LIBS = -lm -L/usr/X11/lib -lpthread -lX11
OBJECTS = carrizo.o
EXEC = carrizo


all: $(EXEC)
	
$(EXEC): $(OBJECTS)
	$(CC) -o $@ $^ $(LIBS) 
	
%.o: %.cpp
	$(CC) -c $< $(CFLAGS)
	
clean:
	rm -f $(EXEC) $(OBJECTS)
	
