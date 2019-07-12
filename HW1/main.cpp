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
    int cycle=0;
	if (argc != 3){
		std::cerr << "Usage: prog exeFile debugOption\n";
		exit(1); 
	}
	int DEBUG = atoi(argv[2]);

	memory mem;
	CPU8080 theCPU(&mem);
	GTUOS	theOS;

    ofstream myfile;
    myfile.open ("exe.mem",std::ofstream::out);
        
    if(!myfile.is_open()){
    	cerr<<"Dosya Acilamadi"<<endl;
        return -1;
    }

	theCPU.ReadFileIntoMemoryAt(argv[1], 0x0000);	
 	srand (time(NULL));
	do	
	{
		cycle+=theCPU.Emulate8080p(DEBUG);
		if(theCPU.isSystemCall()){
                    
		 	cycle+=theOS.handleCall(theCPU);
        }

        if (DEBUG == 2){
                    cin.get();
         }
	}	while (!theCPU.isHalted());
  
        cout<<"Cycle count -> "<<cycle<<endl;
        
    theOS.MemoryContents(myfile,theCPU);
    myfile.close();
	return 0;
}

