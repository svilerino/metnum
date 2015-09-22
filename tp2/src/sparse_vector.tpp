#ifndef SPARSE_VECTOR_TPP
#define SPARSE_VECTOR_TPP

#include <map>
#include <algorithm>
#include <iostream>

typedef unsigned int uint;

template<class T>
class sparse_vector
{
	private:
		typedef typename std::map<uint,T> Map;
		uint vector_size;
		Map map;

	public:
		typedef typename Map::iterator iterator;
		typedef typename Map::const_iterator const_iterator;
		iterator begin() {return map.begin();};
		iterator end() {return map.end();};
		const_iterator cbegin() const {return map.cbegin();};
		const_iterator cend() const {return map.cend();};

		T& operator[] (uint pos)
		{
			T& res(map[pos]);
			vector_size = std::max(vector_size,pos+1);
			return res;
		}
		const T& operator[] (uint pos) const
		{
			const T& res(map[pos]);
			vector_size = std::max(vector_size,pos+1);
			return res;
		}
		bool operator== (const sparse_vector& sv) const
			{return vector_size == sv.vector_size && map == sv.map;};
		bool operator!= (const sparse_vector& sv) const
			{return !(*this == sv);};

		uint size() const {return vector_size;};
		void resize(uint newsize) {vector_size = newsize;};

		sparse_vector() : vector_size(0), map() {};
		sparse_vector(uint size) : vector_size(size), map() {};
}; 

template<class T>
std::ostream& operator<< (std::ostream& os, const sparse_vector<T>& v)
{
	for(auto it=v.cbegin(); it!=v.cend();++it)
		os << "[" << it->first << "] = " << it->second << std::endl;
	return os;
};

#endif
