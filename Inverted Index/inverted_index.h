#ifndef INVERTED_INDEX
#define INVERTED_INDEX

#include <string>
#include <ostream>
#include <fstream>
#include <sstream>

class inverted_index {
private:
	struct ln_node {
		int _ln, _times;
		ln_node *_next;
		
		ln_node(int=-1, int=1);
		~ln_node();
	};
	
	struct ln_list { 
		ln_node *_dummy;
		
		ln_list();
		ln_list(const ln_list&);
		ln_list(ln_list&&);
		
		ln_node* new_list() const;
		void insert(int);
		ln_list& operator=(const ln_list&);
		ln_list& operator=(ln_list&&);
		void stream(std::ostream&) const;
		~ln_list();
	};
	
	struct wd_node {
		std::string _wd;
		ln_list _list;
		wd_node *_next;
		
		wd_node(const std::string&);
		~wd_node();
	};
	
	wd_node *_dummy;
	
	void insert(const std::string&, int);
	void stream(std::ostream&) const;
	wd_node* new_list() const;
	
public:
	inverted_index(const std::string&);
	inverted_index(const inverted_index&);
	
	inverted_index& operator=(const inverted_index&);
	~inverted_index();
	
	friend std::ostream& operator<<(std::ostream&, const ln_list&);
	friend std::ostream& operator<<(std::ostream&, const inverted_index&);
};

inverted_index::ln_node::ln_node(int ln_no, int times) :
_ln(ln_no), _times(times), _next(nullptr)
{ }

inverted_index::ln_node::~ln_node() {
	delete _next;
}

inverted_index::ln_list::ln_list() :
_dummy(new ln_node) 
{ }

inverted_index::ln_list::ln_list(const ln_list& ll) :
ln_list()
{
	_dummy->_next = ll.new_list();
}

inverted_index::ln_list::ln_list(ln_list&& ll) :
_dummy(ll._dummy)
{
	ll._dummy = nullptr;
}

inverted_index::ln_node* inverted_index::ln_list::new_list() const {
	ln_node *ptr = _dummy->_next;
	if (!ptr) return nullptr;
	
	ln_node *r_list = new ln_node(ptr->_ln, ptr->_times);
	ln_node *new_ptr = r_list;
	
	while (ptr = ptr->_next) {
		new_ptr->_next = new ln_node(ptr->_ln, ptr->_times);
		new_ptr = new_ptr->_next;
	}
	
	return r_list;
}

void inverted_index::ln_list::insert(int ln) {
	ln_node *lhs = _dummy, *rhs = lhs->_next;
	
	while (rhs && rhs->_ln < ln) {
		lhs = rhs;
		rhs = rhs->_next;
	}
	
	if (!rhs)
		lhs->_next = new ln_node(ln);
	else if (rhs->_ln == ln)
		++rhs->_times;
	else {
		ln_node *new_node = lhs->_next = new ln_node(ln);
		new_node->_next = rhs;
	}
}

void inverted_index::ln_list::stream(std::ostream& out) const {
	ln_node *ptr = _dummy->_next;
	
	while (ptr) {
		out << ptr->_ln;
		if (ptr->_times > 1)
			out << '(' << ptr->_times << ')';
		
		if (ptr = ptr->_next)
			out << ", ";
	}
}

inverted_index::ln_list& inverted_index::ln_list::operator=(const ln_list& ll) {
	if (this != &ll) {
		delete _dummy->_next;
		_dummy->_next = ll.new_list();
	}
	
	return *this;
}

inverted_index::ln_list& inverted_index::ln_list::operator=(ln_list&& ll) {
	if (this != &ll) {
		ln_node *tmp = _dummy;
		_dummy = ll._dummy;
		ll._dummy = tmp;
	}
	
	return *this;
}

std::ostream& operator<<(std::ostream& out, const inverted_index::ln_list& ll) {
	ll.stream(out);
	return out;
}

inverted_index::ln_list::~ln_list() {
	delete _dummy;
}

inverted_index::wd_node::wd_node(const std::string& args) :
_wd(args), _next(nullptr)
{ }

inverted_index::wd_node::~wd_node() {
	delete _next;
}

inverted_index::inverted_index(const std::string& args) :
_dummy( new wd_node(std::string(), ln_list()) )
{
	std::ifstream file(args);
	std::string line, word;
	std::istringstream stream;
	int line_no = 1;
	
	while ( std::getline(file, line) ) {
		stream.str(line);
		while (stream >> word)
			insert(word, line_no);
			
		stream.clear();
		++line_no;
	}
}

inverted_index::inverted_index(const inverted_index& ii) :
_dummy( new wd_node(std::string()) )
{
	_dummy->_next = ii.new_list();
}

inverted_index::wd_node* inverted_index::new_list() const {
	wd_node *ptr = _dummy->_next;
	if (!ptr) return nullptr;
	
	wd_node *r_list = new wd_node(ptr->_wd, ptr->_list);
	wd_node *new_ptr = r_list;

	while (ptr = ptr->_next) {
		new_ptr->_next = new wd_node(ptr->_wd, ptr->_list);
		new_ptr = new_ptr->_next;
	}
	
	return r_list;
}

void inverted_index::insert(const std::string& args, int ln) {
	wd_node *lhs = _dummy, *rhs = lhs->_next;
	
	while (rhs && rhs->_wd < args) {
		lhs = rhs;
		rhs = rhs->_next;
	}
	
	if (!rhs) {
		wd_node *new_node = lhs->_next = new wd_node(args, ln_list());
		new_node->_list.insert(ln);
	} else if (rhs->_wd == args)
		rhs->_list.insert(ln);
	else {
		wd_node *new_node = lhs->_next = new wd_node(args, ln_list());
		new_node->_list.insert(ln);
		new_node->_next = rhs;
	}
}

inverted_index& inverted_index::operator=(const inverted_index& ii) {
	if (this != &ii) {
		delete _dummy->_next;
		_dummy->_next = ii.new_list();
	}
	
	return *this;
}

void inverted_index::stream(std::ostream& out) const {
	wd_node *ptr = _dummy;
	
	while (ptr = ptr->_next)
		out << ptr->_wd << ": " << ptr->_list << '\n';
}

std::ostream& operator<<(std::ostream& out, const inverted_index& ii) {
	ii.stream(out);
	return out;
}

inverted_index::~inverted_index() {
	delete _dummy;
}

#endif