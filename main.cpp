#include <cmath>
#include <complex>
#include <iostream>
#include <fstream>

const int N = 1024;

double x[N];
std::complex<double> y[N];
std::complex<double> rx[N];

void loadXSquare(double x[], int N)
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

void loadXSinus(double x[], int N)
{
    double step = 2 * M_PI / N;    

    for(int i = 0; i < N; i++) {
        x[i] = sin(30 * i * step) + sin(60 * i * step) + 2 * sin(90 * i * step);
    }
}

void loadXpulse(double x[], int N)
{
     for(int i = 0; i < N; i++) {
        if ( (i > 502) && (i < 522) ) {
            x[i] = 1;
        } else {
            x[i] = 0;
        }
    }
}

void loadXSawTooth(double x[], int N)
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

void dft(double x[], std::complex<double> y[], int N) {
    const std::complex<double> j(0.0, 1.0);
    const std::complex<double> two(2, 0);
    const std::complex<double> complexN(N, 0);
    std::complex<double> complex_k(0, 0);
    std::complex<double> complex_n(0, 0);

    for(int k = 0; k < N; k++) {
        y[k] = 0;
        complex_k = k;
        for(int n = 0; n < N; n++) {
            complex_n = n;
            y[k] = y[k] + x[n] * exp(-j * two * M_PI * complex_k * complex_n / complexN);
        }
    }    
}

void idft(std::complex<double> y[], std::complex<double> x[], int N) {
    const std::complex<double> j(0.0, 1.0);
    const std::complex<double> one(1, 0);
    const std::complex<double> two(2, 0);
    const std::complex<double> complexN(N, 0);
    std::complex<double> complex_k(0, 0);
    std::complex<double> complex_n(0, 0);

    for(int n = 0; n < N; n++) {
        x[n] = 0;
        complex_n = n;
        for(int k = 0; k < N; k++) {
            complex_k = k;
            x[n] = x[n] + (one / complexN) * y[k] * exp(j * two * M_PI * complex_k * complex_n / complexN);
        }
    }    
}

void saveDatFile(double x[], std::complex<double> y[], int N, char *fileName) {
    std::ofstream f(fileName);

    for(int i = 0; i < N; i++) {
        f << i << "  " << x[i] << "  " << abs(y[i]) << "  " << arg(y[i]) << std::endl;
    }
    
    f.close();
    
    // gnuplot: plot "fileName" using 3 with impulses
    // i, x[i], module(y[i]), phase(y[i])
}

void saveDatFile(std::complex<double> x[], std::complex<double> y[], int N, char *fileName) {
    std::ofstream f(fileName);

    for(int i = 0; i < N; i++) {
        f << i << "  " << abs(x[i]) << "  " << arg(x[i]) << "  " << abs(y[i]) << "  " << arg(y[i]) << std::endl;
    }
    
    f.close();
    
    // gnuplot: plot "fileName" using 3 with impulses
    // i, x[i], module(y[i]), phase(y[i])
}

int main(int argc, char** argv)
{
    // loadXSquare(x, N);
    // loadXSinus(x, N);
    loadXpulse(x, N);
    // loadXSawTooth(x), N;
    
    dft(x, y, N);
    saveDatFile(x, y, N, "result.dat");
    
    idft(y, rx, N);
    saveDatFile(y, rx, N, "result2.dat");
    
    std::cout << "Done!" << std::endl;

    return 0;
}
