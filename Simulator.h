#ifndef SIMULATOR_H
#define SIMULATOR_H

#include "Pager.h"
#include "Essentials.h"
#include "utility.h"
#include <queue>
#include <vector>

using namespace std;
class Simulator{
public:
    void run();
    void report();
    string toString();
    Simulator(string input_f, string rnd_f, Pager* pgr,bool *dop);
private:
    unsigned long long int cost = 0;
    unsigned long ctx_switches = 0, inst_count = 0;
    int free_list_index = 0;
    int num_of_process = -1;
    Process *current_process;
    Pager *THE_PAGER;
    bool *display_options;
    queue<Instr> instruction_queue;
    vector<Process*> process_array;
    
    Frame* get_frame();
    Frame* allocate_frame_from_free_list();
    Instr get_next_instruction();
    bool hasNextInst();

};
#endif /* SIMULATOR_H */
