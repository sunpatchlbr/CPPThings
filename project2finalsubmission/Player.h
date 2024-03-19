// Luke Ravsten LBR210000 CS2337.002
#ifndef PLAYER_H
#define PLAYER_H
#include <string>
#include <iostream>
#include <iomanip>
class Player
{
   public:
      Player(); //default constructor
      Player(std::string); //overloaded constructor to create player from string from line in input file
      bool operator<(const Player&); //overloaded operators for comparison outside of class
      bool operator>(const Player&);
      bool operator==(const Player&);
      Player& operator=(const Player&); //copy assignment operator
      Player& operator+=(const Player&); //adds parameter's record to this player's record, used when a player has a record on multiple lines in main
      Player(const Player& p); //copy constuctor operator
      friend std::ostream& operator<<(std::ostream& out, const Player& p); //overloaded output stream
      float CalcBattingAverage() const; //functions for calculated floating point values
      float CalcOnBasePercent() const;
      std::string recordString = ""; //default record
      std::string toString() const; //tostring method
      
      const char statChars[6] = {'H', 'O', 'K', 'W', 'P', 'S'}; //char array used to count player's record
      int statsArray[6] = {0}; //player's stats array of integers values, zero by default
      std::string name; //players name
      void ParseRecord(std::string inputString); //method to count player's record from record string
      
      private:
   };
#endif