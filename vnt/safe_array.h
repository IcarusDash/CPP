#ifndef SAFE_ARRAY
#define SAFE_ARRAY

#include <ostream>
#include <istream>
#include <string>
#include <utility>
#include <stdexcept>

template <typename T> 
class safe_array;

template <typename T>
std::ostream& operator<<(std::ostream&, const safe_array<T>&);

template <typename T>
std::istream& operator>>(std::istream&, safe_array<T>&);

template <typename T>
void sort(safe_array<T>&, int);

template <typename T>
class safe_array { 
private:
	int _lo, _hi;
	T *_arr;
	
	T* arr_cp() const;
	
public:
	safe_array();
	safe_array(int);
	safe_array(int, int);
	safe_array(std::initializer_list<T>);
	safe_array(const safe_array&);
	safe_array(safe_array&&);
	
	T* operator+(int);
	const T* operator+(int) const;
	T& operator[](int);
	const T& operator[](int) const;
	safe_array& operator=(const safe_array&);
	safe_array& operator=(safe_array&&);
	
	~safe_array();
	
	friend std::ostream& 
	operator<< <T> (std::ostream&, const safe_array<T>&);
	
	friend std::istream& 
	operator>> <T> (std::istream&, safe_array<T>&);
	
	friend void sort<T>(safe_array<T>&, int);
};

template <typename T>
T* safe_array<T>::arr_cp() const {
	if (_arr == nullptr) 
		return nullptr;

	int size = _hi-_lo+1;
	T *r_array = new T[size];
	for (int i = 0; i < size; ++i)
		r_array[i] = _arr[i];
		
	return r_array;
}

template <typename T>
safe_array<T>::safe_array() :
_lo(0), _hi(-1), _arr(nullptr)
{ }

template <typename T>
safe_array<T>::safe_array(int sz) :
safe_array(0, sz-1)
{ }

template <typename T>
safe_array<T>::safe_array(int low, int high) :
_lo(low), _hi(high)
{
	int size = _hi-_lo+1;
	if (size <= 0)
		throw std::length_error
		(
			"invalid bounds: " + std::to_string(size)
		);
	
	_arr = new T[size];
}

template <typename T>
safe_array<T>::safe_array(std::initializer_list<T> il) :
safe_array(0, il.size()-1)
{
	int i = 0;
	for (auto& elmt: il)
		_arr[i++] = std::move(elmt);
}

template <typename T>
safe_array<T>::safe_array(const safe_array& sa) :
_hi(sa._hi), _lo(sa._lo), _arr( sa.arr_cp() )
{ }

template <typename T>
safe_array<T>::safe_array(safe_array&& sa) :
_hi(sa._hi), _lo(sa._lo), _arr(sa._arr) 
{
	sa._arr = nullptr;
	sa._lo = 0;
	sa._hi = -1;
}

template <typename T>
T* safe_array<T>::operator+(int offset) {
	if (offset < _lo || offset > _hi)
		throw std::out_of_range
		(
			"offset " + std::to_string(offset) + " out of range"
		);

	return _arr + (offset-_lo);
}

template <typename T>
const T* safe_array<T>::operator+(int offset) const {
	if (offset < _lo || offset > _hi)
		throw std::out_of_range
		(
			"offset " + std::to_string(offset) + " out of range"
		);

	return _arr + (offset-_lo);
}

template <typename T>
T& safe_array<T>::operator[](int i) {
	if (i < _lo || i > _hi)
		throw std::out_of_range
		(
			"index " + std::to_string(i) + " out of range"
		);

	return _arr[i-_lo];
}

template <typename T>
const T& safe_array<T>::operator[](int i) const {
	if (i < _lo || i > _hi)
		throw std::out_of_range
		(
			"index " + std::to_string(i) + " out of range"
		);
	
	return _arr[i-_lo];
}

template <typename T>
safe_array<T>& safe_array<T>::operator=(const safe_array& rhs) {
	if (this != &rhs) {
		delete []_arr;
		_arr = rhs.arr_cp();
		_lo = rhs._lo;
		_hi = rhs._hi;
	}
	
	return *this;
}

template <typename T>
safe_array<T>& safe_array<T>::operator=(safe_array&& rhs) {
	if (this != &rhs) {
		T *tmp = _arr;
		_arr = rhs._arr;
		rhs._arr = tmp;
		
		_lo = rhs._lo;
		rhs._lo = 0;
		
		_hi = rhs._hi;
		rhs._hi = -1;
	}
	
	return *this;
}

template <typename T>
std::ostream& operator<<(std::ostream& out, const safe_array<T>& sa) {
	int end = sa._hi-sa._lo;
	for (int i = 0; i < end; ++i)
		out << sa._arr[i] << ' ';
	out << sa._arr[end];
	
	return out;
}

template <typename T>
std::istream& operator>>(std::istream& in, safe_array<T>& sa) {
	for (int i = 0, end = sa._hi-sa._lo; i <= end; ++i)
		in >> sa._arr[i];
	
	return in;
}

template <typename T>
void sort(safe_array<T>& sa, int sz) {
	auto swap = [](T& x, T& y) {
		T tmp( std::move(x) );
		x = std::move(y);
		y = std::move(tmp);
	};

	int size = sa._hi-sa._lo+1;
	int cap = sz > size ? size : sz, i = 0;
	T *arr = sa._arr;
	
	while (i < cap) {
		if (i == 0 || arr[i-1] <= arr[i])
			++i;
		else {
			swap(arr[i-1], arr[i]);
			--i;
		}
	}
}

template <typename T>
safe_array<T>::~safe_array() {
	delete []_arr;
}

#endif
