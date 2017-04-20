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
#include <ctime>

using namespace std;

struct plane {
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
    
    // overloaded comparison operators
    // A plane will be greater than another plane if its priority of
    // arriving/departing is higher
    bool operator > ( const plane& other ) const  {
        
        if(this->fuel < other.fuel) {
            return true;
        }
        if(this->fuel == other.fuel) {
            if(this->Fam || (!this->Fam && !other.Fam)) {
                if(other.Fam || (!this->Fam && !other.Fam)) {
                    if(this->passengers > other.passengers)
                        return true;
                    if(this->passengers == other.passengers) {
                        if(this->cargo > other.cargo)
                            return true;
                        else if(this->cargo == other.cargo) {
                            if(this->originalSchedTime < other.originalSchedTime)
                                return true;
                            else
                                return false;
                        }
                        else
                            return false;
                    }
                    else
                        return false;
                }
                else
                    return true;
            }
            else
                return false;
        }
        
        return false;
    }
    
    bool operator < ( const plane& other ) const  {
        
        if(this->fuel > other.fuel) {
            return true;
        }
        if(this->fuel == other.fuel) {
            if(this->Fam || (!this->Fam && !other.Fam)) {
                if(other.Fam || (!this->Fam && !other.Fam)) {
                    if(this->passengers > other.passengers)
                        return false;
                    if(this->passengers == other.passengers) {
                        if(this->cargo > other.cargo)
                            return false;
                        else if(this->cargo == other.cargo) {
                            if(this->originalSchedTime < other.originalSchedTime)
                                return false;
                            else
                                return true;
                        }
                        else
                            return true;
                    }
                    else
                        return true;
                }
                else
                    return false;
            }
            else
                return true;
        }
        
        return false;
    }
    
} *Arrival, *departure;


int TIME = 0; // The real time

vector<plane> arriving;
vector<plane> departing;
vector<plane> beforeTIME;

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
void addPlanes();
void quickSort(vector<plane>, int, int);
void printPlane(plane);


/*
 *  This can be a function that increments the time.  Here, we would
 *  update fuel, time, vectors, etc.  We can call other methods like
 *  a sort function or a updateStats function.  Each turn, it will
 *  delete two plane structs.
 */
void next()
{
    addPlanes();
    sortPlanes();
    processPlanes(); //calling this method before adjusting fuel will save more planes(I think)
    adjustFuel();
    
    TIME++;
}

/*
 *  Function that decrements the fuel in each plane, and refuels
 *  departing planes when necessary.
 */
void adjustFuel()
{
    for (plane p : departing) {
        p.fuel--;
        if (p.fuel < 20) {
            p.fuel = p.tankSize;
            p.sched_time += 10;
            beforeTIME.push_back(p);
        }
    }
    
    for (plane p : arriving) {
        p.fuel--;
    }
}

/*
 *  Processes two planes, and crashed planes.
 *  Updates crashed stats
 *  Updates stats
 *  In order to determine how many planes can depart, we need to consider
 *  how many planes are arriving, and how many will be arriving.  If 7
 *  planes with the same amount of fuel are arriving in 5 time units, we
 *  will need to land at least two planes immediately.  To come to this
 *  conclusion we may want to look into the beforeTIME list when designing
 *  our algorithm that determines if and how many planes can depart.
 */
void processPlanes()
{
    // process two planes
    // ...algorithm that determines if and how many planes can depart
    
    
    
    //process crashed planes
    for (int i = 0; i < arriving.size(); i++) {
        if (arriving[i].fuel == -1) {
            numCrash++;
            numPeopleKilled += arriving[i].passengers;
            if (arriving[i].Fam) {
                numGrandKilled++;
            }
            cargoDestroyed += arriving[i].cargo;
            arriving.erase(arriving.begin() + i);
            i--;
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
    if(!arriving.empty())
        quickSort(arriving, 0, arriving.size() - 1);
    if(!departing.empty())
        quickSort(departing, 0, departing.size() - 1);
}

void addPlanes()
{
    for (int i = 0; i < beforeTIME.size(); i++)
    {
        //if (p.sched_time <= TIME) {
            if (beforeTIME[i].isArriving) {
                arriving.push_back(beforeTIME[i]);
                //beforeTIME.erase(beforeTIME.begin() + i);
            }
            else {
                departing.push_back(beforeTIME[i]);
                //beforeTIME.erase(beforeTIME.begin() + i);
            }
        //}
    }
}

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

void printPlane(plane p)
{
    cout << p.originalSchedTime << "," << p.isArriving << "," << p.fuel << "," << p.passengers << "," << p.cargo << "," << p.Fam << endl;
}

void quickSort(vector<plane> planes, int left, int right) {
    int i = left, j = right;
    plane tmp;
    plane pivot = planes[(left + right) / 2];
    
    /* partition */
    while (i <= j) {
        while (planes[i] < pivot)
            i++;
        while (planes[j] > pivot)
            j--;
        if (i <= j) {
            tmp = planes[i];
            planes[i] = planes[j];
            planes[j] = tmp;
            i++;
            j--;
        }
    };
    
    /* recursion */
    if (left < j)
        quickSort(planes, left, j);
    if (i < right)
        quickSort(planes, i, right);
}

int main()
{
    
    ifstream input;
    string line;
    char command;
    char wait; // variable to allow user to press enter to continue reading file
    
    input.open("/Users/erniedefoy/Desktop/planes.txt", ios::in);
    
    if (!input.is_open())
    {
        cout << "File did not open" << endl;
    }
    
    while (!input.eof())
    {
        input >> command;
        
        if (command == 'D') {
            plane p = plane();
            
            input >> command; //get rid of comma
            
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
            
            a_or_d == "A" ? p.isArriving = true : p.isArriving = false;
            
            beforeTIME.push_back(p);
        }
        else if (command == 'P') {
            // Print Statistics
            /*while(!arriving.empty() && !departing.empty()) {
             next();
             }*/
            
            printStats();
        }
        else if (command == 'W') {
            // Wait... How?
            
            cin >> wait;
        }
    }
    
    input.close();
    
    addPlanes();
    //sortPlanes();
    
    for(plane p : departing) {
        printPlane(p);
    }
    
    /*while(!arriving.empty() || !departing.empty()) {
     next();
     }*/
}
