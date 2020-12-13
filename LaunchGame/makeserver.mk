.PHONY: clean

RM = rm -f

gameserv: serv.o
		gcc -o gameserv serv.o -lpthread

serv.o: serv.c
		gcc -c serv.c

clean:
		$(RM) *.o
	
