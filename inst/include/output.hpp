#ifndef OUTPUT_HPP
#define OUTPUT_HPP

#include <algorithm>
#include <vector>

#include <string>
#include <sstream>

#include <iostream>
#include <iomanip>

#include "data.hpp"

class Output {
public:	
	double aic, sse, cp; 
	std::vector<unsigned int> columns;


	Output( );	
	Output( double, double, double, std::vector<unsigned int>);
	Output* clone() const;
	
	virtual Output& operator=( const Output& );
	virtual bool operator==( const Output& ) const;
	virtual bool operator!=( const Output& ) const;
	virtual bool operator<( const Output& ) const;
	virtual bool operator>( const Output& ) const;
	virtual bool operator>=( const Output& ) const;
	virtual bool operator<=( const Output& ) const;
	
	
	virtual void clear();
	
	
	void print();
	void printMin();
	std::string opString();
	
	double getAIC();
};

void output_finalizer( Output* );

#endif /* OUTPUT_HPP */
