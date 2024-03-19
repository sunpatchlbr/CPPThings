// Luke Ravsten LBR210000 CS2337.002
#ifndef TERM_H
#define TERM_H
#include <string>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <bits/stdc++.h>

template <typename T>
class BinTree;


class Term
{
      public:
            Term() {} //default constructor
            Term(std::string inputString); //constructor from a string from input file
            Term(int exponent, int coefficient, int denominator); //constructor using exponent, coefficient, and denominator, not used
            void antiDerive(); //turns this term into its antiderivative
            void simplify(); //simplifies the fraction
            float CalcSumOverInterval(int lower, int upper); //calculutes the sum of this term over the interval
            
            
            //comparison operators will be used to check term exponent to allow simplification of terms within expression
            bool operator<(const Term&); //overloaded operators for comparison outside of class
            bool operator>(const Term&);
            bool operator==(const Term&);
            
            
            //assignment operators will add coefficients only when exponents match, check in bintree
            //Term& operator=(const Term&); //copy assignment operator
            Term& operator+=(const Term&); //adds parameter's coefficient to this term's coefficient, used when a term has a matching coefficient with a later term
            //Term(const Term& t);
            
            friend std::ostream& operator<<(std::ostream& out, const Term& t); //overloaded output stream
            friend std::stringstream& operator<<(std::stringstream& ssout, const Term& t); //overloaded ss output stream
            
            int getCoefficient(){return coefficient;} //allowes outside use, not really used
            int getExponent(){return exponent;}
            
      private:
            int coefficient = 1;
            int denominator = 1;
            int exponent = 0;
      
};

void Term::simplify()
{
    if(coefficient % denominator == 0) //first check if a whole number can be made
      {
          coefficient = coefficient / denominator;
          denominator = 1;
      }
    else
    {
       for (int i = 2; i <= abs(coefficient) && i <= abs(denominator); i++) //GCD must be found
        {
        if ((coefficient % i == 0) && (denominator % i == 0))
	       {
	            coefficient = coefficient / i;
	            denominator = denominator / i;
	       }
        }
     
    }
    
    if(denominator<0) //move negative to numerator if denominator is negative
        {
            denominator = -denominator;
            coefficient = -coefficient;
        }
    
}

void Term::antiDerive()
{
    if(exponent!=-1) //functions with exponent negative one are taken out to reduce complexity
        denominator = (denominator * exponent+1); //divide denominator by new exponent plus 1
    exponent+=1; //increment exponent
    simplify(); //simplify
}

float Term::CalcSumOverInterval(int lower, int upper)
{
      if(denominator==0 || coefficient == 0) //prevent division by zero and return zero sum if coefficient is zero
            return 0;
      return (((float)coefficient/(float)denominator) * (float)pow(upper, exponent)) - (((float)coefficient/(float)denominator) * (float)pow(lower, exponent));
}

Term::Term(int e, int c, int d) //construct by exponent, coefficient and denominator
{
    this->exponent = e;
    this->coefficient = c;
    this->denominator = d;
    simplify();
}


Term::Term(std::string inputString) //makes a term from a string containing one term
{
      std::string coefficientString = "";
      std::string exponentString = "";
      for(unsigned int i = 0; i < inputString.length();i++)
      {
            for(;i < inputString.length() && inputString.at(i)!='x';i++) //for loop for getting coefficient
            {
                  if(inputString.at(i) == '-' || isdigit(inputString.at(i)))//if negative sign or integer is detected, use stoi to get coefficientString
                        coefficientString += (inputString.at(i));
            }
            if(inputString.find("x")!=std::string::npos) //if x is found then exponent is possibly 1, changed from default 0 exponent
                exponent = 1;
            i++;//skip x
            for(;i < inputString.length();i++)
            {
                if(inputString.at(i) == '^') //found exponent
                {
                    i++; //everything after exponent is added to exponent string
                    for(;i < inputString.length();i++)
                        exponentString+=inputString.at(i);
                }
            }
      }
      if(coefficientString == "-") //if coefficient is just a negative sign then it must be negative 1
            coefficient = -1;
      else if(coefficientString > "") //if coefficient string is not blank then use stoi to get coefficient
            coefficient = std::stoi(coefficientString);
      if(exponentString > "")
            exponent = std::stoi(exponentString);//if exponent string is not blank then use stoi to get exponent
      simplify();//simplify fraction
}

bool Term::operator<(const Term& t) //compare by exponent
{
      return this->exponent < t.exponent;
}

bool Term::operator>(const Term& t) //compare by exponent
{
      return this->exponent > t.exponent;
}

bool Term::operator==(const Term& t) //compare by exponent
{
      return this->exponent == t.exponent;
}

Term& Term::operator+=(const Term& t) //add coefficients
{
      this->coefficient += t.coefficient;
      return *this;
}

/*Term& Term::operator=(const Term& t) 
{
      this->coefficient = t.coefficient;
      this->exponent = t.exponent;
      this->denominator = t.denominator;
      this->simplify();
      return *this;
}*/

std::ostream& operator<<(std::ostream& out, const Term& t) //overloaded output stream
{
    std::stringstream SS;
    SS << t;
    out << SS.str();
    return out;
}

std::stringstream& operator<<(std::stringstream& ssout, const Term& t) //overloaded output stream
{
    //only print coefficient if constant, exponent is zero
    //or coefficient not equal to one
    int outC = abs(t.coefficient);
    if(t.coefficient != 0)
    {
        if(t.coefficient>0) //output signs
            ssout << "+ ";
        else if(t.coefficient<0)
            ssout << "- ";
        if(t.denominator == 1 && (t.exponent == 0 || outC != 1)) //output absolute value of coefficient because sign is already printed
            ssout << outC;
        else if(t.denominator!=1) //if non one denominator then use parentheses
            ssout << "(" << outC << "/" << t.denominator << ")";
        if(t.exponent != 0) //only print x is exponent is not zero
        {
            ssout << "x";
            if(t.exponent != 1)
                ssout << "^" << t.exponent; //only print exponent if not 1
        }
    }
    else
        ssout << "0"; //output 0
    
    return ssout;
}

#endif