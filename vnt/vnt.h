#ifndef VNT
#define VNT

#include <iostream>
#include <utility>
#include <climits>
#include "safe_matrix.h"

class vnt { 
private:
	int _m, _n;
	safe_matrix<int> _vnt;
	
public:
	vnt(int, int);
	vnt(vnt&&);
	
	bool full() const;
	bool empty() const;
	bool add(int);
	int min();
	bool find(int) const;
	void clear();
	bool sort(int[], int);
	auto operator[](int i) const -> decltype(_vnt[i]);
	
	vnt& operator=(const vnt&);
	vnt& operator=(vnt&&);
	
	friend std::ostream& operator<<(std::ostream&, const vnt&);
};

vnt::vnt(int m, int n) :
_m(m), _n(n), _vnt(_m, _n)
{ 
	clear();
}

vnt::vnt(vnt&& v) :
_m(v._m), _n(v._n), _vnt( std::move(v._vnt) )
{ }

inline bool vnt::full() const {
	return _vnt[_m-1][_n-1] != INT_MAX;
}

inline bool vnt::empty() const {
	return _vnt[0][0] == INT_MAX;
}

bool vnt::add(int val) {
	if ( full() ) return false;
	
	int i = _m-1, j = _n-1;
	bool swapped = true;
	_vnt[i][j] = val;
	
	while (swapped) {
		swapped = false;
	
		if (i == 0) {
			if (swapped = (j > 0 && _vnt[i][j] < _vnt[i][j-1])) {
				std::swap(_vnt[i][j], _vnt[i][j-1]);
				--j;
			}	
		} else if (j == 0) {
			if (swapped = (i > 0 && _vnt[i][j] < _vnt[i-1][j])) {
				std::swap(_vnt[i][j], _vnt[i-1][j]);
				--i;
			}
		} else if (_vnt[i-1][j] >= _vnt[i][j-1]) {
			if (swapped = (_vnt[i][j] < _vnt[i-1][j])) {
				std::swap(_vnt[i][j], _vnt[i-1][j]);
				--i;
			}
		} else {
			if (swapped = (_vnt[i][j] < _vnt[i][j-1])) {
				std::swap(_vnt[i][j], _vnt[i][j-1]);
				--j;
			}
		}
	}
	
	return true;
}

int vnt::min() {
	int i = 0, j = 0;
	int val = _vnt[i][j];
	_vnt[i][j] = INT_MAX;
	bool swapped = true;
	
	while (swapped) {
		if (i == _m-1) {
			if (swapped = j < _n-1 && _vnt[i][j] > _vnt[i][j+1]) {
				std::swap(_vnt[i][j], _vnt[i][j+1]);
				++j;
			}
		} else if (j == _n-1) {
			if (swapped = i < _m-1 && _vnt[i][j] > _vnt[i+1][j]) {
				std::swap(_vnt[i][j], _vnt[i+1][j]);
				++i;
			}
		} else  if (_vnt[i][j+1] <= _vnt[i+1][j]) {
			if (swapped = _vnt[i][j] > _vnt[i][j+1]) {
				std::swap(_vnt[i][j], _vnt[i][j+1]);
				++j;
			}
		} else {
			if (swapped = _vnt[i][j] > _vnt[i+1][j]) {
				std::swap(_vnt[i][j], _vnt[i+1][j]);
				++i;
			}
		}
	}
	
	return val;
}

bool vnt::find(int val) const {
	int i = _m-1, j = 0;
	
	while (i >= 0 && j < _n) {
		if (_vnt[i][j] == val)
			return true;
		else if (val < _vnt[i][j])
			--i;
		else
			++j;
	}
	
	return false;
}

inline void vnt::clear() {
	for (int i = 0; i < _m; ++i)
		for (int j = 0; j < _n; ++j)
			_vnt[i][j] = INT_MAX;
}

bool vnt::sort(int arr[], int size) {
	if (size > _m*_n) return false;
	
	clear();
			
	for (int k = 0; k < size; ++k)
		add(arr[k]);
	
	for (int k = 0; k < size; ++k)
		arr[k] = min();
		
	return true;
}

auto vnt::operator[](int i) const -> decltype(_vnt[i]) {
	return _vnt[i];
}

std::ostream& operator<<(std::ostream& out, const vnt& v) {
	return out << v._vnt;
}

#endif
