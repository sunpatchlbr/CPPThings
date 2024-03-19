//LBR210000 Luke Ravsten CS 2337.002
/*#include <string>
#include <iostream>
#include <iomanip>

template <typename T>
class GenericHashMap
{
        public:
                unsigned int currentSize;
        
                T hashtable[] //array of T objects
                int emptyInfo[] //array of same size, 0 means empty since start, 1 means removed, 2 is occupied
                bool isEmpty(int i) {return emptyInfo[i] < 2}
    
                bool isEmptySinceStart(int i) {return emptyInfo[i] < 1}
    
                float calculateLoad() {return num of elements / current size}
    
                default constructor {default size is 11} //will grow as rehashed
    
                bool Insert(T data) // double hashed
    
                Removed(T data) // double hashed
    
                Rehash() //resize table reinsert everything
    
                int GetNewSize() // find next prime after doubling current size
    
                bool isPrime()

        private:
                
}

Rehash()
{
    vector int everything
    
    iterate through table
        if not empty, pushback to vector 
    
    delete old array 
    
    hashtable = new int[GetNewSize()];
    
    iterate through everything vector;
        Insert(item)
}

bool Insert(T data)
{
    if(calculateload > loadlimit)   
        Rehash();

    int i = 0;
    bucketsProbed = 0;
    int maxProbes;
    
    int bucket = hash(data->key) % currentSize
    
    while(bucketsProbed < maxProbes)
    {
        check if hashtable bucket == data key 
            then player already exists
            call += operator to update existing player
            existing object += data //overloaded within player class
    
        //insert into next empty
        if(hashtable[bucket] is empty)
            hashtable[bucket] = data
            return true
            
        //increment i and redo index
        
        i++
        
        bucket = (hash(data->key) + i + i*i)
        bucketsProbed++
    }
    
    return false
}

int GetNewSize()
{
    
    int d = currentSize*2 
    d++;
    
    for(;;d+=2)
    {
        if(isPrime(d))
            return d;
    }
    return 0;
}

bool isPrime(int n)
{
    for(int i = 3; i < root n;i +=2)
    {
        if(n % i == 0)
            return false;
    }
    return true;
}

*/