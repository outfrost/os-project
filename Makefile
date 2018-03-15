compileargs = -Wall -Wextra -Wpedantic
linkargs = -pthread -lncurses
objects = out/main.o out/environment.o out/supplier.o out/consumer.o \
		  out/profanities.o

threading : $(objects) out
	cc -o out/threading $(objects) $(linkargs)

run : threading
	out/threading

out/main.o : main.c environment.h out
	cc -c -o out/main.o main.c $(compileargs)

out/environment.o : environment.c environment.h supplier.h consumer.h out
	cc -c -o out/environment.o environment.c $(compileargs)

out/supplier.o : supplier.c environment.h out
	cc -c -o out/supplier.o supplier.c $(compileargs)

out/consumer.o : consumer.c environment.h out
	cc -c -o out/consumer.o consumer.c $(compileargs)

out/profanities.o : profanities.c out
	cc -c -o out/profanities.o profanities.c $(compileargs)

out :
	mkdir out/
