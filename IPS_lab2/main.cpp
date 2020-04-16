#include <cilk/cilk.h>
#include <cilk/cilk_api.h>
#include <cilk/reducer_max.h>
#include <cilk/reducer_min.h>
#include <cilk/reducer_vector.h>
#include <chrono>
#include <iostream>

using namespace std::chrono;
using namespace std;

/// Функция ReducerMaxTest() определяет максимальный элемент массива,
/// переданного ей в качестве аргумента, и его позицию
/// mass_pointer - указатель исходный массив целых чисел
/// size - количество элементов в массиве
void ReducerMaxTest(int *mass_pointer, const long size)
{
	cilk::reducer<cilk::op_max_index<long, int>> maximum;
	cilk_for(long i = 0; i < size; ++i)
	{
		maximum->calc_max(i, mass_pointer[i]);
	}
	printf("Maximal element = %d has index = %d\n\n",
		maximum->get_reference(), maximum->get_index_reference());
}
/// Функция ReducerMinTest() определяет минимальный элемент массива,
/// переданного ей в качестве аргумента, и его позицию
/// mass_pointer - указатель исходный массив целых чисел
/// size - количество элементов в массиве
void ReducerMinTest(int *mass_pointer, const long size)
{
	cilk::reducer<cilk::op_min_index<long, int>> minimum;
	cilk_for(long i = 0; i < size; ++i)
	{
		minimum->calc_min(i, mass_pointer[i]);
	}
	printf("Minimal element = %d has index = %d\n\n",
		minimum->get_reference(), minimum->get_index_reference());
}

/// Функция ParallelSort() сортирует массив в порядке возрастания
/// begin - указатель на первый элемент исходного массива
/// end - указатель на последний элемент исходного массива
duration<double> ParallelSort(int *begin, int *end)
{
	high_resolution_clock::time_point start = high_resolution_clock::now();
	if (begin != end)
	{
		--end;
		int *middle = std::partition(begin, end, std::bind2nd(std::less<int>(), *end));
		std::swap(*end, *middle);
		cilk_spawn ParallelSort(begin, middle);
		ParallelSort(++middle, ++end);
		cilk_sync;
	}
	high_resolution_clock::time_point finish = high_resolution_clock::now();
	duration<double> duration = (finish - start);
	return duration;
}
/// Функция CompareForAndCilk_For() сравнивает for и cilk_for
/// sz - размер массива данных
void CompareForAndCilk_For(size_t sz)
{
	vector<int> vec;
	cilk::reducer<cilk::op_vector<int>>red_vec;
	high_resolution_clock::time_point sFor = high_resolution_clock::now();
	for (int i = 0; i < sz; i++)
		vec.push_back(rand() % 20000 + 1);
	high_resolution_clock::time_point fFor = high_resolution_clock::now();
	cilk_for(int i = 0; i < sz; i++)
		red_vec->push_back(rand() % 20000 + 1);
	high_resolution_clock::time_point fCilkFor = high_resolution_clock::now();
	duration<double> duration_for = (fFor - sFor);
	duration<double> duration_cilk = (fCilkFor - fFor);
	cout << "------------------------" << endl;
	cout << "Vector size is: " << sz << endl;
	cout << "Duration for is: " << duration_for.count() << " seconds" << endl;
	cout << "Duration cilk_for is: " << duration_cilk.count() << " seconds" << endl;
}

int main()
{
	srand((unsigned)time(0));
	// устанавливаем количество работающих потоков = 4
	__cilkrts_set_param("nworkers", "4");

	long i;
	const long mass_size = 10000 * 100;
	int *mass_begin, *mass_end;
	int *mass = new int[mass_size]; 

	for(i = 0; i < mass_size; ++i)
	{
		mass[i] = (rand() % 25000) + 1;
	}
	
	mass_begin = mass;
	mass_end = mass_begin + mass_size;
	cout << "Massive size: " << mass_size << endl << endl;
	ReducerMaxTest(mass, mass_size);
	ReducerMinTest(mass, mass_size);
	duration<double> duration = ParallelSort(mass_begin, mass_end);
	cout << "Duration is: " << duration.count() << " seconds" << endl << endl;
	ReducerMaxTest(mass, mass_size);
	ReducerMinTest(mass, mass_size);
	delete[]mass;

	CompareForAndCilk_For(10);
	CompareForAndCilk_For(50);
	CompareForAndCilk_For(100);
	CompareForAndCilk_For(500);
	CompareForAndCilk_For(1000);
	CompareForAndCilk_For(10000);
	CompareForAndCilk_For(100000);
	CompareForAndCilk_For(1000000);
	return 0;
}