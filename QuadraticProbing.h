#ifndef QUADRATIC_PROBING_H
#define QUADRATIC_PROBING_H

#include <vector>
#include <algorithm>
#include <functional>
#include <string>
#include <iostream>
#include <iomanip>
using namespace std;

int nextPrime( int n );

// QuadraticProbing Hash table class
//
// CONSTRUCTION: an approximate initial size or default of 101
//
// ******************PUBLIC OPERATIONS*********************
// bool insert( x )       --> Insert x
// bool remove( x )       --> Remove x
// bool contains( x )     --> Return true if x is present
// void makeEmpty( )      --> Remove all items
// int hashCode( string str ) --> Global method to hash strings

template <typename HashedObj>
class QuadHashTable
{
public:
    explicit QuadHashTable( int size = 100000 ) : array( nextPrime( size ) )
    { makeEmpty( ); }

    bool contains( const HashedObj & x )
    {
        return isActive( findPos( x ) );
    }

    void makeEmpty( )
    {
        currentSize = 0;
        for( auto & entry : array )
            entry.info = EMPTY;
    }

    void printData(){
      printData1();
    }

    bool insert( const HashedObj & x )
    {
        // Insert x as active
        int currentPos = findPos( x );
        if( isActive( currentPos ) )
            return false;

        if( array[ currentPos ].info != DELETED )
            ++currentSize;

        array[ currentPos ].element = x;
        array[ currentPos ].info = ACTIVE;

        // Rehash; see Section 5.5
        if( currentSize > array.size( ) / 2){

            cout << "rehashing" << endl;
            rehash( );
        }

        return true;
    }

    bool insert( HashedObj && x )
    {
        // Insert x as active
        int currentPos = findPos( x );
        if( isActive( currentPos ) )
            return false;

        if( array[ currentPos ].info != DELETED )
            ++currentSize;

        array[ currentPos ] = std::move( x );
        array[ currentPos ].info = ACTIVE;


        // Rehash; see Section 5.5
        if( currentSize > array.size( ) / 2 ){
            rehash( );
          }

        return true;
    }

    bool remove( const HashedObj & x )
    {
        int currentPos = findPos( x );
        if( !isActive( currentPos ) )
            return false;

        array[ currentPos ].info = DELETED;
        return true;
    }

    enum EntryType { ACTIVE, EMPTY, DELETED };

private:
    struct HashEntry
    {
        HashedObj element;
        EntryType info;

        HashEntry( const HashedObj & e = HashedObj{ }, EntryType i = EMPTY )
        : element{ e }, info{ i } { }

        HashEntry( HashedObj && e, EntryType i = EMPTY )
        : element{ std::move( e ) }, info{ i } { }
    };

    // 4 arrays to hold search data
    int numSearches1[5] = {};
    int numSearches2[5] = { };
    int numProbes1[5] = { };
    int numProbes2[5] = { };

    void printData1(){
      cout << setw(16) << right << "0" << setw(8) << right << "1" << setw(8) << right << "2" << setw(8) << right << "3" << setw(8) << right << "4" << endl;
      cout << setw(15) << left << "numSearches1:" << setw(8) << left << numSearches1[0] << setw(8) << left << numSearches1[1] <<
       setw(8) << left << numSearches1[2] << setw(8) << left << numSearches1[3] << setw(8) << left << numSearches1[4] << endl;

      cout << setw(15) << left << "numSearches2:" << setw(8) << left << numSearches2[0] << setw(8) << left << numSearches2[1] <<
        setw(8) << left << numSearches2[2] << setw(8) << left << numSearches2[3] << setw(8) << left << numSearches2[4] << endl;

      cout << setw(15) << left << "numProbes1:" << setw(8) << left << numProbes1[0] << setw(8) << left << numProbes1[1] <<
         setw(8) << left << numProbes1[2] << setw(8) << left << numProbes1[3] << setw(8) << left << numProbes1[4] << endl;

      cout << setw(15) << left << "numProbes2:" << setw(8) << left << numProbes2[0] << setw(8) << left << numProbes2[1] <<
          setw(8) << left << numProbes2[2] << setw(8) << left << numProbes2[3] << setw(8) << left << numProbes2[4] << endl;

    }


    vector<HashEntry> array;
    int currentSize;

    bool isActive( int currentPos ) const
    { return array[ currentPos ].info == ACTIVE; }

    int findPos( const HashedObj & x )
    {
        int offset = (19 - (myhash(x) % 19));
        int currentPos = myhash( x );
        int curProbes = 0;

        while( array[ currentPos ].info != EMPTY &&
              array[ currentPos ].element != x )
        {
            curProbes++;
            currentPos += offset;  // Compute ith probe
            //offset += 2;
            if( currentPos >= array.size( ) )
                currentPos -= array.size( );
        }

        float xxxx = currentSize;
        float y = array.size();
        float loadFac = xxxx/y;

        if(array[currentPos].element == x){
          //Determin where to place info based on current load factor
          if(loadFac <= .1){
            numProbes1[0] = numProbes1[0] + curProbes;
            numSearches1[0]++;
          }
          else if(loadFac <= .2){
            numProbes1[1] = numProbes1[1] + curProbes;
            numSearches1[1]++;
          }
          else if(loadFac <= .3){
            numProbes1[2] = numProbes1[2] + curProbes;
            numSearches1[2]++;
          }
          else if(loadFac <= .4){
            numProbes1[3] = numProbes1[3] + curProbes;
            numSearches1[3]++;
          }
          else if(loadFac <= .5){
            numProbes1[4] = numProbes1[4] + curProbes;
            numSearches1[4]++;
          }
        }
        else{ // Failed search
          if(loadFac <= .1){
            numProbes2[0] = numProbes2[0] + curProbes;
            numSearches2[0]++;
          }
          else if(loadFac <= .2){
            numProbes2[1] = numProbes2[1] + curProbes;
            numSearches2[1]++;
          }
          else if(loadFac <= .3){
            numProbes2[2] = numProbes2[2] + curProbes;
            numSearches2[2]++;
          }
          else if(loadFac <= .4){
            numProbes2[3] = numProbes2[3] + curProbes;
            numSearches2[3]++;
          }
          else if(loadFac <= .5){
            numProbes2[4] = numProbes2[4] + curProbes;
            numSearches2[4]++;
          }
        }
        return currentPos;
    }

    void rehash( )
    {
        vector<HashEntry> oldArray = array;

        // Create new double-sized, empty table
        array.resize( nextPrime( 2 * oldArray.size( ) ) );
        for( auto & entry : array )
            entry.info = EMPTY;

        // Copy table over
        currentSize = 0;
        for( auto & entry : oldArray )
            if( entry.info == ACTIVE )
                insert( std::move( entry.element ) );
    }

    size_t myhash( const HashedObj & x ) const
    {
        static hash<HashedObj> hf;
        return hf( x ) % array.size( );
    }
};


// LinearHashTable
template <typename HashedObj>
class LinHashTable
{
public:
    explicit LinHashTable( int size = 100000 ) : array( nextPrime( size ) )
    { makeEmpty( ); }

    bool contains( const HashedObj & x )
    {
        return isActive( findPos( x ) );
    }

    void makeEmpty( )
    {
        currentSize = 0;
        for( auto & entry : array )
            entry.info = EMPTY;
    }

    void printData(){
      printData1();
    }

    bool insert( const HashedObj & x )
    {
        // Insert x as active
        int currentPos = findPos( x );
        if( isActive( currentPos ) )
            return false;

        if( array[ currentPos ].info != DELETED )
            ++currentSize;

        array[ currentPos ].element = x;
        array[ currentPos ].info = ACTIVE;

        // Rehash; see Section 5.5
        if( currentSize > array.size( ) / 2 )
            rehash( );

        return true;
    }

    bool insert( HashedObj && x )
    {
        // Insert x as active
        int currentPos = findPos( x );
        if( isActive( currentPos ) )
            return false;

        if( array[ currentPos ].info != DELETED )
            ++currentSize;

        array[ currentPos ] = std::move( x );
        array[ currentPos ].info = ACTIVE;

        // Rehash; see Section 5.5
        if( currentSize > array.size( ) / 2 )
            rehash( );

        return true;
    }

    bool remove( const HashedObj & x )
    {
        int currentPos = findPos( x );
        if( !isActive( currentPos ) )
            return false;

        array[ currentPos ].info = DELETED;
        return true;
    }

    enum EntryType { ACTIVE, EMPTY, DELETED };

private:
    struct HashEntry
    {
        HashedObj element;
        EntryType info;

        HashEntry( const HashedObj & e = HashedObj{ }, EntryType i = EMPTY )
        : element{ e }, info{ i } { }

        HashEntry( HashedObj && e, EntryType i = EMPTY )
        : element{ std::move( e ) }, info{ i } { }
    };

    vector<HashEntry> array;
    int currentSize;

    int numSearches1[5] = {};
    int numSearches2[5] = { };
    int numProbes1[5] = { };
    int numProbes2[5] = { };

    void printData1(){
      cout << setw(16) << right << "0" << setw(8) << right << "1" << setw(8) << right << "2" << setw(8) << right << "3" << setw(8) << right << "4" << endl;
      cout << setw(15) << left << "numSearches1:" << setw(8) << left << numSearches1[0] << setw(8) << left << numSearches1[1] <<
       setw(8) << left << numSearches1[2] << setw(8) << left << numSearches1[3] << setw(8) << left << numSearches1[4] << endl;

      cout << setw(15) << left << "numSearches2:" << setw(8) << left << numSearches2[0] << setw(8) << left << numSearches2[1] <<
        setw(8) << left << numSearches2[2] << setw(8) << left << numSearches2[3] << setw(8) << left << numSearches2[4] << endl;

      cout << setw(15) << left << "numProbes1:" << setw(8) << left << numProbes1[0] << setw(8) << left << numProbes1[1] <<
         setw(8) << left << numProbes1[2] << setw(8) << left << numProbes1[3] << setw(8) << left << numProbes1[4] << endl;

      cout << setw(15) << left << "numProbes2:" << setw(8) << left << numProbes2[0] << setw(8) << left << numProbes2[1] <<
          setw(8) << left << numProbes2[2] << setw(8) << left << numProbes2[3] << setw(8) << left << numProbes2[4] << endl;

    }

    bool isActive( int currentPos ) const
    { return array[ currentPos ].info == ACTIVE; }

    int findPos( const HashedObj & x )
    {
        int offset = 1;
        int currentPos = myhash( x );
        int curProbes = 0;

        while( array[ currentPos ].info != EMPTY &&
              array[ currentPos ].element != x )
        {
            curProbes++;
            currentPos += offset;  // Compute ith probe
            //offset += 2;
            if( currentPos >= array.size( ) )
                currentPos -= array.size( );
        }

        float xxxx = currentSize;
        float y = array.size();
        float loadFac = xxxx/y;

        if(array[currentPos].element == x){
          //Determin where to place info based on current load factor
          if(loadFac <= .1){
            numProbes1[0] = numProbes1[0] + curProbes;
            numSearches1[0]++;
          }
          else if(loadFac <= .2){
            numProbes1[1] = numProbes1[1] + curProbes;
            numSearches1[1]++;
          }
          else if(loadFac <= .3){
            numProbes1[2] = numProbes1[2] + curProbes;
            numSearches1[2]++;
          }
          else if(loadFac <= .4){
            numProbes1[3] = numProbes1[3] + curProbes;
            numSearches1[3]++;
          }
          else if(loadFac <= .5){
            numProbes1[4] = numProbes1[4] + curProbes;
            numSearches1[4]++;
          }
        }
        else{ // Failed search
          if(loadFac <= .1){
            numProbes2[0] = numProbes2[0] + curProbes;
            numSearches2[0]++;
          }
          else if(loadFac <= .2){
            numProbes2[1] = numProbes2[1] + curProbes;
            numSearches2[1]++;
          }
          else if(loadFac <= .3){
            numProbes2[2] = numProbes2[2] + curProbes;
            numSearches2[2]++;
          }
          else if(loadFac <= .4){
            numProbes2[3] = numProbes2[3] + curProbes;
            numSearches2[3]++;
          }
          else if(loadFac <= .5){
            numProbes2[4] = numProbes2[4] + curProbes;
            numSearches2[4]++;
          }
        }

        return currentPos;
    }

    void rehash( )
    {
        vector<HashEntry> oldArray = array;

        // Create new double-sized, empty table
        array.resize( nextPrime( 2 * oldArray.size( ) ) );
        for( auto & entry : array )
            entry.info = EMPTY;

        // Copy table over
        currentSize = 0;
        for( auto & entry : oldArray )
            if( entry.info == ACTIVE )
                insert( std::move( entry.element ) );
    }

    size_t myhash( const HashedObj & x ) const
    {
        static hash<HashedObj> hf;
        return hf( x ) % array.size( );
    }
};

#endif
