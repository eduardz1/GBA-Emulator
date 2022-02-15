#include "headers/bus.hh"
template <typename T>
int Bus<T>::read(Bus* address,int flag){
    return address->bitfield;
    }
