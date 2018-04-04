#include "Pager.h"
#include "Essentials.h"
#include <iostream>
#include <list>

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
    Frame* f = nullptr;
    sort_frame_into_4_classes(++replacement_counter>=10);
    if(replacement_counter>=10) replacement_counter = 0;

    for(int i=0;i<4;i++){
        if(q_class[i].size()>0){
            int idx = pager_rand->myrandom(q_class[i].size());
            list<Frame*>::iterator it = q_class[i].begin();
            std::advance(it,idx);
            f = *it;
            break;
        }
    }
    return f;
}
void Pager_NRU::sort_frame_into_4_classes(bool reset_ref){
    for(int i=0;i<4;i++){
        q_class[i].clear();
    }
    // start sorting
    bool ref = false, mod = false;
    Frame* frame;
    for(int i=0;i<frame_table::num_of_frame;i++){
        frame = frame_table::inverse_mapping[i];
        ref = frame->pte->REFERENCED;
        mod = frame->pte->MODIFIED;
        if(!ref&&!mod){     //0 , 0 first class
            q_class[0].push_back(frame);
        }
        else if(!ref&&mod){     //0 , 1 second class
            q_class[1].push_back(frame);
        }
        else if(ref&&!mod){      //1, 0 3rd class
            q_class[2].push_back(frame);
        }
        else{                   // 0, 0 4th
            q_class[3].push_back(frame);
        }
        //reset REFERENCED bit
        if(reset_ref){
            ref = false;
            frame->pte->REFERENCED = 0;
        }
    }
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
        if(!f->pte->REFERENCED)
            found = true;
        f->pte->REFERENCED = 0;
    }
    return f;
}

/*
*   Pager_Aging
*/
Frame* Pager_Aging::determine_victim_frame(){
    Frame* f;
    unsigned int smallest = 0xffffffff;
    for(int i=0;i<frame_table::num_of_frame;i++){
        age_vector[i]>>=1;
        age_vector[i]&=0x7fffffff;
        if(frame_table::inverse_mapping[i]->pte->REFERENCED){
            age_vector[i]|=0x80000000;
            frame_table::inverse_mapping[i]->pte->REFERENCED = 0;
        }
        if(age_vector[i]<smallest){
            f=frame_table::inverse_mapping[i];
            smallest = age_vector[i];
        }
    }
    return f;
}   
void Pager_Aging::initialize(myRand* r){
    pager_rand = r;
    for(int i=0;i<frame_table::num_of_frame;i++){
        age_vector[i]=0;
    }
}