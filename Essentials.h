#ifndef ESSENTIALS_H
#define ESSENTIALS_H
#include <string>
#include <vector>
#include <memory>
#define MAX_FRAME 128

using namespace std;
struct PTE{
    unsigned int VALID:1;
    unsigned int WRITE_PROTECT:1;
    unsigned int MODIFIED:1;
    unsigned int REFERENCED:1;
    unsigned int PAGEDOUT:1;
    unsigned int frame:7;
    unsigned int file_mapped:1;
    unsigned int rest:19;
    PTE();
    string toString();
};
struct VMAS {
    int start_vpage;
    int end_vpage;
    bool write_protected;
    bool file_mapped;
    VMAS(int s,int e, bool w, bool f );
};
struct process_stats{
    unsigned unmaps = 0,
             maps = 0,
             pstats = 0,
             ins = 0,
             outs = 0,
             fins = 0,
             fouts = 0,
             zeros = 0,
             segv = 0,
             segprot = 0;
};
class Process{
public:
    vector<shared_ptr<VMAS>> vmas_list;
    int num_of_vmas = -1;
    int pid;
    shared_ptr<VMAS> page_in_vmas[64];
    PTE* page_table[64];
    Process(int id);
    bool isBelong(int m);
    bool isWriteProtected(int m);
    void add_vmas(int s,int e, bool w, bool f);
    void add_vmas(string line);
    string page_table_toString();
    process_stats pstats;
    void printStates();
    void print_pagetable();
};
enum Instr_Type {c,r,w};
struct Instr{
    Instr_Type type;
    int val;
    Instr(Instr_Type t, int v);
    static char t2c(Instr_Type t);
};
struct Frame{
    int frame_id;
    int pid;
    int vpage;
    PTE* pte;
    Frame(int id);
};
class frame_table{
public:
    static int num_of_frame;
    static Frame **inverse_mapping;
    static int *Ref_count;
    static bool *Locked;
    static void initialize(int n_frame);
    static string toString();
};
#endif /* ESSENTIALS_H */
