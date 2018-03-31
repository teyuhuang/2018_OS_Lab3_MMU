Frame *get_frame() {
    Frame *frame = allocate_frame_from_free_list();
    if(frame == NULL) frame = THE_PAGER->determine_victim_frame();
    return frame;
}

while (get_next_instruction(&operation, &vpage)) {
    pte = &current_process.page_table[vpage]; // done by hardware in reality
    if (pte->notpresent) {
        Frame *newframe = get_frame();
            //-> figure out if/what to do with old frame if it was mapped
            // see general outline in MM-slides under Lab3 header
            // see whether and how to bring in the content of the access page.
    }
    // simulate instruction execution by hardware by updating the PTE bits
    Update_pte(read/modify/write) bits based on operations.
}