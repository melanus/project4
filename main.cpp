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

	// read config file
	c.readFromFile("configuration.txt");

	// main part of the program will only execute every fetch period
	period = atoi(c.data["PERIOD_FETCH"].c_str());
	if(period == 0)
	{
		cout << "Invalid period specified\nExiting..."<<endl;
		return 1;
	}

	signal(SIGALRM, setflag);
	alarm(period);

	p.readFromFile(c.data["SEARCH_FILE"]);

	// Exiting if the number of threads exceed the limit or are 0
	if ((atoi(c.data["NUM_FETCH"].c_str()) == 0) || (atoi(c.data["NUM_FETCH"].c_str()) > 8)) {
		cout << "Invalid number of fetch threads\nExiting..." << endl;
		return 1;
	}
	if ((atoi(c.data["NUM_PARSE"].c_str()) == 0) || (atoi(c.data["NUM_PARSE"].c_str()) > 8)) {
		cout << "Invalid number of parse threads\nExiting..." << endl;
		return 1;
	}

	// creating number of threads specified
	pthread_t fetchtest[atoi(c.data["NUM_FETCH"].c_str())];
	pthread_t parsetest[atoi(c.data["NUM_PARSE"].c_str())];

	while(1){

		if (flag){
			//if(!s.readFromFile(c.data["SITE_FILE"])) { return 2; }
			s.readFromFile(c.data["SITE_FILE"]);
			
			while(!sites.empty() && !siteData.empty())
			{
				cout << "front of sites is " << sites.q.front() << endl;
				// calling fetch for each thread available
				for (int i=0; i < atoi(c.data["NUM_FETCH"].c_str()); i++)
					pthread_create(&fetchtest[i], NULL, fetch, NULL);
				//parse();
				// calling pares for each thread available
				for (int i=0; i < atoi(c.data["NUM_PARSE"].c_str()); i++)
					pthread_create(&parsetest[i], NULL, parse, NULL);

			}
			flag = false;
			signal(SIGINT, exitHandler);
		}
	}
}


