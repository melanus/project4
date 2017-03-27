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
	alarm(period);
}

void myfunction(int sig){
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

	c.readFromFile("configuration.txt");

	period = atoi(c.data["PERIOD_FETCH"].c_str());

	signal(SIGALRM, setflag);
	alarm(period);

	p.readFromFile(c.data["SEARCH_FILE"]);

	if ((atoi(c.data["NUM_FETCH"].c_str()) == 0) || (atoi(c.data["NUM_FETCH"].c_str()) > 8)) {
		cout << "Invalid number of fetch threads\nExiting..." << endl;
	}
	if ((atoi(c.data["NUM_PARSE"].c_str()) == 0) || (atoi(c.data["NUM_PARSE"].c_str()) > 8)) {
		cout << "Invalid number of parse threads\nExiting..." << endl;
	}

	pthread_t fetchtest[atoi(c.data["NUM_FETCH"].c_str())];
	pthread_t parsetest[atoi(c.data["NUM_PARSE"].c_str())];

	while(1){

		if (flag){
			s.readFromFile(c.data["SITE_FILE"]);
			
			while(!sites.empty())
			{
				for (int i=0; i < atoi(c.data["NUM_FETCH"].c_str()); i++)
					pthread_create(&fetchtest[i], NULL, fetch, NULL);
				//parse();
				for (int i=0; i < atoi(c.data["NUM_PARSE"].c_str()); i++)
					pthread_create(&parsetest[i], NULL, parse, NULL);

			}
			flag = false;
			++run;
			signal(SIGINT, myfunction);
		}
	}
}


