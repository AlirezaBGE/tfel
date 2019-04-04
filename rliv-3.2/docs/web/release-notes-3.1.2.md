% Release notes of the 3.1.2 version of `TFEL`, `MFront` and `MTest`
% Thomas Helfer
% 2018

This is mainly a bug fix version of the `3.1` series. All tickets
solved are described below.

# Tickets fixed

## Ticket #131: Plane stress support in `Abaqus/Explicit` is broken

The documentation of the `vumat` interface has been badly understood in
plane stress. In some cases (namely using the `Native` finite strain
strategy), the conventions about tensors has not been transcribed
appropriately (there have been a confusion with the `Abaqus/Standard`
conventions).

> **Note**
> 
> The `Native` finite strain strategy in `Abaqus/Explicit` (based on a
> corotational formulation) is not supported in `MTest`, so there is no
> unit-tests associated.

For more details, see: <https://sourceforge.net/p/tfel/tickets/131/>

## Ticket #130: The size of arrays of parameters can't be declared using integer constants

The following statement would not work in previous versions:

~~~~{.cxx}
@IntegerConstant Nss = 2;
@Parameter tau0[Nss] = {100e6, 100e6};
~~~~

To solve this issue, a new method `readArrayOfVariablesSize` has been
introduced in the `DSLBase` class. This method is now shared by all
methods which reads the size of an array of variables.

For more details, see: <https://sourceforge.net/p/tfel/tickets/130/>

## Ticket #129: Arrays of parameters are not supported in simple precision

The trouble boils down to the fact that, du to intended limitations of
the `tvector` class, downcasting from double precision to single
precision is not allowed by the assignment operator.

The call to the assignment operator has been replaced by a call to
`tfel::fsalgo::copy` which allows downcasting.

For more details, see: <https://sourceforge.net/p/tfel/tickets/129/>

## Ticket #128: Generation of `MTest` files throws an exception which can't be caught by `Cast3M` in case of forced convergence

With the `Cast3M` interface, the generation of `MTest` files during a
forced convergence sequence throws an exception which can't be caught by
`Cast3M`, leading to a severe crash.

The trouble comes from the fact that during the forced convergence
sequence, the time step is null, which is not handled by the
`MTestFileGenerator` class.

The trouble has been circumvented by introducing a minimal time step of
`1.e-50` (the unit depends on the specific case treated).

For more details, see: <https://sourceforge.net/p/tfel/tickets/128/>

## Ticket #127: Substepping in the `Cast3M` and `Cyrano` interface may lead to a invalid convergence du to integer overflow

Using substeping with a high number of substeps may lead to a to integer
overflow in the counter associated with the remaining steps that have to
be performed.

This counter was declared as an `unsigned short`. For more than 14
substeps, this counter would overflow and was automatically set egal to
0, leading to an erroneous convergence. This counter has been changed to
`unsigned int` to avoid the problem.

For more details, see: <https://sourceforge.net/p/tfel/tickets/127/>

## Ticket #126: Jacobian error in `DDIF2Base.ixx`

There was a mistake in the jacobian computed in `treatFracture` method
of the `DDIF2Base` class du to the complex branching made.

For more details, see: <https://sourceforge.net/p/tfel/tickets/126/>

## Ticket #125: Compiling on gcc 8.0.1 (Fedora 28) fails

Compiling `TFEL\ with `gcc` 8.0.1 (`Fedora 28`) fails in
`CastemGenericPlaneStressHandler.hxx`.

Reported errors *a priori* look like regressions in the compiler rather
than actual bugs in the code. However, as the incriminated code is quite
ambiguous, it was worth fixing it.

For more details, see: <https://sourceforge.net/p/tfel/tickets/125/>

## Ticket #124: StandardElasticity brick: the elastic prediction is wrongly computed in plane stress if the stiffness tensor is computed (`@ComputeStiffnessTensor`) or given (`@RequireStiffnessTensor`)

For more details, see: <https://sourceforge.net/p/tfel/tickets/124/>

## Ticket #123: Implementation mistakes in various behaviours based on Implicit DSL and using a numerical jacobian (use of perturbated values to update auxiliary state variables)

Various behaviours in the MFront' tests base used local variables to
compute the increment of auxiliary state variables (in order to safe
some computational times and reduce the implementation size). This trick
is quite often used with analytical jacobian but can cause various
problems with numerical jacobian if the consistent tangent operator is
requested (in this case, the system is perturbated once after
convergence to get the upper part of the inverse of the jacobian, as
needed by the `getPartialJacobianInvert` method).

The issue can be circumvented using the `perturbatedSystemEvaluation`
flag introduced in `TFEL` `3.1.1` (see
<http://tfel.sourceforge.net/release-notes-3.1.1.html> and
<https://sourceforge.net/p/tfel/tickets/111/> for details ).

The concerned implementations were the following:

- `mfront/tests/behaviours/MonoCrystal.mfront`
- `mfront/tests/behaviours/MonoCrystalNewtonRaphson.mfront`
- `mfront/tests/behaviours/MonoCrystal_DD_CC_Irradiation_NumericalJacobian.mfront`
- `mfront/tests/behaviours/MonoCrystal_DD_CFC_Irradiation.mfront`
- `mfront/tests/behaviours/MonoCrystal_DD_CFC_NumericalJacobian.mfront`

For more details, see: <https://sourceforge.net/p/tfel/tickets/123/>

## Ticket #122: Missing file in the distributed tar ball (`mfront-query/include/CMakeLists`)

The file `mfront-query/include/CMakeLists.txt` was not included in the distributed tar ball.
This led to a warning when compiling with `cmake`, but it did not hinder the compilation.

For more details, see: <https://sourceforge.net/p/tfel/tickets/122/>

## Ticket #121: Modifying the reference values read in a file does not work in MTest

In `MTest`, the following lines was not parsed appropriately:

~~~~{.cpp}
@Test<file> @reference_file@ {
  'SXX' : '$8*1.e6'
} 1.e3;
~~~~

For more details, see: <https://sourceforge.net/p/tfel/tickets/122/>

## Ticket #120: Direct call to Cast3M' umat behaviours in `MTest` is broken

Since `TFEL` `3.1`, `MTest` is able to call raw Cast3M' umat behaviours
(not written using `MFront`). However, the handling of external state
variables was broken.

For more details, see: <https://sourceforge.net/p/tfel/tickets/120/>

## Ticket #119:	Material laws are not declared in the `@AdditionalConvergenceChecks` code block

In the generation of the `additionalConvergenceChecks` method, the
material laws were not declared.

For more details, see: <https://sourceforge.net/p/tfel/tickets/119/>

## Ticket #118: The `BehaviourDescription::areMaterialPropertiesDependantOnStateVariables` method always returns `true`

The
`BehaviourDescription::areMaterialPropertiesDependantOnStateVariables`
method always returned `true`. This bugs led to unnecessary computations
but did not affect the results.

For more details, see: <https://sourceforge.net/p/tfel/tickets/118/>

## Ticket #117: Generated source fails to compile if one ask MTest file generation with array of internal state variables

A `using namespace std;` statement has been removed in 3.1.0 in the generated code, which led to a compile time error (`ostringstream` undeclared) when:

- generating a `MTest` file on failure
- there is an array of internal state variables (this case was not tested)

A new test case has been introduced for testing all the cases.

For more details, see: <https://sourceforge.net/p/tfel/tickets/117/>

## Ticket #115: The stress free expansion handler shall be a `nullptr` for true finite strain behaviours

For more details, see: <https://sourceforge.net/p/tfel/tickets/115/>
