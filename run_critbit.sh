
gcc -O3 -DTIMES=200 critbit.c -o critbit;
time ./critbit;
#gcc -g -DTIMES=200 critbit.c -o critbit;
#gdb -args ./critbit;


# gcc -Wall critbit.c -g -DTIMES=2 -o critbit;
# valgrind --tool=memcheck --leak-check=full ./critbit;

