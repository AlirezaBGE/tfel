% `Implicit` keywords


# The `@Algorithm` keyword

The `@Algorithm` keyword is used to select a numerical algorithm.

The set of available algorithms depends on the domain specific
language. As the time of writting this notice, the following
algorithms are available:

- `euler`, `rk2`, `rk4`, `rk42` , `rk54` and `rkCastem` for the
  `Runge-Kutta` dsl.
- `NewtonRaphson`, `NewtonRaphson_NumericalJacobian,`
  `PowellDogLeg_NewtonRaphson`,
  `PowellDogLeg_NewtonRaphson_NumericalJacobian`, `Broyden`,
  `PowellDogLeg_Broyden`, `Broyden2`, `LevenbergMarquardt`,
  `LevenbergMarquardt_NumericalJacobian` for implicit dsls.

## Example (Runge-Kutta dsl)

~~~~{.cpp}
@Algorithm rk54;
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

## Example (Implicit dsl)

~~~~{.cpp}
@Algorithm NewtonRaphson;
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


# The `@Author` keyword

The `@Author` keyword is used give the name of the person who wrote
the `mfront` file.

All the following words are appended to the author's name up to a
final semi-colon.

Note: The name of the person who formulated the material property,
behaviour or model shall be given in the description section (see the
`@Description` keyword).

## Example

~~~~ {#Author .cpp}
@Author Éric Brunon;
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~



# The `@AuxiliaryStateVar` keyword

The `@AuxiliaryStateVar` keyword is a deprecated synonymous of
`@AuxiliaryStateVariable`.

# The `@AuxiliaryStateVariable` keyword

The `AuxiliaryStateVariable` keyword introduces one or several new
auxiliary state variables. It is followed by a type name and the
name(s) of the variable(s) declared, separated by commas.

## Example

~~~~{.cpp}
// scalar auxiliary state variable
@AuxiliaryStateVariables strain p;
// symmetric tensors auxiliary state variable
@AuxiliaryStateVariables StrainStensor evp,evp2;
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

# The `@Behaviour` keyword

The `@Behaviour` defines the name of the behaviour. The name must be a
valid `C++` class name.

The following characters are legal as the first character of an
identifier, or any subsequent character:

`_` `a` `b` `c` `d` `e` `f` `g` `h` `i` `j` `k` `l` `m`
`n` `o` `p` `q` `r` `s` `t` `u` `v` `w` `x` `y` `z`
`A` `B` `C` `D` `E` `F` `G` `H` `I` `J` `K` `L` `M`
`N` `O` `P` `Q` `R` `S` `T` `U` `V` `W` `X` `Y` `Z`

The following characters are legal as any character in an identifier
except the first:

`0` `1` `2` `3` `4` `5` `6` `7` `8` `9`

## Example

~~~~{.cpp}
@Behaviour Norton;
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

# The `@Bounds` keyword

The `@Bounds` keyword let the user define the domain of validity of
the law.

The `@Bounds` keyword is followed by a variable name, the keyword in
and an interval. The interval may contain the infinity, represented by
the '*' character.

## Effect

What happens if a variable if found to be out of its bounds depends on
the interface used. Most interfaces let the user choose one of three
following policies:

- `None`, which means that nothing is done.
- `Warning`, which means that a message is displayed, but computations
  are not stopped.
- `Strict`, which means that computations are stopped with an error
  message.

## Example

~~~~{.cpp}
@Bounds T in [293.15:873.15];
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


# The `@Brick` keyword

The `@Brick` keyword introduces a behaviour brick.

## Example

~~~~{.cpp}
@Brick "StandardElasticity";
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

# The `@Coef` keyword

The `@Coef` keyword is a deprecated synonymous of
`@MaterialProperty`.

# The `@CompareToNumericalJacobian` keyword

The `@CompareToNumericalJacobian` keyword enables or disables the
comparison of the user defined jacobian to a numerically computed
jacobian. This keyword is followed by a boolean value (`true` or
`false`).

Jacobians are compared by blocks. Blocks for which the comparison
failed are reported on the standard output.

The comparison criterion value can be changed used the
`@JacobianComparisonCriterion` keyword.

## Example

~~~~{.cpp}
@CompareToNumericalJacobian true;
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

# The `@ComputeFinalStress` keyword

The `@ComputeFinalStress` keyword introduces a code block meant to
compute the stress symmetric tensor after the integration.

The code block is called after the update of the state variables. The
auxiliary state variable and the external state variables are not
updated yet.

## Example

~~~~{.cpp}
@ComputeFinalStress{
  const SlidingSystems& ss = SlidingSystems::getSlidingSystems();
  // approximation de l'inverse de \(\Delta\,F_p\)
  inv_dFp = Tensor::Id();
  for(unsigned short i=0;i!=12;++i){
    inv_dFp -= dg[i]*ss.mu[i];
  }
  real J_inv_dFp = det(inv_dFp);
  inv_dFp /= CubicRoots::cbrt(J_inv_dFp);
  // Fe en fin de pas de temps
  Fe  = Fe_tr*inv_dFp;
  // Piola-Kirchhoff II
  S = D*eel;
  // Cauchy
  sig = convertSecondPiolaKirchhoffStressToCauchyStress(S,Fe);
}
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

# The `@ComputeStress` keyword

The `@ComputeStress` keyword introduces a code block meant to compute
the stress symmetric tensor.

This keyword interprets the code block to generate two methods:

- The first one is used before the integration step, using updated
  values for the state variables and external state variables.
- The second one is a candidate for the computation of the stress at
  the end of the integration. This candidate is used if the user does
  not provide an appropriate way of computing the stress at the end of
  the time step using the `@ComputeFinalStress` keyword.

## Note

If the user provide a way of computing the stress at the end of the
time step through the `@ComputeFinalStress` keyword, we consider that
the use of `@ComputeStress` is meaningless and advice the user to
rather compute explicitly the stress as part of the integration step.

## Example

~~~~{.cpp}
@ComputeStress{
  sig = (1-d)*(lambda*trace(eel)*Stensor::Id()+2*mu*eel);
}
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


# The `@ComputeThermalExpansion` keyword

The keyword `@ComputeThermalExpansion` is not documented yet

# The `@ComputedVar` keyword

The keyword `@ComputedVar` is not documented yet

# The `@DSL` keyword

The `@DSL` keyword specify the domain specific language (dsl) used.

The list of available dsl's is returned by the `--list-dsl` option of
`mfront`:

~~~~{.bash}
$ mfront --list-dsl
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

As the time of writting this notice, the following dsl's are available:

- `DefaultDSL`: this parser is the most generic one as it does not
  make any restriction on the behaviour or the integration method that
  may be used.
- `DefaultCZMDSL`: this parser is the most generic one as it does not
  make any restriction on the behaviour or the integration method that
  may be used.
- `DefaultFiniteStrainDSL`: this parser is the most generic one as it
  does not make any restriction on the behaviour or the integration
  method that may be used.
- `Implicit`: this parser provides a generic integrator based on an
  implicit scheme. The elastic strain is not automatically defined as
  a state variable
- `ImplicitII`: this parser provides a generic integrator based on a
  theta method. Unlike `Implicit`, the elastic strain is not
  automatically defined as a state variable.
- `ImplicitFiniteStrain`: this parser provides a generic integrator
  based on a theta method..
- `IsotropicMisesCreep`: this parser is used for standard creep
  behaviours of the form \(\dot{p}=f(s)\) where \(p\) is the equivalent
  creep strain and \(s\) the equivalent mises stress.
- `IsotropicPlasticMisesFlow`: this parser is used for standard
  plastics behaviours with yield surface of the form \(f(s,p)=0\)
  where \(p\) is the equivalent creep strain and \(s\) the equivalent
  mises stress.
- `IsotropicStrainHardeningMisesCreep`: this parser is used for
  standard strain hardening creep behaviours of the form
  \(\dot{p}=f(s,p)\) where \(p\) is the equivalent creep strain and
  \(s\) the equivalent mises stress.
- `MaterialLaw`: this parser is used to define material properties.
- `Model`: this parser is used to define simple material models.
- `MultipleIsotropicMisesFlows `: this parser is used to define
  behaviours combining several isotropic flows. Supported flow type
  are 'Creep' (\(\dot{p}=f(s)\)) 'StrainHardeningCreep'
  (\(\dot{p}=f(s,p)\)) and 'Plasticity' (\(f(p,s)=0\)) where \(p\) is
  the equivalent plastic strain and \(s\) the equivalent mises stress.
- `RungeKutta`: this parser provides a generic integrator based on one
  of the many Runge-Kutta algorithm.

## Example

~~~~{.cpp}
@DSL Implicit;
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

# The `@Date` keyword

The `@Date` keyword allows the user to precise when the mfront file
was written.

All the following words are appended to the date up to a final
semi-colon.

## Example

~~~~{.cpp}
@Date 2008-11-17;
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


# The `@Description` keyword

The `@Description` describes the material property, behaviour or model
which is implemented in a mfront file.

This keyword is followed by a block containing all the relevant piece
of information including:

- the references of the article or technical report from which the
  material property, behaviour or model is extracted
- the name of the authors
- the modifications made

## Example

~~~~{.cpp}
@Description
{
  Corrélation établie sur la nuance V-4Cr-4Ti.

  Propriétés extraites du document :

  Propriétés et comportement mécanique d alliages
   de Vanadium avant, après et sous irradiation

  Marion Le Flem, Jean-Luc Bechade, Annick Bougault,
  Aurore Michaux, Lionel Gosmain, Jean-Louis Seran
  DMN/SRMA/LA2M/NT/2008-2967/A
}
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

# The `@Epsilon` keyword

The `@Epsilon` keyword let the user define the convergence criterion
value. It is followed by a positive floating-point number.

## The epsilon parameter

The `@Epsilon` keyword defines the default value for the `epsilon`
parameter. This parameter can be changed at runtime.

## Example

~~~~{.cpp}
@Epsilon 1.e-12;
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

# The `@ExternalStateVar` keyword

The `@ExternalStateVar` keyword is a deprecated synonymous of
`@ExternalStateVariable`.

# The `@ExternalStateVariable` keyword

The `ExternalStateVariable` keyword introduces one or several new
external state variables. It is followed by a type name and the
name(s) of the variable(s) declared, separated by commas.

The external state variables names must be valid `C++` identifiers.

The following characters are legal as the first character of an
identifier, or any subsequent character:

`_` `a` `b` `c` `d` `e` `f` `g` `h` `i` `j` `k` `l` `m`
`n` `o` `p` `q` `r` `s` `t` `u` `v` `w` `x` `y` `z`
`A` `B` `C` `D` `E` `F` `G` `H` `I` `J` `K` `L` `M`
`N` `O` `P` `Q` `R` `S` `T` `U` `V` `W` `X` `Y` `Z`

The following characters are legal as any character in an identifier
except the first:

`0` `1` `2` `3` `4` `5` `6` `7` `8` `9`

## Arrays

One may declare an array of external state variables by specifying the
array size after the external state variable name.

## External names

It is recommended to associate to a external state variable a glossary
or an entry name through the methods `setGlossaryName` or
`setEntryName` respectively.

## Example

~~~~{.cpp}
// scalar external state variable
@ExternalStateVariable strain s;
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

# The `@Import` keyword

The `@Import` keyword allows the inclusion of one or several
(external) mfront files. This keyword is followed by a string or an
array of strings. Each string stands for a mfront file name that is
meant to be imported.

This instruction interrupts the treatment of the current file and
starts the sequential treatment of each file to be imported.

## Search paths

Files to be imported are searched, in that order:

- starting from the current directory.
- starting from directories specified with one of the `--search-path`
  or the `--include` (`-I`) `mfront`' command line options.
- staring from directories specified through the `MFRONT_INCLUDE_PATH`
  environment variable.

## Example

~~~~{.cpp}
@Import "SlidingSystemsCC.mfront";
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

# The `@Includes` keyword

The `@Includes` introduces a block were the user may define some
preprocessor directives, typically #include directives (hence the
name).

## Example

~~~~{.cpp}
@Includes{
#include<fstream>
}
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

# The `@InitJacobian` keyword

The `@InitJacobian` keyword is a deprecated synonymous of
`@InitializeJacobian`.

# The `@InitJacobianInvert` keyword

The `@InitJacobianInvert` keyword is a deprecated synonymous of
`@InitializeJacobianInvert`.

# The `@InitLocalVariables` keyword

The `@InitLocalVariables` keyword is a deprecated synonymous of
`@InitializeLocalVariables`.

# The `@InitLocalVars` keyword

The `@InitLocalVars` keyword is a deprecated synonymous of
`@InitializeLocalVariables`.

# The `@InitializeJacobian` keyword

The `@InitializeJacobian` keyword let the user introduce an initial
estimate for the jacobian matrix used by quasi-newton methods, such as
the first Broyden method.

## Example

~~~~{.cpp}
@InitializeJacobian{
  // setting the jacobian to identity (which is the default)
  fill(this->jacobian.begin(),this->jacobian.end(),real(0));
  for(unsigned short i=0;i!=this->jacobian.size();++i){
    this->jacobian(i,i)=real(1);
  }
}
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

# The `@InitializeJacobianInvert` keyword

The `@InitializeJacobianInvert` keyword let the user introduce an
initial estimate for the jacobian matrix used by quasi-newton methods,
such as the second Broyden method.

## Example

~~~~{.cpp}
@InitializeJacobianInvert{
  // setting the inverse of the jacobian (still called jacobian by the
  // way) to identity (which is the default)
  fill(this->jacobian.begin(),this->jacobian.end(),real(0));
  for(unsigned short i=0;i!=this->jacobian.size();++i){
    this->jacobian(i,i)=real(1);
  }
}
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

# The `@InitializeLocalVariables` keyword

The `@InitializeLocalVariables` introduces a code block meant to
initialize the local variables (see the `@LocalVariable` keyword).

## Example

~~~~{.cpp}
//! volumic fraction of each phase
@LocalVariable real  fv[Np];
//! interaction matrix
@LocalVariable tfel::math::tmatrix<Nss,Nss,real> mh;

@InitLocalVariables<Append>{
  // volumic fraction of each phase
  for(unsigned short i=0;i!=Np;++i){
    fv[i]=1.0/static_cast<real>(Np) ; 
  }
  // interaction matrix
  const real h1 = 1.;
  const real h2 = 1.;
  const real h3 = 1.;
  const real h4 = 1.;
  const real h5 = 1.;
  const real h6 = 1.;
  mh = InteractionMatrix::getInteractionMatrix(h1,h2,h3,
					       h4,h5,h6);
}
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~



# The `@IntegerConstant` keyword

The keyword `@IntegerConstant` is not documented yet

# The `@IntegrationVariable` keyword

The `IntegrationVariable` keyword introduces one or several new
integration variables. It is followed by a type name and the name(s)
of the variable(s) declared, separated by commas.

The integration variables names must be valid `C++` identifiers.

The following characters are legal as the first character of an
identifier, or any subsequent character:

`_` `a` `b` `c` `d` `e` `f` `g` `h` `i` `j` `k` `l` `m`
`n` `o` `p` `q` `r` `s` `t` `u` `v` `w` `x` `y` `z`
`A` `B` `C` `D` `E` `F` `G` `H` `I` `J` `K` `L` `M`
`N` `O` `P` `Q` `R` `S` `T` `U` `V` `W` `X` `Y` `Z`

The following characters are legal as any character in an identifier
except the first:

`0` `1` `2` `3` `4` `5` `6` `7` `8` `9`

## Arrays

One may declare an array of integration variables by specifying the array
size after the integration variable name.

## Example

~~~~{.cpp}
// scalar integration variable
@IntegrationVariables strain p;
// symmetric tensors integration variable
@IntegrationVariables StrainStensor evp,evp2;
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

# The `@Integrator` keyword

The keyword `@Integrator` is not documented yet

# The `@Interface` keyword

The `@Interface` keyword let the user specify interfaces to be
used. The keyword is followed by a list of interface name, separated
by commas.

Using this keyword is considered a bad pratice. The user shall use the
`--interface` command line argument to specify which interface shall
be used.

## Example

~~~~{.cpp}
@Interface umat;
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


# The `@IsTangentOperatorSymmetric` keyword

The keyword `@IsTangentOperatorSymmetric` is not documented yet

# The `@IsotropicBehaviour` keyword

The `@IsotropicBehaviour` declares the behaviour to be isotropic. As
this symmetry is the default one, this keyword is seldom used.

## Example

~~~~{.cpp}
@IsotropicBehaviour;
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

# The `@IsotropicElasticBehaviour` keyword

The `@IsotropicElasticBehaviour` is used to declare that the elastic
behaviour is isotropic even though the material as been declared
orthotropic. This declaration affects the `@RequireStiffnessTensor`
behaviour.

## Example

~~~~{.cpp}
@IsotropicElasticBehaviour;
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

# The `@IterMax` keyword

The `@IterMax` keyword is a deprecated synonymous of
`@MaximumNumberOfIterations`.

# The `@JacobianComparisonCriterion` keyword

The `@JacobianComparisonCriterion` keyword defines the criterion value
used for the comparison of the user-defined jacobian to a numerically
computed one. This keyword must be followed by a positive
floating-point number.

## The jacobianComparisonCriterion parameter

The `@JacobianComparisonCriterion` keyword defines the default value
for the `jacobianComparisonCriterion` parameter. This parameter can be
changed at runtime.

## Example

~~~~{.cpp}
@JacobianComparisonCriterion 1.e-6;
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~



# The `@JacobianComparisonCriterium` keyword

The `@JacobianComparisonCriterium` keyword is a deprecated synonymous
of `@JacobianComparisonCriterion`.

# The `@Library` keyword

The `@Library` keyword let the user specify part of the generated
libary name. This keyword is followed by the name of library.

This name must be a valid C++ identifier. The following characters are
legal as the first character of an identifier, or any subsequent
character:

`_` `a` `b` `c` `d` `e` `f` `g` `h` `i` `j` `k` `l` `m`
`n` `o` `p` `q` `r` `s` `t` `u` `v` `w` `x` `y` `z`
`A` `B` `C` `D` `E` `F` `G` `H` `I` `J` `K` `L` `M`
`N` `O` `P` `Q` `R` `S` `T` `U` `V` `W` `X` `Y` `Z`

The following characters are legal as any character in an identifier
except the first:

`0` `1` `2` `3` `4` `5` `6` `7` `8` `9`

## Example

~~~~{.cpp}
@Library AlcyoneLibrary;
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

# The `@Link` keyword

The `@Link` keyword let the user specify additional linker flags used
to build their shared library/executables. This keyword must be
followed by a string or an array of strings.

Usage of the `@Link` keyword is deprecated a linker flags are not
portable. The user may prefer using the `LDFLAGS` environment
variable.

## Example

~~~~{.cpp}
// explicit link with libm.so
// (not necessary in pratice)
@Link "-lm";
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


# The `@LocalVar` keyword

The `@LocalVar` keyword is a deprecated synonymous of
`@LocalVariable`.

# The `@LocalVariable` keyword

The keyword `@LocalVariable` is not documented yet

# The `@MFront` keyword

The keyword `@MFront` is not documented yet

# The `@Material` keyword

The `@Material` keyword let the user specify which material is treated
by the current file. This keyword is followed by the name of the
material.

This name must be a valid `C++` identifier. The following characters
are legal as the first character of an identifier, or any subsequent
character:

`_` `a` `b` `c` `d` `e` `f` `g` `h` `i` `j` `k` `l` `m`
`n` `o` `p` `q` `r` `s` `t` `u` `v` `w` `x` `y` `z`
`A` `B` `C` `D` `E` `F` `G` `H` `I` `J` `K` `L` `M`
`N` `O` `P` `Q` `R` `S` `T` `U` `V` `W` `X` `Y` `Z`

The following characters are legal as any character in an identifier
except the first:

`0` `1` `2` `3` `4` `5` `6` `7` `8` `9`

## Example

~~~~{.cpp}
@Material UO2;
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

# The `@MaterialLaw` keyword

The `@MaterialLaw` keyword imports the definition of a material law
defined in a mfront file and compiles, as part of the current library,
an function using the `mfront` interface. This function is available
in every standard code blocks.

The `@MaterialLaw` keyword is followed by a string or an array of
string identifying `mfront` files. 

## Search paths

Files to be imported are searched, in that order:

- starting from the current directory.
- starting from directories specified with one of the `--search-path`
  or the `--include` (`-I`) `mfront`' command line options.
- staring from directories specified through the `MFRONT_INCLUDE_PATH`
  environment variable.

## Note

The `mfront` interface has been created to avoid names conflict.

For internal reasons, `mfront` reports the creation of an auxiliary
library which is of no use.

## Example

~~~~{.cpp}
@MaterialLaw "UO2_YoungModulus.mfront";
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

# The `@MaterialProperty` keyword

The `@MaterialProperty` keyword let the user define one or several
material properties. This keyword is followed by the type of the
material property and a list of material properties names separated by
commas.

The material properties names must be valid `C++` identifiers.

The following characters are legal as the first character of an
identifier, or any subsequent character:

`_` `a` `b` `c` `d` `e` `f` `g` `h` `i` `j` `k` `l` `m`
`n` `o` `p` `q` `r` `s` `t` `u` `v` `w` `x` `y` `z`
`A` `B` `C` `D` `E` `F` `G` `H` `I` `J` `K` `L` `M`
`N` `O` `P` `Q` `R` `S` `T` `U` `V` `W` `X` `Y` `Z`

The following characters are legal as any character in an identifier
except the first:

`0` `1` `2` `3` `4` `5` `6` `7` `8` `9`

## Arrays

One may declare an array of material properties by specifying the
array size after the material property name.

## External names

It is recommended to associate to a material property a glossary or an
entry name through the methods `setGlossaryName` or `setEntryName`
respectively.

## Example

~~~~{.cpp}
// scalar material property
@MaterialProperty stress young;
young.setGlossaryName("YoungModulus");
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

# The `@MaximumIncrementValuePerIteration` keyword

The keyword `@MaximumIncrementValuePerIteration` is not documented yet

# The `@MaximumNumberOfIterations` keyword

The `@MaximumNumberOfIterations` keyword let the user define the
maximum number of iterations allowed. It is followed by a positive
integer.

## The iterMax parameter

The `@MaximumNumberOfIterations` keyword defines the default value for
the `itermax` parameter. This parameter can be changed at runtime.

## Example

~~~~{.cpp}
@MaximumNumberOfIterations 200;
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

# The `@Members` keyword

The keyword `@Members` is not documented yet

# The `@ModellingHypotheses` keyword

The keyword `@ModellingHypotheses` is not documented yet

# The `@ModellingHypothesis` keyword

The keyword `@ModellingHypothesis` is not documented yet

# The `@OrthotropicBehaviour` keyword

The `@OrthotropicBehaviour` declares the behaviour to be orthotropic.

## Example

~~~~{.cpp}
@OrthotropicBehaviour;
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

# The `@Parameter` keyword

The `@Parameter` keyword declares a new parameter or a list of new
parameters. Optionally, the default value of the declared parameters
may also be given following various C++ standard assignment syntaxes.

The default value of a parameter can also be declared after its
declaration using the `setDefaultValue` method.

## Example

~~~~{.cpp}
@Parameter  R0 = 500;
@Parameter  Q1{1000000000},b1{0.000001};
@Parameter  Q2(0),b2(0);
@Parameter  fc;
fc.setDefaultValue(1.e-2);
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


# The `@Parser` keyword

The `@Paser` keyword is a deprecated synonymous of `@DSL`.

# The `@PerturbationValueForNumericalJacobianComputation` keyword

The `@PerturbationValueForNumericalJacobianComputation` keyword let the
user defines the pertubation value used to compute the numerical
jacobian.

If \(Y\) is the vector of integration variables and \(F\) the function
defining the implicit system, the \(j^{\text{th}}\) column of the
numerical jacobian \(J^{n}\) is defined by the centered finite difference
formulae:
\[
J^{n}(i,j)=\frac{F(Y_{i}^{+\epsilon})-F(Y_{i}^{-\epsilon})}{2\,\epsilon}
\]
where:
\[
Y_{i}^{+\epsilon}(j)=
\left\{
  \begin{aligned}
    Y(j)          & \text{ si } &j \neq i \\
    Y(i)+\epsilon & \text{ si } &j =    i
  \end{aligned}
\right.
\]

## Example

~~~~{.cpp}
@PerturbationValueForNumericalJacobianComputation 1.e-7;
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

# The `@PhysicalBounds` keyword

The `@PhysicalBounds` keyword let the use define the physical domain
of a variable.

The `@PhysicalBounds` keyword is followed by a variable name, the
keyword in and an interval. The interval may contain the infinity,
represented by the '*' character.

## Effect

In implicit schemes, if physical bounds are set on a integration
variable, this variable is bounded to satisfy them during the internal
iterations.

If a variable is found to be out of its physical bounds, the
computations are stopped. The tests are performed at different stages
of the integration depending on the nature of the variable.

## Example

~~~~{.cpp}
// a temperature (in Kelvin) can't be negative
@Bounds T in [0:*[;
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


# The `@PredictionOperator` keyword

The keyword `@PredictionOperator` is not documented yet

# The `@Predictor` keyword

The keyword `@Predictor` is not documented yet

# The `@Private` keyword

The `@Private` keyword let the user define private methods or members
of in the generated behaviour class.

## Example

~~~~{.cpp}
@Private{
  void display(void) const{
    std::cout << "eto " << this->eto << std::endl;
  }
} // end of @Private
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

# The `@Profiling` keyword

The `@Profiling` keyword is followed by a boolean. If true, several
high resolutions clocks will be introduced in the generated code to
profile performance bottlenecks. The total time spend in various
portions of the generated code will be stored and displayed when the
calling process exits.

## Example

~~~~{.cpp}
@Profiling true;
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

# The `@RequireStiffnessOperator` keyword

The `@RequireStiffnessOperator` keyword is a deprecated synonymous of
`@RequireStiffnessTensor`.

# The `@RequireStiffnessTensor` keyword

The `@RequireStiffnessTensor` keyword requires the stiffness tensor to
be computed by the calling code. This generally means that some extra
material properties will be introduced and handled by the interface
before the behaviour integration.

By default, the stiffness tensor will have the same symmetry than the
behaviour (see `@IsotropicElasticBehaviour` to change this in the case
of an orthotropic behaviour).

~~~~{.cpp}
@RequireStiffnessTensor true;
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~



# The `@RequireThermalExpansionCoefficientTensor` keyword

The `@RequireThermalExpansionCoefficientTensor` keyword requires the
thermal expansion coefficient tensor to be computed by the calling
code. This generally means that some extra material properties will be
introduced and handled by the interface before the behaviour
integration.

The thermal expansion tensor will have the same symmetry than the
behaviour. For example, for an isotropic behaviour, the thermal
expansion tensor will be proportional to the identity. For an
orthotropic behaviour, the thermal expansion tensor is diagonal and
expressed in the material frame.

~~~~{.cpp}
@RequireThermalExpansionCoefficientTensor true;
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

# The `@Sources` keyword

The `@Sources` keyword let the user define a code block that will be
integrated in the generated sources of a behaviour. This allows the
user to implement their own classes or functions. This declarations of
such classes or functions can be made in a code block introduced by
the `@Includes` keyword.

~~~~{.cpp}
@Includes{
  //! declaration of the function f
  void f(void);
}

@Sources{
  // source of the function
  void f(void){
    std::cout << "Example of a function " << std::endl;
  } // end of f
}
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


# The `@StateVar` keyword

The `@StateVar` keyword is a deprecated synonymous of
`@StateVariable`.

# The `@StateVariable` keyword

The `StateVariable` keyword introduces one or several new state
variables. It is followed by a type name and the name(s) of the
variable(s) declared, separated by commas.

The state variables names must be valid `C++` identifiers.

The following characters are legal as the first character of an
identifier, or any subsequent character:

`_` `a` `b` `c` `d` `e` `f` `g` `h` `i` `j` `k` `l` `m`
`n` `o` `p` `q` `r` `s` `t` `u` `v` `w` `x` `y` `z`
`A` `B` `C` `D` `E` `F` `G` `H` `I` `J` `K` `L` `M`
`N` `O` `P` `Q` `R` `S` `T` `U` `V` `W` `X` `Y` `Z`

The following characters are legal as any character in an identifier
except the first:

`0` `1` `2` `3` `4` `5` `6` `7` `8` `9`

## Arrays

One may declare an array of state variables by specifying the array
size after the state variable name.

## External names

It is recommended to associate to a state variable a glossary or an
entry name through the methods `setGlossaryName` or `setEntryName`
respectively.

## Example

~~~~{.cpp}
// scalar state variable
@StateVariables strain p;
// symmetric tensors state variable
@StateVariables StrainStensor evp,evp2;
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

# The `@StaticVar` keyword

The `@StaticVar` keyword is a deprecated synonymous of
`@StaticVariable`.

# The `@StaticVariable` keyword

The `@StaticVariable` keyword let the user define a constant
value. Unlike parameters (see the `@Parameter` keyword), static
variables' values can't be changed after compilation.

This keyword must be followed by the type of the constant, its name,
an equal sign and its value.

## Example

~~~~{.cpp}
@StaticVariable real A = 1.234e56;
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

# The `@TangentOperator` keyword

The `TangentOperator` keyword introduces a code block used to define
the tangent operator. This code is called once the integration
variables, the stresses and the auxiliary state variables (see the
`@UpdateAuxiliaryStateVariables` keyword) have been updated.

The kind of tangent operator requested is given by variable named
`smt` (stiffness matrix type). As the time of writting this notice,
the possible values for `smt` are the following:

- `ELASTIC`: the elastic operator is requested (undamaged).
- `SECANT`: the secant operator is requested. This operator is only
  meaningful for behaviours describing brittle damage. The expected
  operator is then the elastic operator modified by the damage
  variables.
- `TANGENTOPERATOR`: the tangent operator is requested. This operator
  is seldom implemented as the consistent tangent operator is more
  interesting (and easier to compute).
- `CONSISTENTTANGENTOPERATOR`: the consistent tangent operator is
  requested. If an implicit scheme is used in small strain, the
  consistent tangent operator can be computed using the jacobian of
  the implicit system, see the mfront behaviours documentation for
  details.

## Example

~~~~{.cpp}
@TangentOperator{
  using namespace tfel::material::lame;
  if((smt==ELASTIC)||(smt==SECANTOPERATOR)){
    computeAlteredElasticStiffness<hypothesis,Type>::exe(Dt,lambda,mu);
  } else if (smt==CONSISTENTTANGENTOPERATOR){
    StiffnessTensor De;
    Stensor4 Je;
    computeElasticStiffness<N,Type>::exe(De,lambda,mu);
    getPartialJacobianInvert(Je);
    Dt = De*Je;
  } else {
    return false;
  }
}
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

# The `@Theta` keyword

The `Theta` keyword is used to define the default value of \(\theta\)
parameter used by implicit schemes. If the `Theta` keyword is not
used, implicit domain specific languages provide their own specific
default value (either \(0.5\) or \(1\)).

The value given to \(\theta\) must be in the range \(]0:1]).

The value of \(\theta\) can be changed at runtime by modifying the
`theta` parameter (see the `@Parameter` keyword).

## Example

~~~~{.cpp}
@Theta 0.5;
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

# The `@UpdateAuxiliaryStateVariables` keyword

The `UpdateAuxiliaryStateVariables` introduces a code block meant to
update the auxiliary state variables after integration.

In implicit domain specific languages, the code declared by
`UpdateAuxiliaryStateVariables` is called once the integration
variables (including state variables) and stresses (see the
`@ComputeFinalStress` keyword) have been updated. The external state
variables are not updated.

In Runge-Kutta domain specific languages, the code declared by
`UpdateAuxiliaryStateVariables` is called after each successful time
step. Keep in mind that most Runge-Kutta algorithms performs internal
substeppings: in this case, the code declared by
`UpdateAuxiliaryStateVariables` may be called several time during the
behaviour integration. An additional variable called `dt_`, which is
lower than the total time step increment `dt` if substeppings is
performed, gives the current time increment. The external state
variables are set to their values at the current date.

## Example (Implicit dsl)

~~~~{.cpp}
@UpdateAuxiliaryStateVariables{
  // valeur de milieu de pas, q ayant
  // déjà été mis à jour
  const real q_ = q-(1-theta)*dq;
  const real Q = Q0 + (Qm - Q0) * (1 - exp(-2 * Mu * q_) );
  R+=b*(Q-R)*dp;
}
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

## Example (Runge-Kutta dsl)

~~~~{.cpp}
@UpdateAuxiliaryStateVariables{
  sigeq = sqrt(sig|sig);
}
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

# The `@UpdateAuxiliaryStateVars` keyword

The `@UpdateAuxiliaryStateVars` keyword is a deprecated synonymous of
`@UpdateAuxiliaryStateVariables`.

# The `@UsableInPurelyImplicitResolution` keyword

The `@UsableInPurelyImplicitResolution` is a specifier which states
that the behaviour is usable in a purely implicit resolution, i. e. if
we can call the behaviour by only providing the values of external
state variables at the end of the time step and by setting their
increments to zero.

This behaviour characteristic may or may not have any influence
depending on the calling code. As the time of writting this notice,
only the `Licos` fuel performance code take advantage of this
characteristic. If true, the `Licos` code will simplify the definition
of the external state variables (called material loadings in this
context) which may result in some gain in time and memory space.

## Example

~~~~{.cpp}
@UsableInPurelyImplicitResolution true;
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


# The `@UseQt` keyword

The `UseQt` keyword (use quantities) specify if the behaviour
compilation should perform compile-time units checks. It is followed
by a boolean.

## Note

This feature is still experimental and is disabled in most cases.

## Example

~~~~{.cpp}
@UseQt true;
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
