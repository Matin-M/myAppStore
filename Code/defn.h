#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

using namespace std;

namespace defn {

    struct app_info {
        string category; // Name of category
        string app_name; // Name of the application
        string version; // Version number
        float size; // Size of the application
        string units; // GB or MB
        float price; // Price in $ of the application
    };

    struct app { // A binary search tree
        app_info record; // Information about the application
        app *left;  // Pointer to the left subtree
        app *right;  // Pointer to the right subtree
    };

    struct category {
        string category; // Name of category
        app *root;  // Pointer to root of search tree for this category
    };

    struct hash_table_entry {
        string app_name; // Name of the application
        app *app_node; // Pointer to node in tree containing the application information
        hash_table_entry *next; // Next pointer for chain in separate chaining
    };

}