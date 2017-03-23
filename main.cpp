#include "classes.cpp"

#include <fstream>
#include <pthread.h>

using namespace std;

int main()
{
	Phrases p;
	Sites s;
	Configuration c;
	Fetcher fetcher;
	Parser parser;

	c.readFromFile("configuration.txt");

	p.readFromFile(c.data["SEARCH_FILE"]);

	s.readFromFile(c.data["SITE_FILE"]);
	
	run = 1;   //this will increment every time the timer goes
	
	while(!sites.empty())
	{
		fetcher.fetch();
		parser.parse();

	}
}
