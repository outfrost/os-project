compileargs = -Wall -Wextra -Wpedantic
linkargs = -pthread -lncurses
objects = out/main.o out/environment.o out/supplier.o out/consumer.o \
		  out/profanities.o

threading : $(objects) out/
	@echo "###### Linking executable..."
	cc -o out/threading $(objects) $(linkargs)

run : threading
	@echo
	out/threading

out/main.o : main.c environment.h typedefs.h out/
	cc -c -o out/main.o main.c $(compileargs)

out/environment.o : environment.c environment.h supplier.h consumer.h profanities.h typedefs.h out/
	cc -c -o out/environment.o environment.c $(compileargs)

out/supplier.o : supplier.c environment.h typedefs.h out/
	cc -c -o out/supplier.o supplier.c $(compileargs)

out/consumer.o : consumer.c environment.h typedefs.h out/
	cc -c -o out/consumer.o consumer.c $(compileargs)

out/profanities.o : profanities.c environment.h typedefs.h out/
	cc -c -o out/profanities.o profanities.c $(compileargs)

out/ :
	mkdir out/
