#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "defn.h"

using namespace std;
using namespace defn;

//n represents number of categories.
static int n;
//Static array holding categories.
static category** categories = NULL;
//Static array holding hashTable.
static hash_table_entry** hashTable;
//Size of hashtable.
static int m = 0;

/**
 * Test if a number is prime or not.
 * @param val
 * @return
 */
static bool TestForPrime( int val )
{
    int limit, factor = 2;

    limit = (long)( sqrtf( (float) val ) + 0.5f );
    while( (factor <= limit) && (val % factor) )
        factor++;

    return( factor > limit );
}

/**
 * Instantiates a new hash table.
 */
static void createHashTable()
{
    m = m*2;
    bool cond = true;
    while (true)
    {
        m++;
        if(TestForPrime(m))
        {
            break;
        }
    }
    //Instantiate new hashtable array.
    hashTable = new hash_table_entry*[m];

}

/**
 * Hash function that generates a position using an input key.
 * @param key
 * @return
 */
static int hashFunc(string key)
{
    int charTotal = 0;
    for(int i = 0; i<key.length(); i++)
    {
        charTotal = charTotal + key[i];
    }

    return charTotal % m;
}

/**
 * Inserts an app into the hash table using separate chaining.
 * @param input
 */
static void insertHash(hash_table_entry* input)
{
    //Insert at head.
    string appName = input->app_name;
    int k = hashFunc(appName);

    if(hashTable[k] == NULL)
    {
        hashTable[k] = input;
    }else{
        input->next = hashTable[k];
        hashTable[k] = input;
    }

}

/**
 * Search hash table and associated linkedlists.
 * @param key
 * @return
 */
static hash_table_entry* searchHash(string key)
{
    int k = hashFunc(key);
    hash_table_entry* l = hashTable[k];
    while(l)
    {
        if(l->app_name.compare(key) == 0)
        {
            return l;
        }
        l = l->next;
    }
    return NULL;
}

/**
 * A recursive procedure that inserts a new node into a binary search tree.
 * @param rootNode
 * @param newApp
 * @return
 */
static app* insertBST(app* rootNode, app* newApp)
{
    if(rootNode == NULL)
    {
        rootNode = newApp;
    }
    else if((rootNode->record.app_name).compare(newApp->record.app_name) < 0)
    {
        rootNode->right = insertBST(rootNode->right, newApp);
    }
    else
    {
        rootNode->left = insertBST(rootNode->left, newApp);
    }

    return rootNode;
}

/**
 * A helper function used to narrow down app category to search in.
 * @param cat
 * @param newApp
 */
static void insertHelperBST(string cat, app* newApp)
{

    for(int i = 0; i < n; i++)
    {

        if((categories[i]->category).compare(cat) == 0)
        {
            categories[i]->root = insertBST(categories[i]->root, newApp);
            break;
        }
    }

}

/**
 * Main method.
 * @return
 */
int main() {
    //Parse text file.
    string input = "";
    getline(cin, input);
    n = stoi(input);
    categories = new category*[n];
    //Obtain categories and store in category array.
    for(int i = 0; i < n; i++)
    {
        getline(cin, input);
        category* newCat = new category();
        newCat->category = input;
        categories[i] = newCat;
    }

    //Get number of apps.
    getline(cin, input);
    int numApps = stoi(input);
    m = numApps;
    createHashTable();
    //Populate the data structures with apps.
    for(int i = 0; i < numApps; i++)
    {
        int catIndex = 0;
        string getCat = "";
        getline(cin, getCat);

        //New hash table and BST entries
        app* newApp = new app();
        hash_table_entry* newAppHash = new hash_table_entry();
        newAppHash->app_node = newApp;

        //Store all attributes within strings
        string appName, version, size, units, price = "";
        getline(cin, appName);
        getline(cin, version);
        getline(cin, size);
        getline(cin, units);
        getline(cin, price);

        //Assign properties
        newAppHash->app_name = appName;
        newApp->record.app_name = appName;
        newApp->record.version = version;
        newApp->record.size = size;
        newApp->record.units = units;
        newApp->record.price = price;
        newApp->record.category = getCat;

        //Insert app into BST
        insertHelperBST(getCat, newApp);
        insertHash(newAppHash);

    }
    //Skip next line, move onto request lines.
    getline(cin, input);

    //Process requests.
    string searchRequest = "";
    input = "";
    while(getline(cin, input))
    {
        searchRequest = "";
        if(input.length() != 0)
        {
            for(int i = 9; i < input.length(); i++)
            {
                searchRequest += input[i];
            }
            hash_table_entry* search = searchHash(searchRequest);
            if(search == NULL)
            {
                cout << "Application " << searchRequest << " not found." << endl;
            }else
            {
                cout << "Found application: " << searchRequest << endl;
                cout << "\tCategory: " << search->app_node->record.category << endl;
                cout << "\tApplication Name: " << search->app_node->record.app_name << endl;
                cout << "\tVersion: " << search->app_node->record.version << endl;
                cout << "\tSize: " << search->app_node->record.category << endl;
                cout << "\tUnits: " << search->app_node->record.units << endl;
                cout << "\tPrice: " << search->app_node->record.price << endl << endl;
            }

        }
    }

    return 0;
}
