CC = gcc

# Para mais informações sobre as flags de warning, consulte a informação adicional no lab_ferramentas
CFLAGS = -g -std=c17 -D_POSIX_C_SOURCE=200809L \
		 -Wall -Werror -Wextra \
		 -Wcast-align -Wconversion -Wfloat-equal -Wformat=2 -Wnull-dereference -Wshadow -Wsign-conversion -Wswitch-enum -Wundef -Wunreachable-code -Wunused \
		 

ifneq ($(shell uname -s),Darwin) # if not MacOS
	CFLAGS += -fmax-errors=5
endif

all: client

client: main.c api.o parser.o
	$(CC) $(CFLAGS) $(SLEEP) -o client main.c api.o parser.o

%.o: %.c %.h
	$(CC) $(CFLAGS) -c ${@:.o=.c}

run: client
	@./client 1 /tmp/TradingView < client_tests1.txt

clean:
	rm -f *.o client

format:
	@which clang-format >/dev/null 2>&1 || echo "Please install clang-format to run this command"
	clang-format -i *.c *.h
