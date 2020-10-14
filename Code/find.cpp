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


static void createHashTable(int n)
{
    m = n*2;
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
    else if((rootNode->record.app_name).compare(newApp->record.app_name) > 0)
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
    category* bin = NULL;
    for(int i = 0; i < n; i++)
    {
        if((categories[i]->category).compare(cat) == 0)
        {
            bin = categories[i];
        }
    }

    app* categoryNode = bin->root;
    bin->root = insertBST(categoryNode, newApp);
}




int main() {
    cout << "Find.cpp" << endl;

    n = 3;
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

    app* testBST2 = new app();
    testBST2->record.app_name = "Test App2";
    insertHelperBST("Games", testBST2);


    if(categories[0]->root->left == NULL)
    {
        cout << "Root DNE!\n";
    }else{
        cout << "Test NameL: " << categories[0]->root->left->record.app_name;
    }

    //Create and insert a new app
    hash_table_entry* newAppHash = new hash_table_entry();
    newAppHash->app_name = "App 1";
    hash_table_entry* newAppHash2 = new hash_table_entry();
    newAppHash->app_name = "App 1";
    insertHash(newAppHash);
    insertHash(newAppHash2);

    hash_table_entry* test = searchHash("Random APp");
    if(test == NULL)
    {
        cout << "Null!";
    }else{
        cout << test->app_name;
    }


    return 0;
}
