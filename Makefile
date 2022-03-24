FLAGS = -Wall -Werror -Wextra

all : p1main p2main
	
p1main : p1main.c p1exo1.o p1exo1.h p1exo2.o p1exo2.h
	gcc $(FLAGS) p1main.c p1exo1.o p1exo2.o -o p1main

p2main : p2main.c p1exo1.o p1exo1.h p1exo2.o p1exo2.h p2exo3.o p2exo3.h
	gcc $(FLAGS) p2main.c p1exo1.o p1exo2.o p2exo3.o -o p2main

p1exo1.o : p1exo1.c p1exo1.h
	gcc $(FLAGS) -c p1exo1.c -o p1exo1.o

p1exo2.o : p1exo2.c p1exo2.h
	gcc $(FLAGS) -c p1exo2.c -o p1exo2.o

p2exo3.o : p2exo3.c p2exo3.h
	gcc $(FLAGS) -c p2exo3.c -o p2exo3.o

clean :
	rm -rf *.o
	rm -f p?main