#!/usr/bin/env python3
import os, re, shutil, argparse
import subprocess as sub
from pathlib import Path
from itertools import chain


SOURCE_NAME = "src"
TEST_NAME   = "test"
AUTOGEN_NAME = "autogenerated"
EXE_NAME = "dan_basic"
EXE_TEST_NAME = EXE_NAME + "_test"

def build():
    """
    python script for building, compiling, 
    and running the interpreter
    """
    sub.run("clear")

    # set up some paths 
    project_root = Path(".")
    src = Path(SOURCE_NAME)
    test = Path(TEST_NAME)
    autogen = Path(AUTOGEN_NAME)

    # set up parser
    parser = setup_parser()
    args = parser.parse_args()

    # cleanup files before rebuilding
    if args.clean:
        print("...removing old exe...")
        if os.path.exists(EXE_NAME):
            os.remove(EXE_NAME)
        if os.path.exists(EXE_TEST_NAME):
            os.remove(EXE_TEST_NAME)

        print("...removing old function headers...")
        if os.path.exists(AUTOGEN_NAME):
            shutil.rmtree(autogen)
        os.mkdir(autogen)

    # generate function headers
    if args.autogen:
        print("...generating function headers...")
        generate_function_headers(src, test, autogen)

    # compile
    if args.compile:
        print("...compiling...")
        compile_c(src, test, autogen)

    # run tests
    if args.test:
        print("...running tests...")
        compile_c(src, test, autogen, testing=True)
        sub.run(f"./{EXE_TEST_NAME}")

    # run repl
    if args.repl:
        print("...entering interpreter...")
        sub.run(f"./{EXE_NAME}")

    # run file
    if args.file:
        if not os.path.exists(args.file):
            raise FileNotFoundError(f"{args.file} does not exist")
        sub.run([f"./{EXE_NAME}", args.file])

    # create new .h / .c files 
    if args.newhead:
        raise NotImplementedError("use new_header.sh for now");


def setup_parser():
    description = """
    python script for building, compiling, 
    and running the interpreter
    """
    parser = argparse.ArgumentParser(description=description)

    description = "compile the interpreter"
    parser.add_argument("-c", "--compile", 
                        action="store_true", help=description)

    description = "compile and run with testing enabled"
    parser.add_argument("-t", "--test", 
                        action="store_true", help=description)

    description = "run compiled interpreter"
    parser.add_argument("-r", "--repl", 
                        action="store_true", help=description)

    description = "clean up old files / autogenerated headers"
    parser.add_argument("-m", "--clean", "--maintenance",
                        action="store_true", help=description)

    description = "build autogenerated header files"
    parser.add_argument("-g", "--autogen",
                        action="store_true", help=description)

    description = "generate new header file NOT IMPLEMENTED"
    parser.add_argument("-n", "--newhead", 
                        action="store_true", help=description)

    description = "specifies file to run"
    parser.add_argument("-f", "--file", 
                        metavar="file name", help=description)

    return parser


def get_file_contents(filename):
    with open(filename) as f:
        content = f.read()
    return content


def generate_function_headers(src, test, autogen):
    """
    This function looks through .c files for function definitions
    and copies function definitions into a corresponding file in the 
    "autogenerated" folder. This allows me to not have to copy
    each function definition into my header files.
    """

    # A good-enough pattern for matching function declarations
    pattern = r"^((\w|\d|\_|\*)+\s+)+(\w|\d|\_|\*)+\([^\)]*\)"
    flags = re.MULTILINE
    regex = re.compile(pattern, flags)
    files = chain(src.glob("*.c"), test.glob("*.c"))

    for file in files:
        str_file = str(file)

        # drop ".c" extension and folder name
        filename = str_file[str_file.index('/')+1:len(str_file)-2]

        if filename == "main":
            continue

        contents = get_file_contents(str_file)

        with open(autogen / (filename + ".c.generated.h"), "w") as f:
            for match in regex.finditer(contents, flags):
                f.write(match[0] + ";\n")


def compile_c(src, test, autogen, testing=False):
    """
    compiles c program based on files in the src and autogenerated directory 
    """
    files = list(src.glob("*.c"))
    exe = EXE_NAME
    folders = [f"-I{autogen}", f"-I{src}"] 

    if testing:
        files.remove(src / "main.c")
        files += list(test.glob("*.c"))
        exe = EXE_TEST_NAME
        folders += [f"-I{test}"]

    args = ["cc", "-DIS_DEBUG=1", "-std=c99"] + folders \
         + ["-Wall", "-Wextra", "-o", exe]
    sub.run(args + files)


if __name__ == "__main__":
    build()

