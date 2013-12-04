#include "outvec.hpp"
	
void outvec_finalizer( OutVec* ov ) {
	ov->clear();
}

OutVec::OutVec() : data() {}

OutVec::OutVec(const OutVec& ov) : data(ov.data) {}

OutVec* OutVec::clone() const {
	return new OutVec(*this);
}

OutVec& OutVec::operator=(const OutVec& rhs) {
	if (*this != rhs) {
		OutVec* ov = rhs.clone();
		clear();
		insert( *ov );
	}
	
	return *this;
}

bool OutVec::operator==( const OutVec& rhs ) const {
	OutVec* right = rhs.clone();
	
	if ( right->getData().size() != data.size() ) {
		return false;
	}	else {
		for (size_t i = 0; i < data.size(); i++) {
			if ( data[i] != right->data[i] ) return false;
		}
	}
	
	return true;
}

bool OutVec::operator!=( const OutVec& rhs ) const {
	return !(*this == rhs);
}

Output& OutVec::operator[]( size_t place ) {
	return data[place];
}

Output& OutVec::at(size_t val) {
	val--; // compensate for R's 1-indexing
	return data.at(val);
}

size_t OutVec::size() {
	return data.size();
}

void OutVec::clear() {
#pragma omp critical (outvec_major)
{
	data.clear();
	data.shrink_to_fit();
}
}

const std::vector< Output >& OutVec::getData() const {
	return data;
}

void OutVec::insert(Output op) {
#pragma omp critical (outvec_major)
{	
	data.push_back(op);
}
	prune();
}

void OutVec::insert(const OutVec& ov) {
	OutVec* local = ov.clone();
	for (size_t i = 0; i < local->size(); i++) {
		insert(local->data[i]);
	}
	prune();
}

void OutVec::sort() {
#pragma omp critical (outvec_major)
{
	std::sort( data.begin(), data.end() );
}
}

void OutVec::prune() {
#pragma omp critical (outvec_major)
{
	if (data.size() > max_size) {
		data.resize(max_size);
		data.shrink_to_fit();
	}
}
}

void OutVec::print() {
	std::cout << "AIC" << "\t\t" << "CP" << "\t\t" << "SSE" << "\t\t" << 
									"Columns" << std::endl;
	std::string ss;
	
	for (size_t i = 0; i < data.size(); i++) {
		data[i].printMin();
	}
}
