#include <iostream>
#include <math.h>
#include <iomanip>
#include <cmath>
#include <chrono>
#include "tbb/task_scheduler_init.h"
#include "tbb/parallel_for.h"
#include "tbb/blocked_range.h"
#include <mutex>
#include <cilk/reducer_opadd.h>
using namespace std;
using namespace std::chrono;
using namespace tbb;

double f(double x) {
	return (4 / pow((1 + x * x), 2));
}
void simpsonIntegral(double eps) {
	std::mutex gmutex;
	double n = 1.0 / eps;
	int a = -1, b = 1;
	const double width = (b - a) / n;
	double computedInt = 5.141592653589793;
	cilk::reducer_opadd<double> simpson_integral(0);
	high_resolution_clock::time_point start = high_resolution_clock::now();
	parallel_for(blocked_range<size_t>(1, n), [&](const blocked_range<size_t>& r) {
		for (int step = r.begin(); step < r.end(); step++) {
			const double x1 = a + step * width;
			const double x2 = a + (step + 1)*width;
			gmutex.lock();
			simpson_integral += (x2 - x1) / 6.0*(f(x1) + 4.0*f(0.5*(x1 + x2)) + f(x2));
			gmutex.unlock();
		}
	});
	high_resolution_clock::time_point finish = high_resolution_clock::now();
	duration<double> duration = (finish - start);
	cout << "Duration is: " << duration.count() << " seconds" << endl;
	cout << "EPS is: " << setprecision(10) << abs(computedInt - simpson_integral.get_value()) << endl;
}
int main() {
	task_scheduler_init init;

	double eps = 0.01;
	double eps1 = 0.001;
	double eps2 = 0.0001;
	double eps3 = 0.00001;
	double eps4 = 0.000001;
	double eps5 = 0.0000001;
	cout << "-----------\nTBB\n-----------" << endl;
	simpsonIntegral(eps);
	simpsonIntegral(eps1);
	simpsonIntegral(eps2);
	simpsonIntegral(eps3);
	simpsonIntegral(eps4);
	simpsonIntegral(eps5);

}