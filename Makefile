compileargs = -Wall -Wextra -Wpedantic
linkargs = -pthread -lncurses
objects = out/main.o out/environment.o out/worker.o out/ui.o \
		  out/signalhandler.o

out/threading : $(objects)
	@echo "###### Linking executable..."
	cc -o out/threading $(objects) $(linkargs)

threading : out/threading

run : out/threading
	@echo
	out/threading

init :
	mkdir out/

out/main.o : main.c environment.h worker.h
	cc -c -o out/main.o main.c $(compileargs)

out/environment.o : environment.c environment.h worker.h ui.h
	cc -c -o out/environment.o environment.c $(compileargs)

out/worker.o : worker.c worker.h environment.h
	cc -c -o out/worker.o worker.c $(compileargs)

out/ui.o : ui.c ui.h signalhandler.h environment.h worker.h
	cc -c -o out/ui.o ui.c $(compileargs)

out/signalhandler.o : signalhandler.c signalhandler.h ui.h
	cc -c -o out/signalhandler.o signalhandler.c $(compileargs)
