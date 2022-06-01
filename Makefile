CFLAGS = -Wall -Wpedantic -O2

builddir:
	mkdir -p build

bench:
	curl  -X GET localhost:8080/index.html -w '\nTime: %{time_total}s\n' --output /dev/null

clean:
	rm -rf build

server.o: builddir
	cc $(CFLAGS) -c server.c -o build/server.o

server:	server.o
	cc build/server.o -o build/server

format:
	clang-format -i ./server.c

run: server
	./build/server

run_flask:
	python3 flask_example.py