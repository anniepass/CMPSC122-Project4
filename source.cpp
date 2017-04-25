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
    int originalSchedTime;
    int sched_time;
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
                            if(this->originalSchedTime <= other.originalSchedTime)
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

int takeOffWaitTime = 0;
int landingWaitTime = 0;
int numCrash = 0;
int numDeparting = 0;
int numArriving = 0;
int numPeopleSafe = 0;
int numPeopleKilled = 0;
int numGrandKilled = 0;
int waitTimeGrand = 0;
int numCargoSafe = 0;
int cargoDestroyed = 0;
int grandChildDeparted = 0;
int grandChildArrived = 0;

// declare methods
void next();
void adjustFuel();
void processPlanes();
void sortPlanes();
void printStats();
void addPlanes();
void quickSort(vector<plane>&, int, int);
void printPlane(plane);
void departPlanes(int);
void landPlanes(int);


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
    for (int i = 0; i < departing.size(); i++) {    
        departing[i].fuel--;
        if (departing[i].fuel < 20) {                   //if a departing plane has less than 20 units of fuel has to refuel
            departing[i].fuel = departing[i].tankSize;  //fuel goes back to original amount which is the size of the tank
            departing[i].sched_time += 10;              //adds 10 units of time to the scheduled time of the plane
            beforeTIME.push_back(departing[i]);
            departing.erase(departing.begin() + i);
            i--;
        }
    }
    
    /*for (plane p : arriving) {
        p.fuel--;
    }*/
    
    for(int i = 0; i < arriving.size(); i++) {
        arriving[i].fuel--;
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
 *
 *  Need to check for big differences in # of passengers
 */
void processPlanes()
{
    // process two planes
    // ...algorithm that determines if and how many planes can depart
    
    
    if(arriving.size() <= 0) {   //if there are no planes in the arriving list depart 2 planes
        departPlanes(2);
    }
    else if(departing.size() <= 0) {  //if there are no planes in the departing list let 2 planes arrive
        landPlanes(2);
    }
    else {
        bool done = false;
        for(int i = arriving.size() - 1; i >= 0 && !done; i++) {
            if(arriving[i].fuel <= (arriving.size() - 1 - i)) { //if a planes fuel is less than or equal to its index then 2 planes need to land
                landPlanes(2);
                done = true;
            }
        }
        
        if(!done) {
            landPlanes(1);
            departPlanes(1);
        }
    }
    
    
    //process crashed planes
    for (int i = 0; i < arriving.size(); i++) {     //loops through the arriving plane list 
        if (arriving[i].fuel == -1) {               // if an arriving plane has no fuel left the number of crashed planes increases
            numCrash++;                             
            numPeopleKilled += arriving[i].passengers;  //the number of people killed also increases by the amount of passengers were on the plane
            if (arriving[i].Fam) {                      //if the plane had grandchildren on it the number of grand children killed also increase
                numGrandKilled++;                       
            }
            cargoDestroyed += arriving[i].cargo;        //the number of cargo destroyed increases by the amount of cargo that was on the plane
            arriving.erase(arriving.begin() + i);
            i--;
        }
    }
}

void departPlanes(int num) {
    while(departing.size() > 0 && num > 0) {
        plane p = departing[departing.size() - 1];
        departing.pop_back();
        numDeparting++;
        takeOffWaitTime += TIME - p.originalSchedTime;
        if(p.Fam) {
            waitTimeGrand += TIME - p.originalSchedTime;
            grandChildDeparted++;
        }
        num--;
    }
}

void landPlanes(int num) {  //accounts for the number of planes that landed safely
    while(arriving.size() > 0 && num > 0) {
        plane p = arriving[arriving.size() - 1];
        arriving.pop_back();
        numArriving++;
        landingWaitTime += TIME - p.originalSchedTime;
        numPeopleSafe += p.passengers;
        numCargoSafe += p.cargo;
        if(p.Fam) {
            grandChildArrived++;
        }
        num--;
    }
}

/*
void initialize() {
    
}

int checkCrashes(vector<plane> planes) {
    int count = 0;
    
    for(plane p : planes) {
        if(p.fuel < 0)
            count++;
    }
    
    return count;
}

void decrementFuel(vector<plane> &planes) {
    for(plane p : planes) {
        if(p.isArriving)
            p.fuel--;
    }
}

bool solveNQUtil(int board[N][N], int col)
{
    if (col >= N)
        return true;
    for (int i = 0; i < N; i++)
    {
        if ( isSafe(board, i, col) )
        {
            board[i][col] = 1;
            if (solveNQUtil(board, col + 1) == true)
                return true;
            board[i][col] = 0;
        }
    }
    return false;
}

f findSol(vector<plane> planes, crashCount) {
    if(planes.size() == 0)
        return 0;
    
    vector<plane> copy1 = planes;
    vector<plane> copy2 = planes;
    
    for(int i = 0; i <= 2; i++) {
        
    }
           
    
}*/


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
        if (beforeTIME[i].sched_time <= TIME) {
            if (beforeTIME[i].isArriving) {
                arriving.push_back(beforeTIME[i]);
                beforeTIME.erase(beforeTIME.begin() + i);
                i--;
            }
            else {
                departing.push_back(beforeTIME[i]);
                beforeTIME.erase(beforeTIME.begin() + i);
                i--;
            }
        }
    }
}

void printStats()      //prints the statistics that were calculated in the process planes, depart planes, land planes fucntions
{
    cout << "Average Take off wait time: " << (double)takeOffWaitTime / numDeparting << endl;
    cout << "Average landing wait time: " << (double)landingWaitTime / numArriving <<  endl;
    cout << "Number of plan crash: " << numCrash << endl;
    cout << "Number of plans departing: " << numDeparting << endl;
    cout << "Number of planes arriving: " << numArriving << endl;
    cout << "Number of people that landed safely: " << numPeopleSafe << endl;
    cout << "Number of people killed: " << numPeopleKilled << endl;
    cout << "Number of Grandchildren killed: " << numGrandKilled << endl;
    cout << "Average wait time for a grandchild  Arriving or departing.  (Dead grandchildren not included): " << (double)waitTimeGrand / (grandChildDeparted + grandChildArrived) << endl;
    cout << "Amount of cargo that landed safely: " << numCargoSafe << endl;
    cout << "Amount of destroyed Cargo: " << cargoDestroyed << endl;
    cout << "Amount of time it takes to process a input file: " << TIME << endl;
}

void printPlane(plane p)
{
    cout << p.originalSchedTime << "," << p.isArriving << "," << p.fuel << "," << p.passengers << "," << p.cargo << "," << p.Fam << endl;
}

void quickSort(vector<plane> &planes, int left, int right) {
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
    
    input.open("/Users/erniedefoy/Desktop/planes.txt", ios::in);    //opens the input file give
    
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
            
            //printStats();
        }
        else if (command == 'W') {
            // Wait... How?
            
            cin >> wait;
        }
    }
    
    input.close();
    
    do {
        next();
    } while(!arriving.empty() || !departing.empty());

    printStats();
}
