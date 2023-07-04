#pragma once

#include<iostream>
#include<vector>
#include<tuple>
#include<functional>
#include<set>

#include "types.h"


class Hungarian
{
	size_t dwarfs_no;

	float** cost_matrix;
	float** original_costs;

	ZeroMark** zeros_marks;

	std::vector<bool> row_cov;
	std::vector<bool> col_cov;

	std::vector<std::pair<size_t, size_t>> path;


	void subtract_min_col();
	void subtract_min_row();
	void mark_starred_zeros();
	bool check_cover_is_min();
	bool exists_uncovered_zero(size_t& rw, size_t& cl);
	bool zero_in_row(size_t rw, size_t& cl, ZeroMark mark_type);
	bool zero_in_col(size_t& rw, size_t cl, ZeroMark mark_type);
	void augment_path();
	void clear_covers();
	void erase_primes();
	float find_min_uncovered();
	bool step_four();
	void step_five();
	void update_costs_with_min();

	static float distance(std::pair<float, float> dwarf, std::pair<float, float> mine);

	template<class T>
	T** create_matrix(size_t size, T init_val);
	template<class T>
	T** create_matrix(size_t size, std::function<T(size_t, size_t)>const& func);

	friend std::ostream& operator<< (std::ostream& os, Hungarian const& cost_mat);

public:
	std::vector<std::pair<int, int>> make_assignment();
	std::vector<std::pair<int, int>> get_assignemnt();
	float calc_cost() const;

	std::set<std::pair<size_t, size_t>> get_zeros();

	Hungarian(std::vector<dwarf_t>& dwarfs, std::vector<mine_t>& ores);
	
	Hungarian(float matrix[], size_t size);
	Hungarian(std::set<std::pair<size_t, size_t>> const& initial_assignement, std::vector<dwarf_t> const& dwarfs, std::vector<mine_t> const& ores);
};

