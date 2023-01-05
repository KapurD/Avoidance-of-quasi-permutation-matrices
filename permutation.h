#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

class Permutation
{
private:
	bool overflow; // indicator that the method next() returned false

public:
	unsigned size;
	vector<int> permutation; // permutation stored as vector of elements {0, ..., size-1}

	Permutation(unsigned _size) :
			overflow(false),
			size(_size),
			permutation(_size)
	{
		// initialize the permutation by the lowest
		// permutation in the lexikographic ordering
		for (int i = 0; i < this->size; i++)
			this->permutation[i] = i;
	}

	/*
	 * Remove 'overflow' flag and initalize 'permutation' 
	 * by the lowest permutation in the lexikographix ordering.
	 */
	void reset()
	{
		overflow = false;
		for (int i = 0; i < this->size; i++)
			this->permutation[i] = i;
	}

	/*
	 * If 'overflow' flag is not set, then replace the current permutation
	 * by its successor (in the lexikographic ordering) and return true.
	 *
	 * If 'overflow' flag is set or no successor exists, return false, 
	 * initalize 'permutation' by the lowest permutation in the lexikographic ordering 
	 * (note: this is done automatically in next_permutation()), and set 
	 * 'overflow' flag.
	 */
	bool next()
	{
		if (overflow == true)
			return false;

		if (next_permutation(this->permutation.begin(), this->permutation.end()) == true)
		{
			return true;
		}
		else
		{
			overflow = true;
			return false;
		}
	}

	/*
	 * Print the current permutation in one-line notation.
	 */
	void print(bool increment = false, bool delimiter = false)
	{
		if (size > 10)
			delimiter = true;

		for(int i = 0; i < this->size; i++)
		{
			if (delimiter == true && i > 0)
				cout << ',';

			if (increment == true)
				cout << this->permutation[i] + 1;
			else
				cout << this->permutation[i];
		}
		cout << '\n';
	}

	/*
	 * Iterate over all permutations that are equal or greater
	 * than (in lexikographic ordering) the current permutation
	 * and print them one by one (all in one-line notation).
	 *
	 * Warning: this method set 'overflow' flag.
	 *
	 *
	 * TODO: Consider to implement this function in a such way 
	 * that it does not modify the structure.
	 */
	void print_the_rest(bool increment = false, bool delimiter = false)
	{
		this->print(delimiter);
		while(this->next())
			this->print(increment, delimiter);
	}

};
