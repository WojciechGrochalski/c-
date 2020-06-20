

#include <thread>
#include <ctime>
#include <stdio.h>
#include <cassert>
#include <iostream>
#include <chrono>
#include <string>
#include <unistd.h>

//#include "../CUDA_Lib/common/book.h"
#include <atomic>

#include <mutex>          // std::mutex

std::mutex mtx;
using namespace std::chrono;


using namespace std;
typedef std::chrono::system_clock Clock;

#define imin(a,b) (a<b?a:b)
#define N  (200 * 1024)
void add3(  int* a,  int* b,  int* c) {
    clock_t startCP = clock();
    
    int tid = 0; //To jest CPU zero, a więc zaczynamy od zera
        while (tid < N) {
            c[tid] = a[tid] + b[tid];
            tid += 1; // Mamy tylko jeden CPU, a więc zwiększamy o jeden
        }
        tid = 0; //To jest CPU zero, a więc zaczynamy od zera
        while (tid < N) {
            c[tid] = a[tid] + b[tid];
            tid += 1; // Mamy tylko jeden CPU, a więc zwiększamy o jeden
        }

        printf("Czas wykonywania CPU : %lu ms\n", clock() - startCP);
   
}
void add3( int* a,  int* b,   int* c,string parallel) {


 
    int tid = 0; //To jest CPU zero, a więc zaczynamy od zera
    #pragma omp parallel private(tid)
    {
        while (tid < N) {
            c[tid] = a[tid] + b[tid];
            tid += 1; // Mamy tylko jeden CPU, a więc zwiększamy o jeden
        }
    }
 


}

void addThread(int* a, int* b, int* c,int num ) {
    mtx.lock();
    clock_t oo= clock();
    mtx.unlock();
    
    int tid = num; //To jest CPU zero, a więc zaczynamy od zera
        while (tid < N) {
            c[tid] = a[tid] + b[tid];
            tid += 4; // Mamy tylko jeden CPU, a więc zwiększamy o jeden
        }
        mtx.lock();
        
        printf("Czas wykonywania CPU 4 thread %d: %lu ms\n",num, clock() - oo);
        mtx.unlock();
}
void addThread2(int* a, int* b, int* c,int num ) {
    mtx.lock();
    clock_t o= clock();
    mtx.unlock();
    
    int tid = num; //To jest CPU zero, a więc zaczynamy od zera
        while (tid < N) {
            c[tid] = a[tid] + b[tid];
            tid += 4; // Mamy tylko jeden CPU, a więc zwiększamy o jeden
        }
        mtx.lock();
        
        printf("Czas wykonywania CPU 4 thread %d: %lu ms\n",num, clock() - o);
        mtx.unlock();
}
void addThread3(int* a, int* b, int* c,int num ) {
    mtx.lock();
    clock_t o= clock();
    mtx.unlock();
    
    int tid = num; //To jest CPU zero, a więc zaczynamy od zera
        while (tid < N) {
            c[tid] = a[tid] + b[tid];
            tid += 4; // Mamy tylko jeden CPU, a więc zwiększamy o jeden
        }
        mtx.lock();
        
        printf("Czas wykonywania CPU 4 thread %d: %lu ms\n",num, clock() - o);
        mtx.unlock();
}


void addThread4(int* a, int* b, int* c,int num ) {
    mtx.lock();
    clock_t o= clock();
    mtx.unlock();
    
    int tid = num; //To jest CPU zero, a więc zaczynamy od zera
        while (tid < N) {
            c[tid] = a[tid] + b[tid];
            tid += 4; // Mamy tylko jeden CPU, a więc zwiększamy o jeden
        }
        mtx.lock();
        
        printf("Czas wykonywania CPU 4 thread %d: %lu ms\n",num, clock() - o);
        mtx.unlock();
}
void t(int* a1, int* b1, int* c1){
    thread t1(addThread, a1, b1, c1, 0);
     thread t2(addThread2, a1, b1, c1, 1);
     thread t3(addThread3, a1, b1, c1, 2);
     thread t4(addThread4, a1, b1, c1, 3);
     t1.detach();
     t2.detach();
     t3.detach();
     t4.detach();

}

int main(void) {

    //unsigned char* buffer =      (unsigned char*)big_random_block(N);
    string ss = std::to_string(N);
    
    
    cout << "Size of Vector: " << ss << endl;
    
    int* a, * b, * c;
   //  int* dev_a, * dev_b, * dev_c;

    // allocate the memory on the CPU
    a = (int*)malloc(N * sizeof( int));
    b = ( int*)malloc(N * sizeof( int));
    c = (int*)malloc(N * sizeof( int));

    int* a1,* b1,* c1;
     int* a2,* b2,* c2;
     a1 = (int*)malloc(N * sizeof(int));
     b1 = (int*)malloc(N * sizeof(int));
     c1 = (int*)malloc(N * sizeof(int));
     a2 = (int*)malloc(N * sizeof(int));
     b2 = (int*)malloc(N * sizeof(int));
     c2 = (int*)malloc(N * sizeof(int));

    // Alokacja pamięci na GPU

  
    
    

    for (int i = 0; i < N; i++) {
        a2[i] = -i;
        b2[i] = i + i;
    }
    clock_t startC = clock();
   add3(a2, b2, c2);
   // usleep(20000);
    printf("Czas wykonywania  CPU: %lu ms\n", clock() - startC);

    
      clock_t startCp = clock();
     add3(a2, b2, c2,"ad");
     // usleep(20000);
      printf("Czas wykonywania  CPU:P %lu ms\n", clock() - startCp);
    for (int i = 0; i < N; i++) {
        a1[i] = -i;
        b1[i] = i + i;
    }

    //add3(a1, b1, c1, "parallel");

    int n = thread::hardware_concurrency();
        cout << "Ilosc dostepnych watkow na PC: " << n << endl;
    thread t1 (t,a1,b1,c1);
    usleep(20000);
   // Sleep(1500);
 //   t1.join();
  //  t2.join();
   // t3.join();
  //  t4.join();
    
    // Wyświetlenie wyników
    for (int i = 0; i < N; i++) {
        if (a[i] != a1[i]) {
            cout << "Bad result" << endl;
            cout << "tab a=" << a[i] << " tab2= " << a1[i] << endl;
            break;
        }
    }
    // Zwolnienie pamięci alokowanej na GPU


    // free the memory we allocated on the CPU
    free(a);
    free(b);
    free(c);
    free(a1);
    free(b1);
    free(c1);
    free(a2);
    free(b2);
    free(c2);
    
    t1.join();
   // system("Pause");
    return 0;
}








