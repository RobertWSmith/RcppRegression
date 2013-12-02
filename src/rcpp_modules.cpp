
#include "data.hpp"
#include "output.hpp"
#include "outvec.hpp"
#include "ls.hpp"

#include <vector>
#include <RcppArmadillo.h>


typedef std::vector< Output > op_vec;

RCPP_EXPOSED_CLASS(Data)

RCPP_EXPOSED_CLASS(LS)

RCPP_EXPOSED_CLASS(Output)

RCPP_EXPOSED_CLASS(OutVec)

RCPP_MODULE(Regression) {
	using namespace Rcpp;
	class_< Output >( "Output" ) 
		.constructor( "Default Constructor" )
		
		.field_readonly( "aic", &Output::aic )
		.field_readonly( "sse", &Output::sse )
		.field_readonly( "cp", &Output::cp )
		.field_readonly( "columns", &Output::columns )
		
		.method( "print", &Output::print )
	;

	class_< OutVec >( "OutVec" )
		.constructor( "Default Constructor" )
		
		.field_readonly( "data", &OutVec::data )
//		.property( "list", &OutVec::asList )
		.property("data.frame", &OutVec::asDataFrame)
		
		.method( "at", &OutVec::at )
		.method( "print", &OutVec::print )
	;

	//' @exportClass Data
	class_< Data >( "Data" )
		.constructor( "Default Constructor" )
		.constructor< arma::colvec, arma::mat >( "Main Constructor" )
		
		.field( "Y", &Data::Y, "Values to be predicted" )
		.property( "X", &Data::getX, &Data::setX, "Predictor matrix" )
		.property( "nrow.X", &Data::nrowX, "nrows of X" )
		.property( "ncol.X", &Data::ncolX, "ncols of X" )
		.property( "nrow.Y", &Data::nrowY, "nrows of Y" )
		.property( "ncol.Y", &Data::ncolY, "ncols of Y -- should be one" )
		
		.method( "subsetX", &Data::subsetX, "method to subset X by column and return a reference to the requested submatrix" ) 
		.method( "print", &Data::print, "print the input data" ) 
	;

	class_< LS >( "LS" ) 
		.derives< Data >( "Data" )
		.constructor( "Default Constructor" )
		.constructor< arma::colvec, arma::mat >( "Main Constructor" )
		
		.field_readonly( "data", &LS::data )
		
		.method( "OLS", &LS::OLS )
		.method( "BestSubset", &LS::BestSubset )
		.method( "ompBestSubset", &LS::ompBestSubset )
		.method( "print", &LS::print )
		.method( "clear", &LS::clear )
	;
}


