#pragma once

#include<string>

namespace First
{

enum State
{
	EMPTY,
	DELETE,
	EXITS,
};

template<class K>
class HashTable
{
public:
	HashTable(size_t capacity = 10)
		:_tables(new K[capacity])
		,_states(new State[capacity])
		,_size(0)
		,_capacity(capacity)
	{
		//memset(_states, EMPTY, sizeof(State)*_capacity);
		for (size_t i = 0 ; i < _capacity; ++i)
		{
			_states[i] = EMPTY;
		}
	}

	HashTable(const HashTable<K>& ht)
		:_tables(NULL)
		,_states(NULL)
		,_size(0)
		,_capacity(0)
	{
		HashTable<K> tmp(ht._capacity);
		for (size_t i = 0; i < ht._capacity; ++i)
		{
			if (ht._states[i] == EXITS)
			{
				tmp.Insert(ht._tables[i]);
			}
		}
		this->Swap(tmp);
	}

	HashTable<K>& operator=(HashTable<K> ht)
	{
		this->Swap(ht);
		return *this;
	}

	~HashTable()
	{
		if (_tables)
		{
			delete[] _tables;
			delete[] _states;
		}
	}

	bool Insert(const K& key)
	{
	/*	if (_size == _capacity)
		{
			cout<< "HashTable is Full"<<endl;
			return false;
		}*/
		_CheckCapacity();

		size_t index = _HashFunc(key);
		// 线性探测
		while(_states[index] == EXITS)
		{
			if (_tables[index] == key)
			{
				return false;
			}

			++index;
			if (index == _capacity)
			{
				index = 0;
			}
		}

		_tables[index] = key;
		_states[index] = EXITS;
		++_size;
		return true;
	}

	bool Find(const K& key)
	{
		size_t index = _HashFunc(key);
		size_t start = index;
		while (_states[index] != EMPTY)
		{
			if (_tables[index] == key)
			{
				if (_states[index] != DELETE)
					return true;
				else
					return false;
			}
			
			++index;
			if (index == _capacity)
				index = 0;

			if (index == start)
				return false;
		}

		return false;
	}

	bool Remove(const K& key)
	{
		size_t index = _HashFunc(key);
		size_t start  = index;
		while (_states[index] != EMPTY)
		{
			if (_tables[index] == key)
			{
				_states[index] = DELETE;
				--_size;
				return true;
			}

			++index;
			if (index = _capacity)
				index = 0;

			if (index == start)
				return  false;
		}
	
		return false;

	}

	size_t _HashFunc(const K& key)
	{
		return key%_capacity;
	}

	void Print()
	{
		for (size_t i = 0; i < _capacity; ++i)
		{
			printf("[%d:%d],", _states[i], _tables[i]);
		}
		cout<<endl;
	}

	void Swap(HashTable<K>& ht)
	{
		swap(_tables, ht._tables);
		swap(_states, ht._states);
		swap(_size, ht._size);
		swap(_capacity, ht._capacity);
	}

protected:
	void _CheckCapacity()
	{
		if (_size*10/_capacity == 7)
		{
			/*_capacity = 2*_capacity;
			T* tmp = new T[_capacity];

			for (size_t i = 0; i < )
			{
			}*/

			HashTable<K> tmp(2*_capacity);
			for (size_t i = 0; i < _capacity; ++i)
			{
				if (_states[i] == EXITS)
				{
					tmp.Insert(_tables[i]);
				}
			}

			this->Swap(tmp);
		}
	}

protected:
	K* _tables;			// 哈希表
	State* _states;		// 状态表

	size_t _size;
	size_t _capacity;
};

void TestHashtable()
{
	HashTable<int> ht;
	ht.Insert(89);
	ht.Insert(18);
	ht.Insert(49);
	ht.Insert(58);
	ht.Insert(9);

	ht.Print();

	ht.Remove(49);
	cout<<"Find 58?"<<ht.Find(58)<<endl;
	ht.Print();
}

void TestHashtableExpand()
{
	HashTable<int> ht;
	ht.Insert(89);
	ht.Insert(18);
	ht.Insert(49);
	ht.Insert(58);
	ht.Insert(9);

	ht.Insert(15);
	ht.Insert(20);
	ht.Insert(21);

	ht.Print();
}

}

namespace Second
{
enum State
{
	EMPTY,
	DELETE,
	EXITS,
};

template<class K, class V>
struct HashTableNode
{
	K _key;
	V _value;
};

template<class K>
struct __HashFunc
{
	size_t operator()(const K& key)
	{
		return key;
	}
};

//struct __HashFuncString
//{
//	size_t operator()(const string& key)
//	{
//		size_t hash = 0;
//		for (size_t i = 0; i < key.size(); ++i)
//		{
//			hash += key[i];
//		}
//
//		return hash;
//	}
//};

template<>
struct __HashFunc<string>
{
	static size_t BKDRHash(const char * str)
	{
		unsigned int seed = 131; // 31 131 1313 13131 131313
		unsigned int hash = 0;
		while (*str )
		{
			hash = hash * seed + (*str++);
		}
		
		cout<<hash<<endl;
		return  hash;
	}

	size_t RSHash(const char *str)  
	{  
		register size_t hash = 0;  
		size_t magic = 63689;     
		while (size_t ch = (size_t)*str++)  
		{  
			hash = hash * magic + ch;  
			magic *= 378551;  
		}

		cout<<hash<<endl;

		return hash;  
	} 

	size_t operator()(const string& key)
	{
		/*size_t hash = 0;
		for (size_t i = 0; i < key.size(); ++i)
		{
			hash += key[i];
		}

		return hash;*/

		//return BKDRHash(key.c_str());
		return RSHash(key.c_str());
	}
};

template<class K, class V, class HashFunc = __HashFunc<K>>
class HashTable
{
	typedef HashTableNode<K,V> Node;

public:
	HashTable(size_t capacity = 10)
		:_tables(new Node[capacity])
		,_states(new State[capacity])
		,_size(0)
		,_capacity(capacity)
	{
		//memset(_states, EMPTY, sizeof(State)*_capacity);
		for (size_t i = 0 ; i < _capacity; ++i)
		{
			_states[i] = EMPTY;
		}
	}

	bool Insert(const K& key, const V& value)
	{
		size_t index = _HashFunc0(key);
		int i = 1;
		while (_states[index] == EXITS)
		{
			if (_tables[index]._key == key)
			{
				return false;
			}

			// 二次探测
			//index = _HashFunc(key) + i*i;
			index = _HashFunci(index, i++);

			index %= _capacity;
		}

		_tables[index]._key = key;
		_tables[index]._value = value;
		_states[index] = EXITS;
		++_size;

		return true;
	}

	Node* Find(const K& key)
	{
		size_t index = _HashFunc0(key);
		int i = 1;
		while (_states[index] != EMPTY)
		{
			if (_tables[index]._key == key)
			{
				if (_states[index] != DELETE)
				{
					return &(_tables[index]);
				}
				else
				{
					return NULL;
				}
			}
			
			//index = _HashFunc(key) + i*i;
			//index %= _capacity;
			index = _HashFunci(index, i++);
			++i;
		}

		return NULL;
	}

	size_t _HashFunc0(const K& key)
	{
		//return key%_capacity;
		HashFunc hashfunc;
		return hashfunc(key)%_capacity;
	}

	size_t _HashFunci(size_t prevHash, int i)
	{
		return (prevHash+2*i-1)%_capacity;
	}

	void Print()
	{
		for (size_t i = 0; i < _capacity; ++i)
		{
			//printf("[%d:%d],", _states[i], _tables[i]);
			cout<<"["<<_tables[i]._key<<":"<<
				_tables[i]._value<<"]"<<"-";
		}
		cout<<endl;
	}

protected:
	//K* _tables;			// 哈希表
	Node* _tables;
	State* _states;			// 状态表

	size_t _size;
	size_t _capacity;
};

void TestHashtable()
{
	HashTable<int, double> ht;
	ht.Insert(1, 1);
	ht.Insert(11, 1);
	ht.Insert(21, 1);
	ht.Insert(2, 2);

	ht.Print();
}

void TestDict()
{
	//HashTable<string, string, __HashFuncString> dict;
	HashTable<string, string> dict;
	dict.Insert("dict", "字典");
	dict.Insert("hash", "哈希");
	dict.Insert("function", "函数");

	dict.Insert("abcd", "函数");
	dict.Insert("bcda", "函数");

	dict.Print();

	HashTableNode<string, string>* ret = dict.Find("hash");
	if (ret)
	{
		cout<<"hash的中文:"<<ret->_value<<endl;
	}
}


}