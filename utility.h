#ifndef UTILITY_H
#define UTILITY_H
#include <string>
#include <list>

using namespace std;

typedef int stime_t;

struct Util{
    static int s2i(string s);
};
struct Discrete_Time_Accummulator{
    list<pair<int,int>> timeline;
    void removeAndMerge(list<pair<int,int>>::iterator it);
    void addTimeSpan(int,int);
    int getTotalTime();
    void printTimeLine();
};
struct myRand{
    bool isFileLoaded = false;
    int MAX_ofs = -1;
    int ofs=0;
    int* randvals;
    int myrandom(int size);
    int open(string filename);  //load a rfile
    myRand();
    myRand(string filename);

};
#endif /* UTILITY_H */
