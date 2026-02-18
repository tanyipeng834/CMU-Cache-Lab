
#ifndef CSIM_H

typedef struct cache_line
{
    unsigned long tag;
    
    unsigned long time_counter;
    // 0 or 1 
    int valid;
} cache_line_t;

void printHelp(char* program);

void processFile(char* traceFilePath , cache_line_t** cache, int versbose, int lines, int byteBits, int setBits, unsigned long * globalTimer);

// S represents the number of sets and
// E represents the number of lines in the set.
cache_line_t** initCache(int S , int E);

// get the set from the memory address


unsigned long static inline getTag(unsigned long memoryAddress, int b, int s);
int static inline getSet(unsigned long memoryAddress, int b , int s);
int updateCache(unsigned long tag, unsigned long set, cache_line_t ** cache, int noOfLines, unsigned long * globalTimer);


#endif
