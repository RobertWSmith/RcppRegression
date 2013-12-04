#ifndef REGRESSION_HPP
#define REGRESSION_HPP

#include <vector>
#include <algorithm>
#include <limits>

#include <iostream>
#include <iomanip>
#include <RcppArmadillo.h>

/**
 * @brief Data Class for Regression -- container
 */

class Data {
public:
	arma::colvec Y;
	arma::mat X;

	Data( );
	Data( arma::colvec, arma::mat );
	Data( const Data& );
	
	virtual Data* clone( ) const;	
	
	virtual Data& operator=( const Data& );
	virtual bool operator==( const Data& ) const;
	virtual bool operator!=( const Data& ) const;
	
	virtual void clear();

	virtual arma::mat* subsetX( std::vector<unsigned int> );	
	
	virtual void setX( const arma::mat& );
	virtual void setY( const arma::colvec& ); 
	virtual const arma::mat& getX( ) const;
	virtual const arma::colvec& getY() const;
	
	size_t nrowY();
	size_t nrowX();
	size_t ncolY();
	size_t ncolX();
	
	virtual void print();
};

void data_finalizer(Data*);

#endif /* REGRESSION_HPP */
