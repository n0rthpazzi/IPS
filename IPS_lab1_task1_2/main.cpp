#include <iostream>
#include <math.h>
#include <iomanip>
#include <cmath>
#include <chrono>
#include <thread>
#include <mutex>

using namespace std;
using namespace std::chrono;

// функци€ интеграла
double f(double x) {
	return (4 / pow((1 + x * x), 2));
}
// вычисление интеграла без дополнительных инструкций
void computeIntegral(double eps) {
	double a = -1, b = 1, computedInt = 5.141592653589793;
	double I = eps + 1, I1 = 0;
	high_resolution_clock::time_point start = high_resolution_clock::now();
	for (int N = 2; (N <= 4) || (fabs(I1 - I) > eps); N *= 2)
	{
		double h, sum2 = 0, sum4 = 0, sum = 0;
		h = (b - a) / (2 * N);
		for (int i = 1; i <= 2 * N - 1; i += 2)
		{
			sum4 += f(a + h * i);
			sum2 += f(a + h * (i + 1));
		}
		sum = f(a) + 4 * sum4 + 2 * sum2 - f(b);
		I = I1;
		I1 = (h / 3) * sum;
	}
	high_resolution_clock::time_point finish = high_resolution_clock::now();
	duration<double> duration = (finish - start);
	cout << "Duration is: " << duration.count() << " seconds" << endl;
	cout << "EPS is: " << setprecision(10) << abs(computedInt - I1) << endl;
}
// вычисление интеграла с распараллеливанием на 8 потоков с помощью pramga
void computeIntegralParallel(double eps) {
	double a = -1, b = 1, computedInt = 5.141592653589793;
	double I = eps + 1, I1 = 0;
	high_resolution_clock::time_point start = high_resolution_clock::now();
	#pragma loop(hint_parallel(8))
	for (int N = 2; (N <= 4) || (fabs(I1 - I) > eps); N *= 2)
	{
		double h, sum2 = 0, sum4 = 0, sum = 0;
		h = (b - a) / (2 * N);
		for (int i = 1; i <= 2 * N - 1; i += 2)
		{
			sum4 += f(a + h * i);
			sum2 += f(a + h * (i + 1));
		}
		sum = f(a) + 4 * sum4 + 2 * sum2 - f(b);
		I = I1;
		I1 = (h / 3) * sum;
	}
	high_resolution_clock::time_point finish = high_resolution_clock::now();
	duration<double> duration = (finish - start);
	cout << "Duration is: " << duration.count() << " seconds" << endl;
	cout << "EPS is: " << setprecision(10) << abs(computedInt - I1) << endl;
}

// глобальные вспомогательные переменные
mutex MUTEX_VAR;
double sum_all = 0.0;
//вспомогательна€ функци€ дл€ ручного создани€ потоков
void INT(double a, double b, double I, double I1, double eps) {
	for (int N = 2; (N <= 4) || (fabs(I1 - I) > eps); N *= 2)
	{
		double h, sum2 = 0, sum4 = 0, sum = 0;
		h = (b - a) / (2 * N);

		for (int i = 1; i <= 2 * N - 1; i += 2)
		{

			sum4 += f(a + h * i);
			sum2 += f(a + h * (i + 1));

		}
		sum = f(a) + 4 * sum4 + 2 * sum2 - f(b);
		I = I1;
		I1 = (h / 3) * sum;

	}
	MUTEX_VAR.lock();
	sum_all += I1;
	MUTEX_VAR.unlock();
}
// создание тредов вручную
void computeIntegralThreads(double eps) {
	double computedInt = 5.141592653589793;
	double I = eps + 1, I1 = 0;

	high_resolution_clock::time_point start = high_resolution_clock::now();
	thread t1(INT, -1, -0.75, I, I1, eps);
	thread t2(INT, -0.75, -0.5, I, I1, eps);
	thread t3(INT, -0.5, -0.25, I, I1, eps);
	thread t4(INT, -0.25, 0, I, I1, eps);
	thread t5(INT, 0, +0.25, I, I1, eps);
	thread t6(INT, 0.25, 0.5, I, I1, eps);
	thread t7(INT, 0.5, 0.75, I, I1, eps);
	thread t8(INT, 0.75, 1, I, I1, eps);
	t1.join();
	t2.join();
	t3.join();
	t4.join();
	t5.join();
	t6.join();
	t7.join();
	t8.join();

	high_resolution_clock::time_point finish = high_resolution_clock::now();
	duration<double> duration = (finish - start);
	cout << "Duration is: " << duration.count() << " seconds" << endl;
	cout << "EPS is: " << setprecision(10) << abs(computedInt - sum_all) << endl;
	sum_all = 0;
}



int main() {
	double eps = 0.01;
	double eps1 = 0.001;
	double eps2 = 0.0001;
	double eps3 = 0.00001;
	double eps4 = 0.000001;
	double eps5 = 0.0000001;
	cout << "-----------\nNo Parallel\n-----------" << endl;
	computeIntegral(eps);
	computeIntegral(eps1);
	computeIntegral(eps2);
	computeIntegral(eps3);
	computeIntegral(eps4);
	computeIntegral(eps5);
	cout << "-----------\nWith Parallel\n-----------" << endl;
	computeIntegralParallel(eps);
	computeIntegralParallel(eps1);
	computeIntegralParallel(eps2);
	computeIntegralParallel(eps3);
	computeIntegralParallel(eps4);
	computeIntegralParallel(eps5);
	cout << "-----------\nWith threads\n-----------" << endl;
	computeIntegralThreads(eps);
	computeIntegralThreads(eps1);
	computeIntegralThreads(eps2);
	computeIntegralThreads(eps3);
	computeIntegralThreads(eps4);
	computeIntegralThreads(eps5);
}