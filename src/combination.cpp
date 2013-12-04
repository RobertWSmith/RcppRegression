#include "combination.hpp"

void combination_finalizer( Combination* nc ) {
	nc->clear();
}


Combination::Combination( ) : data(), k(), next_exists( true ) {}

Combination::Combination( std::vector<unsigned int> data_in, unsigned int k_in ) :
data(data_in), k(k_in), next_exists( true ) {}

Combination::Combination( const Combination& orig ) :
data(), k(), next_exists() {
	Combination* nc = orig.clone();
	
	setData( nc->getData() );
	setK( nc->getK() );
	setNextExists( nc->getNextExists() );
}

Combination* Combination::clone( ) const {
	return new Combination(*this);
}

Combination& Combination::operator=( const Combination& rhs ) {
	if (*this != rhs) {
		Combination* combo = rhs.clone();
		
		setData( combo->getData() );
		setK( combo->getK() );
		setNextExists( combo->getNextExists() );
	}
	
	return *this;
}

bool Combination::operator==( const Combination& rhs ) const {
	Combination* cmb = rhs.clone();
	
	if ( next_exists != cmb->getNextExists() ) return false;
	else if ( k != cmb->getK() ) return false;
	else if ( data.size() != cmb->getData().size() ) return false;
	else {
		std::vector<unsigned int> left( data ), right( cmb->getData() );
		std::sort( left.begin(), left.end() );
		std::sort( right.begin(), right.end() );
		for (size_t i = 0; i < data.size(); i++) {
			if ( left[i] != right[i] ) return false;
		}
	}
	
	return true;
}

bool Combination::operator!=(const Combination& rhs) const {
	return !(*this == rhs);
}

std::vector<unsigned int> Combination::nextCombo() {
	std::vector<unsigned int> local;
#pragma omp critical (combo_update)
{
	if (next_exists) {
		local.insert( local.begin(), data.begin(), data.begin() + k);
		next_exists = boost::next_combination( data.begin(), data.begin()+k, data.end() );
	} 
}
	return local;
}

void Combination::reset() {
	std::sort(data.begin(), data.end());
}

void Combination::clear() {
	data.clear();
	data.shrink_to_fit();
}

unsigned long Combination::getCombinations( unsigned long n, unsigned long k) {
	return combination(n, k);
}

unsigned long Combination::getPermutations( unsigned long n ) {
	return factorial(n);
}

void Combination::setData( const std::vector<unsigned int>& data_in) {
	data.clear();
	data.insert(data.begin(), data_in.begin(), data_in.end());
	data.shrink_to_fit();
}

const std::vector<unsigned int>& Combination::getData() const {
	return data;
}

std::vector<unsigned int> Combination::getData() {
	return data;
}

// setting K resets the whole combination engine
void Combination::setK( unsigned int K_in) {
	k = K_in;
	std::sort(data.begin(), data.end());
}

unsigned int Combination::getK() {
	return k;
}

void Combination::setNextExists(bool ne_in) {
	next_exists = ne_in;
}

bool Combination::getNextExists() {
	return next_exists;
}


unsigned long Combination::factorial(unsigned long N) {
	if (N <= 1) {
		return 1;
	}
	else {
		return N * factorial( N - 1 );
	}
}

unsigned long Combination::combination(unsigned long N, unsigned long K) {
	return factorial(N) / (factorial(K) * factorial(N-K));
}
