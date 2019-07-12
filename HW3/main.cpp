#include <cstdlib>
#include <iostream>
#include <fstream>
#include "8080emuCPP.h"
#include "gtuos.h"
#include "memory.h"
#include <time.h>
	// This is just a sample main function, you should rewrite this file to handle problems 
	// with new multitasking and virtual memory additions.
using namespace std;
int main (int argc, char**argv)
{
    
    	memory mem;
	CPU8080 theCPU(&mem);
	GTUOS	theOS(&theCPU,argv[1],argv[2],atoi(argv[3]));
	theOS.run();

	return 0;
}

