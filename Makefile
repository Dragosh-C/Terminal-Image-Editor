build:
	gcc main.c -o bmp -lm -g

run:
	./bmp

clean:
	rm -f bmp
