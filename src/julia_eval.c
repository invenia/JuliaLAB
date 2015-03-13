#include "julia_common.h"

#define MXFREE_NOT_NULL(x) if (x != NULL) { mxFree(x); }

void mexFunction( int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[] )
{
	char * strang = NULL;
	jl_value_t * ret = NULL;
	size_t length;
	bool success = false;

    jl_init("/usr/local/bin");

	JL_GC_PUSH1(&ret);

	plhs[0] = mxCreateLogicalScalar( false );

	if (nrhs > 0)
	{
		if (mxIsChar(prhs[0]))
		{
			length = mxGetNumberOfElements(prhs[0]) + 1;
			strang = (char *)mxMalloc(length);
			mxGetString(prhs[0], strang, length);
			ret = jl_eval_string(strang);

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

    jl_atexit_hook();

	MXFREE_NOT_NULL(strang);

	plhs[0] = mxCreateLogicalScalar(success);
}
