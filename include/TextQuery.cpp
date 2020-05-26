#include "TextQuery.h"

#include <cstddef>
#include <memory>  // 
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <iostream>
#include <fstream>
#include <cctype>
#include <cstring>
#include <utility>

using std::size_t;
using std::shared_ptr;
using std::istringstream;
using std::string;
using std::getline;
using std::vector;
using std::map;
using std::set;
using std::cerr;
using std::cout;
using std::cin;
using std::ostream;
using std::endl;
using std::ifstream;
using std::ispunct;
using std::tolower;
using std::strlen;
using std::pair;

// because we can't use auto, we'll define typedefs 
// to simplify our code

// type of the lookup map in a TextQuery object
typedef map<string, shared_ptr<set<TextQuery::line_no> > > wmType;

// the iterator type for the map
typedef wmType::const_iterator wmIter;

// type for the set that holds the line numbers
typedef shared_ptr<set<TextQuery::line_no> > lineType;

// iterator into the set
typedef set<TextQuery::line_no>::const_iterator lineIter;

// read the input file and build the map of lines to line numbers
TextQuery::TextQuery(ifstream &is): file(new vector<string>)
{
    string text;
    while (getline(is, text)) {       // for each line in the file
		file->push_back(text);        // remember this line of text
		int n = file->size() - 1;     // the current line number
		istringstream line(text);     // separate the line into words
		string word;               
		while (line >> word) {        // for each word in that line
            word = cleanup_str(word);
            // if word isn't already in wm, subscripting adds a new entry
            lineType &lines = wm[word]; // lines is a shared_ptr 
            if (!lines) // that pointer is null the first time we see word
                lines.reset(new set<line_no>); // allocate a new set
            lines->insert(n);      // insert this line number
		}
	}
}

// not covered in the book -- cleanup_str removes
// punctuation and converts all text to lowercase so that
// the queries operate in a case insensitive manner
string TextQuery::cleanup_str(const string &word)
{
    string ret;
    for (string::const_iterator it = word.begin(); 
				it != word.end(); ++it) {
        if (!ispunct(*it))
            ret += tolower(*it);
    }
    return ret;
}

QueryResult
TextQuery::query(const string &sought) const
{
	// we'll return a pointer to this set if we don't find sought
	static lineType nodata(new set<line_no>); 

    // use find and not a subscript to avoid adding words to wm!
    wmIter loc = wm.find(cleanup_str(sought));

	if (loc == wm.end()) 
		return QueryResult(sought, nodata, file);  // not found
	else 
		return QueryResult(sought, loc->second, file);
}

// 功能：当输入文本word数超过1时在word后面加s
std::string make_plural(size_t ctr, const std::string &word, const std::string &ending) {
    return (ctr == 1) ? word : word + ending; 
}

ostream &print(ostream & os, const QueryResult &qr)
{
    // if the word was found, print the count and all occurrences
    os << qr.sought << " occurs " << qr.lines->size() << " "
       << make_plural(qr.lines->size(), "time", "s") << endl;

    // print each line in which the word appeared
	// for every element in the set 
	for (lineIter num = qr.lines->begin();
				num != qr.lines->end(); ++num) 
		// don't confound the user with text lines starting at 0
        os << "\t(line " << *num + 1 << ") " 
		   << *(qr.file->begin() + *num) << endl;
	return os;
}

// debugging routine, not covered in the book
void TextQuery::display_map()
{
    wmIter iter = wm.begin(), iter_end = wm.end();

    // for each word in the map
    for ( ; iter != iter_end; ++iter) {
        cout << "word: " << iter->first << " {";

        // fetch location vector as a const reference to avoid copying it
        lineType text_locs = iter->second;
        lineIter loc_iter = text_locs->begin(),
                        loc_iter_end = text_locs->end();

        // print all line numbers for this word
        while (loc_iter != loc_iter_end)
        {
            cout << *loc_iter;

            if (++loc_iter != loc_iter_end)
                 cout << ", ";

         }

         cout << "}\n";  // end list of output this word
    }
    cout << endl;  // finished printing entire map
}
