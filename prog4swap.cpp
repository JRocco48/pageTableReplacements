#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <vector>
#include <algorithm>

using namespace std;

int main(int argc, char ** argv) {
	
	srand(time(NULL));
	
	int memSize = 5;
	string policy = "OPT";
	string workload = "No-locality";

	//parse command line arguments
	int opt;
	while((opt = getopt(argc, argv, "m:r:w:")) != -1) {
		switch(opt) {
			case 'm':
				memSize = atoi(optarg);	
				break;
			case 'r':
				policy = optarg;
				break;
			case 'w':
				workload = optarg;
				break;
		}
	}
	

	//populates workload
	int i;
	vector<int> work;
	if(workload == "No-locality") {
		for(i = 0; i < 10000; i++) {
			work.push_back(rand() % 100);
		}
	} else if(workload == "80-20") {
		for(i = 0; i < 8000; i++) {
			work.push_back(rand() % 20); //hot pages
		}
		for(; i < 10000; i++) {
			work.push_back((rand() % 80) + 20); //cold pages
		}
		random_shuffle(work.begin(), work.end());
	} else if(workload == "Looping") {
		for(i = 0; i < 10000; i++) {
			work.push_back(i % 50);
		}
	} else {
		perror("ERROR: -w <workload> must be \"No-locality\", \"80-20\", or \"Looping\"");
		exit(1);	
	}
	

	int hits = 0;
	vector<int> pageTable(memSize);
	for(i = 0; i < memSize; i++) pageTable[i] = -1; //initialize page table
	int j = 0;
	int k = 0;
	if(policy == "OPT") {
		vector<int> indices(memSize);
		for(i = 0; i < indices.size(); i++) indices[i] = -1; //initialize index table
		for(j = 0; j < work.size(); j++) {
			for(i = 0; i < memSize; i++) {
				if(pageTable[i] == work[j]) {
					hits++;
					//code to update indices[i]
					for(k = j+1; k < work.size(); k++) {
						if(work[k] == pageTable[i]) break;
					}
					indices[i] = k;
					break;
				}
				if(pageTable[i] == -1) {
					pageTable[i] = work[j];
					//code to find index in work where the next pageTable[i] appears
					for(k = j+1; k < work.size(); k++) {
						if(work[k] == pageTable[i]) break;
					}
					indices[i] = k;
					break;
				}
			}
			if(i == memSize) {
				//replace a page
				int max = -1;
				int maxIndex = 0;
				for(k = 0; k < indices.size(); k++) {
					if(indices[k] > max) {
						max = indices[k];
						maxIndex = k;
					}
				}		
				pageTable[maxIndex] = work[j]; //replacing furthest page in page table
				for(k = j+1; k < work.size(); k++) {
					if(work[k] == pageTable[maxIndex]) break;
				}
				indices[maxIndex] = k;
			}	
		}
	} else if(policy == "LRU") {
		vector<int> history(memSize);
		for(i = 0; i < memSize; i++) history[i] = -1;
		for(j = 0; j < work.size(); j++) {
			for(i = 0; i < pageTable.size(); i++) {
				if(pageTable[i] == work[j]) {
					hits++;
					history[i] = 0;
					break;
				}
				else if(pageTable[i] == -1) {
					pageTable[i] = work[j];
					history[i] = 0;
					break;
				}
			} 
			if(i == pageTable.size()) {
				//page replacement
				int max = history[0];
				int maxIndex = 0;
				for(k = 0; k < history.size(); k++) {
					if(history[k] > max) {
						max = history[k];
						maxIndex = k;
					}
				}
				pageTable[maxIndex] = work[j];
				history[maxIndex] = 0;
			}
			for(i = 0; i < history.size(); i++) {
				if(history[i] != -1) history[i]++; //increment histories
			}
		}

	} else if(policy == "FIFO") {

		int iter = 0;
		for(j = 0; j < work.size(); j++) {
			for(i = 0; i < pageTable.size(); i++) {
				if(pageTable[i] == work[j]) {
					hits++;
					break;
				} else if(pageTable[i] == -1) {
					pageTable[i] = work[j];
					iter++;
					break;
				}
			}
			if(i == pageTable.size()) {
				pageTable[iter%pageTable.size()] = work[j];
				iter++;
			}
		}
	} else if(policy == "Rand") {
		for(j = 0; j < work.size(); j++) {
			for(i = 0; i < pageTable.size(); i++) {
				if(pageTable[i] == work[j]) {
					hits++;
					break;
				} else if(pageTable[i] == -1) {
					pageTable[i] = work[j];
					break;
				}
			}
			if(i == pageTable.size()) {
				pageTable[rand()%pageTable.size()] = work[j];
			}
		}
	} else if(policy == "Clock") {
		vector<int> clock(memSize);
		int clockIter = 0;
		int p;
		for(i = 0; i < clock.size(); i++) clock[i] = 0;
		for(j = 0; j < work.size(); j++) {
			for(i = 0; i < pageTable.size(); i++) {
				if(pageTable[i] == work[j]) {
					hits++;
					clock[i] = 1;
					clockIter++;
					break;
				} else if(pageTable[i] == -1) {
					pageTable[i] = work[j];
					clock[i] = 1;
					clockIter++;
					break;
				}
			}
			if(i == pageTable.size()) {
				for(k = clockIter; k < clock.size() + clockIter; k++) {
					if(clock[k%clock.size()] == 0) {
						pageTable[k%clock.size()] = work[j];
						break;
					}
				} 
				if(k == clock.size() + clockIter) {
					for(p = 0; p < clock.size(); p++) clock[p] = 0;	
					pageTable[clockIter%clock.size()] = work[j];
				}
			}
		}
	} else { 
		perror("ERROR: -r <policy> must be \"OPT\", \"LRU\", \"FIFO\", \"Rand\", or \"Clock\"");
		exit(1);
	}

	cout << hits;
	return 0;
}
