#include <iostream>
#include <math.h>
#include <iomanip>
#include <cmath>
#include <chrono>
#include <cilk/cilk.h>
#include <cilk/reducer_opadd.h>

using namespace std;
using namespace std::chrono;

// функция интеграла
double f(double x) {
	return (4 / pow((1 + x * x), 2));
}
// вычисление интеграла без дополнительных инструкций
void computeIntegralCilkFor(double eps) {
	double a = -1, b = 1, computedInt = 5.141592653589793;
	double I = eps + 1, I1 = 0;
	high_resolution_clock::time_point start = high_resolution_clock::now();
	for (int N = 2; (N <= 4) || (fabs(I1 - I) > eps); N *= 2)
	{
		
		cilk::reducer_opadd<double> sum2(0);
		cilk::reducer_opadd<double> sum4(0);
		double h, sum = 0.0 ;
		h = (b - a) / (2 * N);
		cilk_for (int i = 1; i <= 2 * N - 1; i += 2)
		{
			 sum4 += f(a + h * i);
			 sum2 += f(a + h * (i + 1));
		}
		sum = f(a) + 4 * sum4.get_value() + 2 * sum2.get_value() - f(b);
		I = I1;
		I1 = (h / 3) * sum;
	}
	high_resolution_clock::time_point finish = high_resolution_clock::now();
	duration<double> duration = (finish - start);
	cout << "Duration is: " << duration.count() << " seconds" << endl;
	cout << "EPS is: " << setprecision(10) << abs(computedInt - I1) << endl;
}


int main() {
	double eps = 0.01;
	double eps1 = 0.001;
	double eps2 = 0.0001;
	double eps3 = 0.00001;
	double eps4 = 0.000001;
	double eps5 = 0.0000001;
	cout << "-----------\ncilk for\n-----------" << endl;
	computeIntegralCilkFor(eps);
	computeIntegralCilkFor(eps1);
	computeIntegralCilkFor(eps2);
	computeIntegralCilkFor(eps3);
	computeIntegralCilkFor(eps4);
	computeIntegralCilkFor(eps5);

	
}