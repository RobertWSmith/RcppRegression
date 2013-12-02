#ifndef LS_HPP
#define LS_HPP

#include "combination.hpp"

#include "data.hpp"
#include "output.hpp"
#include "outvec.hpp"

#include <array>
#include <stdexcept>
#include <algorithm>
#include <vector>
#include <iostream>
#include <iomanip>

#include <omp.h>
// [[Rcpp::plugins(openmp)]]

#include <RcppArmadillo.h>

typedef std::vector< Output > op_vec;
typedef std::vector< unsigned int > uint_vec;
typedef std::vector< uint_vec > uint_mtx;

// LS for "least squares"
// [[Rcpp::export]]
class LS : public virtual Data {
public:
	OutVec data;
	
	LS() : 
			Data(), data() {}
	
	LS(arma::colvec Y_, arma::mat X_) :
			Data(Y_, X_), data() {}
		
	LS(SEXP s) :
			Data(), data() {
		Rcpp::List input(s);
		X = Rcpp::as<arma::mat>( input("X") );
		Y = Rcpp::as<arma::colvec>( input("Y") );
		data = Rcpp::as< OutVec >( input("data") );
	}
	
	LS(const LS& o) :
			Data(), data() {
		
		X = o.X;
		Y = o.Y;
		data = o.data;
	}
	
	LS* clone() const {
		return new LS(*this);
	}
	
	size_t getMaxSize() {
		return data.max_size;
	}
	
	virtual operator SEXP() {
		return Rcpp::List::create(
			Rcpp::_["X"] = X,
			Rcpp::_["Y"] = Y,
			Rcpp::_["data"] = data
			);
	}
	
	// single OLS fit
	Output OLS(uint_vec input) {
		arma::mat X = subsetX(input);
		arma::mat Y = getY();
		
		Output o;
		o.columns = input;
		arma::colvec beta = arma::solve(X, Y);
		arma::colvec error = Y - X * beta;
		o.sse = arma::sum( arma::sum( error.t() * error ) ); 
		o.cp = (o.sse / (o.sse / (X.n_rows - X.n_cols))) - X.n_rows + 2*X.n_cols;
		o.aic = X.n_rows * log(o.sse) - X.n_rows * log(X.n_rows) + 2*X.n_cols;
		
		return o;
	}
	
	/**
	 * 
	 */
	void BestSubsetHelper(unsigned int chooseK) {
		try {
			uint_vec combination( ncolX(), 0 );
			uint_vec temp_combos( chooseK, 0 );
			
			Rcpp::Rcout << "Values to choose from" << std::endl;
			for(unsigned int i = 0; i < X.n_cols; i++) {
				combination[i] = (1+i);
				Rcpp::Rcout << combination[i] << " ";
			}
			Rcpp::Rcout << std::endl;
			
			Rcpp::Rcout << "Unique Combinations" << std::endl;
			
			do {
				for ( unsigned int i = 0; i < chooseK; i++ ) {
					temp_combos[i] = combination[i];
					Rcpp::Rcout << temp_combos[i] << " "; 
				}
				Rcpp::Rcout << std::endl;
				
				data.insert( OLS( temp_combos ) );
				
			} while (boost::next_combination(combination.begin(), combination.begin() + chooseK, combination.end()));
		} catch (std::exception &ex) {
			forward_exception_to_r(ex);
		} catch (...) {
			::Rf_error("c++ exception (unknown reason)"); 
		}
	}
	
	void BestSubset() {
		clear(); // remove older values
		
		for (size_t i = 1; i < ( 1 + ncolX() ); i++) {
			BestSubsetHelper( i );
		}
		
		data.prune();
	}
	
	void ompBestSubsetHelper(unsigned int chooseK) {
		try {
			OutVec local;
			uint_vec combination( ncolX(), 0 );
			uint_vec temp_combos( chooseK, 0 );
			
			for(unsigned int i = 0; i < X.n_cols; i++) {
				combination[i] = (1+i);
			}
			
			do {
				for ( unsigned int i = 0; i < chooseK; i++ ) {
					temp_combos[i] = combination[i];
				}
				
				local.insert( OLS( temp_combos ) );
				
				
			#pragma omp critical
			{
				data.insert(local);
			}
				
			} while (boost::next_combination(combination.begin(), combination.begin() + chooseK, combination.end()));
		} catch (std::exception &ex) {
			forward_exception_to_r(ex);
		} catch (...) {
			::Rf_error("c++ exception (unknown reason)"); 
		}
	}
	
	virtual void ompBestSubset() {
		clear(); // remove older values
		OutVec local;
		
	#pragma omp parallel for schedule(dynamic) default(shared) private(local)
		for (size_t i = 1; i < ( 1 + ncolX() ); i++) {
			ompBestSubsetHelper( i ) ;
		}
	}
	
	void clear() {
		data.clear();
	}
	
	void print() {
		Data::print();
		data.print();
	}
	
};

#endif /* LS_HPP */
