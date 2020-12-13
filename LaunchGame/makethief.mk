.PHONY: clean

RM = rm -f

thiefclnt: t_clnt.o mission.o mission.h move.h minimap.h
		gcc -o thiefclnt t_clnt.o mission.o -lpthread -lcurses -lncurses

t_clnt.o: t_clnt.c mission.o mission.h move.h minimap.h
		gcc -c t_clnt.c

mission.o: mission.c mission.h
		gcc -c mission.c

clean: 
	 $(RM) *.o
