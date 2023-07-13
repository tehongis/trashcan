run 	: runme.exe
		./runme.exe

main.o	:	main.cpp
	g++ -c main.cpp

runme.exe	: main.o
	g++ main.o -o runme.exe -lGL -lsfml-graphics -lsfml-audio -lsfml-window -lsfml-system

clean	:
		rm -rf runme.exe main.o
