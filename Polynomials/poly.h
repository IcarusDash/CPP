#ifndef POLY
#define POLY

#include <iostream>
#include <string>
#include <sstream>

class poly {
private:
	struct term {
		int _coeff, _exp;
		term *_next;
		
		term(int=0, int=0);
		term(const term*);
		
		~term();
	} *_dummy;
	
	term* new_list() const;
	void insert(std::istream&);
	void stream(std::ostream&) const;
	
public:
	poly();
	poly(const poly&);
	poly(poly&&);
	poly(const std::string&);
	
	void insert(int, int);
	
	poly operator+(const poly&) const;
	poly& operator+=(const poly&);
	
	poly operator-(const poly&) const;
	poly& operator-=(const poly&);
	
	poly operator*(const poly&) const;
	poly& operator*=(const poly&);
	
	poly& operator=(const poly&);
	poly& operator=(poly&&);
	
	~poly();

	friend std::ostream& operator<<(std::ostream&, const poly&);
	friend std::istream& operator>>(std::istream&, poly&);
};

poly::term::term(int co, int e) :
_coeff(co), _exp(e), _next(nullptr)
{ }

poly::term::term(const term* argt) :
term(argt->_coeff, argt->_exp)
{ }

poly::term::~term() {
	delete _next;
}

poly::poly() :
_dummy(new term)
{ }

poly::term* poly::new_list() const {
	term *ptr = _dummy, new_ptr = new term;
	term *r_ptr = new_ptr;
	
	while (ptr = ptr->_next) {
		new_ptr->_next = new term(ptr);
		new_ptr = new_ptr->_next;
	}
	
	return r_ptr;
}

poly::poly(poly&& argp) :
_dummy(argp._dummy)
{
	argp._dummy = nullptr;
}

poly::poly(const poly& argp) :
_dummy( argp.new_list() )
{ }

poly::poly(const std::string& args) :
poly()
{
	std::istringstream stream(args);
	insert(stream);
}

void poly::insert(std::istream& stream) {
	int co, exp;
	while (stream >> co >> exp)
		insert(co, exp);
}

void poly::insert(int coeff, int exp) {
	if (coeff == 0) return;
	term *lhs = _dummy, *rhs = lhs->_next;
	
	while (rhs && rhs->_exp > exp) {
		lhs = rhs;
		rhs = rhs->_next;
	}
	
	if (!rhs)
		lhs->_next = new term(coeff, exp);
	else if (rhs->_exp == exp) {
		if ((rhs->_coeff += coeff) == 0) {
			term *del_term = rhs;
			lhs->_next = del_term->_next;
			del_term->_next = nullptr;
			delete del_term;
		}
	} else {
		term *new_term = lhs->_next = new term(coeff, exp);
		new_term->_next = rhs;
	}
}

poly poly::operator+(const poly& argp) const {
	poly new_poly(*this);
	term *ptr = argp._dummy;
	
	while (ptr = ptr->_next)
		new_poly.insert(ptr->_coeff, ptr->_exp);
	
	return new_poly;
}

poly& poly::operator+=(const poly& argp) {
	return *this = *this + argp;
}

poly poly::operator-(const poly& argp) const {
	poly new_poly(*this);
	term *ptr = argp._dummy;
	
	while (ptr = ptr->_next)
		new_poly.insert(-(ptr->_coeff), ptr->_exp);
	
	return new_poly;
}

poly& poly::operator-=(const poly& argp) {
	return *this = *this - argp;
}

poly poly::operator*(const poly& argp) const {
	poly new_poly;
	term *first = _dummy;
	
	while (first = first->_next) {
		term *second = argp._dummy;
		int co1 = first->_coeff, e1 = first->_exp;
		
		while (second = second->_next) {
			int co2 = second->_coeff, e2 = second->_exp;
			new_poly.insert(co1*co2, e1+e2);
		}
	}
	
	return new_poly;
}

poly& poly::operator*=(const poly& argp) {
	return *this = *this * argp;
}

poly& poly::operator=(const poly& argp) {
	if (this != &argp) {
		delete _dummy;
		_dummy = argp.new_list();
	}
	
	return *this;
}

poly& poly::operator=(poly&& argp) {
	if (this != &argp) {
		term *temp = _dummy;
		_dummy = argp._dummy;
		argp._dummy = temp;
	}
	
	return *this;
}

poly::~poly() {
	delete _dummy;
}

void poly::stream(std::ostream& out) const {
	term *term_ptr = _dummy->_next;
	
	if (!term_ptr)
		out << "0 0";
	else {
		while (term_ptr) {
			out << term_ptr->_coeff << ' ' << term_ptr->_exp;
			if (term_ptr = term_ptr->_next) out << ' ';
		}
	}
}

std::ostream& operator<<(std::ostream& out, const poly& argp) {
	argp.stream(out);
	return out;
}

std::istream& operator>>(std::istream& in, poly& argp) {
	argp.insert(in);
	return in;
}

#endif
