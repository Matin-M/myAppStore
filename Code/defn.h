#include <stdlib.h>
#include <stdio.h>
#include <string.h>

using namespace std;

//Define a namespace to avoid redundant resolution operator calls.
namespace defn {

    //Struct containing app details.
    struct app_info {
        string category; // Name of category
        string app_name; // Name of the application
        string version; // Version number
        string size; // Size of the application
        string units; // GB or MB
        string price; // Price in $ of the application
    };

    //Struct for BST insertion.
    struct app { // A binary search tree
        app_info record; // Information about the application
        app *left;  // Pointer to the left subtree
        app *right;  // Pointer to the right subtree
    };

    //Struct used to contain app categories.
    struct category {
        string category; // Name of category
        app *root;  // Pointer to root of search tree for this category
    };

    //Struct for hashtable entries.
    struct hash_table_entry {
        string app_name; // Name of the application
        app *app_node; // Pointer to node in tree containing the application information
        hash_table_entry *next; // Next pointer for chain in separate chaining
    };

}