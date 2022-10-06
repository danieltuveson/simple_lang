# dan_basic
An implementation of a simple language sort of looks like visual basic. I'm trying to keep it simple, so no functions / modules / classes for now, just simple looping / conditionals.

## Example
``` vb
dim x
x = 1 + 1
if x == 2 then 
    print("Hello!")
end if 
```

## How to build
If you have zsh, then just run `zsh run.sh`. If you run with no arguments, you will be dropped into the repl. If you run with a filename as the argument, it will execute that file. If your build was successful, you can just rerun by running the binary `dan_basic` file that is created by the `run.sh` script. I've only tested this on macOS (Apple clang version 13.0.0), so it may not build on all systems / compilers.

## TODO
- Build parser (in progress)
  - Option for else / elseif
  - Fix operator precedence
  - Need to handle parentheses and prefix operators
  - Need to handle function calls
    - Could do this before function definitions so that we 
      can at least call builtin c functions
  - Need to handle function definitions
  - Probably want to add builtin list or array structure
  - maybe add dictionary / object syntax
  - Modules
  - ... macros?
- Add test suite (for everything!)
  - Unit testing
  - Maybe fuzz testing?
- Modify build scripts so this can be run on windows
- Build interpreter (in progress)
  - Need to implement everything mentioned above in the "Build parser" list
  - Need to implement proper hash table for storage
  - Need to implement proper scoping / local variables
  - *Maybe* try to build bytecode compiler / interpreter?
- Build standard library in-language, hopefully helps work out the kinks
