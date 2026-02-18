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
    cache_line_t **cache ;
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


    
    // pass in the number of lines in a set and number of sets.
    cache = initCache(e,s);
    processFile(t,cache,e,b,s);
 




    


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

void processFile(char* traceFilePath , cache_line_t** cache, int lines, int byteBits, int setBits){
    char op;
    unsigned long memoryAddress;
    size_t size;

    FILE * traceFile = fopen(traceFilePath,"r");
    if (!traceFile){
        perror("File does not exist");
        exit(1);
    }

    while(EOF!=(fscanf(traceFile," %c %lx,%zu",&op,&memoryAddress,&size)))
    {

        if(op =='I') continue; 

        
       // The tag is to uniquely identify a cache line within a set.  
       unsigned long  tag = getTag(memoryAddress,byteBits,setBits);
       int  set = getSet(memoryAddress,byteBits,setBits);

       char cacheBehaviorStr[100] ;
       char* strP = cacheBehaviorStr;

       
       int cacheBehavior = updateCache(tag,set,cache,lines);

      switch (cacheBehavior){

            case 0 :
                strP +=sprintf(strP,"miss");
                break;
            case 1 :
                strP += sprintf(strP,"hit");
                break;
            case 2 :
                strP += sprintf(strP,"miss eviction");
                break;
        

      }

      if (op=='M'){
        strP+=sprintf(strP," hit");
      }

      // print to stdout

      printf("%c %lx %zu %s\n",op,memoryAddress,size,cacheBehaviorStr);

    







        


    }

    fclose(traceFile);






}


cache_line_t ** initCache(int e , int s){
    cache_line_t ** cache;
    int sets = 1<<s;
    cache = malloc(sets*sizeof (cache_line_t *));
    if(!cache){perror("Malloc Failed"); exit(1);}
    for (int i =0;i<sets;i++)
    {
        cache[i] = calloc(e,sizeof(cache_line_t));
        if (!cache[i]){perror("Calloc Failed"); exit(1);}


    }

    return cache;

    



}

unsigned long static inline getTag(unsigned long memoryAddress, int b, int s){
    // logical shift to get the tag
    return memoryAddress >>(b+s);
}

int static inline getSet(unsigned long memoryAddress, int b , int s){
 
    return (memoryAddress >>(b)) & ((1UL<<s)-1);
}

// 4 possiblities miss , hit miss + eviction represented by number 0 , a and 2
int updateCache(unsigned long tag, unsigned long set, cache_line_t ** cache, int noOfLines){

        int invalidSetIndex =-1;
        cache_line_t * cacheSet = cache[set];
        for (int i =0; i<noOfLines;i++){
            cache_line_t currentLine = cacheSet[i];
            if(currentLine.tag == tag && currentLine.valid ==1) return 1;
            if (currentLine.valid==0 && invalidSetIndex ==-1) invalidSetIndex =i;
           
        }

        // replace with the first line but we will implement LRU later.

        if(invalidSetIndex!=-1){
            cache_line_t *invalidLine = &cacheSet[invalidSetIndex];
            invalidLine->tag = tag;
            invalidLine->valid =1;
            return 0;


        }
       

        // find the set with the LRU cache and evict it

        return 2;



    

        

            
    }







