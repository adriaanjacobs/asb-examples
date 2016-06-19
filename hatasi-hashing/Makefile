%.o: %.cpp
	../../ASBDetection/objectize.sh -O3 $^

instr: main.o hatasi.o
	../../ASBDetection/link.sh clang++ $^

clean:
	rm -rf *.o hatasi-hashing a.out

all:
	g++ -g -O0 -Wall *.cpp -o hatasi-hashing
