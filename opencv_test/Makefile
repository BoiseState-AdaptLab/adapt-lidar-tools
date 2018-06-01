all: WriteOnImage.cpp
	g++ $^ `pkg-config --cflags --libs opencv`

clean:
	rm -f a.out *.o
