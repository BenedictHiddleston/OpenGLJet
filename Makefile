CC=g++
# LIBS on osx are in the framework
#LIBS= -lglut -lGL -lGLU
INCLUDES=  
CCFLAGS= -O0 -g -I. -DOSX -framework OpenGL -framework GLUT
SOURCES=  Vertex.cc Normal.cc Material.cc Face.cc
OBJECTS=$(SOURCES:.cc=.o)
EXECUTABLE=jet

# --- targets
all:  $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): jet.cc $(OBJECTS)
	$(CC) jet.cc $(OBJECTS) -o $(EXECUTABLE) $(CCFLAGS) $(INCLUDES)  $(LIBS) 

.cc.o:
	$(CC) $(CCFLAGS) $(INCLUDES) $< -c

clean:
	rm -f $(EXECUTABLE) $(OBJECTS)
