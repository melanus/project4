#include <iostream>
#include <string>
#include <queue>
//#include <ifstream>
#include <fstream>

using namespace std;

class Phrases {

  public:
	queue<string> q;
	
	void readFromFile(string filename) {
		string line;
		ifstream file;
		file.open(filename);
		if(file.is_open())
		{
			while(getline(file, line)) 
			{
				q.push(line);
			}
		}
		else
		{ 
			cout << "Unable to open file " << filename << endl;
		}
	}
}

int main()
{

	Phrases p;

	p.readFromFile("phrases.txt");

	queue<string>::iterator i;
	for(i = p.q.begin(); i != p.q.end(); i++)
		cout << *i << endl;





}
