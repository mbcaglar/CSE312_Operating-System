/***********************************************************/
/*                M.BEDİRHAN ÇAĞLAR                        */
/*                   141044073                             */
/*                                                         */
/*					30.03.2018                             */
/*                                                         */
/* Bu program verilen asm kodunu simula ederek gtuos system*/
/* call larıyla kodu çalıştırır.						   */
/* 									                       */
/*                                                         */
/*                                                         */
/***********************************************************/
#include <iostream>
#include "8080emuCPP.h"
#include "gtuos.h"
#include "memory.h"
#include <sstream>
#include <fstream>
#include <stdlib.h>
#include <time.h>
#include <string>

using namespace std;
uint64_t GTUOS::handleCall(const CPU8080 & cpu){

    if(cpu.state->a==1){
        cycle_count=PRINT_B(cpu);
    }
    else if(cpu.state->a==2){
        cycle_count=PRINT_MEM(cpu);
    }
    else if(cpu.state->a==3){
        cycle_count=READ_B(cpu);
    }
    else if(cpu.state->a==4){
        cycle_count=READ_MEM(cpu);
    }
    else if(cpu.state->a==5){
        cycle_count=PRINT_STR(cpu);
    }
    else if(cpu.state->a==6){
        cycle_count=READ_STR(cpu);
    }
    else if(cpu.state->a==7){
        cycle_count=GET_RND(cpu);
    }

	return cycle_count ;
}
int GTUOS::PRINT_B(const CPU8080 & cpu){
    
    uint8_t b = cpu.state->b;
	cout<<"System Call PRINT_B"<<endl;
	cout<<"(decimal) B : "<<int(b)<<endl;  
  
    
    return 10;
}

int GTUOS::PRINT_MEM(const CPU8080 & cpu){
    
    uint16_t address;
	cout<<"System Call PRINT_MEM"<<endl;

    address = (cpu.state->b << 8) | cpu.state->c;
    cout<<"Memory[BC] -> "<<(int)cpu.memory->at(address)<<endl;  
    
    return 10;
}
int GTUOS::READ_B(const CPU8080 & cpu){
    int b;
    cout<<"System Call READ_B"<<endl;
    cout<<"Bir sayi giriniz(0-255) : ";
    cin>>b;

    if(b>0 && b<255){

        cpu.state->b = b;
      
    }else{
        cout<<"B bu deger araliginda olamaz..!!!"<<endl;
        cpu.state->b=0;
    }

    return 10;  

}
int GTUOS::READ_MEM(const CPU8080 & cpu){
    
    int number;
    uint16_t address;
	cout<<"System Call READ_MEM"<<endl;
    cout<<"Bir sayi giriniz(0-255) : ";
    cin>>number;

    address = (cpu.state->b << 8) | cpu.state->c;

    if(number>0 && number<255){
       
         cpu.memory->at(address) = number;
        
    }else{
        cout<<"Number verilen deger araliginda degil..!!!"<<endl;
        cpu.memory->at(address)=0;
    }
    
    return 10;
}
int GTUOS::PRINT_STR(const CPU8080 & cpu){
    
    cout<<"System Call PRINT_STR"<<endl;

    uint16_t address = (cpu.state->b << 8) | cpu.state->c;

    while(true){
        if(cpu.memory->at(address)!='\0'){
            cout<<cpu.memory->at(address);
            address++;
            
        }
        else{
            break;
        }

    }  
  cout<<endl;
    
    return 100;
}

int GTUOS::READ_STR(const CPU8080 & cpu){
    
    int i;
    uint16_t address;
    cout<<"System Call READ_STR"<<endl;
	
    cout<<"String giriniz : ";
	cin.ignore();
 	string s;
    getline(cin,s);
    
	address = (cpu.state->b << 8) | cpu.state->c;
    
    for(i=0; i<s.length(); i++){
        cpu.memory->at(address+i) = s[i];
    }
	cpu.memory->at(address+i)='\0';

    return 100;
}
int GTUOS::GET_RND(const CPU8080 & cpu){
    
    uint8_t num;
    cout<<"System Call GET_RND"<<endl;
	
    num=rand()%256;
    cpu.state->b=num;
    
    return 5;
}

void GTUOS::print_all_mem(const CPU8080 & cpu){

    for(int i=0; i<0x1000; i++){
        printf("%04x ",i*16);
        for(int j=0; j<0x10; j++) {
            printf("%02x ",cpu.memory->at(i*16+j));    
        }
        printf("\n");
    }
        
    
}
void GTUOS::MemoryContents(ofstream& dosya,const CPU8080& cpu){


for(int i=0; i<4096; i++){
        char temp[5];
        sprintf(temp,"%04x",i*16);
        dosya<<temp<<" ";
        for(int j=0; j<16; j++) {
            sprintf(temp,"%02x",cpu.memory->at(i*16+j));
            dosya << temp << " ";
        }
        dosya<<endl;
}

/*
    for(int i=0;i<0x1000;++i){

		dosya<<hex<<i*16<<" ";

        for(int j=0;j<0x10;++j) {
			dosya<<hex<<cpu.memory->at(i*16+j)<<" ";

        }
        dosya<<endl;
    }
*/
}
