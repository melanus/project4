#include "classes.cpp"
#include <fstream>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>

using namespace std;

bool flag = true;
int period = 180;

void setflag(int s)
{
	flag = true;
	run++;
	signal(SIGALRM, setflag);
	alarm(period);
}

void exitHandler(int sig){
	cout << "\nExited successfully" << endl;
	exit(0);
}
	
int main()
{
	Phrases p;
	Sites s;
	Configuration c;

	if(!c.readFromFile("configuration.txt")) {return 2;}
	// read config file and exit if there's an error

	//read the period from the config and error check it
	period = atoi(c.data["PERIOD_FETCH"].c_str());
	if(period == 0)
	{
		cout << "Invalid period specified\nExiting..."<<endl;
		return 1;
	}

	//set alarm
	signal(SIGALRM, setflag);
	alarm(period);

	if(!p.readFromFile(c.data["SEARCH_FILE"])) { return 2; }

	int nfetch = atoi(c.data["NUM_FETCH"].c_str());
	int nparse = atoi(c.data["NUM_PARSE"].c_str());

	if (nfetch <= 0 || nfetch > 8) {
	// Exiting if the number of threads exceed the limit or are 0
		cout << "Invalid number of fetch threads\nExiting..." << endl;
		return 1;
	}
	if (nparse <= 0 || nparse > 8) {
		cout << "Invalid number of parse threads\nExiting..." << endl;
		return 1;
	}

	pthread_t fetchtest[nfetch];
	pthread_t parsetest[nparse];
	// creating number of threads specified

	while(1){
		num_parsed = 0;
		fthreads = 0; pthreads = 0;
		//keep track of the number of currently running threads

		if (flag){
			
			//read in sites to check
			if(!s.readFromFile(c.data["SITE_FILE"])) { return 2; }

			//however many sites there are is how many items
			//that we need to parse.  so keep track of progress
			while(num_parsed < num_sites)
			{
				//create up to the number of speficied threads
				while(fthreads < nfetch)	
				{			
					pthread_create(&fetchtest[fthreads], NULL, fetch, NULL);
					fthreads++;
				}

				while(pthreads < nparse)
				{
					pthread_create(&parsetest[pthreads], NULL, parse, NULL);
					pthreads++;
				}

			}
			//reset and wait for next period/signal interrupt
			flag = false;
			signal(SIGINT, exitHandler);
		}
	}
}


