
all: generate_data matrix_extension_XY

generate_data: generate_data.cpp permutation.h quasi_permutation_matrix.h stats.h
	g++ -O2 generate_data.cpp -o generate_data

matrix_extension_XY: matrix_extension_XY.cpp permutation.h quasi_permutation_matrix.h stats.h
	g++ -O2 matrix_extension_XY.cpp -o matrix_extension_XY

clean:
	rm generate_data matrix_extension_XY

run_partial:
	make
	./generate_data 8 0

run_exact:
	make
	./generate_data 8 1

run_extension:
	make matrix_extension_XY
	./matrix_extension_XY
