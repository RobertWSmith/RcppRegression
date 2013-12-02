#ifndef OUTPUT_HPP
#define OUTPUT_HPP

#include "data.hpp"

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

class Output {
public:	
	double aic, sse, cp; 
	uint_vec columns;
	 
	Output() : aic(), sse(), cp(), columns() {}
	
	Output(SEXP o) : aic(), sse(), cp(), columns() {
		Rcpp::List input(o);
		
		aic = Rcpp::as<double>(input("aic"));
		sse = Rcpp::as<double>(input("sse"));
		cp = Rcpp::as<double>(input("cp"));
		columns = Rcpp::as<uint_vec>(input("cp"));
	}
	
	operator SEXP() {
		return Rcpp::List::create(
			Rcpp::_["aic"] = aic,
			Rcpp::_["sse"] = sse,
			Rcpp::_["cp"] = cp,
			Rcpp::_["columns"] = columns
		);
	}
	
	friend std::ostream& operator<<(std::ostream& ss, const Output& op) {
		ss << op.aic << "\t" << op.cp << "\t" << op.sse << "\t";
		for (size_t i = 0; i < op.columns.size(); i++) {
			ss << op.columns[i] << "\t";
		}
	
		return ss;
	}
	
	void print() {
		Rcpp::Rcout << "AIC" << "\t\t" << "CP" << "\t\t" << "SSE" << "\t\t" << 
										"Columns" << std::endl;
		
		std::string ss = opString();
		
		Rcpp::Rcout << ss << std::endl;
	}
	
	void printMin() {
		std::string ss = opString();
		
		Rcpp::Rcout << std::fixed;
    Rcpp::Rcout << std::setprecision(2);
		
		Rcpp::Rcout << ss << std::endl;
	}
	
	std::string opString() {
		
		std::stringstream ss;
		ss.precision(2);		
		
		ss << std::fixed << aic << "\t\t" << cp << "\t\t" << sse << "\t\t";
		
		for (auto elem : columns) {
			ss << elem << "\t\t";
		}
		
		return ss.str();
	}
};

bool operator<(const Output& lhs, const Output& rhs) {
	return lhs.aic < rhs.aic;
}


#endif /* OUTPUT_HPP */
