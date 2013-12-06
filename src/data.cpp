#include "data.hpp"

// finalizer method for Rcpp
void data_finalizer(Data* dt) {
	dt->clear();
}


Data::Data() : Y(), X() {}
	
Data::Data( arma::colvec Y_, arma::mat X_ ) : Y(), X() { 
	setY(Y_);
	setX(X_);
}
	
Data::Data(const Data& orig) : Y(), X() {
	Data* ip = orig.clone();
	Y = (ip->Y);
	setX( ip->getX() );
}

Data* Data::clone() const {
	return new Data(*this);
}

Data& Data::operator=(const Data& rhs) {
	if (*this == rhs) {
		return *this;
	} else {
		Data* ip = rhs.clone();
		
		setY( ( ip->getY() ) );
		setX( ( ip->getX() ) );
		
		return *this;
	}
}

bool Data::operator==(const Data& rhs) const {
	bool xcomp = (arma::accu(X - rhs.X) < std::numeric_limits< double >::epsilon( ) );
	bool ycomp = (arma::accu(Y - rhs.Y) < std::numeric_limits< double >::epsilon( ) );
	
	return (xcomp && ycomp);
}

bool Data::operator!=( const Data& rhs ) const {
	return !(*this == rhs);
}

void Data::clear() {
	X.reset();
	Y.reset();
}

/**
 * @brief Subset the X matrix
 * @param \code{columns} to keep
 * @return \code{arma::mat} reference to perform operations on.
 */ 
arma::mat Data::subsetX(std::vector<unsigned int> columns) {
	std::sort( columns.begin(), columns.end() );
	arma::mat output( getX().n_rows, ( 1 + columns.size() ), arma::fill::zeros );
	
	for (size_t i = 0; i < columns.size(); i++) {
		output.col(i+1) = X.col( columns[i]-1 );
	}
	output.col(0).ones();
	
	return output;
}
	
void Data::setX(const arma::mat& Xi) {
	X.clear();
	X.copy_size(Xi);
	X = Xi;
}
	
const arma::mat& Data::getX() const {
	return X;
}
	
void Data::setY(const arma::colvec& Yi) {
	Y.clear();
	Y.copy_size(Yi);
	Y = Yi;
}
	
const arma::colvec& Data::getY() const { 
	return Y;
}	
	
size_t Data::nrowY() {
	return Y.n_rows;
}
	
size_t Data::nrowX() {
	return X.n_rows;
}
	
size_t Data::ncolY() {
	return Y.n_cols;
}
	
size_t Data::ncolX() {
	return X.n_cols;
}
	
void Data::print() {
	std::cout << std::fixed;
	std::cout << std::setprecision(2);
	
	std::cout << "Y Column Vector " << std::endl << Y << std::endl; 
	std::cout << "X Matrix " << std::endl << X << std::endl;
}

