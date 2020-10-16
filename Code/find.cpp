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

static bool TestForPrime( int val )
{
    int limit, factor = 2;

    limit = (long)( sqrtf( (float) val ) + 0.5f );
    while( (factor <= limit) && (val % factor) )
        factor++;

    return( factor > limit );
}


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

static int hashFunc(string key)
{
    int charTotal = 0;
    for(int i = 0; i<key.length(); i++)
    {
        charTotal = charTotal + key[i];
    }

    return charTotal % m;
}

static void insertHash(hash_table_entry* input)
{
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

}

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

static void insertHelperBST(string cat, app* newApp)
{

    for(int i = 0; i < n; i++)
    {
        //cout << cat << endl;
        //cout << categories[i]->category;
        if((categories[i]->category).compare(cat) == 0)
        {
            //cout << "INNER LOOP EXECUTED!";
            categories[i]->root = insertBST(categories[i]->root, newApp);
            break;
        }
    }

}




int main() {
    //Testing code.

    /*
    categories = new category*[n];
    createHashTable(n);
    string strCat[3] = {"Games","Productivity","Media"};
    for(int i = 0; i < n; i++)
    {
        category* newCat = new category();
        newCat->category = strCat[i];
        categories[i] = newCat;
    }

    app* testBST = new app();
    testBST->record.app_name = "Test App";
    insertHelperBST("Games", testBST);

    if(categories[0]->root->left == NULL)
    {
        cout << "Root DNE!\n";
    }else{
        cout << "Test NameL: " << categories[0]->root->left->record.app_name;
    }

    //Create and insert a new app
    hash_table_entry* newAppHash = new hash_table_entry();
    newAppHash->app_name = "App 1";
    insertHash(newAppHash);
    */

    cout << "testing1" << endl;

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
    m = stoi(input);
    createHashTable();
    //Populate the data structures with apps.
    for(int i = 0; i < m; i++)
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

        //Insert app into BST
        insertHelperBST(getCat, newApp);
        insertHash(newAppHash);

    }

    hash_table_entry* test1 = searchHash("MC");
    cout << test1->app_name;


    return 0;
}
