
all:
	mkdir -p bin
	
	gcc src/main.c src/tracker.c src/analyzer.c src/utils.c -pthread -o bin/project

clean:
	rm -f bin/project