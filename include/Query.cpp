#include "Query.h"
#include "TextQuery.h"

#include <memory>
using std::shared_ptr; 

#include <set>
using std::set;

#include <algorithm>
using std::set_intersection;

#include <iostream>
using std::ostream;
using std::cout; using std::cin; 

#include <fstream>
using std::ifstream;

#include <cstddef>
using std::size_t;

#include <iterator>
using std::inserter; 

#include <vector>
using std::vector;

#include <string>
using std::string;

#include <stdexcept>
using std::runtime_error;

// returns the lines not in its operand's result set
QueryResult
NotQuery::eval(const TextQuery& text) const
{
    // virtual call to eval through the Query operand 
    QueryResult result = query.eval(text);

	// start out with an empty result set
    shared_ptr<set<line_no> > ret_lines(new set<line_no>);

	// we have to iterate through the lines on which our operand appears
	QueryResult::line_it beg = result.begin(), end = result.end();

    // for each line in the input file, if that line is not in result,
    // add that line number to ret_lines
	vector<string>::size_type sz = result.get_file()->size();
    for (size_t n = 0; n != sz; ++n) {
		// if we haven't processed all the lines in result
		// check whether this line is present
		if (beg == end || *beg != n) 
			ret_lines->insert(n);  // if not in result, add this line 
		else if (beg != end) 
			++beg; // otherwise get the next line number in result if there is one
	}
	return QueryResult(rep(), ret_lines, result.get_file());
}

// returns the intersection of its operands' result sets
QueryResult
AndQuery::eval(const TextQuery& text) const
{
    // virtual calls through the Query operands to get result sets for the operands
    QueryResult left = lhs.eval(text), right = rhs.eval(text);

	// set to hold the intersection of left and right
    shared_ptr<set<line_no> > ret_lines(new set<line_no>);  

    // writes the intersection of two ranges to a destination iterator
    // destination iterator in this call adds elements to ret
    set_intersection(left.begin(), left.end(), 
                   right.begin(), right.end(),
                   inserter(*ret_lines, ret_lines->begin()));
    return QueryResult(rep(), ret_lines, left.get_file());
}

// returns the union of its operands' result sets
QueryResult
OrQuery::eval(const TextQuery& text) const
{
    // virtual calls through the Query members, lhs and rhs 
	// the calls to eval return the QueryResult for each operand
    QueryResult right = rhs.eval(text), left = lhs.eval(text);  

	// copy the line numbers from the left-hand operand into the result set
	shared_ptr<set<line_no> > 
		ret_lines(new set<line_no>(left.begin(), left.end()));

	// insert lines from the right-hand operand
	ret_lines->insert(right.begin(), right.end());
	// return the new QueryResult representing the union of lhs and rhs
    return QueryResult(rep(), ret_lines, left.get_file());
}

// these functions are declared in Query.h
TextQuery get_file(int argc, char **argv)
{
    // get a file to read from which user will query words
	ifstream infile;
	if (argc == 2)
    	infile.open(argv[1]);
    if (!infile) {
        throw runtime_error("No input file!");
    }

    return TextQuery(infile);  // builds query map
}

bool get_word(string &s1)
{
    cout << "enter a word to search for, or q to quit: ";
    cin >> s1;
    if (!cin || s1 == "q") return false;
    else return true;
}

bool get_words(string &s1, string &s2)
{

    // iterate with the user: prompt for a word to find and print results
    cout << "enter two words to search for, or q to quit: ";
    cin  >> s1;

    // stop if hit eof on input or a "q" is entered
    if (!cin || s1 == "q") return false;
    cin >> s2;
    return true;
}