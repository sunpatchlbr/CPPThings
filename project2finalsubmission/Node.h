// Luke Ravsten LBR210000 CS2337.002
#ifndef NODE_H
#define NODE_H
#include "Player.h"
#include <string>
#include <iostream>
#include <sstream>
#include <iomanip>

template <typename T>
class LinkList;

template<typename T>
class Node
{
   public:
      Node(); //default constructor
      Node(T); //construct by input data
      bool operator<(Node& n);
      bool operator>(Node& n);
      bool operator==(Node& n);
      template <typename U> friend std::ostream& operator<<(std::ostream& out, const Node<U>& n); //output using template
      std::string toString() const;
      T& getData(); //return data for outside use
      Node<T>* getNext() {return next;}
   private:
      Node<T>* next = nullptr; //points to next node
      T dataObject; //data
      friend class LinkList<T>; //allow linklist as friend

};
// Luke Ravsten LBR210000 CS2337.002
template<typename T>
Node<T>::Node() //default constructor left empty
{
   }

template <typename T>
Node<T>::Node(T inputObject) //overloaded constructor, construct using data parameter
{
   dataObject = inputObject;
   }

template <typename T>
T& Node<T>::getData() //return data for outside use
{
   return dataObject;
   }

template<typename T>
bool Node<T>::operator<(Node& other) //compare by data
{
   return dataObject < other.getData();
   }
 
template<typename T> 
bool Node<T>::operator>(Node& other) //compare by data
{
   return dataObject > other.getData();
   }

template<typename T>
bool Node<T>::operator==(Node& other) //compare by data
{
   return dataObject == other.getData();
   }

template<typename T>
std::ostream& operator<<(std::ostream& out, const Node<T>& n) //output by data's output operator
{
   out << (n.getData());
   return out;
   }

template<typename T>
std::string Node<T>::toString() const //tostring using output operator of data
{
   std::stringstream s;
   s << dataObject;
   return s.str();
   }

//template class Node<Player>;

#endif