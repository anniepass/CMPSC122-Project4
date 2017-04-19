/*
*
*
*
*
*
*
*
*/
#include <fstream>
#include <iostream>
#include <string>
#include <list>
#include <vector>
using namespace std;

struct plane {
	struct plane *before;
	struct plane *after;
	int originalSchedTime;
	int sched_time;
	int actual_time;
	double fuel;
	int tankSize;
	int passengers;
	int cargo;
	bool Fam;
	bool successful;
	bool isArriving;
} *Arrival, *departure;


int TIME = 0; // The real time

list<plane> arriving;
list<plane> departing;
list<plane> beforeTIME; 

int takeOffWaitTime;
int landingWaitTime;
int numCrash;
int numDeparting;
int numArriving;
int numPeopleSafe;
int numPeopleKilled;
int numGrandKilled;
int waitTimeGrand;
int numCargoSafe;
int cargoDestroyed;


// declare methods
void next();
void adjustFuel();
void processPlanes();
void sortPlanes();
void printStats();
void updateStats();
void addPlanes();



/*
*  This can be a function that increments the time.  Here, we would
*  update fuel, time, vectors, etc.  We can call other methods like
*  a sort function or a updateStats function.  Each turn, it will
*  delete two plane structs.
*
*  In order to determine how many planes can depart, we need to consider
*  out how many planes are arriving.  If 7 planes with the same amount
*  of fuel are arriving in 5 time units, we will need to land at least
*  two planes immediately.
*
*
*/
void next()
{
	addPlanes();
	sortPlanes();
	processPlanes(); //calling this method before adjusting fuel will save more planes(I think)
	adjustFuel();
	updateStats();

	TIME++;
}

/*
*  Function that decrements the fuel in each plane, and refuels
*  departing planes when necessary.
*/
void adjustFuel()
{
	for (plane p_d : departing) {
		p_d.fuel--;
		if (p_d.fuel < 20) {
			p_d.fuel = p_d.tankSize;
			p_d.sched_time += 10;
			beforeTIME.push_back;
		}
	}

	for (plane p_a : arriving) {
		p_a.fuel--;
	}
}

/*
*  Processes two planes, and crashed planes.
*  Updates crashed stats
*  Updates stats
*/
void processPlanes()
{
	// process two planes
	// ...



	//process crashed planes
	for (plane p_a : arriving) {
		if (p_a.fuel == -1) {
			numCrash++;
			numPeopleKilled += p_a.passengers;
			if (p_a.Fam) {
				numGrandKilled++;
			}
			cargoDestroyed += p_a.cargo;

			arriving.remove(p_a);
		}
	}
}

/*
*  Sorts the planes into a queue(the first plane will be the next to
*  arrive or depart).  Prioritizes grandchildren, passengers, cargo,
*  and wait time.
*/
void sortPlanes()
{

}

void addPlanes()
{
	for (plane p : beforeTIME)
	{
		if (p.sched_time <= TIME) {
			if (p.isArriving) {
				arriving.push_back(p);
			}
			else {
				departing.push_back(p);
			}
		}
	}
}

/*
•Average Take off wait time
•Average landing wait time
•Number of plan crash
•Number of plans departing
•Number of planes arriving
•Number of people that landed safely
•Number of people killed
•Number of Grandchildren killed
•Average wait time for a grandchild  Arriving or departing.  (Dead grandchildren not included)
•Amount of cargo that landed safely
•Amount of destroyed Cargo
•Amount of time it takes to process a input file

int landingWaitTime;
int numCrash;
int numDeparting;
int numArriving;
int numPeopleSafe;
int numPeopleKilled;
int numGrandKilled;
int waitTimeGrand;
int numCargoSafe;
int cargoDestroyed;
*/
void printStats()
{
	cout << "Average Take off wait time: " << takeOffWaitTime << endl;
	cout << "Average landing wait time: " << landingWaitTime <<  endl;
	cout << "Number of plan crash: " << numCrash << endl;
	cout << "Number of plans departing: " << numDeparting << endl;
	cout << "Number of planes arriving: " << numArriving << endl;
	cout << "Number of people that landed safely: " << numPeopleSafe << endl;
	cout << "Number of people killed: " << numPeopleKilled << endl;
	cout << "Number of Grandchildren killed: " << numGrandKilled << endl;
	cout << "Average wait time for a grandchild  Arriving or departing.  (Dead grandchildren not included): " << waitTimeGrand << endl;
	cout << "Amount of cargo that landed safely: " << numCargoSafe << endl;
	cout << "Amount of destroyed Cargo: " << cargoDestroyed << endl;
	cout << "Amount of time it takes to process a input file: " << TIME << endl;
}



int main() {
	ifstream input;
	string line;
	string wait; // variable to allow user to press enter to continue reading file


	input.open("V:/Desktop/planes/myfile.txt", ios::in);//you need a path to your file here

																//while (!input.eof())
	if (input.is_open())
	{
		cout << "working\n";
	}


	while (!input.eof())
	{
		//getline(input, line, ','); //this will read the contents between commas one at a time
		//so underneath you can one at a time place information into their locations in the structure

		getline(input, line, ',');

		if (line == "D") {
			plane p = plane();

			getline(input, line, ',');
			p.originalSchedTime = std::stoi(line);
			p.sched_time = std::stoi(line);

			getline(input, line, ',');
			string a_or_d = line;

			getline(input, line, ',');
			p.fuel = std::stoi(line);
			p.tankSize = std::stoi(line);

			getline(input, line, ',');
			p.passengers = std::stoi(line);

			getline(input, line, ',');
			p.cargo = std::stoi(line);

			getline(input, line);
			line == "Y" ? p.Fam = true : p.Fam = false;

			if (a_or_d == "A") {
				p.isArriving = true;
			}
			else {
				p.isArriving = false;
			}
		
		}
		else if (line == "P") {
			// Print Statistics
			/*while(!arriving.empty() && !departing.empty()) {
			next();
			}*/

			printStats();
		}
		else if (line == "W") {
			// Wait... How?

			cin >> wait;
		}
	}



	/*
	for(plane pl: arriving) {
	cout << "hi"; //test prints "hi" for each arriving plane
	}
	*/

	input.close();

	cout << "hi";

	/*while(!arriving.empty() || !departing.empty()) {
	next();
	}*/

}
