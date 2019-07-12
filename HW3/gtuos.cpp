/***********************************************************/
/*                M.BEDİRHAN ÇAĞLAR                        */
/*                   141044073                             */
/*                                                         */
/*					19.04.2018         						*/
/*                                                         */
/* Bu program verilen asm kodunu simula ederek gtuos system*/
/* call larıyla kodu çalıştırır.			   				*/
/* 					                   						*/
/*                                                         */
/*                                                         */
/***********************************************************/
#include <iostream>
#include "8080emuCPP.h"
#include "memory.h"
#include <sstream>
#include <fstream>
#include <stdlib.h>
#include <time.h>
#include <string>

#include "gtuos.h"
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <time.h>
#include <stdio.h>


using namespace std;


GTUOS::GTUOS(CPU8080* cpu8080,const char* filename,const char* file_dat,int debug) {

  thread_table= (thread_inf *)calloc(sizeof(thread_inf), 20);
  
  theCPU=cpu8080;

  thread_table[0].t_id = 0;
  thread_table[0].cycle = 0;
  thread_table[0].start_cycle=0;
  thread_table[0].thread_state = RUNNING;
  thread_table[0].isAlive = 1;
  thread_table[0].index = 0; 

  file=filename;
  filedat=file_dat;
  debugMode = debug;
  curr_thread_ind = 0;
  file_index=0;
  curr_ind=0;
  control=0;
  control2=0;


  
  for(int i=1; i<255; i++){
      thread_table[i].isAlive=0;
      thread_table[i].wait_tid=-1;
  }
  
     
}


uint64_t GTUOS::handleCall(){

    if(theCPU->state->a==1){
        cycle_count=PRINT_B();
    }
    else if(theCPU->state->a==2){
        cycle_count=PRINT_MEM();
    }
    else if(theCPU->state->a==3){
        cycle_count=READ_B();
    }
    else if(theCPU->state->a==4){
        cycle_count=READ_MEM();
    }
    else if(theCPU->state->a==5){
        cycle_count=PRINT_STR();
    }
    else if(theCPU->state->a==6){
        cycle_count=READ_STR();
    }
    else if(theCPU->state->a==7){
        cycle_count=GET_RND();
    }
    else if(theCPU->state->a==11){
        cycle_count=TCreate();
    }
    else if(theCPU->state->a==9){
        cycle_count=TJoin();
    }
    else if(theCPU->state->a==8){
        cycle_count=TExit();
    }
    else if(theCPU->state->a==10){
        cycle_count=TYield();
    }
    else if(theCPU->state->a==11){
        cycle_count=TYield();
    }
    else if(theCPU->state->a==12){
        cycle_count=FileCreate();
    }
 	else if(theCPU->state->a==13){
        cycle_count=FileClose();
    }
    else if(theCPU->state->a==14){
        cycle_count=FileOpen();
    }
  
  else if(theCPU->state->a==15){
        cycle_count=FileRead();
    }
    else if(theCPU->state->a==16){
        cycle_count=FileWrite();
    }
    else if(theCPU->state->a==17){
        cycle_count=FileSeek();
    }
    else if(theCPU->state->a==18){
        cycle_count=DirRead();
    }
	return cycle_count ;
}
uint8_t GTUOS::PRINT_B(){
    
    uint8_t b = theCPU->state->b;
	cout<<"System Call PRINT_B"<<endl;
	cout<<"(decimal) B : "<<int(b)<<endl;  
  
    
    return 10;
}

uint8_t GTUOS::PRINT_MEM(){
    
    uint16_t address;
	cout<<"System Call PRINT_MEM"<<endl;

    address = (theCPU->state->b << 8) | theCPU->state->c;
    cout<<"Memory[BC] -> "<<(int)theCPU->memory->at(address)<<endl;  
    
    return 10;
}
uint8_t GTUOS::READ_B(){
    int b;
    cout<<"System Call READ_B"<<endl;
    cout<<"Bir sayi giriniz(0-255) : ";
    cin>>b;

    if(b>0 && b<255){

        theCPU->state->b = b;
      
    }else{
        cout<<"B bu deger araliginda olamaz..!!!"<<endl;
        theCPU->state->b=0;
    }

    return 10;  

}
uint8_t GTUOS::READ_MEM(){
    
    int number;
    uint16_t address;
    cout<<"System Call READ_MEM"<<endl;
    cout<<"Bir sayi giriniz(0-255) : ";
    cin>>number;

    address = (theCPU->state->b << 8) | theCPU->state->c;

    if(number>0 && number<255){
       
         theCPU->memory->at(address) = number;
        
    }else{
        cout<<"Number verilen deger araliginda degil..!!!"<<endl;
        theCPU->memory->at(address)=0;
    }
    
    return 10;
}
uint8_t GTUOS::PRINT_STR(){
    
    cout<<"System Call PRINT_STR"<<endl;

    uint16_t address = (theCPU->state->b << 8) | theCPU->state->c;
	cout<<"the data which was readed from file : ";
    while(true){
        if(theCPU->memory->at(address)!='\0'){
            cout<<theCPU->memory->at(address);
            address++;
            
        }
        else{
            break;
        }

    }  
  cout<<endl;
    
    return 100;
}

uint8_t GTUOS::READ_STR(){
    
    int i;
    uint16_t address;
    cout<<"System Call READ_STR"<<endl;
	
    cout<<"(For Open)Filename giriniz : ";

 	string s;
    getline(cin,s);

	address = (theCPU->state->b << 8) | theCPU->state->c;
    
    for(i=0; i<s.length(); i++){
        theCPU->memory->at(address+i) = s[i];
    }
	theCPU->memory->at(address+i)='\0';

    return 100;
}
uint8_t GTUOS::GET_RND(){
    
    uint8_t num;
    cout<<"System Call GET_RND"<<endl;
	
    num=rand()%256;
    theCPU->state->b=num;
    
    return 5;
}
uint8_t GTUOS::FileCreate(){

	int filedesc;
	char ad[20];
	int i=0;
 	uint16_t address;
	int flag=0;

    cout<<"System Call FileCreate"<<endl;
	
    cout<<"(For Create)Filename giriniz : ";
 	
  	cin>>ad;

    for(int k=0; k<file_index; k++){
		if(strcmp(file_table[k].name,ad)==0){
			flag=1;
			break;
		}
	}
	if(flag==1){
		cout<<"There is the file name like that..."<<endl;
	}

	address = (theCPU->state->b << 8) | theCPU->state->c;
    for(i=0; i<strlen(ad); i++){
        theCPU->memory->at(address+i) = ad[i];
    }
	theCPU->memory->at(address+i)='\0';

    if(!flag){

		strcpy(file_table[file_index].name,ad);
	
		file_table[file_index].file_dsc=file_index;	
		file_table[file_index].open=0;
		file_table[file_index].size=0;


		time_t now = time(0);

		strcpy(file_table[file_index].time[0],(char*)ctime(&now));
		strcpy(file_table[file_index].time[1],(char*)ctime(&now));
		strcpy(file_table[file_index].time[2],(char*)ctime(&now));
	
		strcpy(file_table[file_index].icerik,"");

		file_index++;
	}



FILE *f=fopen(filedat,"wb");
for(int k=0; k<file_index; k++){
	fwrite(&file_table[k],sizeof(file_att),1,f);
}

fclose(f);

	return 1;

}
uint8_t GTUOS::FileClose(){

	
    cout<<"System Call FileClose"<<endl;
 
	string tmp;
	curr_ind=theCPU->state->b;

	if(curr_ind < 0)
		    return 0;

	time_t now = time(0);

	file_table[curr_ind].open=0;

	strcpy(file_table[curr_ind].time[1],(char*)ctime(&now));
	strcpy(file_table[curr_ind].time[2],(char*)ctime(&now));

	file_table[curr_ind].position=0;



FILE *f=fopen(filedat,"wb");

for(int k=0; k<file_index; k++){
	fwrite(&file_table[k],sizeof(file_att),1,f);
}
fclose(f);



	return 1;
	
}
uint8_t GTUOS::FileOpen(){

	int i;
	int filedesc;
 	string ffilename="";
	int flag=0;
    cout<<"System Call FileOpen"<<endl;

    uint16_t address = (theCPU->state->b << 8) | theCPU->state->c;

    while(true){
        if(theCPU->memory->at(address)!='\0'){
            ffilename+=theCPU->memory->at(address);
			address++;
            
        }
        else{
            break;
        }

    }


	for(int i=0; i<file_index; i++){

		if(file_table[i].name==ffilename){
			curr_ind=i;
			flag=1;
			break;	
		}
	}
	if(flag==0){
		cout<<"Not found the file!!!"<<endl;
		return 0;
	}

	file_table[curr_ind].file_dsc=curr_ind;
	file_table[curr_ind].position=0;
	file_table[curr_ind].open=1;

	
	if(file_table[curr_ind].file_dsc < 0){
		cout<<"--------"<<endl;
       return 0;
	}
/*	if (!write(filedesc, "BEDOOOOOOOOOOOOOOOyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyOO\n", 50) ) {
        return -1;
    }
*/	
	theCPU->state->b=file_table[curr_ind].file_dsc;

	return 1;
 
}
uint8_t GTUOS::FileWrite(){

	int i;
	int j;
	int filedesc;
	int byte;
	int tmp=0;
 	string filename;
    cout<<"System Call FileWrite"<<endl;

    uint16_t address = (theCPU->state->b << 8) | theCPU->state->c;
	curr_ind=theCPU->state->e;
	byte=theCPU->state->d;

	if(file_table[curr_ind].open==0){
		theCPU->state->b=0;
		cout<<"The file is not open.!!!"<<endl;
		return 0;
	}

 while(byte>0 ){


			tmp=(int)theCPU->memory->at(address);
			char str[20];
			strcpy(str,"");
			string Result;        
			ostringstream convert;

			convert << tmp;     

			Result = convert.str();
			for(j=0; j<Result.length(); j++){

				str[j]=Result[j];
			}
			str[j]='\0';

		 	strcat(file_table[curr_ind].icerik,str);
		 	strcat(file_table[curr_ind].icerik," ");
			file_table[curr_ind].size++;
			file_table[curr_ind].size++;
			file_table[curr_ind].position++;
			file_table[curr_ind].position++;
            address++;
			address++;
			byte--;

}

	return file_table[curr_ind].size;
 
}

uint8_t GTUOS::FileRead(){

	int i=0;
	int filedesc;
	int byte;

	int readed=0;
 	string filename;
    cout<<"System Call FileRead"<<endl;

    uint16_t address = (theCPU->state->b << 8) | theCPU->state->c;

	curr_ind=theCPU->state->e;
	byte=theCPU->state->d;
	if(file_table[curr_ind].open==0){
		theCPU->state->b=0;
		return 0;
	}
if(byte>file_table[curr_ind].size){
	byte=strlen(file_table[curr_ind].icerik);
}


 
for(i=0; i<byte; i++){
	theCPU->memory->at(address+i) = file_table[curr_ind].icerik[i];
	file_table[curr_ind].position++;
	readed++;

}


	return readed;
 
}
uint8_t GTUOS::FileSeek(){

	int byte;

    cout<<"System Call FileSeek"<<endl;

	byte=theCPU->state->b;
	curr_ind=theCPU->state->d;
	byte=theCPU->state->d;

	return 1;
 
}

uint8_t GTUOS::DirRead(){

	int i=0;
	int filedesc;
	int byte;
	int size=0;
	string line;
	string mydir;
    cout<<"System Call DirRead"<<endl;

    uint16_t address = (theCPU->state->b << 8) | theCPU->state->c;

 
for (size_t idx = 0; idx < file_index; idx++)
    {
        // If you wanted to search for specific records, 
        // you should do it here! if (idx == 2) ...

        cout << "File #" << idx+1 << endl;
        cout << "Name: " << file_table[idx].name << endl;
        cout << "size: " << file_table[idx].size << endl;
        cout << "icerik: " <<file_table[idx].icerik<< endl;
        cout << "Creat Time: " <<file_table[idx].time[0];
        cout << "Modified Time: " <<file_table[idx].time[1];
        cout << "Access Time: " <<file_table[idx].time[2];

    }


	return 1; 
}



uint8_t GTUOS::TCreate(){
    
    int ind;
    uint16_t address;
    cout<<"System Call TCreate"<<endl;
    
    address = (theCPU->state->b << 8) | theCPU->state->c; 
    
    thread_table[0].index=thread_table[0].index + 1;    
    ind=thread_table[0].index;
    
    
    thread_table[ind].t_id = ind;
    thread_table[ind].cycle = 0;
    thread_table[ind].thread_state = READY;
    thread_table[ind].isAlive = 1;
    thread_table[ind].address=address;
    theCPU->state->b=thread_table[ind].t_id;

  
    return 80;
  
}
uint8_t GTUOS::TJoin(){
   
    cout<<"System Call TJoin"<<endl;
    
    thread_table[curr_thread_ind].wait_tid=theCPU->state->b;
//    thread_state_update(curr_thread_ind,1,1,thread_table[curr_thread_ind].cycle);
    control2=1;
   
   return 40;
}
uint8_t GTUOS::TYield(){
    
    thread_state_update(curr_thread_ind,1,0,thread_table[curr_thread_ind].cycle);
    context_switch(curr_thread_ind,select_thread());
    
    return 40;
}
uint8_t GTUOS::TExit(){
    
    cout<<"System Call TExit"<<endl;
    //thread_state_update(curr_thread_ind,0,1,thread_table[curr_thread_ind].cycle);
    control=1;
   // context_switch(curr_thread_ind,select_thread());
    return 50;
}

void GTUOS::print_all_mem(){

    for(int i=0; i<0x1000; i++){
        printf("%04x ",i*16);
        for(int j=0; j<0x10; j++) {
            printf("%02x ",theCPU->memory->at(i*16+j));    
        }
        printf("\n");
    }
        
    
}
void GTUOS::MemoryContents(ofstream& dosya){


for(int i=0; i<4096; i++){
        char temp[5];
        sprintf(temp,"%04x",i*16);
        dosya<<temp<<" ";
        for(int j=0; j<16; j++) {
            sprintf(temp,"%02x",theCPU->memory->at(i*16+j));
            dosya << temp << " ";
        }
        dosya<<endl;
}

}

uint64_t GTUOS::run(){
      
    int cycle=0,cycle_temp=0;
    int flag=1;
    ofstream myfile;
    myfile.open ("exe.mem",std::ofstream::out);
       
    if(!myfile.is_open()){
    	cerr<<"Dosya Acilamadi"<<endl;
        return -1;
    }

	theCPU->ReadFileIntoMemoryAt(file, 0x0000);	
 	srand (time(NULL));

FILE *fp=fopen(filedat,"rb");
if(!fp){

	fp=fopen(filedat,"wb");
	fclose(fp);
	fp=fopen(filedat,"rb");
	flag=0;
	
}

while(!feof(fp) && flag==1){

	if(!fread(&file_table[file_index],sizeof(file_att),1,fp))
		break;

	file_index++;
}

fclose(fp);


do	
	{
		cycle+=theCPU->Emulate8080p(debugMode);
		if(theCPU->isSystemCall()){

			if(debugMode==1){
				DirRead();

			}                    
		 	if(handleCall()==0){
				return 0;
			}
			
        }

	}	while (!theCPU->isHalted());


			if(debugMode==0){
			
for (size_t idx = 0; idx < file_index; idx++)
    {
        // If you wanted to search for specific records, 
        // you should do it here! if (idx == 2) ...

        cout << "File #" << idx+1 << endl;
        cout << "Name: " << file_table[idx].name << endl;
        cout << "size: " << file_table[idx].size << endl;
       // cout << "icerik: " <<file_table[idx].icerik<< endl;
        cout << "Creat Time: " <<file_table[idx].time[0];
        cout << "Modified Time: " <<file_table[idx].time[1];
        cout << "Access Time: " <<file_table[idx].time[2];

    }

  			}                    

    cout<<"Good Bye"<<endl;
        
        
    MemoryContents(myfile);
    myfile.close();
    return 0;

}


void GTUOS::thread_state_update(uint8_t t1,int live,int flag,int cycle) {
  
  thread_table[t1].state8080 = *(theCPU->state); // copy current state
  thread_table[t1].isAlive = live;
  thread_table[t1].address=theCPU->state->pc;
 
  
  
    if(flag){
      thread_table[t1].thread_state = BLOCKED;
      //context_switch(t1,select_thread());
  }
    
  if(thread_table[t1].wait_tid>-1){
      thread_table[t1].thread_state == BLOCKED; 
  }
  else{
      thread_table[t1].thread_state ==READY;
  }
 

}

uint8_t GTUOS::select_thread() const {
    
   
   if(!(thread_table[curr_thread_ind].isAlive)){
       
        for(uint8_t i=0; i<255; i++){
             if(thread_table[i].wait_tid==thread_table[curr_thread_ind].t_id){
                     thread_table[i].wait_tid=-1;
                     thread_table[i].thread_state=READY;
             }
         }
   } 
  for (uint8_t i = curr_thread_ind + 1; i < 255; i++){
    if (thread_table[i].isAlive && thread_table[i].thread_state == READY) // get next proc which is live
        return i;
    
  }
  // if there is not alive process forward locations , look backwards
  for (uint8_t i =0; i <= curr_thread_ind; i++){
    if (thread_table[i].isAlive && thread_table[i].thread_state == READY)
        return i;
    }
}

void GTUOS::context_switch(uint8_t t1, uint8_t t2) {

    if(debugMode==2){
        cout<<"thread id -> "<<thread_table[t1].t_id<<"   <-switch->    thread id -> "<<thread_table[t2].t_id<<endl;
        cout<<"id-("<<thread_table[t1].t_id<<") thread spends cycle -> "<<thread_table[t1].cycle<<endl; 
        cout<<"id-("<<thread_table[t2].t_id<<") thread spends cycle -> "<<thread_table[t2].cycle<<endl;
    }
    if(debugMode==3){
      
        for (int i = 0; i < thread_table[0].index+1; ++i) {
            cout << "-----------------------------------------------------------------" << endl;
            cout << "Thread_ID: " << thread_table[i].t_id;
            cout<< "     State: ";
                    if(thread_table[i].thread_state==0){
                        cout<<" READY";
                    }
                    else if(thread_table[i].thread_state==1){
                        cout<<" BLOCKED";
                    }
                    else{
                        cout<<" RUNNING";
                    }
            
          cout<<"     Cycle: "<<thread_table[i].cycle <<"     Alive: "<<(int)thread_table[i].isAlive<<endl;
        }
        cout << "-----------------------------------------------------------------" << endl<<endl;
    }
      
    
    
    
  if (thread_table[t1].thread_state != BLOCKED) 
        thread_table[t1].thread_state = READY;


  *(theCPU->state) = thread_table[t2].state8080;
  thread_table[t2].thread_state = RUNNING;
    
  theCPU->state->pc = thread_table[t2].address;
  curr_thread_ind = t2;

  

}

bool GTUOS::Nolife_threads(){
    for(uint8_t i=1; i<thread_table[0].index; i++){
        
        if(thread_table[i].isAlive==1){
            return true;
        }
    }
    return false;
    
}


