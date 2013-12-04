#include "output.hpp"

void output_finalizer(Output* op) {
	op->clear();
}

Output::Output() : 
aic(), sse(), cp(), columns() {}
	
Output::Output( double aic_, double sse_, double cp_, std::vector<unsigned int> columns_) :
aic(aic_), sse(sse_), cp(cp_), columns(columns_) {}
	
void Output::print() {
	std::cout << "AIC" << "\t\t" << "CP" << "\t\t" << "SSE" << "\t\t" << 
									"Columns" << std::endl;
	
	std::string ss = opString();
	
	std::cout << ss << std::endl;
}

Output* Output::clone() const {
	return new Output(*this);
}
	
void Output::printMin() {
	std::string ss = opString();
	
	std::cout << std::fixed;
  std::cout << std::setprecision(2);
	
	std::cout << ss << std::endl;
}
	
std::string Output::opString() {
	
	std::stringstream ss;
	ss.precision(2);		
	
	ss << std::fixed << aic << "\t\t" << cp << "\t\t" << sse << "\t\t";
	
	for (auto elem : columns) {
		ss << elem << "\t\t";
	}
	
	return ss.str();
}

void Output::clear() {
	columns.clear();
	columns.shrink_to_fit();
}

double Output::getAIC() {
	return aic;
}

Output& Output::operator=( const Output& rhs ) {
	if (*this != rhs) {
		Output* op = rhs.clone();
		
		aic = op->aic;
		sse = op->sse;
		cp = op->cp;
		columns = op->columns;
	}
	
	return *this;
}

bool Output::operator==( const Output& rhs ) const {
	if (columns.size() != rhs.columns.size()) return false;
	else if (aic != rhs.aic) return false;
	else if (sse != rhs.sse) return false;
	else if (cp != rhs.cp) return false;
	else return true;
}

bool Output::operator!=( const Output& rhs ) const {
	return !(*this == rhs);
}

bool Output::operator<( const Output& rhs ) const {
	return (aic < rhs.aic);
}

bool Output::operator>( const Output& rhs ) const {
	return (rhs < *this);
}

bool Output::operator>=( const Output& rhs ) const {
	return !( *this < rhs );
}

bool Output::operator<=( const Output& rhs ) const {
	return !( *this > rhs );
}



