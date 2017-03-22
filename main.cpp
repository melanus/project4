#include "classes.cpp"
#include <fstream>
#include "libcurl.cpp"

using namespace std;

int main()
{
	string site;

	Phrases p;
	Sites s;
	Configuration c;

	c.readFromFile("configuration.txt");

	p.readFromFile(c.data["SEARCH_FILE"]);
	s.readFromFile(c.data["SITE_FILE"]);
	
	ofstream outputFile("results.csv");
	outputFile << "Time,Phrase,Site,Count" << endl;
	int run = 1;   //this will increment every time the timer goes

	while(!s.q.empty()) 
	{
		site = s.q.pop();
		libcurl(site, p.q, run);
	}

}
