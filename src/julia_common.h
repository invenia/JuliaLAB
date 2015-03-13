#ifndef __JULIA_COMMON_H
#define __JULIA_COMMON_H

#include "mex.h"
#include "matrix.h"
#include <julia.h>

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#include <stdbool.h>

// Functions //////////////////////////////////////////////////////////////

jl_datatype_t * mx_to_jl_type(mxClassID);
void mx_to_jl_array(jl_array_t **, const mxArray *);
void check_jl_error(void);
char * matlab_error_id(const char *);
char * copy_string(const char *);
bool has_field(jl_value_t *, const char *);
bool is_jl_type(jl_value_t *, const char *);

///////////////////////////////////////////////////////////////////////////

#endif