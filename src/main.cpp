#include "Query.h"
#include "TextQuery.h"
#include <cctype>
#include <cstring>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>

using std::cerr;
using std::cin;
using std::cout;
using std::endl;
using std::ifstream;
using std::map;
using std::set;
using std::string;
using std::vector;

int main(int argc, char **argv) {
    // gets file to read and builds map to support queries
    TextQuery file = get_file(argc, argv);

    do {
        string sought1, sought2;
        // stop if hit eof on input or a "q" is entered
        if (!get_words(sought1, sought2))
            break;

        // find all the occurrences of the requested string
        Query andq = Query(sought1) & Query(sought2);
        cout << "\nExecuting query for: " << andq << endl;
        QueryResult results = andq.eval(file);
        // report matches
        print(cout, results);

        results = Query(sought1).eval(file);
        cout << "\nExecuted query: " << Query(sought1) << endl;
        // report matches
        print(cout, results);

        results = Query(sought2).eval(file);
        cout << "\nExecuted query: " << Query(sought2) << endl;
        // report matches
        print(cout, results);
    } while (true);

    return 0;
}

// int main(int argc, char **argv)
// {
//     // gets file to read and builds map to support queries
//     TextQuery file = get_file(argc, argv);

//     // iterate with the user: prompt for a word to find and print results
//     while (true) {
//         string sought1, sought2, sought3;
//         if (!get_words(sought1, sought2)) break;
//         cout << "\nenter third word: " ;
//         cin  >> sought3;
//         // find all the occurrences of the requested string
//         Query q = Query(sought1) & Query(sought2)
//                              | Query(sought3);
//         cout << "\nExecuting Query for: " << q << endl;
//         const QueryResult results = q.eval(file);
//         // report matches
// 		print(cout, results);
//      }
//      return 0;
// }

// int main(int argc, char **argv)
// {
//     TextQuery file = get_file(argc, argv);

//     // iterate with the user: prompt for a word to find and print results
//     do {
//         string sought;
//         if (!get_word(sought)) break;

//         // find all the occurrences of the requested string
//         // define synonym for the line_no set
//         Query name(sought);
//         const QueryResult results = name.eval(file);
//         cout << "\nExecuting Query for: " << name << endl;

//         // report no matches
//         print(cout, results) << endl;
//     } while (true);  // loop indefinitely; the exit is inside the loop
//     return 0;
// }