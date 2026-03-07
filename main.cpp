#include <cmath>
#include <complex>
#include <iostream>
#include <fstream>

const int N = 1024;
const std::complex<double> j(0.0, 1.0);
const std::complex<double> dos(2, 0);
const std::complex<double> complexN(N, 0);

double x[N];
std::complex<double> y[N];

void loadXSquare(double x[])
{
    double xx = 0;
    const int HALF_T = 128;

    for(int i = 0; i < N; i++) {
        if(((i % HALF_T) == 0) && (xx == 0)) {
            xx = 1;
        } else if(((i % HALF_T) == 0) && (xx == 1)) {
            xx = 0;
        }
        x[i] = xx;
    }
}

void loadXSinus(double x[])
{
    double step = 2 * M_PI / N;    

    for(int i = 0; i < N; i++) {
        x[i] = sin(30 * i * step) + sin(60 * i * step) + 2 * sin(90 * i * step);
    }
}

void loadXpulse(double x[])
{
     for(int i = 0; i < N; i++) {
        if ( (i > 502) && (i < 522) ) {
            x[i] = 1;
        } else {
            x[i] = 0;
        }
    }
}

void loadXSawTooth(double x[])
{
    int p1 = N / 4;
    int p2 = 3 * N / 4;
    
    int k = 0;
    for(int i = 0; i < p1; i++) {
        x[i] = k;
        k++;
    }

    for(int i = p1; i < p2; i++) {
        x[i] = k;
        k--;
    }
    
    for(int i = p2; i < N; i++) {
        x[i] = k;
        k++;
    }
}

int main(int argc, char** argv)
{
    // loadXSquare(x);
    // loadXSinus(x);
    loadXpulse(x);
    // loadXSawTooth(x);

    std::complex<double> complex_k(0, 0);
    std::complex<double> complex_n(0, 0);

    for(int k = 0; k < N; k++) {
        y[k] = 0;
        complex_k = k;
        for(int n = 0; n < N; n++) {
            complex_n = n;
            y[k] = y[k] + x[n] * exp(-j * dos * M_PI * complex_k * complex_n / complexN);
        }
    }

    std::ofstream f("dades.txt");

    for(int i = 0; i < N; i++) {
        f << i << "  " << x[i] << "  " << abs(y[i]) << "  " << arg(y[i]) << std::endl;
    }
    
    f.close();
    
    // gnuplot: plot "dades.txt" using 3 with impulses
    // i, x[i], module(y[i]), phase(y[i])
    
    std::cout << "Done!" << std::endl;

    return 0;
}
