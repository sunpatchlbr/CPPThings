// Luke Ravsten LBR210000 CS2337.002
#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <fstream>
#include "LinkList.h"
#include "Player.h"


//prototypes
void PrintLeaders(LinkList<Player> l);
std::string makeLeaderString(LinkList<Player> l, int s);

template<typename T>
void checkAndAdd(Node<Player>* tail, Node<Player>* n, T& currentMax, T valToCompare, std::stringstream& names, int& namesCount, T ignores[2], int& c);

template<typename T>
void checkMinAndAdd(Node<Player>* tail, Node<Player>* n, T& currentMin, T valToCompare, std::stringstream& names, int& namesCount, T ignores[2], int& c);

int main()
{ 
   LinkList<Player> League = LinkList<Player>();
   //Linked List of all players in league
   
   std::ifstream inFS; //input file stream
   std::string inputFileName; //file name
   
   std::cin >> inputFileName; //user inputs filename, sample_stats.txt
   
   inFS.open(inputFileName); //input file stream is opened for previously input filename
   
   if (!inFS.is_open()) //check if input filestream opened correctly
   {
      std::cout << "problem opening file" << std::endl; //output to console if something goes wrong
      return -1;
      }
   
   std::string line = ""; //string variable copied from each line in the loop
   //int i = 0;
   while(!inFS.eof() && inFS.good())
   {
      getline(inFS, line); //get line from file, assign it to the string line
      if(line.size()>0) //check that line is not blank before making a new player with it
      {
         //std::cout << "line " << i << std::endl;
         //std::cout << line;
         //instantiate new node with new player from the current file line
         League.Append(Player(line));
         //i++;
         }
      }
   
   inFS.close(); //close input file stream
   
   League.Sort(); //sort league before printing
   
   std::cout << League.toString(); //print each player
   
   PrintLeaders(League); //print leaders
   
   //return zero when everything goes right
   return 0;
   }
   
void PrintLeaders(LinkList<Player> League)
{
   std::cout << std::endl;
   std::cout << "LEAGUE LEADERS" << std::endl; //follow format from rubric
   std::cout << "BATTING AVERAGE" << std::endl << makeLeaderString(League, 6) << std::endl; //output statname, then call makeleaderString for corresponding stat
   std::cout << "ON-BASE PERCENTAGE" << std::endl << makeLeaderString(League, 7) << std::endl;
   std::cout << "HITS" << std::endl << makeLeaderString(League, 0) << std::endl;
   std::cout << "WALKS" << std::endl << makeLeaderString(League, 3) << std::endl;
   std::cout << "STRIKEOUTS" << std::endl << makeLeaderString(League, 2) << std::endl;
   std::cout << "HIT BY PITCH" << std::endl << makeLeaderString(League, 4) << std::endl;
}

std::string makeLeaderString(LinkList<Player> L, int s)
{
    int maxh = 0; //variables for leading values, change as list is traversed
    float maxf = 0.0f;
    Node<Player>* n = L.getHead(); //current node used to traverse linklist
    int c = 0; // counts how many people have been output in total to control output to match format
    int namesCount = 0; //count of names printed to list for a specific place, used to control output to match format
    
    int foundInts[2] = {-1,-1}; // arrays to hold values for highest and second highest values found within list, will be used to ignore later when print leaders for less than first place
    float foundFloats[2] = {-1.0f,-1.0f};
    
    std::stringstream names; //stream for names of leaders for a specfic place
    std::stringstream outputValue; //stream for the value, stringstream is used so both an int or a float can match format in rubric
    std::stringstream totalOutput; //entire string output that is returned as a string at the end of the function
    
    while(n != nullptr && c < 3) //traverse list, stop if 3 places or leaders have been output already
    {
        if(s < 6 && s != 2) //common case where an int stat is being compared to find maximum
            checkAndAdd<int>(L.getTail(), n, maxh, n->getData().statsArray[s], names, namesCount, foundInts, c); 
        else 
        {
            switch(s)
            {
                case 2: //special case find minimum for strikeouts
                    checkMinAndAdd<int>(L.getTail(), n, maxh, n->getData().statsArray[s], names, namesCount, foundInts, c);
                    break;
                case 6: //special case find maximum of calculated float
                    checkAndAdd<float>(L.getTail(), n, maxf, n->getData().CalcBattingAverage(), names, namesCount, foundFloats, c);
                    break;
                case 7: //special case find maximum of calculated float
                    checkAndAdd<float>(L.getTail(), n, maxf, n->getData().CalcOnBasePercent(), names, namesCount, foundFloats, c);
                    break;
            } 
        }
        
        if(n == L.getTail()) //reached end of list
        {
            if(namesCount == 0)
                {
                    c = 3; //no next place value found, stop loop, stop printing out places
                }
            else if(namesCount>0) //at least one nth place holder found, continue to output
            {
                if(s > 5)
                outputValue << std::fixed << std::setprecision(3) << maxf; //match required format if float
                else
                outputValue << maxh;
                
                //update total output 
                totalOutput << outputValue.str() << std::string("\t") << names.str() << std::endl;
                
                c += namesCount; //increase c
                
                n = L.getHead(); //reset values to repeat traversal of linked list
                outputValue.str("");//clear output value
                names.str("");//clear names
                namesCount = 0; //reset values
                maxf = 0.0f;
                maxh = 0.0f;
            }
        }
        else
            n = n->getNext(); //continue traverse
    }
    
    return totalOutput.str(); //return total output, every single place list 
    
}

template<typename T>
void checkAndAdd(Node<Player>* tail, Node<Player>* n, T& currentMax, T valToCompare, std::stringstream& names, int& namesCount, T ignores[2], int& c)
{
    if(valToCompare != ignores[0] && valToCompare != ignores[1]) //ignore the already found places
    {
        if((valToCompare > currentMax) || (valToCompare == 0 && currentMax == 0 && namesCount == 0)) //if valtocompare is greater than or all values are zero (exception because zero cant be greater than zero) or at the beginning of a traverse(namesCount is zero initially)
        {
            currentMax = valToCompare; //update leading value
            names.str("");
            names << n->getData().name; //reset names list
            namesCount = 1; //there exists someone in nth place
        }
        else if(valToCompare == currentMax)
        {
            names << (std::string(", ") + n->getData().name); //found matching value, add to list
            namesCount++;
        }
        if(n == tail && c < 3)
        {
            ignores[c] = currentMax; //add found leading value to values to ignore when searching for lower placed leaders later
        }
      }
}

template<typename T>
void checkMinAndAdd(Node<Player>* tail, Node<Player>* n, T& currentMin, T valToCompare, std::stringstream& names, int& namesCount, T ignores[2], int& c)
{
    if(valToCompare != ignores[0] && valToCompare != ignores[1]) //ignore the already found places
    {
        if((valToCompare < currentMin) || (namesCount == 0)) //if valtocompare is less than or at the beginning of a traverse(namesCount is zero initially)
        {
            currentMin = valToCompare; //update leading value
            names.str("");
            names << n->getData().name; //reset names list
            namesCount = 1; //there exists someone in nth place
        }
        else if(valToCompare == currentMin)
        {
            names << (std::string(", ") + n->getData().name); //found matching value, add to list
            namesCount++;
        }
        if(n == tail && c < 3)
        {
            ignores[c] = currentMin; //add found leading value to values to ignore when searching for lower placed leaders later
        }
      }
}

