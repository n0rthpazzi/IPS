#include "fragmentation.h"
#include <cilk/cilk_api.h>


int main()
{
	__cilkrts_end_cilk(); __cilkrts_set_param("nworkers", "4");
	high_level_analysis main_object; // создание объекта класса

	high_resolution_clock::time_point start = high_resolution_clock::now();
	main_object.GetSolution(); // получение решения
	high_resolution_clock::time_point finish = high_resolution_clock::now();
	duration<double> duration = (finish - start);


	cout << "Precision: " << g_precision << endl; 
	cout << "Duration: " << duration.count() << " seconds" << endl; 
	cout << "Number of workers " << __cilkrts_get_nworkers() << endl;
	
	const char* out_files[3] = { "solution.txt", "boundary.txt", "not_solution.txt" };
	WriteResults(out_files); // запись результата в файлы

	return 0;
}