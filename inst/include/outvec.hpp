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

typedef std::vector<double> dbl_vec;
typedef std::vector<unsigned int> uint_vec;
typedef std::vector< uint_vec > uint_mtx;
typedef std::vector< Output > op_vec;

class OutVec {
public:
	op_vec data;
	const static size_t max_size = 25;
	
	OutVec() : data() {}
	
	OutVec(size_t max_size_) : data() {}
	
	OutVec(const OutVec& ov) : data(ov.data) {}
	
	OutVec* clone() const {
		return new OutVec(*this);
	}
	
	OutVec(SEXP s) : data() {
		Rcpp::List input(s);
		
		for (Rcpp::List::iterator elem = input.begin(); elem != input.end(); ++elem) {
				data.push_back( *elem );
			}
	}
	
	operator SEXP() {
		Rcpp::List output;
		
		for (size_t i = 0; i < data.size(); i++) {
			output.push_back( data[i] );
		}
		
		return output;
	}
	
	void operator=(const OutVec& lhs) {
		data.clear();
		data.insert( data.begin(), lhs.data.begin(), lhs.data.end() );
	}
	
	Output at(size_t val) {
		val--; // compensate for R's 1-indexing
		try {
			return data[val];
			} catch(std::exception &ex) {
			forward_exception_to_r(ex);
		} catch(...) {
			::Rf_error("c++ exception (unknown reason)"); 
		}
	}
	
	void insert(Output op) {
		data.push_back(op);
		if (data.size() > max_size * 2) {
			prune();
		}
	}
	
	void insert(const OutVec& ov) {
		OutVec* local = ov.clone();
		
		for (size_t i = 0; i < local->size(); i++) {
			insert(local->data[i]);
		}
		prune();
	}
	
	void sort() {
		std::sort( data.begin(), data.end() );
	}
	
	void prune() {
		sort();
		if (data.size() > max_size) {
			data.resize(max_size);
		}
	}
	
	void print() {
		Rcpp::Rcout << "AIC" << "\t\t" << "CP" << "\t\t" << "SSE" << "\t\t" << 
										"Columns" << std::endl;
		std::string ss;
		
		for (size_t i = 0; i < data.size(); i++) {
			data[i].printMin();
		}
	}
	
	size_t size() {
		return data.size();
	}
	
	void clear() {
		data.clear();
	}
	
	Rcpp::List asList(size_t sz) {
		Rcpp::List output;
		
		for (size_t i = 0; i < data.size(); i++) {
			output.push_back( data[i] );
		}
		
		return output;
	}
	
	Rcpp::DataFrame asDataFrame() {
		std::vector<double> aic;
		std::vector<double> cp;
		std::vector<double> sse;
		arma::Mat<unsigned int> cols( data.size(), 1 );
		arma::Col<unsigned int> temp( data.size() );
		temp.fill( arma::datum::nan );
		
		for ( size_t i = 0; i < data.size(); i++ ) {
			aic.push_back(data[i].aic);
			cp.push_back(data[i].cp);
			sse.push_back(data[i].sse);
			
			if ( data[i].columns.size() > cols.n_cols ) {
				cols.insert_cols(cols.n_cols, temp);
			}
			
			for (size_t j = 0; j < data[i].columns.size(); j++) {
				cols(i, j) = data[i].columns[j];
			}
		}
		
		cols.transform( [](unsigned int val) { return (val == 0 ? arma::datum::nan : val); } );
		
		return Rcpp::DataFrame::create(
			Rcpp::_["AIC"] = aic,
			Rcpp::_["Cp"] = cp,
			Rcpp::_["SSE"] = sse,
			Rcpp::_["Columns"] = cols
			);
	}
};


#endif /* OUTVEC_HPP */