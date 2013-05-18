
g++ -O3 -DTIMES=200 set.cc -o set;
time ./set;
#g++ -g critbit-test.cc -c 
#g++ -g critbit-test.o critbit.o -o critbit-test

# gcc -Wall critbit.c -g -DTIMES=2 -o critbit;
# valgrind --tool=memcheck --leak-check=full ./critbit;

