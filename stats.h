#include <iostream>
#include <iomanip>
#include <vector>
#include <cmath>

using namespace std;

vector<int> factorial {1, 1, 2, 6, 24, 120, 740, 5040, 40320, 362880, 3628800};

class Stats
{
private:

	// the following two vectors determine a submatrix of 'quasi_mat_target'
	// (we compare this submatrix with 'quasi_mat_source')
	vector<int> rows;
	vector<int> cols;

	/*
	 * Reset the considered submatrix of 'quasi_mat_target'.
	 * ---> set rows to (1, 2, ..., quasi_mat_source.row_size)
	 * ---> set cols to (1, 2, ..., quasi_mat_source.col_size)
	 */
	void reset_submatrix()
	{
		for ( int i = 0; i < this->quasi_mat_source.row_size; i++)
			this->rows[i] = i;

		for ( int i = 0; i < this->quasi_mat_source.col_size; i++)
			this->cols[i] = i;
    }

	/*
	 * Let k be the size of rows or columns in 'quasi_mat' 
	 * and let N = {1, 2, ..., perm_mat.size()}.
	 * 
	 * Cosinde the set M of all k-tuples on N such that k1 < k2 < ... kk
	 * (the elements in the k-tuples are in strictly increasing order).
	 *
	 * Given such k-tuple, the method return its successor and true (or false 
	 * if there is no successor).
	 */
	bool next_line(vector<int> &vec, int k)
	{
		unsigned cnt = 1;
		for ( vector<int>::reverse_iterator it = vec.rbegin(); it != vec.rend(); it++ )
		{
			if ( *it < k - cnt )
			{
				(*it)++;
				int val = *it;
				while ( it != vec.rbegin() )
				{
					it--;
					val++;
					*it = val;
				}

				return true;
			}
			cnt++;
		}
		return false;
	}

	/*
	 * Create the next submatrix of 'perm_matrix'
	 * --> redefine the set of rows and columns.
	 *
	 * Return false, if there is no next submatrix 
	 * of 'perm_matrix' to be considered.
	 */
	bool next()
	{
		// consider the next rows
		if (next_line(this->rows, quasi_mat_target.row_size) == true)
			return true;
		
		// consider the next set of columns and set rows to default value
		int val = 0;
		for ( int& x : this->rows )
		{
			x = val;
			val++;
		}

		return next_line(this->cols, quasi_mat_target.col_size);
	}

	/*
	 * Test whether submatrix of 'quasi_mat_target' determined by 
	 * 'rows' and 'cols' is equal to 'quasi_mat_source'.
	 *
	 * If 'exact' is false, then we allow that zeroes in 'quasi_mat_source'
	 * can be mapped into zeroes or ones in 'quasi_mat_target'.
	 */
	bool contain_current(bool exact = true)
	{
		int row_source = 0;
		for (int row_target : this->rows)
		{
			int col_source = 0;
			for (int col_target : this->cols)
			{
				if (exact == true)
				{	
					if (quasi_mat_source.get_value(row_source, col_source) != quasi_mat_target.get_value(row_target, col_target))
						return false;
				}
				else
				{
					if (quasi_mat_source.get_value(row_source, col_source) > quasi_mat_target.get_value(row_target, col_target))
						return false;
				}
				col_source++;
			}
			row_source++;
		}
		return true;
	}

	/*
	 * Test whether 'quasi_mat_source' contains 'quasi_mat_target' as submatrix.
	 */
	bool contain(bool exact=true)
	{
		// reset the considered submatrix of 'perm_mat_source'
		reset_submatrix();

		while (contain_current(exact) == false)
		{
			// consider the next submatrix of 'perm_mat'
			if (next() == false)
				return false;
		}
		return true;
	}

public:

	QuasiPermutationMatrix quasi_mat_source;
	QuasiPermutationMatrix quasi_mat_target;

	/*
	 * Initialize quasi-permutation matrices by the given parametrs.
	 */
	Stats(unsigned source_row_size, unsigned source_col_size, unsigned source_num_of_ones,
          unsigned target_row_size, unsigned target_col_size, unsigned target_num_of_ones) :
			quasi_mat_source(source_row_size, source_col_size, source_num_of_ones),
			quasi_mat_target(target_row_size, target_col_size, target_num_of_ones)
	{
		// initialize the set of rows
		for ( int i = 0; i < this->quasi_mat_source.row_size; i++)
			this->rows.push_back(i);

		// initialize the set of columns
		for ( int i = 0; i < this->quasi_mat_source.col_size; i++)
			this->cols.push_back(i);
	}

	/*
	 * Return the number of ones in the target matrix.
	 */
	int get_ones_target()
	{
		return quasi_mat_target.permutation.size;
	}

	/*
	 * Replace the source quasi-permutation matrix 'q' by its
	 * successor in the lexicographic ordering and return true.
	 * If there is no successor, return false.
	 */
	bool next_source_mat()
	{
		return quasi_mat_source.next();
	}

	/*
	 * For the currently store source quasi-permutation matrix 'q',
	 * compute the number of permutation matrices that avoid 'q'.
	 */
	int avoid_current(bool exact=true)
	{
		// if the following condition is true, then the all target matrices avoid 'q'
		if (quasi_mat_target.row_size < quasi_mat_source.row_size || 
						quasi_mat_target.row_size < quasi_mat_source.col_size)
			//return factorial[this->quasi_mat_source.permutation.size];
			return factorial[this->quasi_mat_target.permutation.size];

		int bad_target = 0; // the number of target matrices that avoid 'q'
		do
		{
			if (contain(exact) == false)
			{
				bad_target++;
			}
		} 
		while (quasi_mat_target.next());
		quasi_mat_target.reset();

		return bad_target;
	}

	bool matrix_equality(vector<vector<int>> p)
	{
		for (int i = 0; i < this->quasi_mat_source.row_size; i++)
		{
			for (int j = 0; j < this->quasi_mat_source.col_size; j++)
			{
				if (p[i][j] != quasi_mat_source.get_value(i,j))
					return false;
			}
		}
		return true;
	}

    /*
     * Given a patter 'p', compute the number of permutation matrices that avoid 'p'.
     * 
     * !!! We assume that 'p' has the same parametres (n,k,l) as 'quasi_mat_source'.
     */
    int avoid_single(vector<vector<int>> p, bool exact=true)
    {
        // if the following condition is true, then the all target matrices avoid the source matrix
		if (quasi_mat_target.row_size < quasi_mat_source.row_size || quasi_mat_target.row_size < quasi_mat_source.col_size)
			return factorial[this->quasi_mat_target.permutation.size];

		// we want to count the avoidance only for the pattern 'p'
		while (matrix_equality(p) == false)
			quasi_mat_source.next();

		return avoid_current(exact);
	}
};
