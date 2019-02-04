Simulates five different page table replacement algorithms on three different workload types.
Workload generates 10000 numbers from 0-99 to represent requests to the page table.

Algorithms:
	-OPT: least recently to be used (best possible algorithm but not realistic)
	-LRU:  least recently used
	-FIFO: first in first out
	-Rand: randomly selects page to be replaced
	-Clock: each frame gets two chances to be used until evicted - selected in circular list
	
Workloads:
	-No-locality: page requests are completely random 
	-80-20: 80% of page requests are from 20% of the pages
	-Looping: page requests loop to 49 (0, 1, ..., 48, 49, 0, 1, ...)

To compile:
	"make" in directory
To run:
	./prog4swap [-m size-of-memory] [-r replacement-policy] [-w workload]
	
	Notes:
	-m must be a positive integer
	-r must be "OPT", "LRU", "FIFO", "Rand", or "Clock"
	-w must be "No-locality", "80-20", or "Looping"
