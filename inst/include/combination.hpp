
#ifndef COMBINATION_HPP
#define COMBINATION_HPP

#include "next_combination.hpp"

#include <algorithm>
#include <vector>
#include <omp.h>

class Combination {
private:
	std::vector< unsigned int > data;
	unsigned int k;
	bool next_exists;

public:
	Combination( );
	Combination( std::vector<unsigned int>,  unsigned int );
	
	Combination( const Combination& );
	Combination* clone( ) const;
	
//	virtual ~Combination();
	
	virtual Combination& operator=( const Combination& );
	virtual bool operator==( const Combination& ) const;
	virtual bool operator!=( const Combination& ) const;
	
	std::vector<unsigned int> nextCombo( );
	
	void reset( );
	void clear( );
	
	unsigned long getCombinations( unsigned long n, unsigned long k);
	unsigned long getPermutations( unsigned long n );
	
	void setData( const std::vector<unsigned int>& );
	const std::vector<unsigned int>& getData( ) const;
	std::vector<unsigned int> getData();
	
	void setK( unsigned int );
	unsigned int getK( );
	
	void setNextExists( bool );
	bool getNextExists( );
	
	unsigned long factorial( unsigned long );
	unsigned long combination( unsigned long, unsigned long );
};

void combination_finalizer( Combination* );

#endif /* COMBINATION_HPP */
