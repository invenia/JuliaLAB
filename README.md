# JuliaLAB
Embedded Julia in MATLAB

## How it works

JuliaLAB is a MEX library that links against libjulia. It uses Julia's and MATLAB's C APIs to call functions, convert and pass data between languages, and handle exceptions.

## How to use

1. Either add `JuliaLAB/src` to the MATLAB path or change to that directory.
2. Run `julia_compile(<path_to_julia-config.jl>)`.
3. Run `julia_eval(<str>)` or `julia_call(<function_name>, arg1, arg2, ...)`

## Caveats

All MATLAB variables are converted to Julia arrays of at least 2 dimensions (as this is how MATLAB arrays are represented). 

The library is largely incomplete at the moment, but more features are coming. Currently the functions will:

1. Evaluate the function or string.
2. If a Julia exception occurred, throw a MATLAB exception.
3. If the result of the evaluation was a `Float64`, print it and return `true`, else return `false`.

## Current major bug

There is currently a major bug that causes MATLAB to crash sometime after running these functions. I would appreciate your help! See [this thread](https://groups.google.com/forum/#!topic/julia-users/yV4rsNokZH4) and [this gist](https://gist.github.com/iamed2/e883c6b0b8ff4220d946).

## Requirements

- Julia 0.3+
- MATLAB (tested on 2012b, but should work with older versions)

## License

JuliaLAB is MIT licensed.
