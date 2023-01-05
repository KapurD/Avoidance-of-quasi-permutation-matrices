#include <iostream>
#include "permutation.h"
#include "quasi_permutation_matrix.h"
#include "stats.h"

using namespace std;


int main()
{
	bool exact = false;

    vector<vector<int>> X = { {0,0,0,0,1}, {0,0,1,0,0}, {1,0,0,0,0} }; // the parametrs of 'p' must coincide with the parametrs above !!!
	vector<vector<int>> Y = { {0,0,0,0,1}, {1,0,0,0,0}, {0,0,1,0,0} }; // the parametrs of 'p' must coincide with the parametrs above !!!
	unsigned quasi_source_row_size = 3;
    unsigned quasi_source_col_size = 5;
    unsigned source_num_of_ones = 3;
    unsigned quasi_target_row_size = 6;
    unsigned quasi_target_col_size = 6;
    unsigned target_num_of_ones = 6;

	Stats s_X(quasi_source_row_size, quasi_source_col_size, source_num_of_ones,
                quasi_target_row_size, quasi_target_col_size, target_num_of_ones);
    Stats s_Y(quasi_source_row_size, quasi_source_col_size, source_num_of_ones,
                quasi_target_row_size, quasi_target_col_size, target_num_of_ones);
	

    cout << "p^P_" << target_num_of_ones << "(X) = " << s_X.avoid_single(X, exact) << '\n';
    cout << "p^P_" << target_num_of_ones << "(Y) = " << s_Y.avoid_single(Y, exact) << '\n';

    return 0;
}

