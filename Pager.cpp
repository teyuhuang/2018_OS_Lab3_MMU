#include "Pager.h"
#include "Essentials.h"
#include <iostream>

using namespace std;
/*
*   Pager base
*/
void Pager::initialize(myRand* r){
    pager_rand = r;
}
/*
*   Pager_FIFO
*/
Frame* Pager_FIFO::determine_victim_frame(){
    Frame* f= page_queue.front();
    page_queue.pop();
    page_queue.push(f);
    return f;
}
void Pager_FIFO::initialize(myRand*){
    for(int i=0;i<frame_table::num_of_frame;i++){
        page_queue.push(frame_table::inverse_mapping[i]);
    }
}
/*
*   Pager_Sec_Chance
*/
Frame* Pager_Sec_Chance::determine_victim_frame(){
    bool found = false;
    Frame* f;
    while(!found){
        f= page_queue.front();
        page_queue.pop();
        page_queue.push(f);
        if(f->pte->REFERENCED)
            f->pte->REFERENCED = 0;
        else
            found = true;
    }
    return f;
}
/*
*   Pager_Random
*/

Frame* Pager_Random::determine_victim_frame(){
    int idx = pager_rand->myrandom(frame_table::num_of_frame);
    if(idx>=frame_table::num_of_frame){
        cout<<idx<<" "<<frame_table::num_of_frame<<endl;
        exit(0);
    }

    return frame_table::inverse_mapping[idx];
}

/*
*   Pager_NRU
*/

Frame* Pager_NRU::determine_victim_frame(){
    //we shall reset the ref bits every 10th page replacement request before
    //you implement the replacement operation
    
    return new Frame(5);
}
void Pager_NRU::initialize(){

}

/*
*   Pager_Clock
*/
Frame* Pager_Clock::determine_victim_frame(){
    bool found = false;
    Frame* f;
    while(!found){
        f= frame_table::inverse_mapping[page_idx++];
        if (page_idx>=frame_table::num_of_frame) page_idx = 0;
        if(f->pte->REFERENCED)
            f->pte->REFERENCED = 0;
        else
            found = true;
    }
    return f;
}

/*
*   Pager_Aging
*/
Frame* Pager_Aging::determine_victim_frame(){
    return new Frame(5);
}   
void Pager_Aging::initialize(){

}