#include<stdio.h>
#include<stdlib.h>

#define stream 1
#define main_memory 1024
#define cache_line 10
#define cache_block_size 8
int cache_tag[cache_line];

int total_memory_access=0;

int total_hit=0;

// print hit ratio
void print_hit_ratio(void) {
     float ratio;
     if(total_hit == 0)
       ratio=0;
     else
       ratio = (float)total_hit/(float)total_memory_access;

     ratio *=100;
     if(total_hit == 0) ratio=0;

     printf("Hit ratio:%.2f%\n",ratio);
     }

void write_cache_table(void) {
     int i,j;
     FILE *ofp;
     if(stream)
        ofp = fopen("cache.out","w");
     else
        ofp= stdout;

     fprintf(ofp,"%6s|","Lines ");
     for(i=0;i<cache_block_size;i++) {
          fprintf(ofp,"-------");
     }
     fprintf(ofp,"\n");

     for(i=0;i<cache_line;i++){
          fprintf(ofp,"%6d|",i);
          for(j=0;j<cache_block_size;j++){
             if(cache_tag[i]==0)
                 fprintf(ofp,"%6d|",0);
             else
                 fprintf(ofp,"%6d|",cache_tag[i]+j);
          }
          fprintf(ofp,"\n");
     }
     if (stream) fclose(ofp);
}

int usage(void){
    printf("Please pass a file\n");
    printf("\\Usage: cachesim <file>\n");
    return -1;
    }

int main(int argc, char *argv[]){
    FILE *ifp;
    int address_requested = 0;

    if (argc != 2){
       exit(usage());
    }
    
    ifp = fopen(argv[1], "r");
    if(ifp == NULL){
        printf("ERROR: File %s does not exist\n",argv[1]);
        exit(usage());
    }

    int i;
    for(i=0;i<cache_line;i++){
        cache_tag[cache_line] = 0;
    }
    int lastwrote = -1;

    while(fscanf(ifp,"%d\n",&address_requested) !=EOF)
    {
        int done = 0;

        for(i=0;i<cache_line;i++)
        {
         if(address_requested >= cache_tag[i] && (address_requested - cache_tag[i]) < cache_block_size)
              {
               total_hit++;
               done =1;
               break;
              }
        }
        if(done == 0)
        {
                lastwrote = (++lastwrote)%cache_line;
                cache_tag[lastwrote] = address_requested;
        }

        total_memory_access++;
    }




    print_hit_ratio();
    write_cache_table();
    fclose(ifp);


    }
