
#include "combination.hpp"
#include "output.hpp"
#include "data.hpp"
#include "outvec.hpp"
#include "ls.hpp"

#include <RcppCommon.h>

namespace Rcpp {
	template<> SEXP wrap(const Combination&);
	template<> Combination as(SEXP);
	
	template<> SEXP wrap(const Output&);
	template<> Output as(SEXP);
	
	template<> SEXP wrap(const Data&);
	template<> Data as(SEXP);
	
	template<> SEXP wrap(const OutVec&);
	template<> OutVec as(SEXP);
	
	template<> SEXP wrap(const LS&);
	template<> LS as(SEXP);
}

#include <RcppArmadillo.h>

namespace Rcpp {
	template<> 
	SEXP wrap(const Combination& comb) {
		Combination* nc = comb.clone();
		
		return Rcpp::List::create(
			Rcpp::_["data"] = nc->getData(),
			Rcpp::_["k"] = nc->getK(),
			Rcpp::_["next_exists"] = nc->getNextExists()
			);
	}
	
	template<>
	Combination as(SEXP sxp) {
		Rcpp::List input(sxp);
		Combination nc;
		
		nc.setData( Rcpp::as< std::vector<unsigned int> >( input("data") ) );
		nc.setK( Rcpp::as< unsigned int >( input("k") ) );
		nc.setNextExists( Rcpp::as< bool >( input("next_exists") ) );
		
		return nc;
	}
	
	template<>
	SEXP wrap(const Output& o) {
		Output* op = o.clone();
		return Rcpp::List::create(
			Rcpp::_["aic"] = op->aic,
			Rcpp::_["sse"] = op->sse,
			Rcpp::_["cp"] = op->cp,
			Rcpp::_["columns"] = op->columns
		);
	}
	
	template<>
	Output as(SEXP o) {
		Rcpp::List input(o);
		Output op;
		
		op.aic = Rcpp::as<double>(input("aic"));
		op.sse = Rcpp::as<double>(input("sse"));
		op.cp = Rcpp::as<double>(input("cp"));
		op.columns = Rcpp::as< std::vector<unsigned int> >(input("cp"));
		
		return op;
	}
	
	template<>
	OutVec as (SEXP sxp) {
		OutVec ov;
		Rcpp::List input(sxp);
		
		for (Rcpp::List::iterator elem = input.begin(); elem != input.end(); ++elem) {
			ov.insert( Rcpp::as< Output> ( *elem ) );
		}
		
		return ov;
	}
	
	template<>
	SEXP wrap(const OutVec& ov) {
		OutVec* input = ov.clone();
		Rcpp::List output;
		
		for (size_t i = 0; i < input->data.size(); i++) {
			output.push_back( input->at(i) );
		}
		
		return output;
	}
	
	template<>
	SEXP wrap(const Data& dt) {
		Data* ip = dt.clone();
		return Rcpp::List::create(
			Rcpp::_["Y"] = ip->getY(),
			Rcpp::_["X"] = ip->getX()
		);
	}
	
	template<>
	Data as(SEXP sxp) {
		Rcpp::List input(sxp);
		Data d;
		d.setY( Rcpp::as<arma::colvec>( input("Y") ) );
		d.setX( Rcpp::as<arma::mat>( input("X") ) );
		return d;
	}
	
	template<>
	SEXP wrap(const LS& ls) {
		LS* local = ls.clone();
		
		return Rcpp::List::create(
			Rcpp::_["X"] = local->getX(),
			Rcpp::_["Y"] = local->getY(),
			Rcpp::_["data"] = local->getData()
		);
	}
	
	template<>
	LS as(SEXP sxp) {
		Rcpp::List input(sxp);
		LS ls;
		
		ls.setY( Rcpp::as<arma::colvec>( input("Y") ) );
		ls.setX( Rcpp::as<arma::mat>( input("X") ) );
		ls.setData( Rcpp::as< OutVec >( input("data") ) );
		
		return ls;
	}
}

typedef std::vector< Output > op_vec;

RCPP_EXPOSED_CLASS(Combination)

RCPP_EXPOSED_CLASS(Data)

RCPP_EXPOSED_CLASS(LS)

RCPP_EXPOSED_CLASS(Output)

RCPP_EXPOSED_CLASS(OutVec)

RCPP_MODULE(Regression) {
	using namespace Rcpp;
	
	class_< Combination >( "Combination" )
		.constructor("Default Constructor")
		.constructor< std::vector<unsigned int>,  unsigned int >("Main Constructor")
		
		.property("data", &Combination::getData, &Combination::setData)
		.property("k", &Combination::getK, &Combination::setK)
		.property("next_exists", &Combination::getNextExists)
		
		.method("nextCombo", &Combination::nextCombo)
		.method("reset", &Combination::reset)
		.method("getCombinations", &Combination::getCombinations)
		.method("getPermutations", &Combination::getPermutations)
		
		.finalizer( &combination_finalizer )
	;
	
	class_< Output >( "Output" ) 
		.constructor( "Default Constructor" )
		
		.field_readonly( "aic", &Output::aic )
		.field_readonly( "sse", &Output::sse )
		.field_readonly( "cp", &Output::cp )
		.field_readonly( "columns", &Output::columns )
		
		.method( "print", &Output::print )
		
		.finalizer( &output_finalizer )
	;

	class_< OutVec >( "OutVec" )
		.constructor( "Default Constructor" )
		
		.field_readonly( "data", &OutVec::data )
//		.property( "list", &OutVec::asList )
//		.property("data.frame", &OutVec::asDataFrame)
		
		.method( "at", &OutVec::at )
		.method( "print", &OutVec::print )
		
		.finalizer( &outvec_finalizer )
	;

	//' @exportClass Data
	class_< Data >( "Data" )
		.constructor( "Default Constructor" )
		.constructor< arma::colvec, arma::mat >( "Main Constructor" )
		
		.property( "Y", &Data::getY, &Data::setY, "Values to be predicted" )
		.property( "X", &Data::getX, &Data::setX, "Predictor matrix" )
		.property( "nrow.X", &Data::nrowX, "nrows of X" )
		.property( "ncol.X", &Data::ncolX, "ncols of X" )
		.property( "nrow.Y", &Data::nrowY, "nrows of Y" )
		.property( "ncol.Y", &Data::ncolY, "ncols of Y -- should be one" )
		
		.method( "subsetX", &Data::subsetX, "method to subset X by column and return a reference to the requested submatrix" ) 
		.method( "print", &Data::print, "print the input data" ) 
		
		.finalizer( &data_finalizer )
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
		
		.finalizer( &ls_finalizer )
	;
}






//	Rcpp::List asList(size_t sz) {
//		Rcpp::List output;
//		
//		for (size_t i = 0; i < data.size(); i++) {
//			output.push_back( data[i] );
//		}
//		
//		return output;
//	}
//	
//	Rcpp::DataFrame asDataFrame() {
//		std::vector<double> aic;
//		std::vector<double> cp;
//		std::vector<double> sse;
//		arma::Mat<unsigned int> cols( data.size(), 1 );
//		arma::Col<unsigned int> temp( data.size() );
//		temp.fill( arma::datum::nan );
//		
//		for ( size_t i = 0; i < data.size(); i++ ) {
//			aic.push_back(data[i].aic);
//			cp.push_back(data[i].cp);
//			sse.push_back(data[i].sse);
//			
//			if ( data[i].columns.size() > cols.n_cols ) {
//				cols.insert_cols(cols.n_cols, temp);
//			}
//			
//			for (size_t j = 0; j < data[i].columns.size(); j++) {
//				cols(i, j) = data[i].columns[j];
//			}
//		}
//		
//		cols.transform( [](unsigned int val) { return (val == 0 ? arma::datum::nan : val); } );
//		
//		return Rcpp::DataFrame::create(
//			Rcpp::_["AIC"] = aic,
//			Rcpp::_["Cp"] = cp,
//			Rcpp::_["SSE"] = sse,
//			Rcpp::_["Columns"] = cols
//			);
//	}

