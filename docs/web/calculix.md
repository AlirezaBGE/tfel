% How to use `MFront` in `CalculiX`
% Thomas Helfer
% 2/02/2017

# User defined behaviour in `CalculiX`

User-defined mechanical behavior can be implemented using three
different interfaces:

- The  native `CalculiX` interface.
- The `ABAQUS` `umat` routines for linear materials (small strain
  analyses).
- The `ABAQUSNL` `umat` routines for non linear materials (finite
  strain analyses).

There are two ways of introducing user-defined mechanical behavior:

- Modify the `CalculiX` sources. This option is supported for the
  three interfaces.
- Call a behavior defined in shared libraries. This option is only
  supported for the `ABAQUS` and `ABAQUSNL` interfaces. 

Each of these approaches has its own advantages and its own
pitfalls.

The first one is intrusive and requires a partial recompilation of
`CalculiX`, which means having access to the sources and the rights to
install the executable if it is meant to be deployed on a system-wide
location.

The second one does not require any modification to `CalculiX`, is
generally easier to set up and is very flexible. There is no intrinsic
limitations on the number of shared libraries and functions that can
be dynamically loaded. It is thus quite feasible to create mechanical
behaviours databases by creating a shared library by specific
material. Such libraries will only be loaded if needed. In such an
approach, the mechanical behaviour is dedicated to a specific material
and is self-contained: it has no external parameter.

Shared libraries can be shared between co-workers by moving them on a
shared folder.

However, experience shows that using shared libraries can be confusing
for some user as they have to update their environment variables
(`PATH` on Windows or `LD_LIBRARY_PATH` on Unixes) for the libraries
to be usable. Shared libraries can also be more sensible to system
updates.

## Calling behaviour in shared libraries

For performance reasons, `CalculiX` supports two kinds of interfaces
to `Abaqus/Standard`'s `umat` behaviours:

- `ABAQUS` is meant to describe linear materials.
- `ABAQUSNL` is meant to describe no linear materials. For nonlinear
  materials the logarithmic strain and infinitesimal rotation are
  calculated, which slows down the calculation. Consequently, the
  nonlinear routine should only be used if necessary.

Those two interface supports the call to behaviours in external shared
libraries.

Calling shared libraries is triggered by putting the `@` character in
front material name. The material name is then decomposed into three
parts, separated by the `_` character:

- The name of the interface (`ABAQUS` or `ABAQUSNL`).
- The base name of the shared library (see below).
- The name of the function implementing the behavior.

For instance, if we want to call a small strain behavior in a linear
analysis, implemented by the `CHABOCHE` function in the
`libABAQUSBEHAVIOURS.so` shared library (Under `Windows`, the library
name has the `dll` extension.), one would declare the following
material name:

`@ABAQUS_ABAQUSBEHAVIOURS_CHABOCHE`

Here, the library name has been stripped from system-specific
convention (the leading lib and the .so extension). **The base name of
the library and the name of the function must be upper-cased}. This is
due to the way `CalculiX` interprets the input file**.

# Using the `MFront` behaviours generated through the `abaqus` interface

As described in the previous section, calling shared libraries in only
supported for the `ABAQUS` or `ABAQUSNL` interfaces. This means that
we can call mechanical behaviours generated by `MFront` through the
`abaqus` interface quite easily.

The `abaqus` interface is described [here](abaqus.html).

## A first example, the Saint-Venant Kirchoff hyperelastic behaviour

### Generation of the shared library

Consider the Saint-Venant Kirchoff hyperelastic behaviour as
implemented
[here](gallery/hyperelasticity/SaintVenantKirchhoffElasticity.mfront).

The library is generated like this:

~~~~~{.bash}
$ mfront --obuild --interface=abaqus SaintVenantKirchhoffElasticity.mfront 
Treating target : all
The following library has been built :
- libABAQUSBEHAVIOUR.so :  SAINTVENANTKIRCHHOFFELASTICITY_AXIS SAINTVENANTKIRCHHOFFELASTICITY_PSTRAIN SAINTVENANTKIRCHHOFFELASTICITY_3D
~~~~~

This shows that the library `libABAQUSBEHAVIOUR.so` has been
generated. Then, we have one implementation per modelling
hypothesis. For a \(3D\) computation, one shall use the
`SAINTVENANTKIRCHHOFFELASTICITY_3D` function.

Thus, the material name to be used is:
`@ABAQUSNL_ABAQUSBEHAVIOUR_SAINTVENANTKIRCHHOFFELASTICITY_3D`.

### A simple tensile test

The description of a simple tensile test is as follows:

~~~~~{.fortran}
**
** GEOMETRY
**
*Node
      1,           0.,         0.,         1.
      2,           0.,         1.,         1.
      3,           0.,         0.,         0.
      4,           0.,         1.,         0.
      5,           1.,         0.,         1.
      6,           1.,         1.,         1.
      7,           1.,         0.,         0.
      8,           1.,         1.,         0.
*Element, type=C3D8, elset=cube
1, 5, 6, 8, 7, 1, 2, 4, 3
*Solid Section, elset=cube, material=@ABAQUSNL_ABAQUSBEHAVIOUR_SAINTVENANTKIRCHHOFFELASTICITY_3D
,
*Nset, nset=sx2, generate
 5,  8,  1
*Nset, nset=sx1, generate
 1,  4,  1
*Nset, nset=sy1
 1,  3,  5,  7
*Nset, nset=sy2
 2,  4,  6,  8,
*Nset, nset=sz1
 3,  4,  7,  8
*Nset, nset=sz2
 1,  2,  5,  6,
**
** MATERIAL
** 
*Material, name=@ABAQUSNL_AbaqusBehaviour_SaintVenantKirchhoffElasticity_3D
*Depvar
   0
* User Material, constants=2
  150e9, 0.34
**
** LOADING
** 
*Step, nlgeom=YES
*Static
0.02, 1., 1e-05, 0.2
*Boundary
sx1, 1, 1
*Boundary
sy1, 2,2
*Boundary
sz1, 3,3
*Boundary
sx2, 1, 1, 0.2
*EL PRINT, ELSET=cube
E, S
*End Step
~~~~~~

> **Named state variables**
>
> `MFront` generates an example showing how to use the behaviour in
> `Abaqus/Standard`. Such example names each state variables, which is
> usefull for post-processing. However this feature is currently not
> supported by `CalculiX`.

<!-- Local IspellDict: english -->