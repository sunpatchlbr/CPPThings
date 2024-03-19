// Luke Ravsten LBR210000 CS2337.002
#include "Node.h"
#include "Term.h"
#include <string>
#include <iostream>
#include <sstream>
#include <iomanip>

template<typename T>
class BinTree
{
public:
  BinTree(){} //default constructor
  void output (std::ostream& out, Node<T>* n) const; //output tree to stream, not used anymore
  void output (std::stringstream& ssout, Node<T>* n) const ; //output tree to stream, not used anymore
  
  //Node<T>* Search (T key);	//search by data
  Node<T>* RecursiveSearch(Node<T>* n, T key);
  
  //void Insert (T data);		// basic insert function by data
  //void Insert (Node<T>* n);	// basic insert by node pointer
  Node<T>* RecursiveInsert(T data, Node<T>* treeNode); //recursive insert by data
  Node<T>* RecursiveInsert(Node<T>* n, Node<T>* treeNode); //recursive insert by node
  
  Node<T>* Remove (T data);	//remove by data
  Node<T>* RecursiveRemove(T data, Node<T>* current, Node<T>* parent); //recursive remove by data
  
  Node<T>* getRoot() const {return root;} //return root pointer
  
  BinTree(const BinTree<T>&); //copy constructor
  BinTree<T>& operator=(const BinTree<T>&); //copy assignement operator
  void recursiveCopy(BinTree<T>& newTree, Node<T>* n); //recursive copy constructor, used by copy constructor and copy assignment
  
private:
  Node<T>* root = nullptr;		// points to root Node
};

template<typename T>
BinTree<T>& BinTree<T>::operator=(const BinTree<T>& BT)
{
    //std::cout<<"operator= called" << std::endl;
    recursiveCopy(*this, BT.root); //use recursivey copy starting at root
    return *this;
}

template<typename T>
BinTree<T>::BinTree(const BinTree<T>& BT)
{
    //std::cout<<"copy constructor called" << std::endl;
    operator=(BT); //use copy assignment
}

template<typename T>
void BinTree<T>::recursiveCopy(BinTree<T>& newTree, Node<T>* newNode) //copy new node and all its descendants into the new tree
{
      //std::cout<<"recursive copy called" << std::endl;
      if(newNode != nullptr) //stop recursion at base case a nullptr node
      {
            newTree.RecursiveInsert(newNode->dataObject, newTree.getRoot());//insert current node into new tree
            recursiveCopy(newTree,newNode->left);//recursive copy left descendants
            recursiveCopy(newTree,newNode->right);//recursive copy right descendants
      }
}

template<typename U>
std::ostream& operator<<(std::ostream& out, const BinTree<U>& BT) //output by data's output operator
{
   BT.output(out, BT.getRoot()); //use recursive output
   return out;
   }

template<typename T>
void BinTree<T>::output (std::ostream& out, Node<T>* n) const //recursively outputs greatest to least
{
      if(n == nullptr) //stop at nullptr
            return;
      output(out, n->right); //recursive output right descendants
      out << n->dataObject << " "; //in order output greatest to least
      output(out, n->left); //recursive output left descendants
}

template<typename T>
void BinTree<T>::output (std::stringstream& ssout, Node<T>* n) const //recursively outputs nodes greatest to least
{
      if(n == nullptr)
            return;
      output(ssout, n->right); //recursive output right descendants
      ssout << n->dataObject << " "; //in order output greatest to least
      output(ssout, n->left); //recursive output left descendants
}

template<typename T>
Node<T>* BinTree<T>::RecursiveInsert(T data, Node<T>* treeNode) //textbook insert function, with added equality case to allow terms of like exponent to be added together immediately
{
    if(treeNode != nullptr) //input was not null traverse tree if not inserting as root
    {
        if(data < treeNode->dataObject)
        {
            if(treeNode->left == nullptr) 
            {
                Node<T>* n = new Node<T>(data);
                treeNode->left = n; //insert as left
                return n;
            }
            else
                RecursiveInsert(data, treeNode->left); //continue traverse left
        }
        else if(data == treeNode->dataObject)
        {
              treeNode->dataObject += data; //add coefficients only if exponents match
              return treeNode; //break loop
        }
        else
        {
            if(treeNode->right == nullptr)
            {
                Node<T>* n = new Node<T>(data);
                treeNode->right = n; //insert as right
                return n;
            }
            else
                RecursiveInsert(data, treeNode->right); //continue traverse right
        }
    }
    else //treenode was null so tree is empty, insert at root
    {
        Node<T>* n = new Node<T>(data);
        root = n;
        return n;
    }
    return nullptr;
}

template<typename T>
Node<T>* BinTree<T>::RecursiveInsert(Node<T>* n, Node<T>* treeNode) //textbook insert function, with added equality case to allow terms of like exponent to be added together immediately
{
    if(treeNode != nullptr) //traverse non empty tree
    {
        if(n->dataObject < treeNode->dataObject)
        {
            if(treeNode->left == nullptr) 
            {
                treeNode->left = n; //insert as left
                return n;
            }
            else
                RecursiveInsert(n, treeNode->left); //continue traverse left
        }
        else if(n->dataObject == treeNode->dataObject)
        {
              treeNode->dataObject += n->dataObject; //add coefficients only if exponents match
              return treeNode; //break loop
        }
        else
        {
            if(treeNode->right == nullptr)
            {
                treeNode->right = n; //insert as right
                return n;
            }
            else
                RecursiveInsert(n, treeNode->right); //continue traverse right
        }
    }
    else //treenode was null so tree is empty, insert at root
        {
            treeNode = n;
            return n;
        }
}


//previous non recursive functions
/*template<typename T>
void BinTree<T>::Insert(Node<T>* n) //textbook insert function, with added equality case to allow terms of like exponent to be added together immediately
{
    Node<T>*current = root;
    if(root == nullptr) //empty tree cases
        root = n;
    else
    {
        while(current != nullptr) //traverse tree
        {
            if(n->dataObject < current->dataObject)
            {

                if(current->left == nullptr) 
                {
                    current->left = n; //insert as left
                    current = nullptr;
                }
                else
                    current = current->left; //continue traverse left
            }
            else if(n->dataObject == current->dataObject)
            {
                  current->dataObject += n->dataObject; //add coefficients only if exponents match
                  current = nullptr; //break loop
            }
            else
            {
                if(current->right == nullptr)
                {
                    current->right = n; //insert as right
                    current = nullptr;
                }
                else
                    current = current->right; //continue traverse right
            }
        }
    }
}

template<typename T>
void BinTree<T>::Insert(T data) //insert by data, makes a new node from data then inserts
{
    Node<T>* n = new Node<T>(data);
    Insert(n);
}*/

//previous non recursive functions
/*template<typename T>
Node<T>* BinTree<T>::Search(T key)
{
    Node<T>* current = root; //start at root
    while(current != nullptr)
    {
        if(current->data == key) //if matches key return current
            return current;
        else if(key < current->data) //continue search to the left
            current = current->left;
        else
            current = current->right; //continue search to the right
    }
    return nullptr;
}*/

template<typename T>
Node<T>* BinTree<T>::RecursiveSearch(Node<T>* n, T key)
{
    if (n != nullptr)
    {
        if(n->data == key) //if matches key return current
            return n;
        else if(key < n->data) //continue search to the left
            Search(n->left, key);
        else
            Search(n->right, key); //continue search to the right
    }
    else
        return nullptr;
}

/*template<typename T>
Node<T>* BinTree<T>::Remove(T key)
{
    //cases  
    //remove leaf, break from tree, return node
    //remove internal with one child, either right or left, connect parent to removed node's single child, break node away from tree and return
    //remove internal with two children, find successor to removed node, which is the leftmost child of right sub tree, assign node's data as successive node's data, remove successive node, return node
    //not found, return null
      
    Node<T>* parent = nullptr; //start traverse at root of tree
    Node<T>* current = root;
      
    while (current != nullptr)
    {
        if(current->data == key)
        {
            if(current->left && current->right == nullptr) //remove leaf
            {
                if(parent == nullptr) //remove root which is a leaf
                    root = nullptr;
                else if(parent->left == current)
                    parent->left = nullptr;
                else
                    parent->right = nullptr;
                  }
                else if(current->right == nullptr) // remove node with one left child
                {
                    if(parent == nullptr) //remove root case
                        root  = current->left;
                    else if(parent->left == current)
                        parent->left = current->left;
                    else
                        parent->right = current->left;
                }
                else if(current->right == nullptr) //remove node with one right child
                {
                    if(parent == nullptr) //remove root
                        root  = current->right;
                    else if(parent->left == current)
                        parent->left = current->right;
                    else
                        parent->right = current->right;
                }
                else //remove node with two children
                {
                    Node<T>* successive = current->right;
                    while(successive != nullptr) //obtains leftmost of right subtree
                    {
                        successive = successive->left;
                    }
                        
                    T sucData = successive->data; //copy data
                    BSTRemove(sucData); //remove successor
                    current->data = sucData;
                }
                return current; // may need to be changed
            }
            else if(current->data < key) //continue search right
            {
                parent = current;
                current = current->right;
            }
            else //continue search left
            {
                parent = current;
                current = current->left;
            }
        
      }
    return nullptr;
    
    
}*/

template<typename T> //current will be initially called with root, and parent intially nullptr
Node<T>* BinTree<T>::RecursiveRemove(T key, Node<T>* current, Node<T>* parent)
{
    //cases  
    //remove leaf, break from tree, return node
    //remove internal with one child, either right or left, connect parent to removed node's single child, break node away from tree and return
    //remove internal with two children, find successor to removed node, which is the leftmost child of right sub tree, assign node's data as successive node's data, remove successive node, return node
    //not found, return null
      
    if (current != nullptr) // if root isnt null
    {
        if(current->data == key) //if key is found
        {
            if(current->left && current->right == nullptr) //remove leaf
            {
                if(parent == nullptr) //remove root which is a leaf
                    root = nullptr;
                else if(parent->left == current) //update parent pointers
                    parent->left = nullptr;
                else
                    parent->right = nullptr;
            }
            else if(current->right == nullptr) // remove node with one left child
            {
                if(parent == nullptr) //remove root case
                    root  = current->left; //root becomes left child
                else if(parent->left == current)
                    parent->left = current->left; //current is a left child, parent's left becomes current's left child
                else
                    parent->right = current->left; //current is a right child, parent's right becomes right child
            }
            else if(current->left == nullptr) //remove node with one right child
            {
                if(parent == nullptr) //remove root
                    root  = current->right; //root becomes right child
                else if(parent->left == current)
                    parent->left = current->right; //current is a left child, parent's left becomes current's right
                else
                    parent->right = current->right; //current is right child, parent's right becomes current's right
            }
            else //remove node with two children
            {
                Node<T>* successive = current->right; //find next greatest node in entire tree
                while(successive->left != nullptr) //obtains leftmost of right subtree
                {
                    successive = successive->left;
                }
                T sucData = successive->data; //copy data
                RecursiveRemove(sucData, root, nullptr); //remove successor node from tree
                current->data = sucData; //change current data to what was the successive's data to maintain identity of binary tree
            }
            return current; // returns node removed from tree
            }
        else if(current->data < key) //continue traverse right
        {
            RecursiveRemove(key, current->left, current); //pass in left child as current and current as parent
        }
        else //continue traverse left
        {
            RecursiveRemove(key, current->right, current); //pass in right child as current and current as parent
        }
        
      }
    return nullptr; //if root is null, tree is empty return null
}



