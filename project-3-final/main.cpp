// Luke Ravsten LBR210000 CS2337.002
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include "BinTree.h"
#include "Term.h"
#include "Node.h"

class Expression //container class for expressions made from terms read from input file line
{
    public:
        Expression() {} //default constructor
        Expression(std::string&); // constructor that takes in a string, which would be a copy of a line in the input file, reads terms from string

        Expression returnAntiderivative(); // returns a new expression whose terms are the antiderivatives of this expression
        
        void IntegrateOverInterval(); // outputs the antiderivative, bounds, and then sum over those bounds of this expression
        
        friend std::ostream& operator<<(std::ostream& out, const Expression& e); //output using template
        
        bool isDefinite() {return definite;} //allows outside of class to check if definite
        
    private:
        BinTree<Term> ExpressionTree; //binary tree of type term containing terms which make the expression
        
        bool definite = false; //definite is true if bounds are read from input file line, otherwise false and bounds can be ignored
        int lowerBound = 0;
        int upperBound = 0;
        
        void AntiDerive(Node<Term>* n); //recursive antiderive function for antideriving the entirety of a binary tree of type term's contents
        float CalculateSum(Node<Term>* n); //recursive calculate sum for adding of the sum of the terms of a binary tree of type term
};

float Expression::CalculateSum(Node<Term>* n)
{
      if(n == nullptr) //base case a nullptr node is accessed, returns zero to stop sum
            return 0;
      return n->getData().CalcSumOverInterval(lowerBound, upperBound) + CalculateSum(n->getLeft()) + CalculateSum(n->getRight());
      //calls sum function from term class for the parameter's term and calls this function on the parameter's two children to recursively traverse through entire tree
}

void Expression::AntiDerive(Node<Term>* n)
{
    if(n!=nullptr) //recursion continues if paremeter is not null, stops if null
    {
        n->getData().antiDerive(); //antiderive accessed node's term
        AntiDerive(n->getLeft()); //call recursion on left
        AntiDerive(n->getRight()); //call recursion on right, these go through entire tree and antiderivate each node's term
    }
}

Expression Expression::returnAntiderivative()
{
    Expression antiDerivative; //creates new expression
    antiDerivative.ExpressionTree.recursiveCopy(antiDerivative.ExpressionTree, this->ExpressionTree.getRoot()); //copies this expression's nodes into new expression tree
    AntiDerive(antiDerivative.ExpressionTree.getRoot()); //antiderives all the nodes in new expression's binary tree
    return antiDerivative; // returns new expression
}

void Expression::IntegrateOverInterval()
{
    std::stringstream SS;
    Expression integrated = returnAntiderivative();
    SS << integrated; //inserts antiderivative of this expression into string stream
    std::string anti = SS.str(); //string stream is made into a string then the final space is erased to match format
    anti.erase(anti.length()-1);
    std::cout << anti << ", " << lowerBound << "|" << upperBound << " = " << std::fixed << std::setprecision(3) << CalculateSum(integrated.ExpressionTree.getRoot()) << std::endl; //bounds and sum are output to match format
}

Expression::Expression(std::string &line)
{
    long unsigned int integralLocation = line.find("|"); //position of pipe character and dx are found
    long unsigned int dxLocation = line.find("dx");
    //std::cout << line << std::endl;
    
    //takes in bounds of integral, then cuts string to be only the expression
    if(integralLocation>0 || isdigit(line.at(integralLocation))) //if pipe character is adjacent to bounds
    {
        definite = true; //then expression must be definite
        lowerBound = std::stoi(line.substr(0, integralLocation)); //lower bound is taken from string before pipe character
        long unsigned int firstSpaceAfterUpperBound = line.substr(integralLocation+1).find(" "); //first space found after pipe character would be after the upper bound
        upperBound = std::stoi(line.substr(integralLocation+1)); //upper bound is taken between pipe character and first space found
        line = line.substr(firstSpaceAfterUpperBound+integralLocation+2, (dxLocation - integralLocation - 3) ); //line is shortened to exclude bounds and dx at end
    }
    else
        line = line.substr(integralLocation+2, (dxLocation - integralLocation - 2) ); //otherwise leave as indefinite and shorten string
        
    //std::cout << "cut line to be expression only" << std::endl;
    //std::cout << line << std::endl;
    
    for(unsigned int k = 0; k < line.length(); k++) //remove spaces from line string to make later parsing simpler
    {
        if(line.at(k) == ' ')
            line.erase(line.begin()+k);
    }
    
    //std::cout << "removed spaces from line" << std::endl;
    
    
    std::string singleTermString = ""; //string that as added to to create single terms, then create term objects with, cleared after each object is made
    
    //traverses through entire line, adding one term at a time to the single term string, then inserting that term into the expresion for that line
    for(unsigned int i = 0; i < line.length();i++)//iterate through line character by character, making terms
    {
        //std::cout << line.substr(i) << std::endl;
        //std::cout << "first for iteration " << i << std::endl;
        singleTermString = ""; //clear single term string
        unsigned int j = 0; //iterator for this specific term
        
        //take out from negatives first to simplifiy main for loops within a term's contents in the main line string
        if(line.at(i) == '-')//if negative sign at front, increment j
        {
            singleTermString=+'-';
            j++;
        }
        else if(i>0 && line.at(i-1) == '-')//if negative sign behind, meaning it was skipped by the previous loop on exit, add to string without incrementing j
        {
            singleTermString=+'-';
        }
        
        for(;(i + j < line.length()) && (line.at(i + j) != '+') && (line.at(i + j) != '-'); j++) //create term from closest term in string
        {
            //std::cout << line.substr(i) << std::endl;
            //for(int k = 0; k < j; k++)
            //    std::cout << " ";
            //std::cout << "^" << std::endl;
            
            //std::cout << "second for iteration " << j ;
            if(line.at(i+j) == '^') //start exponent
                {
                    singleTermString+=line.at(i + j); //this makes sure that the exponent symbol and the character at least immediately after are added to prevent the negative in the exponent from being skipped by the for loops condition checks
                    //std::cout << " added to single term string (" << singleTermString << ")" << std::endl;
                    j++;
                }//after those two characters are added, then the normal conditions like exiting on negatives apply
            singleTermString+=line.at(i + j);
            //std::cout << " added to single term string (" << singleTermString << ")" << std::endl;
        }
        i = i + j;//update i after single term string is complete

        //std::cout << "attempting to insert (" << singleTermString << ") into expression tree" << std::endl;
        
        ExpressionTree.RecursiveInsert(Term(singleTermString), ExpressionTree.getRoot()); //create term from single term string and insert into tree 
        //ExpressionTree.Insert(Term(singleTermString)); //create term from single term string and insert into tree 
        
        //std::cout << "inserted into tree" << std::endl;
    }
}

std::ostream& operator<<(std::ostream& out, const Expression& e) //overloaded output stream
{
    std::stringstream SS; //create string stream
    SS << e.ExpressionTree; //output tree to string stream, and turn it into a string to manipulate each character
    std::string treeOutput = SS.str();
    if(treeOutput.substr(0,3) == "- (")//fix negative fractions at front of string to match format
     treeOutput.insert(3,std::string("-"));
    else if(treeOutput.at(0) == '-') //move negative at front next to first coefficient
      treeOutput.insert(2,std::string("-"));
    if(treeOutput.at(0) == '+' || treeOutput.at(0) == '-') //remove either sign at front and space
     treeOutput.erase(0,2);
    out << treeOutput; //now return the formatted string
    return out;
}

int main()
{
   std::vector<Expression> Expressions; //vector of expressions for each line in file
   std::ifstream inFS; //input file stream
   std::string inputFileName; //file name
   
   std::cin >> inputFileName; //user inputs filename
   inFS.open(inputFileName); //input file stream is opened for previously input filename
   
   if (!inFS.is_open()) //check if input filestream opened correctly
   {
      std::cout << "problem opening file" << std::endl; //output to console if something goes wrong
      return -1;
      }
   
   std::string line = ""; //string variable copied from each line in the loop
   while(!inFS.eof() && inFS.good())
   {
      getline(inFS, line); //get line from file, assign it to the string line
      if(line.size()>0) //check that line is not blank before making a new expression with it
         Expressions.push_back(Expression(line)); //create expression from line, add it to expressions vector
   }
   
   inFS.close(); //close input file stream
   std::cout << std::endl;
   
   for(int unsigned i = 0; i < Expressions.size(); i++) //output all expressions from vector
    std::cout << Expressions.at(i) << std::endl;
   std::cout << std::endl;
   
   for(unsigned int i = 0; i < Expressions.size(); i++) //output all integrals from vector
    {
      if(Expressions.at(i).isDefinite()) //if definite then call integrate over integral to output with sum at end
        Expressions.at(i).IntegrateOverInterval();      
      else
        std::cout << Expressions.at(i).returnAntiderivative() << "+ C" << std::endl; //otherwise indefinite and add constant
    }

   std::cout << std::endl;
   return 0;
   }
