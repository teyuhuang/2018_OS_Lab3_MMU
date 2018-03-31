#include <iostream>
#include <fstream>
#include <sstream>
#include "utility.h"
using namespace std;

int Util::s2i(string s){
    return stoi(s,nullptr,10);
    }

int myRand::myrandom(int size) {
    if(isFileLoaded){
         int rand = (randvals[ofs++] % size);
        if(ofs>=MAX_ofs) ofs = 0;
        return rand;
    }
    else{
        cerr << "Read random file first\n";
        exit(0);
    }
}
int myRand::open(std::string filename){
    ifstream inFile;
    string inputStr;
    int idx = 0;
    ofs = 0;

    try {
        inFile.open(filename);
        if(inFile.is_open()){
            while(getline(inFile, inputStr)){
                if(MAX_ofs<0){      // first line
                    MAX_ofs = Util::s2i(inputStr);
                    randvals = new int[MAX_ofs];
                }
                else{
                    randvals[idx++] = Util::s2i(inputStr);
                }
            }
            inFile.close();
            if(idx != MAX_ofs){
                cerr << "[Random File] number of entry doesn't meet\n";
                MAX_ofs = idx;
            }
        }
    }
    catch (ifstream::failure e) {
        cerr << "Exception opening/reading/closing file\n";
        return 1;
    }
    isFileLoaded = true;
    return 0;
}
myRand::myRand(std::string filename){
    this->open(filename);
}
myRand::myRand(){}
void Discrete_Time_Accummulator::removeAndMerge(list<pair<int,int>>::iterator it){
    list<pair<int,int>>::iterator it_other;
    int start = it->first;
    int end = it->second;
    for(it_other = timeline.begin(); it_other!= timeline.end();it_other++){
        if (it==it_other) continue;
        else if(start<=it_other->first&&it_other->first<=end&&
                start<=it_other->second &&it_other->second<=end
                ){   //it overlap it_other
            timeline.erase(it_other);
        }
        else if(it_other->first<=start &&start<=it_other->second&&
                it_other->first<=end &&end<=it_other->second){   //it_other overlap it
            timeline.erase(it);
            return;
        }
            
        else if(start>=it_other->first&& start<=it_other->second && end>it_other->second){
            it_other->second = end;
            timeline.erase(it);
            removeAndMerge(it_other);
            return;
        }
        else if(end>=it_other->first&& end<=it_other->second && start<it_other->first){
            it_other->first = start;
            timeline.erase(it);
            removeAndMerge(it_other);
            return;
        }
    }
}
void Discrete_Time_Accummulator::addTimeSpan(int start, int end){
    list<pair<int,int>>::iterator it;
    if(start > end){
        int tmp;
        tmp = start;
        start = end;
        end = start;
    }
    for(it = timeline.begin(); it!= timeline.end();it++){
        if (start>=it->first&&end<=it->second) return;
        else if((start>=it->first)&& (start<=it->second) && (end>it->second)){
            it->second = end;
            removeAndMerge(it);
            return;
        }
        else if((end>=it->first)&& (end<=it->second) && (start< it->first)){
            it->first = start;
            removeAndMerge(it);
            return;
        }
    }
    timeline.push_back({start,end});
}
void Discrete_Time_Accummulator::printTimeLine(){
    for(auto span:timeline){
        cout<<"("<<span.first<<","<<span.second<<"), ";
    }
    cout<<endl;
}
int Discrete_Time_Accummulator::getTotalTime(){
    int total = 0;
    for(auto span:timeline){
        total += span.second -span.first;
    }
    return total;
}