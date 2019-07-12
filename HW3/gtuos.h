#ifndef H_GTUOS
#define H_GTUOS

#include <string>
#include "8080emuCPP.h"
#include "memory.h"
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <time.h>
#include <stdio.h>


#define RRS_CYCLE 100 // context switch cycle



using namespace std;


typedef enum {
    READY = 0,
    BLOCKED = 1,
    RUNNING = 2
}thr_state;


typedef struct {

    State8080 state8080;    // saved registers like a,b,c,sp,pc
    uint16_t t_id;
    uint64_t cycle; // how many cycle the process hadd used so far
    thr_state  thread_state; // ready, blocked, running
    uint64_t address;   //physical addres of the memory location of process
    uint8_t isAlive;
    uint16_t wait_tid;
    int index;
    int start_cycle;

   
}thread_inf;


typedef struct {

	char name[20];
	char time[3][50];
    int size;
	int file_dsc;
	int open;
	int position;
	char icerik[300];
   
}file_att;


class GTUOS{
	public:
            GTUOS(CPU8080* cpu8080,const char* filename,const char* file_dat,int debug);
            uint64_t handleCall();
            void print_all_mem();
            void MemoryContents(ofstream& output);
            uint64_t run();
            bool Nolife_threads();
            thread_inf *thread_table ;
			file_att file_table[20];
            const char *file;
            const char *filedat;
            uint8_t debugMode;
            int curr_thread_ind;
			int curr_ind;
			int file_index;
			struct stat fileStat;
            int cycle_count; 
            int control;
            int control2;
            
    private:
        
            CPU8080 *theCPU;
            uint8_t PRINT_B();
            uint8_t PRINT_MEM();
            uint8_t READ_B();
            uint8_t READ_MEM();
            uint8_t PRINT_STR();
            uint8_t READ_STR();
            uint8_t GET_RND();
            uint8_t TCreate();
            uint8_t TExit();
            uint8_t TJoin();
            uint8_t TYield();
            uint8_t select_thread() const;
            void context_switch(uint8_t t1, uint8_t t2);
            void thread_state_update(uint8_t t1,int live,int flag,int cycle);
			uint8_t FileCreate();
			uint8_t FileClose();
			uint8_t FileOpen();
			uint8_t FileRead();
			uint8_t FileWrite();
			uint8_t FileSeek();
			uint8_t DirRead();
	            
};

#endif
