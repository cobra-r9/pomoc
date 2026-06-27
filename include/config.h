#ifndef CONFIG_H 
#define CONFIG_H 

#include "state.h"

// the config path to load the config file from.
extern char *CONFIG_PATH;

// the Data Struct to load the data into. A function to store to the struct. 
int config_load(PomData *data);


#endif 
