//LBR210000 Luke Ravsten CS 2337.002
#include <iomanip>
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <algorithm>
#include "Player.h"
#include "GenericHashMap.h"
#include <unordered_map>

class Team //nested class to manage a group of players
{
    public:
        std::string outputTeam(); //will output this instance of a team to console in format
        std::string outputLeaders(); //outputs the leaders of this instance of a team
        std::string makeLeaderString(int stat);//create the string of output for the leaders of input stat, 0 - 5 are the counted stats, 6 and 7 are the calculated floats
        void ProcessPlay(std::string line, int play, char team);//takes the player name out of the line and the corresponding play value and checks to make a new player or update existing player stats
    
    private:
        std::unordered_map<std::string, Player> playerTable;//hash table of player objects with play strings as keys
        
        template<typename T>//helper method in makeleaderstring(), checks if current player n should be added to a leader list and related logic
        void checkAndAdd(Player & n, T& currentMax, T valToCompare, std::vector<Player> & homeLeaders, std::vector<Player> & awayLeaders, int& namesCount, T ignores[2]);

        template<typename T>//same idea as above function but checking for minimums, if statement conditions are slightly different for 0
        void checkMinAndAdd(Player & n, T& currentMin, T valToCompare, std::vector<Player> & homeLeaders, std::vector<Player> & awayLeaders, int& namesCount, T ignores[2]);
};

//Team Functions
std::string Team::outputTeam()
{
    std::vector<Player> allPlayers;//adds all players to a vector to then output only their names, because the overloaded player operator will output their stats too
    std::stringstream SS;
    
    //iterate through hash table, add players to vector
    for(std::unordered_map<std::string, Player>::iterator it = playerTable.begin(); it != playerTable.end(); it++)
        allPlayers.push_back(it->second); 
        
    sort(allPlayers.begin(), allPlayers.end());//sort players, player object has overridden comparison operators to compare names to allow sorting by alphabetical order
    
    for(unsigned int i = 0; i < allPlayers.size(); i++)//output player names from vector to string stream
        SS << allPlayers.at(i);
    
    return SS.str();//return string
}

std::string Team::outputLeaders()
{
    std::stringstream SS;
    SS << "LEAGUE LEADERS" << std::endl; //follow format from rubric
    SS << "BATTING AVERAGE" << std::endl << makeLeaderString(7) << std::endl; //output statname, then call makeleaderString for corresponding stat
    SS << "ON-BASE PERCENTAGE" << std::endl << makeLeaderString(8) << std::endl;
    SS << "HITS" << std::endl << makeLeaderString(2) << std::endl;
    SS << "WALKS" << std::endl << makeLeaderString(3) << std::endl;
    SS << "STRIKEOUTS" << std::endl << makeLeaderString(1) << std::endl;
    SS << "HIT BY PITCH" << std::endl << makeLeaderString(5) << std::endl;
    SS << std::endl;
    return SS.str();//return string
}

std::string Team::makeLeaderString(int s) //input integer corrsponds to the players' stat value in the player object's statsarray
{
    int maxh = 0; //variables for leading values, change as list is traversed
    float maxf = 0.0f; 
    int c = 0; // counts how many people have been output in total to control output to match format, output stops when c reaches 3
    int namesCount = 0; //count of names printed to list for a specific place, used to control output to match format
    
    int foundInts[2] = {-1,-1}; // arrays to hold values for highest and second highest values found within list, will be used to ignore later when print leaders for less than first place
    float foundFloats[2] = {-1.0f,-1.0f};
    
    std::vector<Player> homeLeaders; //vector of leaders for each team
    std::vector<Player> awayLeaders;
    std::stringstream outputValue; //stream for the value, stringstream is used so both an int or a float can match format in rubric
    std::stringstream totalOutput; //entire string output that is returned as a string at the end of the function
    
    
    for(std::unordered_map<std::string, Player>::iterator it = playerTable.begin(); c < 3;) //traverse hashtable of players, stop if 3 places or leaders have been output already
    {
        //no iterator end condition or iterator increment in for parameters to allow for the loop add checks and prepare to possibly reset itself and find the next place
        //this allows this function to find all three places instead of calling this method three times
        //an exit condition of being at the end of the iterator would prevent it from resetting itself and looking for more places
        
        if(it != playerTable.end())//continue traverse
        {
            if(s < 7 && s != 1) //common case where an int stat is being compared to find maximum
                checkAndAdd<int>(it->second, maxh, it->second.statsArray[s], homeLeaders, awayLeaders, namesCount, foundInts); 
            else
            {
                switch(s)
                {
                    case 1:
                        checkMinAndAdd(it->second, maxh, it->second.statsArray[s], homeLeaders, awayLeaders, namesCount, foundInts);
                        break;
                    case 7: //special case find maximum of calculated float
                        checkAndAdd<float>(it->second, maxf, it->second.CalcBattingAverage(), homeLeaders, awayLeaders, namesCount, foundFloats);
                        break;
                    case 8: //special case find maximum of calculated float
                        checkAndAdd<float>(it->second, maxf, it->second.CalcOnBasePercent(), homeLeaders, awayLeaders, namesCount, foundFloats);
                        break;
                } 
            }
            it++; //continue traverse
        }
        else //reached end of list, allow resetting of values
        {
            if(it == playerTable.end() && c < 3)
            {
                foundInts[c] = maxh; //add found leading value to values to ignore when searching for lower placed leaders later
                foundFloats[c] = maxf;
            }
            if(namesCount == 0)
                {
                    c = 3; //no next place value found, stop loop, stop printing out places
                }
            else if(namesCount>0) //at least one nth place holder found, continue to output
            {
                if(s > 6)
                outputValue << std::fixed << std::setprecision(3) << maxf; //match required format if float
                else
                outputValue << maxh;
                
                //update total output 
                totalOutput << outputValue.str() << std::string("\t");
                
                std::sort(homeLeaders.begin(), homeLeaders.end());//teams are sorted
                std::sort(awayLeaders.begin(), awayLeaders.end());
                
                std::stringstream names;//output away leaders first
                for(unsigned int i = 0; i < awayLeaders.size(); i++)
                    names << ", " << awayLeaders.at(i).name;

                for(unsigned int i = 0; i < homeLeaders.size(); i++)//output home leaders
                    names << ", " << homeLeaders.at(i).name;
                
                totalOutput << names.str().substr(2) << std::endl; //removed comma and space placed before first name in list
                
                c += namesCount; //increase c
                
                it = playerTable.begin(); //reset values to repeat traversal of linked list
                outputValue.str("");//clear output value
                homeLeaders.clear();
                awayLeaders.clear();
                namesCount = 0; //reset values
                maxf = 0.0f;
                maxh = 0.0f;
            }
        }
    }
    
    return totalOutput.str(); //return total output, every single place list 
}

template<typename T> 
void Team::checkAndAdd(Player & n, T& currentMax, T valToCompare, std::vector<Player> & homeLeaders, std::vector<Player> & awayLeaders, int& namesCount, T ignores[2])
{
    //no iterator end condition or iterator increment in for parameters to allow for the loop add checks and prepare to possibly reset itself and find the next place
    //this allows this function to find all three places instead of calling this method three times
    //an exit condition of being at the end of the iterator would prevent it from resetting itself and looking for more places
        
    if(valToCompare != ignores[0] && valToCompare != ignores[1]) //ignore the already found places
    {
        if((valToCompare > currentMax) || (valToCompare == 0 && currentMax == 0 && namesCount == 0)) //if valtocompare is greater than or all values are zero (exception because zero cant be greater than zero) or at the beginning of a traverse(namesCount is zero initially)
        {
            currentMax = valToCompare; //update leading value
            homeLeaders.clear();//clear lists because current player has the new highscore
            awayLeaders.clear();
            if(n.team == 'A') //found new highest value, add to list corresponding to player's team
                awayLeaders.push_back(n);
            else if(n.team == 'H')
                homeLeaders.push_back(n);
            namesCount = 1; //there exists someone in nth place
        }
        else if(valToCompare == currentMax)
        {
            if(n.team == 'A') //found matching value, add to list
                awayLeaders.push_back(n);
            else if(n.team == 'H')
                homeLeaders.push_back(n);
            namesCount++;
        }
    }
}

template<typename T>//same idea as above function but checking for minimums, if statement conditions are slightly different for 0
void Team::checkMinAndAdd(Player & n, T& currentMin, T valToCompare, std::vector<Player> & homeLeaders, std::vector<Player> & awayLeaders, int& namesCount, T ignores[2])
{
    if(valToCompare != ignores[0] && valToCompare != ignores[1]) //ignore the already found places
    {
        if((valToCompare < currentMin) || (namesCount == 0)) //if valtocompare is less than or at the beginning of a traverse(namesCount is zero initially)
        {
            //std::cout << "new min found: " << n.name << " " << currentMin << " " << valToCompare << std::endl;
            currentMin = valToCompare; //update leading value
            homeLeaders.clear();//clear lists because now there is one leader
            awayLeaders.clear();
            if(n.team == 'A') //found new highest value, add to list corresponding to player's team
                awayLeaders.push_back(n);
            else if(n.team == 'H')
                homeLeaders.push_back(n);
            namesCount = 1; //there exists someone in nth place
        }
        else if(valToCompare == currentMin)
        {
            if(n.team == 'A') //found matching value, add to list
                awayLeaders.push_back(n);
            else if(n.team == 'H')
                homeLeaders.push_back(n);
            namesCount++;
        }
    }
    
    //std::cout << "current strikeout min leaders: " << mins.str() << "value checked: " << valToCompare << ", current min: " << currentMin << std::endl;
}

void Team::ProcessPlay(std::string line, int play, char team)
{
    long unsigned int spaceLoc = line.find(" ");
    std::string readName = line.substr(0,spaceLoc);
    try//check if player in input play exists
    {
        playerTable.at(readName).incrementStat(play);//player exists, no exception, player is updated
    }
    catch (const std::out_of_range& oor) //out of range exception, player doesn't exist
    {
        playerTable[readName] = Player(readName); //add new player to team
        playerTable[readName].team = team;
        playerTable[readName].incrementStat(play); //and update player's stat
    }
    
}




//Main Functions
void ReadKeyFile(std::unordered_map<std::string, int> & playMap);
void InsertPlayKeys(std::unordered_map<std::string, int> & playMap, int & playNumber, std::ifstream & kFS);
void ReadInputFile(std::unordered_map<std::string, int> & playMap, Team & home, Team & away, Team & league);
void outputUsingFormat(Team & home, Team & away, Team & league);

void ReadKeyFile(std::unordered_map<std::string, int> & playMap)
{
    // 0, 1, 2, 3, 4, 5, 6 possible plays
    std::ifstream kFS;
    kFS.open("keyfile.txt");
    std::string line;
    int currentPlayNumber = 0;
    while(!kFS.eof() && kFS.good())
    {
        getline(kFS, line); //get line from file, assign it to the string line
        //std::cout << line << ", length: " << line.length() << std::endl;
        if(line.length() > 0) //check that line is not blank before making a new player with it
        {
            if(line.at(0) == '#')
            {
                //std::cout << "reading new play codes for " << currentPlayNumber << std::endl;
                InsertPlayKeys(playMap, currentPlayNumber, kFS);
            }
        }
    }
    
    /*
    std::cout << "printing key map:" << std::endl;
    for(std::unordered_map<std::string, int>::iterator it = playMap.begin(); it != playMap.end(); it++)
    {
        std::cout << "[" << it->first << "] :" << it->second << std::endl;
    }
    std::cout << "end of key map" << std::endl << std::endl;
    */
}

void InsertPlayKeys(std::unordered_map<std::string, int> & playMap, int & currentPlayNumber, std::ifstream & kFS)
{
    std::string line;
    while(!kFS.eof() && kFS.good())
    {
        getline(kFS, line); //get line from file, assign it to the string line
        //std::cout << line << ", length: " << line.length() <<std::endl;
        if(line.length() > 0) //check that line is not blank before making a new player with it
        {
            playMap[line] = currentPlayNumber;
        }
        else
            break;
    }
    //std::cout << "incrementing currentPlayNumber" << std::endl;
    currentPlayNumber++;
}

void ReadInputFile(std::unordered_map<std::string, int> & playMap, Team & home, Team & away, Team & league)
{
    std::ifstream inFS; //input file stream
    std::string inputFileName; //file name
    std::cin >> inputFileName; //user inputs filename, sample_stats.txt
   
    inFS.open(inputFileName); //input file stream is opened for previously input filename
   
    std::string line = ""; //string variable copied from each line in the loop
   
    if (!inFS.is_open()) //check if input filestream opened correctly
    {
       std::cout << "problem opening file" << std::endl; //output to console if something goes wrong
       }
   
    while(!inFS.eof() && inFS.good())
    {
       getline(inFS, line); //get line from file, assign it to the string line
       if(line.size()>0) //check that line is not blank before making a new player with it
       {
           std::string name = line.substr(2, line.substr(2).find(" "));
           long unsigned int afterName = ((line.substr(2)).find(" ") + 3);
           std::string playCode = line.substr(afterName);
           int play = playMap[playCode];
           
           //std::cout << "Play is: " << name << ", " << play << " (" << playCode << ")";
           if(line.at(0) == 'H')
            {
                league.ProcessPlay(name, play, 'H');
                home.ProcessPlay(name, play, 'H');
            }
           else if(line.at(0) == 'A')
            {
                league.ProcessPlay(name, play, 'A');
                away.ProcessPlay(name, play, 'A');
            }
       }
    }
    
    inFS.close(); //close input file stream
    
    std::cout << std::endl << std::endl;
}

void outputUsingFormat(Team & home, Team & away, Team & league)
{
    std::cout << "AWAY" << std::endl << away.outputTeam() << std::endl;
    std::cout << "HOME" << std::endl << home.outputTeam() << std::endl;
    std::cout << league.outputLeaders() << std::endl;
}

int main()
{
   Team home;
   Team away;
   Team league;
   
   std::unordered_map<std::string, int> playMap;
   
   ReadKeyFile(playMap);
   
   ReadInputFile(playMap, home, away, league);
   
   outputUsingFormat(home, away, league);
   
   return 0;
}