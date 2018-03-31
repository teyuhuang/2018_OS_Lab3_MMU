#ifndef PAGER_H
#define PAGER_H
class Pager {
    virtual frame* determine_victim_frame();
};
class Page_FIFO:Pager{
    virtual frame_t* select_frame();    //Select a frame to sacrifice  
}
class Page_Sec_Chance:Page_FIFO{
    frame_t* select_frame();    //Select a frame to sacrifice  
}
class Page_Random:Pager{
    frame_t* select_frame();    //Select a frame to sacrifice  
}
class Page_NRU:Pager{
    frame_t* select_frame();    //Select a frame to sacrifice  
}
class Page_Clock:Pager{
    frame_t* select_frame();    //Select a frame to sacrifice  
}
class Page_Aging:Pager{
    frame_t* select_frame();    //Select a frame to sacrifice  
}
#endif /* PAGER_H */
