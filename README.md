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

## How to Build
Run `run.sh` in the terminal of your choice. You will need a Python 3 interpreter to build the source as well as a C compiler. To enable different options when building, run `python3 build.py -h`.

## TODO
- Build parser (in progress)
  - Option for else / elseif
  - ~~Fix operator precedence~~
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
