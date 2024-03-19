// Luke Ravsten LBR210000 CS2337.002
#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>
#include "Player.h"

Player::Player()
{
   //name = "defaultname";
   //std::string defRec = "HHH";
   //ParseRecord(defRec);
   }
   
Player::Player(std::string inputString)
{
   long unsigned int spaceLocation = inputString.find(" "); //location of space between name and record in inputData string
   name = inputString.substr(0,spaceLocation); //gets the name before space into string name
   std::string record = inputString.substr(spaceLocation+1); //gets data after space into string record
   ParseRecord(record); 
   }

Player& Player::operator=(const Player& p) //copy assignment, copies record from parameter
{
   name = p.name;
   recordString = p.recordString;
   for(int i = 0; i < 6; i++)
      statsArray[i] = p.statsArray[i];
   return *this;
   }

Player::Player(const Player& p) //copy constructor, uses copy assignement operator
{
   operator=(p);
   }

void Player::ParseRecord(std::string inputString)
{
   //std::cout << "old record " << this->recordString;
   this->recordString += inputString; //updates this players record string
   //std::cout << " new record for " << name << " is " << recordString << std::endl;
   
   for(unsigned int i = 0; i < inputString.length(); i++) //iterates through record string to count chars to update statsArray
   {
      for(unsigned int j = 0; j < 6; j++)
      {
         if(inputString.at(i) == statChars[j])
         {
            //std::cout << "j: " << statChars[j] << std::endl;
            statsArray[j]++;
            break;
            }
         }
      }
   }

float Player::CalcBattingAverage() const //returns batting average calculated using formula in rubric
{
   int atBats = statsArray[0] + statsArray[1] + statsArray[2];
   if(atBats == 0) //prevents division by zero
      return 0.0f;
   const float average = (float)statsArray[0] / (float)atBats; //ints cast to floats to do float math
   return average;
   }
   
float Player::CalcOnBasePercent() const //returns on base percent calculated using formula in rubric
{
   int plateAppearances = statsArray[0] + statsArray[1] + statsArray[2] 
   + statsArray[3] + statsArray[4] + statsArray[5];
   if(plateAppearances == 0) //prevents division by zero
      return 0.0f;
   const float onBase = (float)(statsArray[0] + statsArray[3] + statsArray[4]) / (float)plateAppearances; //ints cast to floats to do float math
   return onBase;
   }
   
bool Player::operator<(const Player& p) //comparison operator to compare by alphabetical value of name
{
   //std::cout << "player < operator called for " << this->name << " and " << p.name << (this->name < p.name) << std::endl;
   return this->name < p.name;
   }
   
bool Player::operator>(const Player& p) //comparison operator to compare by alphabetical value of name
{
   //std::cout << "player > operator called for " << this->name << " and " << p.name << (this->name > p.name) << std::endl;
   return this->name > p.name;
   }

bool Player::operator==(const Player& p) //equality operator to check if names are equal
{
   return this->name == p.name;
   }
   
Player& Player::operator+=(const Player& p) //adds the parameter player's record to this player's record
{
   //std::cout << "Player += operator called for " << name << " and " << p.name << std::endl;
   //std::cout << "lhs:" << this->recordString << ", rhs:" << p.recordString << std::endl;
   this->ParseRecord(p.recordString);
   /*for(unsigned int i = 0; i < 6; i++)
   {
      std::cout << this->statsArray[i] << " += " << p.statsArray[i] << std::endl;
      this->statsArray[i] += p.statsArray[i];
      }*/
   //std::cout << "new recordString:" << this->recordString << std::endl;
   return *this;
   }
   
std::string Player::toString() const //outputs player to match format in rubric
{
   std::stringstream s;
   s
   << name << "\t" 
   << (statsArray[0] + statsArray[1] + statsArray[2]) << "\t"
   << statsArray[0] << "\t"
   << statsArray[3] << "\t"
   << statsArray[2] << "\t"
   << statsArray[4] << "\t"
   << statsArray[5] << "\t"
   << std::fixed << std::setprecision(3) << CalcBattingAverage() << "\t" //precision of floating point values matches rubric
   << std::fixed << std::setprecision(3) << CalcOnBasePercent()
   << std::endl;
   return s.str();
   }

std::ostream& operator<<(std::ostream& out, const Player& p) //outputs player to match format in rubric
{
   out 
   << p.name << "\t" 
   << (p.statsArray[0] + p.statsArray[1] + p.statsArray[2]) << "\t"
   << p.statsArray[0] << "\t"
   << p.statsArray[3] << "\t"
   << p.statsArray[2] << "\t"
   << p.statsArray[4] << "\t"
   << p.statsArray[5] << "\t"
   << std::fixed << std::setprecision(3) << p.CalcBattingAverage() << "\t" //precision of floating point values matches rubric
   << std::fixed << std::setprecision(3) << p.CalcOnBasePercent()
   << std::endl;
   return out;
   }