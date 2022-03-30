FLAGS = -Wall -Werror -Wextra -g
LIBM = -lm

all : p1main p2main p3main
	
p1main : p1main.o p1exo1.o p1exo1.h p1exo2.o p1exo2.h
	
	gcc $(FLAGS) p1main.o p1exo1.o p1exo2.o -o p1main $(LIBM)

p1main.o : p1main.c p1exo1.h p1exo2.h
	gcc $(FLAGS) -c p1main.c -o p1main.o 

p2main : p2main.o p1exo1.o p1exo1.h p1exo2.o p1exo2.h p2exo3.o p2exo3.h p2exo4.o p2exo4.h
	gcc $(FLAGS) p2main.o p1exo1.o p1exo2.o p2exo3.o p2exo4.o -o p2main $(LIBM)

p2main.o : p2main.c p1exo1.h p1exo2.h p2exo3.h
	gcc $(FLAGS) -c p2main.c -o p2main.o 

p3main : p3main.o p1exo1.o p1exo1.h p1exo2.o p1exo2.h p2exo3.o p2exo3.h p2exo4.o p2exo4.h p3exo5.o p3exo5.h p3exo6.o p3exo6.h
	gcc $(FLAGS) p3main.o p1exo1.o p1exo2.o p2exo3.o p2exo4.o  p3exo5.o p3exo6.o -o p3main $(LIBM)

p3main.o : p3main.c p2exo3.h p3exo5.h
	gcc $(FLAGS) -c p3main.c -o p3main.o 

p1exo1.o : p1exo1.c p1exo1.h
	gcc $(FLAGS) -c p1exo1.c -o p1exo1.o 

p1exo2.o : p1exo2.c p1exo2.h
	gcc $(FLAGS) -c p1exo2.c -o p1exo2.o 

p2exo3.o : p2exo3.c p2exo3.h
	gcc $(FLAGS) -c p2exo3.c -o p2exo3.o 

p2exo4.o : p2exo4.c p2exo4.h
	gcc $(FLAGS) -c p2exo4.c -o p2exo4.o 

p3exo5.o : p3exo5.c p3exo5.h
	gcc $(FLAGS) -c p3exo5.c -o p3exo5.o 

p3exo6.o : p3exo6.c p3exo6.h
	gcc $(FLAGS) -c p3exo6.c -o p3exo6.o 

clean :
	rm -rf *.o
	rm -f p?main