#ifndef POLY_STL
#define POLY_STL

#include <iostream>
#include <string>
#include <sstream>
#include <map>
#include <utility>

class poly {
private:
	std::map<int, int> _poly;
	
	void insert(std::istream&);
	
public:
	poly();
	poly(const std::string&);
	poly(const poly&);
	poly(poly&&);
	
	void insert(int, int);
	
	poly operator+(const poly&) const;
	poly& operator+=(const poly&);
	
	poly operator-(const poly&) const;
	poly& operator-=(const poly&);
	
	poly operator*(const poly&) const;
	poly& operator*=(const poly&);
	
	poly operator-() const;
	poly& operator=(poly&&);

	friend std::ostream& operator<<(std::ostream&, const poly&);
	friend std::istream& operator>>(std::istream&, poly&);
};

poly::poly() 
{ }

poly::poly(const std::string& args) {
	std::istringstream stream(args);
	insert(stream);
}

poly::poly(const poly& argp) :
_poly(argp._poly)
{ }

poly::poly(poly&& argp) :
_poly( std::move(argp._poly) )
{ }

void poly::insert(std::istream& stream) {
	int co, exp;
	while (stream >> co >> exp)
		insert(co, exp);
}

void poly::insert(int co, int exp) {
	if (co != 0 && (_poly[exp] += co) == 0)
		_poly.erase(exp);
}

poly poly::operator+(const poly& argp) const {
	poly new_poly(*this);
	for (const auto& kv: argp._poly)
		new_poly.insert(kv.second, kv.first);
	
	return new_poly;
}

poly& poly::operator+=(const poly& argp) {
	return *this = *this + argp;
}

poly poly::operator-(const poly& argp) const {
	poly new_poly(*this);
	for (const auto& kv: argp._poly)
		new_poly.insert(-kv.second, kv.first);
	
	return new_poly;
}

poly& poly::operator-=(const poly& argp) {
	return *this = *this - argp;
}

poly poly::operator*(const poly& argp) const {
	poly new_poly;
	for (const auto& kv1: _poly) {
		int co1 = kv1.second, e1 = kv1.first;
		for (const auto& kv2: argp._poly) {
			int co2 = kv2.second, e2 = kv2.first;
			new_poly.insert(co1*co2, e1+e2);
		}
	}
	
	return new_poly;
}

poly& poly::operator*=(const poly& argp) {
	return *this = *this * argp;
}

poly poly::operator-() const {
	return poly() - *this;
}

poly& poly::operator=(poly&& argp) {
	if (this != &argp)
		_poly = std::move(argp._poly);
	
	return *this;
}

std::ostream& operator<<(std::ostream& out, const poly& argp) {
	auto iter = argp._poly.crbegin(), end = argp._poly.crend();
	
	if (iter == end)
		out << "0 0";
	else {
		while (iter != end) {
			out << iter->second << ' ' << iter->first;
			if (++iter != end) out << ' ';
		}
	}
	
	return out;
}

std::istream& operator>>(std::istream& in, poly& argp) {
	argp.insert(in);
	return in;
}

#endif
