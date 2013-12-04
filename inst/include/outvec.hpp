#ifndef OUTVEC_HPP
#define OUTVEC_HPP

#include "output.hpp"

#include <stdexcept>

#include <algorithm>
#include <vector>
#include <string>

#include <iostream>
#include <iomanip>

#include <RcppArmadillo.h>

class OutVec {
public:
	std::vector< Output > data;
	const static size_t max_size = 25;
	
	
	OutVec( );
	OutVec( std::vector< Output > );
	OutVec( const OutVec& );
	
	virtual OutVec* clone() const;
	
//	OutVec( SEXP );
//	operator SEXP();
	
	virtual OutVec& operator=( const OutVec& ); 
	virtual bool operator==(const OutVec& ) const;
	virtual bool operator!=(const OutVec& ) const;
	virtual Output& operator[]( size_t );
	
	Output& at( size_t );
	size_t size();
	void sort();
	void prune();
	virtual void clear();
	
	void insert( Output );
	void insert( const OutVec& ); 
	
	const std::vector< Output >& getData() const;
	
	virtual void print();
};

void outvec_finalizer( OutVec* );

#endif /* OUTVEC_HPP */