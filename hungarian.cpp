

#include<string>
#include<iostream>
#include<limits>
#include<vector>
#include<map>
#include<functional>
#include<tuple>

#include "hungarian.h"



void Hungarian::subtract_min_col()
{
	std::vector<float> mins(dwarfs_no, std::numeric_limits<float>::max());

	for (size_t row = 0; row < dwarfs_no; ++row)
		for (int col = 0; col < dwarfs_no; ++col)
			if (cost_matrix[row][col] < mins[col])
				mins[col] = cost_matrix[row][col];

	for (size_t row = 0; row < dwarfs_no; ++row)
		for (int col = 0; col < dwarfs_no; ++col)
			cost_matrix[row][col] -= mins[col];
}


void Hungarian::subtract_min_row()
{
	for (int row = 0; row < dwarfs_no; ++row) {
		float row_min = std::numeric_limits<float>::max();
		for (int col = 0; col < dwarfs_no; ++col)
			if (cost_matrix[row][col] < row_min)
				row_min = cost_matrix[row][col];

		for (int col = 0; col < dwarfs_no; ++col)
			cost_matrix[row][col] -= row_min;
	}
}


void Hungarian::mark_starred_zeros()
{
	for (int row = 0; row < dwarfs_no; ++row)
		for (int col = 0; col < dwarfs_no; ++col)
			if (cost_matrix[row][col] == 0 && !row_cov[row] && !col_cov[col])
			{
				zeros_marks[row][col] = ZeroMark::starred;
				row_cov[row] = true;
				col_cov[col] = true;
			}
}

bool Hungarian::check_cover_is_min()
{
	for (size_t row = 0; row < dwarfs_no; ++row)
		for (size_t col = 0; col < dwarfs_no; ++col)
			if (zeros_marks[row][col] == ZeroMark::starred)
				col_cov[col] = true;

	for (int col = 0; col < dwarfs_no; ++col)
		if (!col_cov[col])
			return false;

	return true;
}


bool Hungarian::exists_uncovered_zero(size_t& rw, size_t& cl)
{
	for (size_t row = 0; row < dwarfs_no; ++row) {
		for (size_t col = 0; col < dwarfs_no; ++col) {
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
	for (size_t col = 0; col < dwarfs_no; ++col)
		if (zeros_marks[rw][col] == mark_type) {
			cl = col;
			return true;
		}
	return false;
}

bool Hungarian::zero_in_col(size_t& rw, size_t cl, ZeroMark mark_type)
{
	for (size_t row = 0; row < dwarfs_no; ++row)
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
	for (size_t row = 0; row < dwarfs_no; ++row)
		for (size_t col = 0; col < dwarfs_no; ++col)
			if (zeros_marks[row][col] == ZeroMark::primed)
				zeros_marks[row][col] = ZeroMark::notmarked;
}


float Hungarian::find_min_uncovered()
{
	float minval = std::numeric_limits<float>::max();
	for (size_t row = 0; row < dwarfs_no; ++row)
		for (size_t col = 0; col < dwarfs_no; ++col)
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

	for (size_t row = 0; row < dwarfs_no; ++row)
		for (size_t col = 0; col < dwarfs_no; ++col) {
			if (row_cov[row]) cost_matrix[row][col] += minval;
			if (!col_cov[col]) cost_matrix[row][col] -= minval;
		}

	// goto step 4
}


float Hungarian::calc_cost() const
{
	float cost = 0;
	for (size_t row = 0; row < dwarfs_no; ++row)
		for (size_t col = 0; col < dwarfs_no; ++col)
			if (zeros_marks[row][col] == ZeroMark::starred)
				cost += original_costs[row][col];

	return cost;
}

std::vector<std::pair<int, int>> Hungarian::get_assignemnt()
{
	std::vector<std::pair<int, int>> assignment;

	for (size_t row = 0; row < dwarfs_no; ++row)
		for (size_t col = 0; col < dwarfs_no; ++col)
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
	: dwarfs_no{ dwarfs.size() }
{
	if (dwarfs.size() != ores.size())
		throw std::exception("Dimensions mismatch");

	row_cov.resize(dwarfs_no, false);
	col_cov.resize(dwarfs_no, false);

	zeros_marks = create_matrix(dwarfs_no, ZeroMark::notmarked);

	//cost_matrix = create_matrix<float>(dwarfs_no,
	//	[this](size_t row, size_t col) {
	//		return (this->dwarfs[row].type == this->ores[col].type)
	//			? this->min_cost
	//			: this->max_cost;
	//	});

	float maxval = 0;
	original_costs = create_matrix<float>(dwarfs_no,
		[&dwarfs, &ores, &maxval](size_t row, size_t col) {
			float val = dwarfs[row].skills[ores[col].type];
	if (val > maxval) maxval = val;
	return val;
		});

	cost_matrix = create_matrix<float>(dwarfs_no,
		[this, maxval](size_t row, size_t col) {
			return maxval - this->original_costs[row][col];
		});
}

Hungarian::Hungarian(float matrix[], size_t size)
	: dwarfs_no{ size }
{
	row_cov.resize(dwarfs_no, false);
	col_cov.resize(dwarfs_no, false);

	zeros_marks = create_matrix(dwarfs_no, ZeroMark::notmarked);

	original_costs = create_matrix<float>(dwarfs_no,
		[matrix, size](size_t row, size_t col) {
			return matrix[row * size + col];
		});

	cost_matrix = create_matrix<float>(dwarfs_no,
		[this](size_t row, size_t col) {
			return this->original_costs[row][col];
		});
}


std::ostream& operator<< (std::ostream& os, Hungarian const& rhs)
{
	for (int i = 0; i < rhs.dwarfs_no; ++i)
	{
		for (int j = 0; j < rhs.dwarfs_no; ++j)
			os << rhs.cost_matrix[i][j] << '\t';
		os << '|';

		for (int j = 0; j < rhs.dwarfs_no; ++j)
			os << (rhs.zeros_marks[i][j] == ZeroMark::notmarked ? ' '
				: rhs.zeros_marks[i][j] == ZeroMark::starred ? '*'
				: '\'') << ' ';
		os << "|\n";
	}
	std::cout << "-----------------------------\n";
	return os;
}
