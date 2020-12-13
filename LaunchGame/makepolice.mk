.PHONY: clean

RM = rm -f

policeclnt: p_clnt.o mission.o mission.h move.h minimap.h
		gcc -o policeclnt p_clnt.o mission.o -lpthread -lncurses -lcurses

p_clnt.o: p_clnt.c mission.o mission.h move.h minimap.h
		gcc -c p_clnt.c

mission.o: mission.c mission.h
		gcc -c mission.c

clean:
		$(RM) *.o
