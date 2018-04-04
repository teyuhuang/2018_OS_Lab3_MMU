#ifndef PAGER_H
#define PAGER_H
#include <queue>
#include "Essentials.h"
#include "utility.h"
class Pager {
public:
    virtual Frame* determine_victim_frame() = 0;
    virtual void initialize(myRand*);
protected:
    myRand* pager_rand;
};
class Pager_FIFO:public Pager{
public:
    void initialize(myRand*);
    Frame* determine_victim_frame();    //Select a frame to sacrifice 
protected:
    queue<Frame*> page_queue;
};
class Pager_Sec_Chance:public Pager_FIFO{
public:
    Frame* determine_victim_frame();    //Select a frame to sacrifice
};
class Pager_Random:public Pager{
public:
    Frame* determine_victim_frame();    //Select a frame to sacrifice
    void initialize();  
};
class Pager_NRU:public Pager{
public:
    // reset the ref bits every 10th page replacement request before
    // you implement the replacement operation
    Frame* determine_victim_frame();    //Select a frame to sacrifice
    void initialize();  
};
class Pager_Clock: public Pager{
public:
    Frame* determine_victim_frame();    //Select a frame to sacrifice 
protected:
    unsigned int page_idx=0;
};
class Pager_Aging: public Pager{
public:
    Frame* determine_victim_frame();    //Select a frame to sacrifice 
    void initialize(); 
};
#endif /* PAGER_H */
