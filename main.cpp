#include <iostream>
#include <string>
#include <deque>
//#include <ifstream>
#include <fstream>

using namespace std;

class Phrases {

  public:
	deque<string> q;
	
	void readFromFile(string filename) {
		string line;
		ifstream file;
		file.open(filename.c_str());
		if(file.is_open())
		{
			while(getline(file, line)) 
			{
				q.push_front(line);
			}
		}
		else
		{ 
			cout << "Unable to open file " << filename << endl;
		}
	}
};

class Sites {
	
  public:
	deque<string> q;

		void readFromFile(string filename) {
		string line;
		ifstream file;
		file.open(filename.c_str());
		if(file.is_open())
		{
			while(getline(file, line)) 
			{
				q.push_front(line);
			}
		}
		else
		{ 
			cout << "Unable to open file " << filename << endl;
		}
	}
};

int main()
{

	Phrases p;
	Sites s;

	p.readFromFile("phrases.txt");
	s.readFromFile("sites.txt");

	deque<string>::iterator i;
	for(i = p.q.begin(); i != p.q.end(); i++)
		cout << *i << endl;

	cout << endl;

	for(i = s.q.begin(); i != s.q.end(); i++)
		cout << *i << endl;

}
