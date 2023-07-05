#pragma once

#include <global.h>

template< typename T >
class ReuseVector
{
protected:
	using Iterator = typename Vector<T>::iterator;

	Vector<T> backingStore;
	Iterator reuseSpot;
	int reusableCount = 0;

public:

	[[nodiscard]]
	bool IsValid( const Iterator & it ) const;
	void Spawn( T val ) {}

	template< typename ... Args >
	void Spawn( Args && ... args )
	{
		backingStore.emplace_back( std::forward<Args>( args )... );
	}

	void Retire( const Iterator & it ) /*final*/ {
		DoRetire( it );
		reuseSpot = it;
		++reusableCount;
	}

private:
	void DoRetire( const Iterator & it ) const { it->x = INFINITY; }
};

class ReuseFPoints : public ReuseVector<FPoint>
{
	[[nodiscard]]
	bool IsValid( const Iterator & it ) const { return isfinite( it->x ); }
};
