#ifndef REGRESSION_HPP
#define REGRESSION_HPP

#include <vector>
#include <algorithm>

#include <iostream>
#include <iomanip>
#include <RcppArmadillo.h>

typedef std::vector< unsigned int > uint_vec;

/**
 * @brief Data Class for Regression -- container
 */

// [[Rcpp::export]]
class Data {
public:
	arma::colvec Y;
	arma::mat X;
	
	Data() :
		Y(), X() {}
	
	Data( arma::colvec Y_, arma::mat X_ ) :
		Y(Y_), X() { 
		setX(X_);
	}
	
	Data(SEXP d) : Y(), X() {
		Rcpp::List input(d);
		Y = Rcpp::as<arma::colvec>(input("Y"));
		setX(input("X"));
	}
	
	Data(const Data& orig) :
		Y(), X() {
		Data* ip = orig.clone();
		Y = (ip->Y);
		setX( ip->getX() );
	}
	
	virtual Data* clone() const {
		return new Data(*this);
	}
	
	virtual operator SEXP() {
		Data* ip = clone();
		return Rcpp::List::create(
			Rcpp::_["Y"] = ip->Y,
			Rcpp::_["X"] = ip->getX()
		);
	}
	
	virtual void operator=(const Data& lhs) {
		Data* ip = lhs.clone();
		Y = (ip->Y);
		X = ( ip->getX() );
	}
	
	/**
	 * @brief Subset the X matrix
	 * @param \code{columns} to keep
	 * @return \code{arma::mat} reference to perform operations on.
	 */ 
	virtual arma::mat subsetX(uint_vec& columns) {
		try{
			std::sort( columns.begin(), columns.end() );
			arma::mat output( nrowX(), ( 1 + columns.size() ), arma::fill::zeros );
			
			for (size_t i = 0; i < columns.size(); i++) {
				output.col(i+1) = X.col( columns[i]-1 );
			}
			output.col(0).ones();
			
			return output;
			
		} catch(std::exception &ex) {
			forward_exception_to_r(ex);
		} catch(...) {
			::Rf_error("c++ exception (unknown reason)"); 
		}
	}
	
	virtual void setX(const arma::mat& Xi) {
		X = Xi;
	}
	
	virtual const arma::mat& getX() {
		return X;
	}
	
	virtual const arma::colvec& getY() { 
		return Y;
	}	
	
	size_t nrowY() {
		return Y.n_rows;
	}
	
	size_t nrowX() {
		return X.n_rows;
	}
	
	size_t ncolY() {
		return Y.n_cols;
	}
	
	size_t ncolX() {
		return X.n_cols;
	}
	
	virtual void print() {
		Rcpp::Rcout << std::fixed;
    Rcpp::Rcout << std::setprecision(2);
		
		Rcpp::Rcout << "Y Column Vector " << std::endl << Y << std::endl; 
		Rcpp::Rcout << "X Matrix " << std::endl << X << std::endl;
	}
};

#endif /* REGRESSION_HPP */
