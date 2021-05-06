#include "std_lib_facilities.h"

array<int, 10> array1{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
vector<int> vector1 = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
list<int> list1 = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

template<typename type>
void increase(type& TYPE, int n)
{
	for(auto& sum : TYPE)
		sum += n;
}

template<typename Iter1, typename Iter2>
Iter2 mycopy(Iter1 f1, Iter1 e1, Iter2 f2)
{
	while (f1 != e1)
	{
		*f2 = *f1;
		++f1;
		++f2;
	}
	return f2;
}

int main()
{
	array<int, 10> array2;
	for (int i = 0; i < 10; ++i)
	{
		array2[i] = array1[i];
	}
	vector<int> vector2 = vector1;
	list<int> list2 = list1;

	increase(array1, 2);
	increase(vector1, 3);
	increase(list1, 5);

	mycopy(array2.begin(),array2.end(),vector2.begin());
	mycopy(list2.begin(),list2.end(),array2.begin());

	vector<int>::iterator vectorIter;
	vectorIter = find(vector1.begin(), vector1.end(), 3);
	if (vectorIter != vector1.end())
	{
		cout << "Itt van: " << distance(vector1.begin(), vectorIter) << endl;
	}
	else
	{
		cout << "Nem talaltuk meg:" << endl;
	}

	list<int>::iterator listIter;
	listIter = find(list1.begin(), list1.end(), 27);
	if (listIter != list1.end())
	{
		cout << "Itt van: " << distance(list1.begin(), listIter) << endl;
	}
	else
	{
		cout << "Nem talaltuk meg" << endl;
	}

	return 0;
}	
