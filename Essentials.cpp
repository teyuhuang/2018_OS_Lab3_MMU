#include "Essentials.h"
#include "utility.h"
#include <iostream>

VMAS::VMAS(int s,int e, bool w, bool f):start_vpage(s),
                                        end_vpage(e),
                                        write_protected(w),
                                        file_mapped(f){}
bool Process::isBelong(int m){
    if(m<64)
        if(page_in_vmas[m]!=nullptr)
            return true;
        else
            return false;
    else{
        cout<<"[isBelong] out of range"<<endl;
        return false;
    }
}
bool Process::isWriteProtected(int m){
    if(m<64)
        if(page_in_vmas[m]!=nullptr)
            return page_in_vmas[m]->write_protected;
        else{
            cout<<"[isWriteProtected] "<<m<<" not in VMAS"<<endl;
            return false;
        }
    else{
        cout<<"[isWriteProtected] out of range"<<endl;
        return false;
    }
}
Process::Process(int id){
    pid = id;
    for(int i=0; i<64;i++){
        page_table[i] = new PTE();
        page_in_vmas[i] = nullptr;
    }
}
void Process::add_vmas(int s,int e, bool w, bool f){
    shared_ptr<VMAS> p(new VMAS(s,e,w,f));
    vmas_list.push_back(p);
    for(int i=s;i<=e;i++){
        page_in_vmas[i] = p;
        page_table[i]->WRITE_PROTECT = w;
        page_table[i]->file_mapped = f;
    }
}
void Process::add_vmas(string line){
    std::size_t lastFound = 0;
    int parameters[4];
    //convert input string into parameters
    for(int i = 0; i<4; i++){
        std::size_t found = line.find(" ", lastFound);
        if (found!=std::string::npos){
            parameters[i] = Util::s2i(line.substr(lastFound, found-lastFound));
            lastFound = found+1;
            while(lastFound < line.size()&&line.at(lastFound)==' ') lastFound++;
        }
        else if(i ==3 ){
            parameters[i] = Util::s2i(line.substr(lastFound));
        }
        else{
            std::cerr<<"Error input"<<std::endl;
            exit(0);
        }
    }
    bool w = parameters[2]>0;
    bool f = parameters[3]>0;
    add_vmas(parameters[0],parameters[1],w,f);
}
void Process::printStates(){
printf("PROC[%d]: U=%lu M=%lu I=%lu O=%lu FI=%lu FO=%lu Z=%lu SV=%lu SP=%lu\n",
    pid,
    pstats.unmaps, pstats.maps, pstats.ins, pstats.outs,
    pstats.fins, pstats.fouts, pstats.zeros,
    pstats.segv, pstats.segprot);
}
Instr::Instr(Instr_Type t, int v):type(t),val(v){}
char Instr::t2c(Instr_Type t){
    switch(t){
        case c:
            return 'c';
        case r:
            return 'r';
        case w:
            return 'w';
        default:
            return 'X';
    }
}
string PTE::toString(){
    string rtn;
    if(VALID){
        if(REFERENCED) rtn+='R';
        else rtn+='-';
        if(MODIFIED) rtn+='M';
        else rtn+='-';
        if(PAGEDOUT) rtn+='S';
        else rtn+='-';
    }
    else if(PAGEDOUT) rtn = "#";
    else rtn = "*";
    return rtn;
}
PTE::PTE():VALID(0),
            WRITE_PROTECT(0),
            MODIFIED(0),
            REFERENCED(0),
            PAGEDOUT(0),
            frame(0),
            file_mapped(0),
            rest(0)
            {}
Frame::Frame(int id):frame_id(id),pid(-1),vpage(-1),pte(nullptr){}

int frame_table::num_of_frame = -1;
Frame** frame_table::inverse_mapping = nullptr;
int *frame_table::Ref_count = nullptr;
bool *frame_table::Locked = nullptr;

void frame_table::initialize(int n_frame){
    frame_table::num_of_frame = n_frame;
    frame_table::inverse_mapping = new Frame*[n_frame];
    frame_table::Ref_count = new int[n_frame];
    frame_table::Locked = new bool[n_frame];
    for(int i=0;i<n_frame;i++){
        frame_table::inverse_mapping[i] = new Frame(i);
        frame_table::Locked[i] = false;
        frame_table::Ref_count[i] = 0;
    }
}

string frame_table::toString(){
    string s="FT: ";
    for(int i=0;i<frame_table::num_of_frame;i++){
        if(frame_table::inverse_mapping[i]->pte!=nullptr){
            s+=to_string(frame_table::inverse_mapping[i]->pid)+":"+
            to_string(frame_table::inverse_mapping[i]->vpage)+" ";
        }
        else{
            s+="* ";
        }
        
    }
    return s;
}
string Process::page_table_toString(){
    string s;
    for(int i=0;i<64;i++){
        if(page_table[i]->VALID)
            s+=to_string(i)+':'+page_table[i]->toString();
        else
            s+=page_table[i]->toString();
        s+=' ';
    }
    return s;
}
void Process::print_pagetable(){
    cout<<"PT["<<pid<<"]: "<<page_table_toString()<<endl;
}