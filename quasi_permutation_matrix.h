#include <cassert>
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>

using namespace std;

class QuasiPermutationMatrix
{
private:

	/*
	 * Update the map.
	 */
	void update_map(vector<int>& map, vector<int>& empty_vector)
	{
		// clear the map
		fill(map.begin(), map.end(), -1);

		for (int i = 0; i < empty_vector.size(); i++)
			map[empty_vector[i]] = i;

		int i = 0;
		for ( int& x : map )
		{
			if ( x == -1 )
			{
				x = i + empty_vector.size();
				i++;
			}
		}
	}

	/*
	 * Create the vector if indeces of empty rows/columns
	 *
	 * If row = true, then we assume that &vec pointing to row_empty.
	 * If row = false, then we assume that &vec pointing to col_empty.
	 */
	bool next_empty_vector(vector<int> &vec, unsigned size, bool row)
	{
		unsigned cnt = 1;
		for ( vector<int>::reverse_iterator it = vec.rbegin(); it != vec.rend(); it++ )
		{
			if ( *it < size - cnt )
			{
				(*it)++;
				int val = *it;
				while ( it != vec.rbegin() )
				{
					it--;
					val++;
					*it = val;
				}
				
				if (row == true)
					update_map(this->row_map, vec);
				else
					update_map(this->col_map, vec);
				return true;
			}
			cnt++;
		}
		return false;
	}

	/*
	 * Crate the next quasi-permutation matrix with the same underlying permutation.
	 *
	 * Return false, if there is no such quasi-permutation matrix.
	 */
	bool next_empty()
	{
		if (next_empty_vector(this->row_empty, this->row_size, true) == true)
			return true;
	
		for (int i = 0; i < this->row_empty.size(); i++)
			this->row_empty[i] = i;
		for (int i = 0; i < this->row_size; i++)
			this->row_map[i] = i;

		if (next_empty_vector(this->col_empty, this->col_size, false) == true)
			return true;
		else
			return false;
	}

public:

	vector<vector<int>> quasi_perm_matrix; // quasi-permutation matrix
	unsigned row_size; // the number of rows in 'quasi_perm_matrix'
	unsigned col_size; // the number of columns in 'quasi_perm_matrix'
	Permutation permutation; // underlying permutation of 'quasi_perm_matrix'

	// for easier and faster swaping of columns and rows of 'quasi_perm_matrix'
	vector<int> row_map;
	vector<int> col_map;

	// indices of empty rows and columns
	vector<int> row_empty;
	vector<int> col_empty;

	bool overflow; // indicator of that the method next() returned false

	QuasiPermutationMatrix(unsigned _row_size, unsigned _col_size, unsigned num_of_ones) :
		overflow(false),
		row_size(_row_size),
		col_size(_col_size),
		permutation(num_of_ones),
		quasi_perm_matrix(_row_size, vector<int>(_col_size, 0)),
		row_map(vector<int>(_row_size)),
		col_map(vector<int>(_col_size)),
		row_empty(vector<int>(_row_size - num_of_ones)),
		col_empty(vector<int>(_col_size - num_of_ones))
	{
		assert(num_of_ones <= _row_size && num_of_ones <= _col_size);
		init();
	}

	/*
	 * Initialize all variables.
	 */
	void init()
	{
		overflow = false; // unset 'overflow' flag

		unsigned num_of_empty_rows = this->row_size - this->permutation.size;
		unsigned num_of_empty_cols = this->col_size - this->permutation.size;

		// clear 'quasi_perm_matrix'
		for (int i = 0; i < this->quasi_perm_matrix.size(); i++)
		{
			fill(this->quasi_perm_matrix[i].begin(),
					this->quasi_perm_matrix[i].end(), 0);
		}

		// store the underlying permutation into the upper right corner of 'quasi_perm_matrix'
		for (int i = 0; i < permutation.size; i++)
		{
			this->quasi_perm_matrix[num_of_empty_rows + i][num_of_empty_cols + permutation.permutation[i]] = 1;
		}

		for (int i = 0; i < this->row_size; i++)
			row_map[i] = i;
		for (int i = 0; i < num_of_empty_rows; i++)
			row_empty[i] = i;

		for (int i = 0; i < this->col_size; i++)
			col_map[i] = i;	
		for (int i = 0; i < num_of_empty_cols; i++)
			col_empty[i] = i;
	}
	
	/*
	 * For an external reset needs (see 'stats.h'). 
	 */
	void reset()
	{
		// reset the underlying permutation
		permutation.reset();

		init();
	}	

	/*
	 * Return the correct value.
	 */
	inline int get_value(unsigned row, unsigned col)
	{
		return quasi_perm_matrix[row_map[row]][col_map[col]];
	}

	/*
	 * Create the next quasi-permutation matrix.
	 *
	 * The order is given
	 *   1) by the order of underlying permutations,
	 *   2) by the lexikographic order of empty rows,
	 *   3) by the lexikographic order of empty columns.
	 *
	 * Return false, if there is no next quasi-permutation matrix.
	 */
	bool next()
	{
		if (overflow == true)
			return false;

		// try the next position of empty rows and columns
		if (next_empty() == true)
		{
			return true;
		}
		else
			overflow = true;

		// try the next underlying permutation
		if (permutation.next())
		{
			init();
			return true;
		}
		else
			return false;
	}

	/*
	 * Print the current quasi-permutation matrix.
	 */
	void print(bool delimiter=false)
	{
		for (int i = 0; i < this->row_size; i++)
		{
			for (int j = 0; j < this->col_size; j++)
			{	
				if (delimiter == true && j > 0)
					cout << ' ';
				cout << get_value(i, j);
			}
			cout << '\n';
		}
		cout << '\n';
	}

	/*
	 * Print the current permutation matrix, where the order of rows
	 * is reversed (i.e, the first row is printed as the last and so on).
	 */
	void print_cartesian(bool delimiter=false)
	{
		for (int i = this->row_size-1; i >= 0; i--)
		{
			for (int j = 0; j < this->col_size; j++)
			{	
				if (delimiter == true && j > 0)
					cout << ' ';
				cout << get_value(i, j);
			}
			cout << '\n';
		}
		cout << '\n';
	}

	/*
	 * Print quasi-permutation matrix in the one-line notation.
	 */
	void print_inline()
	{
		string out = "(";
		
		for (int j = 0; j < this->col_size; j++)
		{
			if (j > 0)
				out += ',';

			bool nonzero = false;
			for (int i = 0; i < this->row_size; i++)
			{	
				if (get_value(i,j) == 1)
				{
					out += to_string(i+1);
					
					nonzero = true;
					break;
				}
			}
			if (nonzero == false)
			{
				out += '0';
			}
		}
		out += ")_";
		out += to_string(this->row_size);

		cout << left << setw(11) << out;
	}


	/*
	 * Iterate over all quasi-permutation matrices that are equal
	 * or greater than (in the lexikographic ordering) the current 
	 * quasi-permutation matrix and print them one by one.
	 *
	 * Warning: this method set 'overflow' flag.
	 *
	 *
	 * TODO: Consider to implement this function in a such way 
	 * that it does not modify the structure.
	*/
	void print_the_rest(bool cartesian=false, bool delimeter=false)
	{
		int cnt = 1;
		do
		{
			cout << "<" << cnt << ">\n";
			if (cartesian == true)
				this->print_cartesian(delimeter);
			else
				this->print(delimeter);

			cnt++;
		}
		while (this->next());
	}

};
