
#include <iostream>
#include <sys/resource.h>
#include<stdexcept>
#include<new>

using namespace std;

void dataOut(){
    struct rusage CPUusage;
    struct rlimit CPUMax;
    struct rlimit Heap;
    struct rlimit Stack;

    if(getrusage(0,&CPUusage) == 0){
        cout<< "Programm_CPU_Usage = " << CPUusage.ru_utime.tv_sec + CPUusage.ru_utime.tv_usec / 1e6 <<"s" << endl;
        cout << "System_CPU_Usage = " << CPUusage.ru_stime.tv_sec + CPUusage.ru_stime.tv_usec / 1e6 <<"s" << endl;
    }
    else{
        cerr << "Error: Can not scan CPU parameters" << endl;
    }
    if(getrlimit(0, &CPUMax) == 0){
        cout << "CPU_Current_Usage " << CPUMax.rlim_cur <<" s" << endl;
        cout << "CPU_Max_Usage " << CPUMax.rlim_max << "s" << endl;
    }
    else{
        cerr << "Error: Can not scan CPU parameters" << endl;
    }
    /*if(getrlimit(2,&Heap) == 0){
        cout << "Max heap memory " << (((Heap.rlim_cur/1024)/1024)/1024) << "GB" << endl;
    }
    else{
        cerr << "Error: Can not scan CPU parameters" << endl;
    }
    if(getrlimit(3,&Stack) == 0){

        cout << "Max stack memory " << (Stack.rlim_cur/1024)/1024 << "MB" << endl;
    }
    else{
        cerr << "Error: Can not scan CPU parameters" << endl;
    }
    */



}

void funcMem(int allocsize,int scan){

    int *newArray = new int[allocsize];
    struct rusage CPUValue;

        for(int i=0; i<allocsize; i++){
            newArray[i] = i;

            if(i%scan == 0){
                getrusage(RUSAGE_SELF,&CPUValue);
                cout << "Array : " << i << endl;
                cout <<"UserCPUTime : " << CPUValue.ru_utime.tv_sec << "."  << CPUValue.ru_utime.tv_usec <<  endl;
                cout <<"SystemCPUTime : " << CPUValue.ru_stime.tv_sec << "." << CPUValue.ru_stime.tv_usec << endl;
            }
        }


    delete[] newArray;
    }


    void funcRec(int recAmount, int scan,int count, int* stackBegin){

        int Startadress ;

        if(count == 1)
            stackBegin = &Startadress;

        if(count % scan == 0){
            ptrdiff_t Stack = abs(&Startadress - stackBegin) * sizeof(Startadress);
            cout << "Rec " << count << " : " << Stack << " Bytes" << endl;
        }
        if(recAmount >= count)
            funcRec(recAmount,scan,count +1, stackBegin);



    }

int main(int argc, char *argv[])
{

    //funcMem(stoi(argv[1]),stoi(argv[2]));
    funcRec(stoi(argv[3]),stoi(argv[4]),1,nullptr);

}
