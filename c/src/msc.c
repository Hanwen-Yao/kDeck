/* ------------------------------------------------------------------------ */          
/*                                                                          */          
/*                      MSC Functions for main.c							*/          
/*                      Author: Hanwen Yao                                  */          
/*                      Last Updated: 05/20/2019                            */          
/*                                                                          */          
/* ------------------------------------------------------------------------ */          
                                                                                        
#include "main.h"   

/* Print Time */                                                                        
void printTime(time_t start_t, time_t end_t)                                            
{                                                                                       
    time_t duration_t = end_t - start_t;                                                
    int hour = duration_t/3600;                                                         
    int minute = (duration_t-3600*hour)/60;                                             
    int second = duration_t - 3600*hour - 60*minute;                                    
    if (hour>=1)                                                                        
    {                                                                                   
        printf("%2dh,%2dm,%2ds",hour,minute,second);                                    
    }                                                                                   
    else                                                                                
    {                                                                                   
        if(minute>=1)                                                                   
        {                                                                               
            printf("%2dm,%2ds    ",minute,second);                                      
        }                                                                               
        else                                                                            
        {                                                                               
            printf("%2ds        ",second);                                              
        }                                                                               
    }                                                                                   
} 
