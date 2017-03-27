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
	cout << "alarm happened" << endl;
	flag = true;
	run++;
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
//	Fetcher fetcher;
//	Parser parser;

	if(!c.readFromFile("configuration.txt")) {return 2;}

	period = atoi(c.data["PERIOD_FETCH"].c_str());
	if(period == 0)
	{
		cout << "Invalid period specified\nExiting..."<<endl;
		return 1;
	}

	cout << "about to set the alarm" << endl;
	signal(SIGALRM, setflag);
	alarm(period);
	cout << "the alarm has been set" << endl;

	if(!p.readFromFile(c.data["SEARCH_FILE"])) { return 2; }

	int nfetch = atoi(c.data["NUM_FETCH"].c_str());
	int nparse = atoi(c.data["NUM_PARSE"].c_str());

	if (nfetch <= 0 || nfetch > 8) {
		cout << "Invalid number of fetch threads\nExiting..." << endl;
		return 1;
	}
	if (nparse <= 0 || nparse > 8) {
		cout << "Invalid number of parse threads\nExiting..." << endl;
		return 1;
	}

	pthread_t fetchtest[nfetch];
	pthread_t parsetest[nparse];

	int num_parsed;

	while(1){
		//cout << "loop reset" << endl;
		num_parsed = 0;

		if (flag){
			
			if(!s.readFromFile(c.data["SITE_FILE"])) { return 2; }

			cout << num_parsed << "\t" << num_sites << endl;
			while(num_parsed < num_sites)
			{

				for(int i = 0; i < nfetch; i++)
					pthread_create(&fetchtest[i], NULL, fetch, NULL);

				for(int i = 0; i < nparse; i++)
				{
					pthread_create(&parsetest[i], NULL, parse, NULL);
					num_parsed++;
					cout << num_parsed << " threads parsed" << endl;
				}

			}
			flag = false;
			//signal(SIGINT, exitHandler);
		}
	}
}


