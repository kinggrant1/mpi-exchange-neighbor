CFLAGS = -g -Wall

all:
	mpicc $(CFLAGS) -o exchange exchange.c

exchange:
	mpicc $(CFLAGS) -o exchange exchange.c

clean:
	rm -f exchange
