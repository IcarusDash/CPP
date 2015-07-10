#ifndef INVERTED_INDEX_STL
#define INVERTED_INDEX_STL

#include <string>
#include <sstream>
#include <fstream>
#include <ostream>
#include <utility>
#include <map>

class inverted_index {
	std::map<std::string, 
			 std::map<int, int>
			> _ii;

public:
	explicit inverted_index(const char*);
	explicit inverted_index(const std::string&);
	
	inverted_index(const inverted_index&) = default;
	inverted_index(inverted_index&&) = default;
	
	inverted_index& operator=(const inverted_index&) = default;
	inverted_index& operator=(inverted_index&&) = default;
	
	friend std::ostream& operator<<(std::ostream&, const inverted_index&);
};

inverted_index::inverted_index(const char* cstr) {
	std::ifstream file(cstr);
	std::string line, word;
	std::istringstream stream;
	int line_no = 1;
	
	while ( std::getline(file, line) ) {
		stream.str(line);
		while (stream >> word)
			++_ii[std::move(word)][line_no];
			
		stream.clear();
		++line_no;
	}
	
	file.close();
}

inverted_index::inverted_index(const std::string& args) :
inverted_index( args.data() )
{ }

std::ostream& operator<<(std::ostream& out, const inverted_index& ii) {	
	for (auto& kv: ii._ii) {
		out << kv.first << ": ";
		auto iter = kv.second.cbegin(), end = kv.second.cend();
		
		while (iter != end) {
			out << iter->first;
			if (iter->second > 1)
				out << '(' << iter->second << ')';
			
			if (++iter != end) out << ", ";
		}
		
		out << '\n';
	} 
	
	return out;
}

#endif
