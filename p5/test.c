//
// Created by Cesar on 4/11/2017.
//
#include <stdio.h>
#include "mem.h"
int main() {
    allocateMem(0,1000);
    allocateMem(1,400);
    allocateMem(2,200);
    allocateMem(3,500);
    print();
    deleteMem(1);
    print();
    allocateMem(4,100);
    print();
    deleteMem(2);
    print(); // works
    deleteMem(4); // does not work
    print();
    deleteMem(3);
    print();

}

