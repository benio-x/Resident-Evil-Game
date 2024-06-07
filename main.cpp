/*
* Using DFS traversal to simulate a game of resident evil
*/
#include<iostream>
#include<fstream>
#include<unordered_map>
#include<string>
#include<list>

using namespace std;

bool escapeRPD ( string jillsLocation , string roomNeedToReach ,
int jillsHealth , unordered_map <string , list <string > > rpdMap ,
unordered_map < string , string > & predecessor ,
unordered_map < string , int > & healthInRoom ,
unordered_map < string , int > nemesisLocations )
{
    if(jillsHealth<= 0)//check jills health
    return false;

    jillsHealth -= nemesisLocations[jillsLocation];// decrease jills health by nem effect in room

    if(jillsHealth <= 0)// recheck jills health
    return false;

    if(jillsLocation==roomNeedToReach)// if reachec target room return true
    {
        return true;
    }
// iterate through all neighbor rooms available from current location
    for(auto neighbor = rpdMap[jillsLocation].begin();
    neighbor!=rpdMap[jillsLocation].end();++neighbor)
    {
        string nextRoom = *neighbor;// store next room
        predecessor[nextRoom] = jillsLocation;// record path, set predecessor of next room
        if(escapeRPD(nextRoom, roomNeedToReach, jillsHealth,rpdMap,predecessor,
        healthInRoom,nemesisLocations))// try to escape rooom
        {
            return true;// if success return true
        }
    }
    return false;// if paths fail return false
}

void print(const unordered_map<string,string>&predecessor, const string&currentLocation)
{
    if(predecessor.find(currentLocation)==predecessor.end())// check if current is starting point
    {
        cout << currentLocation;// print current location as startingpoint
        return;
    }

    print(predecessor,predecessor.at(currentLocation));//recurse n print path from start to predeccesor
    cout <<" -> "<< currentLocation;
}

int main(int argc, char* argv[])
{
    if(argc < 3)
    {
        cout << argv[0] << "not enough buddy" << endl;
        return 1;
    }
    if(argc>3)
    {
        cout <<"too many arguments" << endl;
        return 1;
    }

    string fileNameRPD = argv[1];
    string nemLocation = argv[2];

    unordered_map<string,list<string> > rpdMap;
    unordered_map<string,int> nemesisLocations;
    unordered_map<string,string> predecessor;
    unordered_map<string,int> healthInRoom;

    ifstream iFileRPD(fileNameRPD);// access RPD map file
    string room01;
    string room02;
// read connected rooms
    while(iFileRPD >> room01 >> room02)
    {// map from room1 to room 2 to show it can be accessed by room 1
        rpdMap[room01].push_back(room02);
    }

    ifstream iFileNem(nemLocation);// access nem location

    string room;
    int effect;

    while(iFileNem >> room >> effect)
    {
        nemesisLocations[room] = effect;// store effect of nem in the room
    }

    string start = "WestStairway1F";
    string target = "Courtyard";
    int startHealth = 20;
// try to escape from start to target with health
    if(escapeRPD(start,target,startHealth, rpdMap,predecessor, healthInRoom, nemesisLocations))
    {// success
        cout <<"Path: ";
        print(predecessor,target);
        cout <<endl<< endl;
        cout << "Somehow...I'm still alive..." << endl;
    }
    else{// fail
        cout <<"STARS!!!" <<endl;
    }
    return 0;
}
