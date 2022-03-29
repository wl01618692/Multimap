all: test_multimap

test_multimap: LLRB-Multimap/multimap_tester.cc LLRB-Multimap/multimap.h
	g++ -Wall -Werror -std=c++11 -o test_multimap multimap_tester.cc multimap.h -pthread -lgtest
clean:
	rm -f *.o test_multimap
