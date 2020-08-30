all: test_multimap cfs_sched

test_multimap: LLRB-Multimap/multimap_tester.cc LLRB-Multimap/multimap.h
	g++ -Wall -Werror -std=c++11 -o test_multimap multimap_tester.cc multimap.h -pthread -lgtest

cfs_sched: cfs_sched.cc LLRB-Multimap/multimap.h
	g++ -Wall -Werror -std=c++11 -o cfs_sched cfs_sched.cc multimap.h -pthread -lgtest

clean:
	rm -f *.o test_multimap cfs_sched
