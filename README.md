# dan_basic
An implementation of a simple language sort of looks like visual basic. I'm trying to keep it simple, so no functions / modules / classes for now, just simple looping / conditionals.

## Example
``` vb
dim x
x = 1 + 1
if x = 2 then 
    print("Hello!")
end if 
```

## How to build
If you have zsh, then just run `zsh run.sh`. If you run with no arguments, you will be dropped into the repl. If you run with a filename as the argument, it will execute that file. If your build was successful, you can just rerun by running the binary `dan_basic` file that is created by the `run.sh` script. I've only tested this on macOS (which I think uses clang), so it may not build on all systems / compilers.

## TODO
- Build parser
- Add test suite
- Build interpreter
- Add operator precedence
