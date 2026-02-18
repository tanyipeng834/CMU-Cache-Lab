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
    int v =0;
    // this is the trace file input
    char * t;
    static unsigned long timer;
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
                v =1;
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
    processFile(t,cache,v,e,b,s,&timer);
 

    freeCache(&cache,s);




    


    
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

void processFile(char* traceFilePath , cache_line_t** cache, int verbose, int lines, int byteBits, int setBits, unsigned long * globalTimer){
    char op;
    unsigned long memoryAddress;
    size_t size;
    int miss =0 , hit=0,evictions=0;



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

       
       int cacheBehavior = updateCache(tag,set,cache,lines,globalTimer);

      switch (cacheBehavior){

            case 0 :
                strP +=sprintf(strP,"miss");
                miss +=1;
                break;
            case 1 :
                strP += sprintf(strP,"hit");
                hit+=1;
                break;
            case 2 :
                strP += sprintf(strP,"miss eviction");
                miss+=1;
                evictions+=1;
                break;
        

      }

      if (op=='M'){
        strP+=sprintf(strP," hit");
        hit+=1;
      }

      // print to stdout

      if(verbose){

      printf("%c %lx %zu %s\n",op,memoryAddress,size,cacheBehaviorStr);
      }

      

    







        


    }

    printSummary(hit, miss, evictions);

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
int updateCache(unsigned long tag, unsigned long set, cache_line_t ** cache, int noOfLines, unsigned long * globalTimer){

        int invalidSetIndex =-1;
        cache_line_t * cacheSet = cache[set];
        for (int i =0; i<noOfLines;i++){
            cache_line_t * currentLine = &cacheSet[i];
            if(currentLine->tag == tag && currentLine->valid ==1){
                currentLine -> time_counter = ++(*globalTimer);
                return 1;

            }
            if (currentLine->valid==0 && invalidSetIndex ==-1) invalidSetIndex =i;
           
        }

        // replace with the first line but we will implement LRU later.

        if(invalidSetIndex!=-1){
            cache_line_t *invalidLine = &cacheSet[invalidSetIndex];
            invalidLine->tag = tag;
            invalidLine -> time_counter = ++(*globalTimer);

            invalidLine->valid =1;
            return 0;


        }
       

        // find the set with the LRU cache and evict it
        unsigned long leastRecentTime = cacheSet[0].time_counter;
        int evictSetIndex=0;
        for (int i =1;i<noOfLines;i++){

            if (cacheSet[i].time_counter<leastRecentTime){
                evictSetIndex= i;
                leastRecentTime = cacheSet[i].time_counter;
            }


        }
        cache_line_t * evictLine  = &cacheSet[evictSetIndex];
        evictLine->tag = tag;
        evictLine->time_counter = ++(*globalTimer);
        evictLine->valid =1;
        return 2;





    

        

            
    }


void freeCache(cache_line_t *** cachep, int s){

    if(!cachep || ! *cachep)return;
    cache_line_t ** cache = *cachep;

    int sets = 1<<s;

    for (i =0;i<sets;i++)
    {
        free(cache[i]);
        cache[i] =NULL;
        

        

    }
    free(cache);
    *cachep =NULL;





}







