// Luke Ravsten LBR210000 CS2337.002


//copied and rewritten from previous projects
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
      char team;
      
      void incrementStat(int whichStat);
      
      bool operator<(const Player&); //overloaded operators for comparison outside of class
      bool operator>(const Player&);
      bool operator==(const Player&);
      Player& operator=(const Player&); //copy assignment operator
      Player& operator+=(const Player&); //adds parameter's record to this player's record, used when a player has a record on multiple lines in main
      Player(const Player& p); //copy constuctor operator
      friend std::ostream& operator<<(std::ostream& out, const Player& p); //overloaded output stream
      
      int CalcAtBats() const;
      int CalcPlateAppearances() const;
      float CalcBattingAverage() const; //functions for calculated floating point values
      float CalcOnBasePercent() const;
      std::string toString() const; //tostring method
      
      //0: outs
      //1: strikeout
      //2: hits
      //3: walk
      //4: sacrifice
      //5: hit by pitch
      //6: error
      
      int statsArray[7] = {0}; //player's stats array of integers values, zero by default
      std::string name; //players name
      
      private:
   };
#endif