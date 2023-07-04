

#include<string>
#include<iostream>
#include<limits>
#include<vector>
#include<map>
#include<functional>
#include<tuple>
#include<set>

#include "hungarian.h"



void Hungarian::subtract_min_col()
{
	std::vector<float> mins(matrix_size, std::numeric_limits<float>::max());

	for (size_t row = 0; row < matrix_size; ++row)
		for (int col = 0; col < matrix_size; ++col)
			if (cost_matrix[row][col] < mins[col])
				mins[col] = cost_matrix[row][col];

	for (size_t row = 0; row < matrix_size; ++row)
		for (int col = 0; col < matrix_size; ++col)
			cost_matrix[row][col] -= mins[col];
}


void Hungarian::subtract_min_row()
{
	for (int row = 0; row < matrix_size; ++row) {
		float row_min = std::numeric_limits<float>::max();
		for (int col = 0; col < matrix_size; ++col)
			if (cost_matrix[row][col] < row_min)
				row_min = cost_matrix[row][col];

		for (int col = 0; col < matrix_size; ++col)
			cost_matrix[row][col] -= row_min;
	}
}


void Hungarian::mark_starred_zeros()
{
	for (int row = 0; row < matrix_size; ++row)
		for (int col = 0; col < matrix_size; ++col)
			if (cost_matrix[row][col] == 0 && !row_cov[row] && !col_cov[col])
			{
				zeros_marks[row][col] = ZeroMark::starred;
				row_cov[row] = true;
				col_cov[col] = true;
			}
}


bool Hungarian::check_cover_is_min()
{
	for (size_t row = 0; row < matrix_size; ++row)
		for (size_t col = 0; col < matrix_size; ++col)
			if (zeros_marks[row][col] == ZeroMark::starred)
				col_cov[col] = true;

	for (int col = 0; col < matrix_size; ++col)
		if (!col_cov[col])
			return false;

	return true;
}


bool Hungarian::exists_uncovered_zero(size_t& rw, size_t& cl)
{
	for (size_t row = 0; row < matrix_size; ++row) {
		for (size_t col = 0; col < matrix_size; ++col) {
			if (cost_matrix[row][col] == 0 && !row_cov[row] && !col_cov[col]) {
				rw = row, cl = col;
				return true;
			}
		}
	}
	return false;
}


bool Hungarian::zero_in_row(size_t rw, size_t& cl, ZeroMark mark_type)
{
	for (size_t col = 0; col < matrix_size; ++col)
		if (zeros_marks[rw][col] == mark_type) {
			cl = col;
			return true;
		}
	return false;
}


bool Hungarian::zero_in_col(size_t& rw, size_t cl, ZeroMark mark_type)
{
	for (size_t row = 0; row < matrix_size; ++row)
		if (zeros_marks[row][cl] == mark_type) {
			rw = row;
			return true;
		}
	return false;
}



void Hungarian::augment_path()
{
	for (auto& zero : path) {
		size_t row, col;
		std::tie(row, col) = zero;
		zeros_marks[row][col] = (zeros_marks[row][col] == ZeroMark::starred)
			? ZeroMark::notmarked
			: ZeroMark::starred;
	}
}


void Hungarian::clear_covers()
{
	for (std::vector<bool>::iterator it = row_cov.begin(); it < row_cov.end(); ++it)
		*it = false;
	for (std::vector<bool>::iterator it = col_cov.begin(); it < col_cov.end(); ++it)
		*it = false;
}


void Hungarian::erase_primes()
{
	for (size_t row = 0; row < matrix_size; ++row)
		for (size_t col = 0; col < matrix_size; ++col)
			if (zeros_marks[row][col] == ZeroMark::primed)
				zeros_marks[row][col] = ZeroMark::notmarked;
}


float Hungarian::find_min_uncovered()
{
	float minval = std::numeric_limits<float>::max();
	for (size_t row = 0; row < matrix_size; ++row)
		for (size_t col = 0; col < matrix_size; ++col)
			if (!row_cov[row] && !col_cov[col])
				if (cost_matrix[row][col] < minval)
					minval = cost_matrix[row][col];

	return minval;
}


bool Hungarian::step_four()	// step four
{
	size_t row, col;
	while (exists_uncovered_zero(row, col)) {
		zeros_marks[row][col] = ZeroMark::primed;
		if (zero_in_row(row, col, ZeroMark::starred)) {
			row_cov[row] = true;
			col_cov[col] = false;
		}
		else {
			path.clear();
			path.emplace_back(row, col);

			return false; // goto step 5
		}
	}
	return true;	// goto step 6
}


void Hungarian::step_five()	// step five
{
	size_t row, col;
	while (zero_in_col(row, col = std::get<1>(path.back()), ZeroMark::starred)) {
		path.emplace_back(row, col);

		if (zero_in_row(row, col, ZeroMark::primed)) {
			path.emplace_back(row, col);
		}
	}

	augment_path();

	// goto step 3
}


void Hungarian::update_costs_with_min()		// step six
{
	float minval = find_min_uncovered();

	for (size_t row = 0; row < matrix_size; ++row)
		for (size_t col = 0; col < matrix_size; ++col) {
			if (row_cov[row]) cost_matrix[row][col] += minval;
			if (!col_cov[col]) cost_matrix[row][col] -= minval;
		}

	// goto step 4
}


float Hungarian::calc_cost() const
{
	float cost = 0;
	for (size_t row = 0; row < matrix_size; ++row)
		for (size_t col = 0; col < matrix_size; ++col)
			if (zeros_marks[row][col] == ZeroMark::starred)
				cost += original_costs[row][col];

	return cost;
}


std::vector<std::pair<int, int>> Hungarian::get_assignemnt()
{
	std::vector<std::pair<int, int>> assignment;

	for (size_t row = 0; row < matrix_size; ++row)
		for (size_t col = 0; col < matrix_size; ++col)
			if (zeros_marks[row][col] == ZeroMark::starred)
				assignment.emplace_back(row, col);

	return assignment;
}


template<class T>
T** Hungarian::create_matrix(size_t size, T init_val)
{
	return create_matrix<T>(size,
		[init_val](size_t row, size_t col) {return init_val; });
}


template<class T>
T** Hungarian::create_matrix(size_t size, std::function<T(size_t, size_t)>const& func)
{
	T** m = new T * [size];
	for (size_t i = 0; i < size; ++i) {
		m[i] = new T[size];
		for (size_t j = 0; j < size; ++j)
			m[i][j] = func(i, j);
	}
	return m;
}


std::vector<std::pair<int, int>> Hungarian::make_assignment()
{
	subtract_min_row();
	//subtract_min_col();
	mark_starred_zeros();
	clear_covers();

	while (!check_cover_is_min()) {
		std::cout << *this;
		while (step_four()) {
			update_costs_with_min();
			std::cout << *this;
		}
		step_five();

		clear_covers();
		erase_primes();
	}

	return get_assignemnt();
}


Hungarian::Hungarian(std::vector<dwarf_t>& dwarfs, std::vector<mine_t>& ores)
	: matrix_size{ std::max(dwarfs.size(), ores.size()) }
{
	if (dwarfs.size() > ores.size())
		throw std::exception("Dimensions mismatch");

	row_cov.resize(matrix_size, false);
	col_cov.resize(matrix_size, false);

	zeros_marks = create_matrix(matrix_size, ZeroMark::notmarked);

	float maxval = 0;
	original_costs = create_matrix<float>(matrix_size,
		[&](size_t row, size_t col) {
			float val = (row < dwarfs.size())
				? dwarfs[row].skills[ores[col].type]
				: 0;

			if (val > maxval) maxval = val;
			return val;
		});

	cost_matrix = create_matrix<float>(matrix_size,
		[this, maxval](size_t row, size_t col) {
			return maxval - this->original_costs[row][col];
		});
}


Hungarian::Hungarian(const std::set<std::pair<size_t, size_t>> & initial_assignement,
	const std::vector<dwarf_t> & dwarfs, const std::vector<mine_t> & ores)
	: matrix_size{ std::max(dwarfs.size(), ores.size()) }
{
	row_cov.resize(matrix_size, false);
	col_cov.resize(matrix_size, false);

	zeros_marks = create_matrix(matrix_size, ZeroMark::notmarked);

	size_t n = matrix_size;
	original_costs = create_matrix<float>(matrix_size,
		[&](size_t row, size_t col) {
			std::set<std::pair<size_t, size_t>>::iterator it;
			return (it = initial_assignement.find(std::make_pair(row, col))) == initial_assignement.end()
				? std::numeric_limits<float>::infinity()
				: it->first < dwarfs.size()
					? distance(dwarfs[it->first].position, ores[it->second].position)
					: 0.f;
		});

	cost_matrix = create_matrix<float>(matrix_size,
		[this](size_t row, size_t col) {
			return this->original_costs[row][col];
		});

}

Hungarian::Hungarian(float matrix[], size_t size)
	: matrix_size{ size }
{
	row_cov.resize(matrix_size, false);
	col_cov.resize(matrix_size, false);

	zeros_marks = create_matrix(matrix_size, ZeroMark::notmarked);

	original_costs = create_matrix<float>(matrix_size,
		[matrix, size](size_t row, size_t col) {
			return matrix[row * size + col];
		});

	cost_matrix = create_matrix<float>(matrix_size,
		[this](size_t row, size_t col) {
			return this->original_costs[row][col];
		});
}


std::set<std::pair<size_t, size_t>> Hungarian::get_zeros()
{
	std::set<std::pair<size_t, size_t>> zeros;
	for (size_t row = 0; row < matrix_size; ++row)
		for (size_t col = 0; col < matrix_size; ++col)
			if (cost_matrix[row][col] == 0)
				zeros.emplace(row, col);

	return zeros;
}


float Hungarian::distance(std::pair<float, float> dwarf, std::pair<float, float> mine)
{
	float dx = dwarf.first - mine.first;
	float dy = dwarf.second - mine.second;
	return dx * dx + dy * dy;
}


std::ostream& operator<< (std::ostream& os, Hungarian const& rhs)
{
	for (int i = 0; i < rhs.matrix_size; ++i)
	{
		for (int j = 0; j < rhs.matrix_size; ++j)
			os << rhs.cost_matrix[i][j] << '\t';
		os << '|';

		for (int j = 0; j < rhs.matrix_size; ++j)
			os << (rhs.zeros_marks[i][j] == ZeroMark::notmarked ? ' '
				: rhs.zeros_marks[i][j] == ZeroMark::starred ? '*'
				: '\'') << ' ';
		os << "|\n";
	}
	std::cout << "-----------------------------\n";
	return os;
}
