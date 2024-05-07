g++ -c -o jetgpio.o jetgpio.c
g++ -o example example.c jetgpio.o
rm jetgpio.o
