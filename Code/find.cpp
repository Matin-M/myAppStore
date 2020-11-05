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
//String value used for search results.
static string searchResult = "";

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
    hash_table_entry* head = hashTable[k];
    while(head != NULL)
    {
        if(head->app_name.compare(key) == 0)
        {
            return head;
        }
        head = head->next;
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
 * Prints out apps in-order for a specific category.
 * @param root
 */
static void inorder(app* root)
{
    if(root != NULL)
    {
        inorder(root->left);

        cout << "\tApplication Name: " << root->record.app_name << endl;
        cout << "\tVersion: " << root->record.version << endl;
        cout << "\tSize: " << root->record.size << endl;
        cout << "\tUnits: " << root->record.units << endl;
        cout << "\tPrice: " << root->record.price << endl << endl;

        inorder(root->right);
    }
}

/**
 * Prints out free apps in-order.
 * @param root
 */
static void findFreeInorder(app* root)
{
    if(root != NULL)
    {
        findFreeInorder(root->left);
        if(stoi(root->record.price) == 0)
        {
            searchResult += "\t" + root->record.app_name + " \n";
        }

        findFreeInorder(root->right);
    }
}

/**
 * Prints out apps in-order within a specific price range.
 * @param root
 */
static void findPriceRange(app* root, float min, float max)
{
    if(root != NULL)
    {
        findPriceRange(root->left, min, max);
        if(stof(root->record.price) >= min && stof(root->record.price) <= max)
        {
            searchResult += "\t" + root->record.app_name + " \n";
        }

        findPriceRange(root->right, min, max);
    }
}

/**
 * Prints out apps in-order within a specific app name range.
 * @param root
 */
static void findAppRange(app* root, string min, string max)
{
    if(root != NULL)
    {
        findAppRange(root->left, min, max);
        if(  (min.compare(root->record.app_name) <= 0) && (max.compare(root->record.app_name) >= 0) )
        {
            searchResult += "\t" + root->record.app_name + " \n";
        }

        findAppRange(root->right, min, max);
    }
}
/**
 * Returns minimum element in the tree for given node.
 * @param node
 * @return
 */
struct app* minValue(struct app* node)
{
    struct app* current = node;

    /* loop down to find the leftmost leaf */
    while (current && current->left != NULL)
        current = current->left;

    return current;
}

/**
 * Delete a node from the BST.
 * @param root
 * @param key
 * @return
 */
struct app* deleteNode(app* root, string key)
{
    // base case
    if (root == NULL)
    {
        return root;
    }

    if ((key).compare(root->record.app_name) < 0)
    {
        root->left = deleteNode(root->left, key);
    }

    else if ((key).compare(root->record.app_name) > 0)
    {
        root->right = deleteNode(root->right, key);
    }


    else
    {
        // node with only one child or no child
        if (root->left == NULL)
        {
            struct app *temp = root->right;
            free(root);
            return temp;
        }
        else if (root->right == NULL)
        {
            struct app *temp = root->left;
            free(root);
            return temp;
        }

        struct app* temp = minValue(root->right);

        root->record = temp->record;

        root->right = deleteNode(root->right, temp->record.app_name);
    }
    return root;
}

void deleteHashEntry(hash_table_entry* head, string key)
{

    hash_table_entry* temp = head;
    hash_table_entry* prev = NULL;

    if (temp != NULL && key.compare(temp->app_name) == 0)
    {
        head = head->next;
        free (temp);
        return;
    }


    while (temp != NULL && key.compare(temp->app_name) != 0)
    {
        prev = temp;
        temp = temp->next;
    }

    if (temp == NULL)
        return;

    prev->next = temp->next;

    free (temp);
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
    while(getline(cin, input) && input.length() != 0)
    {
        searchRequest = "";
        if(input.find("find app") != string::npos)
        {
                for(int i = 10; i < input.length()-1; i++)
                {
                    searchRequest += input[i];
                }
                hash_table_entry* search = searchHash(searchRequest);
                if(search == NULL)
                {
                    cout << "Application " << searchRequest << " not found." << endl << endl;
                }else
                {
                    cout << "Found application: " << searchRequest << endl;
                    cout << "\tCategory: " << search->app_node->record.category << endl;
                    cout << "\tApplication Name: " << search->app_node->record.app_name << endl;
                    cout << "\tVersion: " << search->app_node->record.version << endl;
                    cout << "\tSize: " << search->app_node->record.size << endl;
                    cout << "\tUnits: " << search->app_node->record.units << endl;
                    cout << "\tPrice: " << search->app_node->record.price << endl << endl;
                }


        }else if(input.find("find category") != string::npos)
        {
            searchRequest = "";
            for(int i = 15; i < input.length()-1; i++)
            {
                searchRequest += input[i];
            }

            app* rootNode;
            for(int i = 0; i < n; i++)
            {
                if((categories[i]->category).compare(searchRequest) == 0)
                {
                    rootNode = categories[i]->root;
                    break;
                }
            }

            //Perform inorder traversal.
            if(rootNode != NULL)
            {
                cout << "Category: " << searchRequest << endl;
                inorder(rootNode);
            }else{
                cout << "Category " << searchRequest << " no apps found.";
            }


        }else if(input.find("find price free") != string::npos)
        {
            app* rootNode;
            searchResult = "";
            bool noFree = true;
            for(int i = 0; i < n; i++)
            {
                rootNode = categories[i]->root;
                if(rootNode != NULL)
                {
                    findFreeInorder(rootNode);
                    if(searchResult.compare("") == 0)
                    {

                    }else{
                        cout << "Free Applications in Category: " << categories[i]->category << endl;
                        cout << searchResult << endl;
                        noFree = false;
                    }
                }

                searchResult = "";
            }

            if(noFree == true)
            {
                cout << "No Free Applications found." << endl;
            }

        }else if(input.find("range") != string::npos)
        {

            string categoryName = "";
            searchResult = "";

            //Extract category name.
            for(int i = 7; i < input.length(); i++)
            {
                if(input[i] == '"')
                {
                    break;
                }
                categoryName+=input[i];
            }

            //Extract rootNode from selected category.
            app* rootNode = NULL;
            for(int i = 0; i < n; i++)
            {
                if((categories[i]->category).compare(categoryName) == 0)
                {
                    rootNode = categories[i]->root;
                    break;
                }
            }

            if(input.find("price") != string::npos)
            {
                string low = "";
                string high = "";
                searchResult = "";

                size_t dollarIndexU = input.find("price");
                int dollarIndex = int(dollarIndexU);

                //Calculate upper and lower bounds.
                int j = 0;
                for(j = dollarIndex+6; j < input.size(); j++)
                {
                    if(input[j] != ' ')
                    {
                        low+=input[j];
                    }else{
                        break;
                    }
                }

                for(j = j+1; j < input.size(); j++)
                {
                    if(input[j] != ' ')
                    {
                        high+=input[j];
                    }else{
                        break;
                    }
                }

                //Perform output to the user.
                if(rootNode != NULL)
                {

                    findPriceRange(rootNode, stof(low), stof(high));
                    if(searchResult.compare("") != 0)
                    {

                        cout << "Applications in Price Range ($" << low << ",$" << high << ") in Category: " << categoryName << endl;
                        cout << searchResult << endl;
                    }else
                    {
                        cout << "No applications found in category" << categoryName << " for given price range ($" << low << ",$" << high << ")"  << endl << endl;
                    }

                }else
                {
                    cout << "No apps in price range";
                }

            }else
            {
                searchResult = "";
                size_t uIndex = input.find("app");
                uIndex = input.find('"',uIndex+1);
                int index = int(uIndex);
                string lower;
                string upper;

                //Extract lower range.
                int i = 0;
                for(i = index+1; i < input.size(); i++)
                {
                    if(input[i] != '"')
                    {
                        lower += input[i];
                    }else
                    {
                        break;
                    }

                }

                //Extract upper range.
                for(i = i+3; i < input.size(); i++)
                {
                    if(input[i] != '"')
                    {
                        upper += input[i];
                    }else
                    {
                        break;
                    }

                }


                //Print out results.
                if(rootNode != NULL)
                {

                    findAppRange(rootNode, lower, upper);
                    if(searchResult.compare("") != 0)
                    {
                        cout << "Applications in Range (" << lower << "," << upper << ") in Category: " << categoryName << endl;
                        cout << searchResult << endl;
                    }else
                    {
                        cout << "No applications found in category" << categoryName << " for given range (" << lower << "," << upper << ")"  << endl << endl;
                    }

                }else
                {
                    cout << "No apps in price range";
                }


            }

        }else if(input.find("delete") != string::npos)
        {
            string categoryName = "";
            string appName = "";
            searchResult = "";

            //Extract category name.
            int i = 8;
            for(i; i < input.length(); i++)
            {
                if(input[i] == '"')
                {
                    break;
                }
                categoryName+=input[i];
            }

            //Extract application name.
            for(i = i+3; i < input.length(); i++)
            {
                if(input[i] == '"')
                {
                    break;
                }
                appName+=input[i];
            }

            //Extract rootNode from selected category.
            app* rootNode = NULL;
            for(int i = 0; i < n; i++)
            {
                if((categories[i]->category).compare(categoryName) == 0)
                {
                    rootNode = categories[i]->root;
                    break;
                }
            }

            //Delete node from BST.
            //deleteNode(rootNode, searchHashResult->app_name);

            //Delete from hash table.
            //int posA = hashFunc(searchHashResult->app_name);
            //deleteHashEntry(hashTable[hashFunc(appName)], searchHashResult->app_name);


        }else if(input.find("no report") != string::npos) {

        }else if(input.find("report") != string::npos)
        {

        }




    }

    //Graceful termination.
    //delete(hashTable);
    //delete(categories);


    return 0;
}
