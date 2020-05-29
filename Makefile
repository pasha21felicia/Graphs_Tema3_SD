build:build-reprezentanti build-labirint
build-reprezentanti: 
	gcc reprezentanti.c -o pb2 -std=c99
build-labirint:
	gcc labirint.c -o pb4 -std=c99
run-p2:
	./pb2
run-p4:
	./pb4
clean:
	rm pb2 pb4