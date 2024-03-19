// Luke Ravsten LBR210000 CS2337.002
#ifndef LINKLIST_H
#define LINKLIST_H
#include "Player.h"
#include "Node.h"
#include <string>
#include <iostream>
#include <iomanip>

template<typename T>
class LinkList
{
   public:
      LinkList() {}
      
      void Sort(); //default alphabetical
      Node<T>* Search(T t); //searches if a node with value t exists, otherwise returns nullptr
      
      void Append(T t); //Append by data
      void Prepend(T t); //Prepend by data
      void Prepend(Node<T>* n); //Prepend by node pointer
      
      void InsertAfter(Node<T>* n, Node<T>* o); //inserts node n after node o
      Node<T>* FindInsertPos(T data); //finds insert position for data
      
      void RemoveAfter(Node<T>* n); //removes node after n
      
      std::string toString(Node<T>* n); //tostring using input node
      std::string toString(); //overloaded to string that calls previous tostring using this->head as the parameter
      
      Node<T>* getHead() {return head;} //returns head for outside use
      Node<T>* getTail() {return tail;} //returns tail for outside use
      
   private:
      Node<T>* head = nullptr; //head pointer
      Node<T>* tail = nullptr; //tail pointer
   };
   
template<typename T>
void LinkList<T>::Sort()
{
   Node<T>* beforeCurrent = this->head; 
   Node<T>* current = this->head->next; //starts with second thing in list
   while(current!=nullptr) //only proceeds if second thing is not null
   {
      Node<T>* suc = current->next;
      Node<T>* insertPos = FindInsertPos(current->getData()); //find insertposition
      if(insertPos == beforeCurrent) //if insertpos is behind current, current is already in correct location
         beforeCurrent = current;
      else //otherwise current must be moved
      {
         RemoveAfter(beforeCurrent); //remove current
         if(insertPos == nullptr)//special case insert as head
            Prepend(current); 
         else
            InsertAfter(insertPos, current); //otherwise normal
         }
      current = suc; //continue traverse
      }
   }

template<typename T>
Node<T>* LinkList<T>::FindInsertPos(T data) {
   Node<T>* nodeToReturn = nullptr;
   Node<T>* nodeToCheck = head;
   while (nodeToCheck != nullptr && data > nodeToCheck->getData()) { //searches for earliest node that data can be put after
      nodeToReturn = nodeToCheck;
      nodeToCheck = nodeToCheck->next;
   }
   return nodeToReturn;
}

template<typename T>
std::string LinkList<T>::toString(Node<T>* current) //recursive tostring
{
   std::string output = "";
   if(current == nullptr) //base case end of the list
      return output;
   output += (current->getData().toString() + this->toString(current->next)); //add current data to the total string
   return output; //return total string
   }
   
template<typename T>
std::string LinkList<T>::toString()
{
   return toString(head); //call toString beginning at head
   }

template<typename T>
Node<T>* LinkList<T>::Search(T t)
{
   Node<T>* n = head;
   while(n != nullptr)
   {
      if(n->getData() == t) //searches for node with matching value, returns node pointer if found
         return n;
      n = n->next;
      }
   return nullptr; //otherwise returns null
   }

template<typename T>   
void LinkList<T>::Append(T t) //append by data
{
   //std::cout << "searching for t" << std::endl;
   //check if data alread exists
   if(Search(t) != nullptr)
   {
      //std::cout << "found existing" << std::endl;
      Search(t)->getData() += t; //if matching node already exists, add this data to that data, instead of creating a duplicate node
      }
   else
   {
      //std::cout << "no existing found, making new node" << std::endl;
      Node<T>* n = new Node<T>(t);
      //std::cout << "append reached";
      if(head == nullptr) //special case empty list
      {
         head = n;
         tail = n;
         }
      else //normal case, add as tail
      {
         tail->next = n;
         tail = n;
         }
      //std::cout << "new node's data's record:" << n->getData() << std::endl; 
      }
   }
   
template<typename T>   
void LinkList<T>::Prepend(T t) //prepend by data
{
   Node<T>* n = new Node<T>(t);
   if(head == nullptr) //special case empty list
   {
      head = n;
      tail = n;
      }
   else
   {
      n->next = head; //normal case add as head
      head = n;
      }
   }

template<typename T>   
void LinkList<T>::Prepend(Node<T>* n) //prepend by node
{
   if(head == nullptr) //special case empty list
   {
      head = n;
      tail = n;
      }
   else
   {
      n->next = head; //normal case add as head
      head = n;
      }
   }
   
template<typename T>
void LinkList<T>::InsertAfter(Node<T>* o, Node<T>* n)
{
   if(head == nullptr) //special case empty list
   {
      head = n;
      tail = n;
      }
   else if(o == tail) //special case insert after tail
   {
      n->next = nullptr;
      o->next = n;
      tail = n;
      }
   else //normal case insert inside list
   {
      n->next = o->next;
      o->next = n;
      }
   }
   
template<typename T>
void LinkList<T>::RemoveAfter(Node<T>* n)
{
   if(n == nullptr && head != nullptr) //special case remove head
   {
      head = head->next;
      if(head->next == nullptr)
         tail = nullptr;
      }
      else if(n->next != nullptr) //normal case remove after node in list
      {
         if(n->next->next == nullptr) //special case remove tail
            tail = n;
         n->next = n->next->next;
         }
   }
   

#endif