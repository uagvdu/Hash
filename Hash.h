#include<iostream>
#include<vector>
using namespace std;
//�����������ϣ��ͻ
//step 1�� ��һ��˳�����ÿһ���������������� �õ���ֵ����������ֵ��ͬ�����ӵ�˳����Ӧ��λ�����棬���������ȥ�õ����ǹ���һ��������˳���


template<class K>       //key        int
struct StringNum
{
	size_t operator()(const K& key)
	{
		return key;
	}
};

template<>
struct StringNum<string>         //key             string 
{
	static size_t BKDRHash(const char *str)
	{
		unsigned int seed = 131;// 31 131 1313 13131 131313
		unsigned int hash = 0;
		while (*str)
		{
			hash = hash * seed + (*str++);
		}
		return(hash & 0x7FFFFFFF);
	}
	size_t operator ()(const string& str)
	{
		return BKDRHash(str.c_str());
	}
};



template<class K,class V>
struct HashNode
{
	K _key;
	V _value;
	HashNode<K, V>* _next;
	HashNode(K key = 0,V value = 0)
		:_key(key)
		, _value(value)
		, _next(NULL)
	{}
};

template<class K, class V, class HashString = StringNum<K>>
class Hash
{
	typedef HashNode<K, V> Node;
public:
	Hash()
		:_sz(0)
	{ }

	~Hash()
	{
		for (size_t i = 0; i < _v.size(); i++)
		{
			if (_v[i] != NULL)
			{
				Node* cur = _v[i];
				while (cur)
				{
					Node *del = cur;
				    cur = cur->_next;
					delete del;
				}
				_v[i] = NULL;
			}
			
		}
	}

	bool Find(const K& key)
	{
		/*for (size_t i = 0; i < _v.size(); i++)
		{
			if (_v[i] != NULL)
			{
				Node* cur = _v[i];
				while (cur)
				{
					if (cur->_key == key)
						return true;
					cur = cur->_next;
				}
			}
				 
		}*/
		int index = _Key(key);
		Node* cur = _v[index];
		while (cur)
		{
			if (cur->_key == key)
				return true;
			cur = cur->_next;
		}
		return false;
	}
	void Insert(const K& key,const V& value= 0)
	{
		if (_v.empty())
		{
			_v.resize(10);
		}

		if (Find(key)) //���û������������жϲ��Ҳ��޸�find�������ͻ���ɲ��۲������Ԫ�أ�˳���Ĵ�С��Ϊ�㣬ֵʼ��Ϊ��
		{
			return;
		}

		_CheckCapacity();  //����Ϊ0�������� 
		size_t index = _Key(key);

		Node *old = _v[index];
		Node* cur = new Node(key);
		_v[index] = cur;
		cur->_next = old;
		_sz++;

	}


protected:
	size_t _Key(const K& key)
	{
		return (HashString()(key) % _v.size());
	}

	void _CheckCapacity()
	{
		const int _PrimeSize = 28;
		static const unsigned long _PrimeList[_PrimeSize] =
		{
			53ul, 97ul, 193ul, 389ul, 769ul,
			1543ul, 3079ul, 6151ul, 12289ul, 24593ul,
			49157ul, 98317ul, 196613ul, 393241ul,
			786433ul,
			1572869ul, 3145739ul, 6291469ul, 12582917ul,
			25165843ul,
			50331653ul, 100663319ul, 201326611ul, 402653189ul,
			805306457ul,
			1610612741ul, 3221225473ul, 4294967291ul
		};

		if (_sz == _v.size())//�������أ��Լ�ƽ������
		{
			vector<Node*> tmp;
			size_t index = 0;
			Node* cur = NULL;
			Node *old = NULL;
			//tmp.resize(_sz);
			for (int i = 0; i < _PrimeSize;i++)
			{
				if (_PrimeList[i]>_sz)
				{
					_v.resize(_PrimeList[i]);
					tmp.resize(_PrimeList[i]);
					break;
				}
			}

			for (int i = 0; i < _v.size(); i++)
			{
				cur = _v[i];
				while (cur)
				{
					index = _Key(cur->_key);
					old = tmp[index];

					tmp[index] = cur; //�˴����ܵõ�˳���ÿһ��Ԫ�أ���ȷ�����ݺ���ЩԪ�ص�λ�ã����һ���Ҫ���ǵ�������֮����ЩԪ�ص��ӽڵ�Ӧ�÷ŵ�λ��
					cur = cur->_next;
				
					tmp[index]->_next = old;
				}
			}
			swap(tmp,_v);
		}

	}
protected:
	vector<Node*>  _v;//��ʼ��ΪNULL
	size_t _sz;
};

void HashTest()
{
	Hash<int, int> hs;
	Hash<string, int> hs2;
	int i = 1;
	//while (i++)
	//{ 
	//	hs.Insert(i);
	//	if (i == 78)
	//		break;
	//}
	//hs.Insert(151);
	//hs.Insert(1);
	int array[] = { 1, 2, 5, 8, 9 ,25,99,48,77,6,35,84};
	for (int i = 0; i < sizeof(array) / sizeof(array[0]); i++)
		hs.Insert(array[i]);
	hs.Insert(11);
	hs.Insert(1);
	hs.Insert(58);
	cout << hs.Find(1) << endl;
	char* array2[] = {"hello","www","sky","ddd","www"};
	for (int i = 0; i < sizeof(array2) / sizeof(array2[0]); i++)
		hs2.Insert(array2[i]);

}