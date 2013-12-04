/**
 * @file ls.hpp
 * @author Robert Smith <rws20@zips.uakron.edu>
 */

#ifndef LS_HPP
#define LS_HPP

#include "next_combination.hpp"
#include "data.hpp"
#include "output.hpp"
#include "outvec.hpp"

#include <stdexcept>
#include <algorithm>
#include <vector>
#include <limits>

#include <iostream>
#include <iomanip>

#include <omp.h>
#include <RcppArmadillo.h>


// LS for "least squares"
// [[Rcpp::plugins(openmp)]]

/**
 * @brief Least Squares container & methods
 */

class LS : public virtual Data {
public:
	OutVec data;
	const static size_t max_size = 25;
	
	
	LS();
	LS( arma::colvec, arma::mat );
	
	LS( const LS& );
	LS* clone() const;
	
	virtual LS& operator=( const LS& );	
	virtual bool operator==( const LS& ) const;	
	virtual bool operator!=( const LS& ) const;		
	
	Output OLS( std::vector< unsigned int > );
	
	void BestSubsetHelper( unsigned int );
	void BestSubset();	
	
	void ompBestSubsetHelper( unsigned int );
	void ompBestSubset();	
	
	virtual void clear();
	virtual void print();
	
	void setData( const OutVec& );
	const OutVec& getData() const;
	
	size_t getMaxSize();
};


void ls_finalizer( LS* ); 

#endif /* LS_HPP */
