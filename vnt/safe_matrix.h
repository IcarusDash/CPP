#ifndef SAFE_MATRIX
#define SAFE_MATRIX

#include <ostream>
#include <istream>
#include <utility>
#include <stdexcept>
#include <string>
#include "safe_array.h"

template<typename T>
class safe_matrix;

template<typename T>
std::ostream& operator<<(std::ostream&, const safe_matrix<T>&);

template<typename T>
std::istream& operator>>(std::istream&, safe_matrix<T>&);

template <typename T>
class safe_matrix {
private:
	int _r_lo, _r_hi, _c_lo, _c_hi;
	safe_array<safe_array<T>> _mat;
	
public:
	safe_matrix(int=0);
	safe_matrix(int, int);
	safe_matrix(int, int, int, int);
	safe_matrix(safe_matrix&&);
	
	safe_matrix operator+(const safe_matrix&) const; 
	safe_matrix operator-(const safe_matrix&) const; 
	safe_matrix operator*(const safe_matrix&) const; 
	safe_array<T>& operator[](int);
	const safe_array<T>& operator[](int) const;
	safe_matrix& operator=(safe_matrix&&);
	
	friend std::ostream& 
	operator<< <T>(std::ostream&, const safe_matrix<T>&);
	
	friend std::istream&
	operator>> <T>(std::istream&, safe_matrix<T>&);
};

template <typename T>
safe_matrix<T>::safe_matrix(int dim) :
safe_matrix(0, dim-1, 0, dim-1)
{ }

template <typename T>
safe_matrix<T>::safe_matrix(int rows, int cols) :
safe_matrix(0, rows-1, 0, cols-1)
{ }

template <typename T>
safe_matrix<T>::safe_matrix(int i1, int i2, int j1, int j2) :
_r_lo(i1), _r_hi(i2), _c_lo(j1), _c_hi(j2)
{
	int row_size = _r_hi-_r_lo+1, col_size = _c_hi-_c_lo+1;
	if (row_size <= 0 || col_size <= 0) 
		throw std::length_error
		(
			"invalid bounds. row size: " + std::to_string(row_size) 
			+ ", column size: " + std::to_string(col_size)
		);
	
	_mat = safe_array<safe_array<T>>(_r_lo, _r_hi);
	for (int i = _r_lo; i <= _r_hi; ++i)
		_mat[i] = safe_array<T>(_c_lo, _c_hi);
}

template <typename T>
safe_matrix<T>::safe_matrix(safe_matrix&& sm) :
_r_lo(sm._r_lo), _r_hi(sm._r_hi), _c_lo(sm._c_lo), 
_c_hi(sm._c_hi), _mat( std::move(sm._mat) )
{ }

template <typename T>
safe_matrix<T> safe_matrix<T>::operator+(const safe_matrix& sm) const {
	int this_rsize = _r_hi-_r_lo+1, this_csize = _c_hi-_c_lo+1;
	int othr_rsize = sm._r_hi-sm._r_lo+1, othr_csize = sm._c_hi-sm._c_lo+1;
	
	if (this_rsize != othr_rsize || this_csize != othr_csize) 
		throw std::length_error
		(
			"matrix dimension mismatch"
		);
	
	safe_matrix<T> r_sm(this_rsize, this_csize);
	
	int this_i = _r_lo, othr_i = sm._r_lo;
	for (int i = 0; i < this_rsize; ++i, ++this_i, ++othr_i) {
		int this_j = _c_lo, othr_j = sm._c_lo;
		for (int j = 0; j < this_csize; ++j, ++this_j, ++othr_j) {
			r_sm[i][j] = _mat[this_i][this_j] + sm._mat[othr_i][othr_j];
		}
	}
	
	return r_sm;
}

template <typename T>
safe_matrix<T> safe_matrix<T>::operator-(const safe_matrix& sm) const {
	int this_rsize = _r_hi-_r_lo+1, this_csize = _c_hi-_c_lo+1;
	int othr_rsize = sm._r_hi-sm._r_lo+1, othr_csize = sm._c_hi-sm._c_lo+1;
	
	if (this_rsize != othr_rsize || this_csize != othr_csize)
		throw std::length_error
		(
			"matrix dimension mismatch"
		);
	
	safe_matrix<T> r_sm(this_rsize, this_csize);
	
	int this_i = _r_lo, othr_i = sm._r_lo;
	for (int i = 0; i < this_rsize; ++i, ++this_i, ++othr_i) {
		int this_j = _c_lo, othr_j = sm._c_lo;
		for (int j = 0; j < this_csize; ++j, ++this_j, ++othr_j) {
			r_sm[i][j] = _mat[this_i][this_j] - sm._mat[othr_i][othr_j];
		}
	}
	
	return r_sm;
}

template <typename T>
safe_matrix<T> safe_matrix<T>::operator*(const safe_matrix& sm) const {
	int this_rsize = _r_hi-_r_lo+1, this_csize = _c_hi-_c_lo+1;
	int othr_rsize = sm._r_hi-sm._r_lo+1, othr_csize = sm._c_hi-sm._c_lo+1;

	if (this_csize != othr_rsize)
		throw std::length_error
		(
			"IMPOSSIBLE"
		);
	
	safe_matrix<T> r_sm(this_rsize, othr_csize);
	
	for (int i = 0, this_i = _r_lo; i < this_rsize; ++i, ++this_i) {
		for (int j = 0, othr_j = sm._c_lo; j < othr_csize; ++j, ++othr_j) {
			r_sm[i][j] = 0;
			int this_j = _c_lo, othr_i = sm._r_lo;
			for (int k = 0; k < this_csize; ++k, ++this_j, ++othr_i) {
				r_sm[i][j] += _mat[this_i][this_j] * sm._mat[othr_i][othr_j];
			}
		}
	}
	
	return r_sm;
}

template <typename T>
safe_array<T>& safe_matrix<T>::operator[](int i) {
	return _mat[i];
}

template <typename T>
const safe_array<T>& safe_matrix<T>::operator[](int i) const {
	return _mat[i];
}

template <typename T>
safe_matrix<T>& safe_matrix<T>::operator=(safe_matrix&& sm) {
	if (this != &sm) {
		_r_lo = sm._r_lo;
		_c_lo = sm._c_lo;
		_r_hi = sm._r_hi;
		_c_hi = sm._c_hi;
		std::swap(_mat, sm._mat);
	}
	
	return *this;
}

template <typename T>
std::ostream& operator<<(std::ostream& out, const safe_matrix<T>& sm) {
	for (int i = sm._r_lo, end = sm._r_hi; i <= end; ++i)
		out << sm._mat[i] << '\n';
		
	return out;
}

template <typename T>
std::istream& operator>>(std::istream& in, safe_matrix<T>& sm) {	
	return in >> sm._mat;
}

#endif
