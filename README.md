# Context

Th instrumentation library FLDLib replaces the floating-point types 
in the C, C++ source code by a comparison between an ideal
computation and the effective computation.

The library propagates domains within the program execution. That
means that each stored variable is a float/double/long double but
also a domain of values that propagates along the program execution.

The library considers two worlds: the continuous world defined by the
floating point types and the discrete world defined by the integer
types. The discrete world contains only the execution value. The
continuous world contains the execution value, the floating-point
domain, the real domain and the error domain with the constraint
floating-point value = real value + error value.

Passing from the discrete world to the continuous world (ex cast from
int to float) is straightforward.  But passing from the continuous
world to the discrete world (ex comparison operators, cast from float
to int) may create unsolved branches and unstable branches. 

Unsolved branches appear when the discrete world receives at least
two potential distinct values. For example if the floating-point
variable x has a floating-point value and a real value in the
interval `[0.0, 2.0]`, the comparison expression `x < 1` generates
both the discrete `true` or `1` value and the discrete `false` or
`0` value.
To solve this problem, the abstract execution successively explores
all possible branches with one set of values for each branch: the
`then` branch is explored with a domain intersected with
`[0.0, 1.0[` and the `else` branch is explored with a domain
intersected with `[1.0, 2.0]`.

Unstable branches appear when the discrete world receives different
values for the floating-point value and for the real value. For
example, if the floating-point variable x has a floating-point value
equal to {0.99} and a real value equal to {1.01}, the comparison
expression `x < 1` generates the discrete `false` or `0` value from
the floating-point value; but it generates the discrete `true` or
`1` value from the real value.

Unstable branches and unsolved branches can live together and so it
needs to be managed by specific execution branches. To synchronize
unstable branches, the user needs to insert specific split/merge
macros around the operations from the discrete world to the concrete
world. You can find more about split/merge in the examples and in
the documentation below. With the same macros, a specific option
`-loop` explores all the unsolved/unstable branches until all local
paths are covered.

# Build and use the library

You can build the library with different options. For each package
of options, the library provides three kinds of instrumentation:

  * an exact instrumentation that compares a floating point value
    and a real value approximated with (e,m) bits for the couple
    (exponent,mantissa). By default e = 16 and m = 123 - the library
    uses 5 more bits to print the values. This exact instrumentation
    needs only to manage unstable branches.

  * an interval instrumentation that contains all the floating-point
    and all the real values. This instrumentation is not very
    interesting but since it manages only unsolved branches, it has
    helped to debug this functionality.

  * an affine instrumentation that defines the real domains and the
    error domains with affine forms that share common symbols, which
    expresses some linear dependencies within the domains.

To build and install the FLDLib library, you can type the following
commands

    ./configure --prefix=TheInstallationDirectory
    make
    make install

To check the library

    cd tests
    make

To generate a diagnostic for a project (it should automatically
build the adequate library at top level)

    export FLOATDIAGNOSISHOME=TheInstallationDirectory
    cd tests
    $FLOATDIAGNOSISHOME/bin/comp_float_diagnosis.sh -affine -optim \
      -atomic absorption.c -o absorption.instr_diagnosis_affine

To generate the diagnosis, simply execute

    ./absorption.instr_diagnosis_affine

The diagnosis file is then the new generated file
`absorption_diag_aff_out`.

## Credits

The library has been developed thanks to the feedback from the experiments
carried out jointly with the IRSN (Institut de Radioprotection et de Sûreté
Nucléaire) and the partners of the ANR project CAFEIN to improve the
diagnosis of accuracy in numerical software.

# Generate a comparison table

Some advanced tests requires perl and the package File::ReadBackwards.

To generate an accuracy summary table, you need
- to install python to scan the results in the current directory
- to install ocaml with yojson and biniou to create a summary table

To create the result table in the directory tests, summarizing all
the results for all the projects.

    make diagnosis_affine
    make diagnosis_exact
    
    make table.txt

# Options for the instrumentation library

More precisely, the instrumentation comp_float_diagnosis.sh comes
with many options. Each set of options comes with a specific way to
compile the instrumentation library. The simpler way to experiment
with the options is to try a specific set of options. In case of
failure at link-time, then the user should look at error message,
identify the missing library, go into the parent directory ..,
compile the missing library with 'make -j libFloat...a', go into the
test_compiler directory and play back the specific set of options.

The following options are supported:

  * -interval or -exact or -affine 

  * -optim

  * -atomic

  * -loop

  * -verbose

  * -interface if configure has received --enable-interface as option

  * -print-path

The last options in the command line comp_float_diagnosis.sh are the
compilation options of the project.

You can see some macros in the source code

  * DECLARE_RESOURCES

    To put just after the header of the translation unit file.c(pp)
    containing the main function.

  * INIT_MAIN, END_MAIN

    To put at the beginning and at the end of the main function.

  * FPRINT, DPRINT

    To define the variables whose accuracy is interesting to follow.

  * FLOAT_SPLIT_ALL, FLOAT_MERGE_ALL

    The content of the these macros is defined as follow

    ````
      FLOAT_SPLIT_ALL(ident, out1 >> ... >> double::end(),
          intermediate1 << ... << double::end())

      /* code containing the unstable branch */

      FLOAT_MERGE_ALL(ident, ... << out1 << double::end(),
          ... >> intermediate1 >> double::end())
    ````

    The variables out1, ... are floating-point variables that are
    assigned by at least one of the branches and that are used after
    the MERGE macro. The variables intermediate1, ... are variables
    that are potentially modified or constrained by a branch and
    then used by another branch. By listing them, the user enables
    to store their value before the unstable branch and to restore
    their original value before the abstract execution goes in the
    other branch.

    The SPLIT/MERGE macros are first defined around the unstable
    comparisons. If the comparisons are in the condition of an if
    instruction, then the MERGE macro moves just after the immediate
    post-dominator of the if. If some discrete variables (integers,
    pointers) may escape from the MERGE with potential different
    values, then the MERGE should move until the discrete variable
    is no more useful (in the def/use sense used in Static Single
    Assignment transforms).

    These rules are encoded in the Frama-C plugin
    Frama-C/FLDCompiler, but the user should know them to integrate
    and adapt the results of Frama-C/FLDCompiler in the source code.

# Targetted numerical systems

This library is likely to be used to deliver a diagnosis verdict
on numerical algorithms to medium size numerical systems (some ten
thousands lines of code) with an analysis methodology from tests
to sound static analyses.

If you have questions about

 * the plugin Frama-C/FLDCompiler

 * the methodology

 * how to obtain less overapproximated accuracy results, with
   less analysis time on bigger numerical systems

do not hesitate to contact us at name@cea.fr with name =
franck.vedrine, nikolai.kosmatov or florent.kirchner.

