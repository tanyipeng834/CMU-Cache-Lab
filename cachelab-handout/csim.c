#include "cachelab.h"
#include "csim.h"
#include<getopt.h>
#include<stdlib.h>
#include<unistd.h>
#include <stdio.h>



int main(int argc, char**argv)
{
    // options for my 
    int e,s,b,opt;
    // this is the trace file input
    char * t;
    opterr=0;
    
    while(-1!=(opt=getopt(argc,argv,"hvs:E:b:t:")))
    {
        switch(opt){
            case 's':
                s = atoi(optarg);
                
                break;
            case 'E':
                e = atoi(optarg);
                
                break;
            case 'b':
                b = atoi(optarg);
               
                break;
            case 't':
                t = optarg;
                
                break;
            case 'h':
                printHelp(argv[0]);
               
            case 'v':
                printf("fix this later");
                break;
            case '?' :
                printf("%s: invalid option -- '%c'\n",argv[0],optopt);
                


            
            
            
            

        }


    }

    if ( s==0 || e ==0 || b ==0 ){
        printf("%s: Missing required command line argument\n",argv[0]);
        printHelp(argv[0]);
        exit(1);
    }


    processFile(t);

    


    printSummary(0, 0, 0);
    return 0;
}

void printHelp(char * prog){
     
                printf("Usage: %s [-hv] -s <num> -E <num> -b <num> -t <file>\n", prog);
                printf("Options:\n");
                printf("  -h         Print this help message.\n");
                printf("  -v         Optional verbose flag.\n");
                printf("  -s <num>   Number of set index bits.\n");
                printf("  -E <num>   Number of lines per set.\n");
                printf("  -b <num>   Number of block offset bits.\n");
                printf("  -t <file>  Trace file.\n");
                printf("\n");
                printf("Examples:\n");
                printf("  linux> %s -s 4 -E 1 -b 4 -t traces/yi.trace\n", prog);
                printf("  linux> %s -v -s 8 -E 2 -b 4 -t traces/yi.trace\n", prog);


}

void processFile(char* traceFilePath){
    char op;
    unsigned long memoryAddress;
    size_t size;

    FILE * traceFile = fopen(traceFilePath,"r");
    if (traceFile ==NULL){
        perror("File does not exist");
        exit(1);
    }

    while(EOF!=(fscanf(traceFile," %c %lx,%zu",&op,&memoryAddress,&size)))
    {

        if(op =='I') continue; 
        
        printf("%c\n",op);
        printf("%lx\n",memoryAddress);
        printf("%zu\n",size);





        


    }

    fclose(traceFile);






}
