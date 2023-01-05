#include <iostream>
#include "permutation.h"
#include "quasi_permutation_matrix.h"
#include "stats.h"

using namespace std;

vector<int> factorial_digit {1, 1, 1, 1, 2, 3, 3, 4, 5, 6, 7};

void generate(vector<Stats> stats, bool exact)
{
	bool end = false;
	while (end == false)
	{
		stats[0].quasi_mat_source.print_inline();
		cout << "\t" ;

		int first=1; // formating output
		for (Stats& s : stats)
		{
			int val = s.avoid_current(exact);

			if (first == 1)
			{
				cout << right << setw(factorial_digit[s.get_ones_target()]) << val;
				first=0;
			}
			else
				cout << ", " << right << setw(factorial_digit[s.get_ones_target()]) << val;
		}
		cout << '\n';

		for (Stats &s : stats)
		{
			if (s.next_source_mat() == false)
				end = true;
		}
	}
}

int main(int argc, char* argv[])
{
	if (argc != 3)
	{
		
		cout << "Try, '" << argv[0] << " <N> <e>', where\n" <<
				"  ---> N is the maximal considered size of the target permutation matrix,\n" <<
				"  ---> e is either 0 or 1; if e=1 exact contaiment is used, otherwise, partial containment is used.\n";
		
		return 1;
	}

	int N = stoi(argv[1]);
	int exact = stoi(argv[2]);

	for (int n = 1; n <= 4; n++) {
		for (int k = n; k <= 4; k++) {

			/*	// do not enumerate permutation matrices
			int max_l;
			if (n == k)
				max_l = n-1;
			else
				max_l = n;

			for (int l = 0; l <= max_l; l++){
			*/

			for (int l = 0; l <= n; l++){

				vector<Stats> stats;
				for (int i = 1; i <= N; i++)
				{
					Stats s(n, k, l, i, i, i);
					stats.push_back(s);
				}

				generate(stats, exact);
			}
		}
	}

	return 0;
}

