#include "Simulator.h"
#include "Essentials.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "utility.h"

using namespace std;

Simulator::Simulator(string input_f, string rnd_f, Pager* pgr, bool *dop){
    myRand* rand = new myRand;
    if(rand->open(rnd_f)){
        return;
    }
    this->THE_PAGER = pgr;
    THE_PAGER->initialize(rand);
    this->display_options = dop;
    //Start Loading process into scheduler
    ifstream inFile;
    string line;
    Process* proc;
    int process_counter = 0;
    bool read_vmas = true;
    try {
        inFile.open(input_f);
        if(inFile.is_open()){
            while(getline(inFile, line)){
                if(line.empty()||line[0]=='#'){
                    continue;
                }
                if(num_of_process<0){
                    //read the number of process
                    num_of_process = Util::s2i(line);
                }
                else if(process_counter<num_of_process){
                    if(read_vmas){
                        int n_vmas = Util::s2i(line);
                        process_array.push_back(new Process(process_counter));
                        process_array[process_counter]->num_of_vmas = n_vmas;
                        read_vmas = false;
                    }
                    else{
                        process_array[process_counter]->add_vmas(line);
                        if(process_array[process_counter]->vmas_list.size()>=
                        process_array[process_counter]->num_of_vmas){
                            read_vmas = true;
                            process_counter++;
                        }
                    }
                }
                else{   //reading instructions
                    Instr_Type t;
                    switch(line[0]){
                        case 'c':
                            t=c;
                            break;
                        case 'r':
                            t=r;
                            break;
                        case 'w':
                            t=w;
                            break;
                    }
                    instruction_queue.push(Instr(t, Util::s2i(line.substr(1))));
                }
            }
            inFile.close();
        }
    }
    catch (ifstream::failure e) {
        cerr << "Exception opening/reading/closing file\n";
    }
}
Frame* Simulator::allocate_frame_from_free_list(){
    //TO-DO Implement it!
    if(free_list_index<frame_table::num_of_frame)
        return  frame_table::inverse_mapping[free_list_index++];
    return nullptr;
}
Frame* Simulator::get_frame() {
    Frame *frame = allocate_frame_from_free_list();
    if(frame == nullptr) frame = THE_PAGER->determine_victim_frame();
    return frame;
}
Instr Simulator::get_next_instruction(){
    if(instruction_queue.empty())
        return Instr(c,-1);
    else{
        Instr tmpI = instruction_queue.front();
        instruction_queue.pop();
        return tmpI;
    }
}
bool Simulator::hasNextInst(){
    return !instruction_queue.empty();
}
void Simulator::run(){
    while (hasNextInst()) {
        Instr inst = get_next_instruction();

        // print output if necessary
        if(display_options[0]) 
            printf("%lu: ==> %c %d\n",inst_count++,Instr::t2c(inst.type),inst.val);
        if(inst.type == c){ //context switching
            current_process = process_array[inst.val];
            ctx_switches++;
            cost+=121;
            continue;
        }
        cost+=1;
        PTE *pte = current_process->page_table[inst.val]; // done by hardware in reality
        if (!pte->VALID) {
            if(!current_process->isBelong(inst.val)){    
                // the vpage doesn't belong to the process
                if(display_options[0])
                    cout<<" SEGV"<<endl;
                current_process ->pstats.segv++;
                cost+=240;
                continue;
            }
            Frame *newframe = get_frame();
            if(newframe->pid>=0){    //used frame
                newframe->pte->VALID = 0;
                if(display_options[0])
                    cout<<" UNMAP "<<newframe->pid<<':'<<newframe->vpage<<endl;
                process_array[newframe->pid]->pstats.unmaps++;
                cost+=400;
                if(newframe->pte->MODIFIED){    // needs to be swapped out
                    if(newframe->pte->file_mapped){
                        if(display_options[0])
                            cout<<" FOUT"<<endl;
                        process_array[newframe->pid]->pstats.fouts++;
                        cost+=2500;
                    }
                    else{
                        if(display_options[0])
                            cout<<" OUT"<<endl;
                        process_array[newframe->pid]->pstats.outs++;
                        cost+=3000;
                        newframe->pte->PAGEDOUT = 1;
                    }
                    
                    newframe->pte->MODIFIED = 0;
                }
            }
            //check ZERO,IN or FIN
            if(pte->file_mapped){
                if(display_options[0])
                    cout<<" FIN"<<endl;
                current_process->pstats.fins++;
                cost+=2500;
            }
            else if(pte->PAGEDOUT){
                if(display_options[0])
                    cout<<" IN"<<endl;
                current_process->pstats.ins++;
                cost+=3000;
                
            }
            else{
                if(display_options[0])
                    cout<<" ZERO"<<endl;
                current_process->pstats.zeros++;
                cost+=150;
            }
            // pte->PAGEDOUT = 0;
            //mapping
            if(display_options[0])
                cout<<" MAP "<<newframe->frame_id<<endl;
            current_process->pstats.maps++;
            cost+=400;
            newframe->pid = current_process->pid;
            newframe->vpage = inst.val;
            newframe->pte = pte;
            pte->frame = newframe->frame_id;
        }

        // // simulate instruction execution by hardware by updating the PTE bits
        // Update_pte(read/modify/write) bits based on operations.
        pte->REFERENCED = 1;
        pte->VALID = 1;
        if(inst.type==w){
            if(pte->WRITE_PROTECT){
                if(display_options[0])
                    cout<<" SEGPROT"<<endl;
                current_process->pstats.segprot++;
                cost+=300;
            }
            else{
                pte->MODIFIED = 1;
            }
        }
    }
}
void Simulator::report(){
    if(display_options[1]){ //P(agetable)
        for(auto p:process_array){
            p->print_pagetable();
        }
    }
    if(display_options[2]){ //F(rametable)
        cout<<frame_table::toString()<<endl;
    }
    if(display_options[3]){ //S(um)
        for(auto p:process_array){
            p->printStates();
        }
        printf("TOTALCOST %lu %lu %llu\n", ctx_switches, inst_count, cost);
    }
}
string Simulator::toString(){
    string rtn = "# process: "+to_string(num_of_process)+"\n";
    for(auto p:process_array){
        for(auto v:p->vmas_list){
            rtn+= to_string(v->start_vpage)+" "+
                    to_string(v->end_vpage)+" "+
                    to_string(v->write_protected)+" "+
                    to_string(v->file_mapped)+"\n";
        }
        rtn+="\n\n";
        for(int i=0;i<64;i++){
            if(p->isBelong(i)){
                if(p->isWriteProtected(i)){
                   rtn+= 'w';
                }
                else{
                    rtn+= '*';
                }
            }
            else{
                rtn+= '-';
            }
        }
        rtn+="\n\n";
    }
    rtn+=frame_table::toString()+"\n";
    // while (hasNextInst()) {
    //     Instr inst = get_next_instruction();
    //     rtn+=to_string(inst.type) + " "+to_string(inst.val)+"\n";
    // }
    return rtn;
}