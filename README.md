# JuliaLAB
Embedded Julia in MATLAB

## How it works

JuliaLAB is a MEX library that links against libjulia. It uses Julia's and MATLAB's C APIs to call functions, convert and pass data between languages, and handle exceptions.

## Current major bug

There is currently a major bug that causes MATLAB to crash sometime after running these functions. I would appreciate your help! See [this thread](https://groups.google.com/forum/#!topic/julia-users/yV4rsNokZH4) and [this gist](https://gist.github.com/iamed2/e883c6b0b8ff4220d946).

## Requirements

- Julia 0.3+
- MATLAB (tested on 2012b, but should work with older versions)

## License

JuliaLAB is MIT licensed.
