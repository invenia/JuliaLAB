#include "julia_common.h"

void mx_to_jl_array(jl_array_t ** array_ptr, const mxArray * m)
{
	jl_value_t * array_type;
	jl_datatype_t * data_type;
	jl_tuple_t * dims;
	const mwSize * mdims;
	size_t ndims;
	size_t byte_size;

	JL_GC_PUSH3(&array_type, &data_type, &dims);

	data_type = mx_to_jl_type(mxGetClassID(m));

	if (data_type != jl_typeerror_type)
	{
		byte_size = mxGetNumberOfElements(m) * mxGetElementSize(m);
		ndims = mxGetNumberOfDimensions(m);
		array_type = jl_apply_array_type(jl_float64_type, ndims);

		mdims = mxGetDimensions(m);
		dims = jl_alloc_tuple(ndims);
		for (int i = 0; i < ndims; i++)
		{
			if (sizeof(mwSize) == 4)
			{
				jl_tupleset(dims, i, jl_box_uint32((uint32_t)mdims[i]));
			}
			else
			{
				jl_tupleset(dims, i, jl_box_uint64((uint64_t)mdims[i]));
			}
		}
		*array_ptr = jl_new_array(array_type, dims);
		memcpy(jl_array_data(*array_ptr), (void *)mxGetData(m), byte_size);
	}
	else
	{
		JL_GC_POP();
		mexErrMsgIdAndTxt("julia_eval:InputClass", "Input must be an array of supported type.");
	}

	JL_GC_POP();

	return;
}

jl_datatype_t * mx_to_jl_type(mxClassID class)
{
	switch (class)
	{
		case mxLOGICAL_CLASS:
			return jl_bool_type;
		case mxCHAR_CLASS:
			return jl_char_type;
		case mxDOUBLE_CLASS:
			return jl_float64_type;
		case mxSINGLE_CLASS:
			return jl_float32_type;
		case mxINT8_CLASS:
			return jl_int8_type;
		case mxUINT8_CLASS:
			return jl_uint8_type;
		case mxINT16_CLASS:
			return jl_int16_type;
		case mxUINT16_CLASS:
			return jl_uint16_type;
		case mxINT32_CLASS:
			return jl_int32_type;
		case mxUINT32_CLASS:
			return jl_uint32_type;
		case mxINT64_CLASS:
			return jl_int64_type;
		case mxUINT64_CLASS:
			return jl_uint64_type;
		default:
			return jl_typeerror_type;
	}
}

void check_jl_error()
{
	jl_value_t * e = jl_exception_occurred();
	char * error_id;
	const char * error_type;
	const char * error_msg;
	size_t length;

	JL_GC_PUSH1(&e);

	if (e)
	{
		error_id = matlab_error_id("JuliaException");
		error_type = copy_string(jl_typeof_str(e));

		if (is_jl_type(e, "UndefVarError"))
		{
			error_msg = copy_string(((jl_sym_t *)(jl_get_field(e, "var")))->name);
			JL_GC_POP(); // e
			mexErrMsgIdAndTxt(error_id, "%s: %s not defined\n", error_type, error_msg);
		}
		else if (has_field(e, "msg"))
		{
			error_msg = copy_string(jl_bytestring_ptr(jl_get_field(e, "msg")));
			JL_GC_POP(); // e
			mexErrMsgIdAndTxt(error_id, "%s: %s\n", error_type, error_msg);
		}
		else
		{
			JL_GC_POP(); // e
			mexErrMsgIdAndTxt(error_id, "%s\n", error_type);
		}
	}
}

char * matlab_error_id(const char * error_name)
{
	const char * func_name;
	char * error_id;
	size_t length;

	length = strlen(func_name) + 2 + strlen(error_name) + 1;
	func_name = mexFunctionName();
	error_id = (char *)mxMalloc(length);

	snprintf(error_id, length, "%s:%s", func_name, error_name);

	return error_id;
}

/*
 * Allocates a string copy using mxMalloc, mostly used for error strings
 * where freeing is not necessary or possible.
 */
char * copy_string(const char * orig_string)
{
	size_t length;
	char * new_string;

	length = strlen(orig_string);
	new_string = (char *)mxMalloc(length);
	memcpy(new_string, orig_string, length);

	return new_string;
}

bool has_field(jl_value_t * v, const char * field_name)
{
	bool has_field;
	jl_value_t * field;

	JL_GC_PUSH1(&field);

	field = (jl_value_t *)jl_symbol(field_name);
	has_field = (bool)jl_unbox_bool(jl_call2(jl_get_function(jl_base_module, "isdefined"), v, field));

	JL_GC_POP(); // field

	return has_field;
}

bool is_jl_type(jl_value_t * v, const char * type_name)
{
	jl_value_t * type;
	bool is_jl_type;

	JL_GC_PUSH1(&type);

	type = jl_get_global(jl_current_module, jl_symbol(type_name));
	is_jl_type = jl_typeis(v, type);

	JL_GC_POP(); // type

	return is_jl_type;
}
