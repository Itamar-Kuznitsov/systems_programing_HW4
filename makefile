CC=gcc
AR=ar
OBJECTS_graph = edges.o algo.o nodes.o
FLAGS= -Wall -g

# compiling all programs needed:
all: graph

# make exe file called graph:
graph: algo.o lib_graph.a
	$(CC) $(FLAGS) -o graph algo.o lib_graph.a 

#static library 
lib_graph.a: $(OBJECTS_graph)
	$(AR) -rcs lib_graph.a $(OBJECTS_graph)


algo.o: algo.c algo.h
	$(CC) $(FLAGS) -c algo.c

edges.o: edges.c edges.h
	$(CC) $(FLAGS) -c edges.c 

nodes.o: nodes.c nodes.h
	$(CC) $(FLAGS) -c nodes.c 

.PHONY: clean all

# deleting the files:
clean:
	rm -f *.o *.so graph lib_graph.a
