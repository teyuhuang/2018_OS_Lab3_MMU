#include <iostream>
#include <unistd.h>
#include "utility.h"
#include <string.h>
#include "Simulator.h"
#include "Pager.h"
#include "Essentials.h"
using namespace std;

// [-a<fsrnca>] [-o<OPFSxyfa>] [–f<num_frames>] inputfile randomfile

int main(int argc, char **argv){
    
    bool OPFSxyfa[8];
    for(int i = 0;i<8;i++) OPFSxyfa[i] = false;
    int num_of_frame = 128;
    Pager* pager = nullptr;
    int n_arg = 0;
    int c;
    opterr = 0;
    
    // Reading Argument
    while ((c = getopt(argc, argv, "o:a:f:")) != -1){
        switch (c){
        case 'o':
            n_arg = strlen(optarg);
            if(n_arg==0){
                fprintf(stderr,"-o requires more arguments\n");
                return 1;
            }
            else if(n_arg>8){
                fprintf(stderr,"-o too many arguments are given\n");
                return 1;
            }
            for(int i=0;i<n_arg;i++){
                switch(optarg[i]){
                    case 'O':
                        OPFSxyfa[0] = true;
                        break;
                    case 'P':
                        OPFSxyfa[1] = true;
                        break;
                    case 'F':
                        OPFSxyfa[2] = true;
                        break;
                    case 'S':
                        OPFSxyfa[3] = true;
                        break;
                    case 'x':
                        OPFSxyfa[4] = true;
                        break;
                    case 'y':
                        OPFSxyfa[5] = true;
                        break;
                    case 'f':
                        OPFSxyfa[6] = true;
                        break;
                    case 'a':
                        OPFSxyfa[7] = true;
                        break;
                    default:
                        fprintf(stderr,
                        "[Ignored] Unknown option character %s.\n",
                        optarg);
                }
            }
            break;
        case 'a':
            if(strlen(optarg)==0){
                fprintf(stderr,"-a 1 argument is required\n");
                return 1;
            }
            switch (optarg[0]){
                case 'f':
                    pager = new Pager_FIFO();
                    break;
                case 's':
                    pager = new Pager_Sec_Chance;
                    break;
                case 'r':
                    pager = new Pager_Random;
                    break;
                case 'n':
                    pager = new Pager_NRU;
                    break;
                case 'c':
                    pager = new Pager_FIFO;
                    break;
                case 'a':
                    pager = new Pager_Aging;
                    break;
                default:
                    fprintf(stderr,
                            "Unknown option character %s.\n",
                            optarg);
                    return 1;
            }
            break;
        case 'f':
            num_of_frame = Util::s2i(optarg);
            break;
        case '?':
            fprintf(stderr,
                    "Unknown option character `\\s%s'.\n",
                    optopt);
            return 1;
        default:
            abort();
        }
    }

    frame_table::initialize(num_of_frame);

    string inputName = argv[argc - 2];
    string randFile = argv[argc - 1];
    
    // Create Simulator
    Simulator simulator(inputName,randFile,pager,OPFSxyfa);
    simulator.run();
    simulator.report();
    return 0;
}

