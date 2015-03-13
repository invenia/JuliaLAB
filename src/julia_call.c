#include "julia_common.h"

#define MXFREE_NOT_NULL(x) if (x != NULL) { mxFree(x); }

void mx_to_jl_args(jl_value_t **, const mxArray **, int);

void mexFunction( int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[] )
{
	char * strang = NULL;
	size_t length;
	jl_value_t ** julia_args = NULL;
	jl_function_t * func;
	jl_value_t * ret;
	int num_args;
	bool success = false;

    jl_init("/usr/local/Cellar/julia/HEAD/lib");

	JL_GC_PUSH2(&func, &ret);

	num_args = nrhs - 1;
	JL_GC_PUSHARGS(julia_args, num_args);

	if (nrhs > 1)
	{
		if (mxIsChar(prhs[0]))
		{
			length = mxGetNumberOfElements(prhs[0]) + 1;
			strang = (char *)mxMalloc(length);
			mxGetString(prhs[0], strang, length);

			func = jl_get_function(jl_current_module, strang);

			if (func == NULL)
			{
				JL_GC_POP();
				JL_GC_POP();
				mexErrMsgIdAndTxt(matlab_error_id("UndefinedFunction"), "Undefined function %s.", strang);
			}

			num_args = nrhs - 1;
			mx_to_jl_args(julia_args, prhs + 1, num_args);
			ret = jl_call(func, julia_args, num_args);

			check_jl_error();

			if (jl_is_float64(ret))
			{
				mexPrintf("Result: %f\n", jl_unbox_float64(ret));
				success = true;
			}
			else
			{
				mexPrintf("Return value is not Float64 (%s).\n", jl_typeof_str(ret));
			}
		}
	}

	JL_GC_POP();
	JL_GC_POP();

    jl_atexit_hook();

	MXFREE_NOT_NULL(strang);

	plhs[0] = mxCreateLogicalScalar(success);
}

void mx_to_jl_args(jl_value_t ** julia_args, const mxArray ** args, int nargs)
{
	for (int i = 0; i < nargs; i++)
	{
		mx_to_jl_array((jl_array_t **)&(julia_args[i]), args[i]);
	}
}
