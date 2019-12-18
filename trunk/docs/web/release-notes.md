% Release notes
% Helfer Thomas
% April 21, 2015

# Versions 2.0.x

## Version 2.0.0

First open-source version.

## Version 2.0.1

Minor fixes.

## Version 2.0.2

- The multiplication of two symmetric tensors has been corrected. In
  previous versions, the result was though to be symmetric, which is
  obviously wrong. The static methods `tpld` et `tprd` were removed
  from the `st2tost2` class and added to the `st2tot2` class. Such
  modification leads to small changes in the results of finite strain
  single crystal behaviours.
- The parallel compilation could fail on the documentation generation
  [`cmake`](http://www.cmake.org/): this is now fixed.
- In the results files of [`mtest`](mtest.html), the name of the
  driving variables and the thermodynamic forces are appropriately
  changed for more clarity.
- A new finite strain strategy named `log1D` was added to the `castem`
  interface. This interface allow the use of the logarithmic strain
  formalism in mono-dimensional fuel performance codes. See
  @helfer_extension_2015 for details.

## Version 2.0.3 (9/09/2015)

- Fix of a minor bug in the `CxxTokenizer` class which was appears
  when using the [`clang` `libc++`](http://libcxx.llvm.org/) standard
  library. This prevented many `MTest` tests to work on
  [Mac OS X](http://www.apple.com/fr/osx/).
- Fix of [Ticket #9](https://sourceforge.net/p/tfel/tickets/9/)
- Fix of [Ticket #10](https://sourceforge.net/p/tfel/tickets/10/)
- Fix of [Ticket #11](https://sourceforge.net/p/tfel/tickets/11/)
- The class `ExternalBehaviourDescription` was introduced
- The `AxialGrowth` entry was added to the glossary

A full description of the 2.0.3 release can be found
[here](documents/tfel/tfel-2.0.3.pdf) (in french).

## Version 2.0.4 (22/09/2015)

### Improvements

- Improved MTest python bindings: new wrapped methods `setStrain`,
  `setDeformationGradient`, `setOpeningDisplacement`, `setStress`,
  `setCohesiveForce`, `setStrainEpsilon`, etc.). See also
  [Ticket #32](https://sourceforge.net/p/tfel/tickets/32/).
- New entries in glossary: `Emissivity` and `HeatTransfertCoefficient`
- Better support for the clang compiler.
- in `TFEL/Math`, add the `t2tot2::transpose_derivative` method which
  compute the derivative of the transpose of a tensor with respect to
  this tensor.
- in `MFront`, the keyword `@DSL` is now on alias for `@Parser`
- in `MTest`, the rotation matrix relating the reference frame to the
  material frame can now be defined using either the Euler angles or
  the Miller indices:

  Example:
  `@RotationMatrix<Miller> {1,5,9};`

### Fixes

- Fix of [Ticket #12](https://sourceforge.net/p/tfel/tickets/12/):
  consistent tangent operator was not correctly converted in
  `MTestAsterFiniteStrainBehaviour`.
- Fix of [Ticket #15](https://sourceforge.net/p/tfel/tickets/15/): the
  definition of the `F77_FUNC` macros when using `CMake` is now more
  portable.
- Fix of [Ticket #18](https://sourceforge.net/p/tfel/tickets/18/): the
  altered stiffness tensor in plane stress was not correctly computed
  (corrections in the
  `UMATComputeOrthotropicPlaneStressUnAlteredStiffnessTensor`, and
  `AsterComputeOrthotropicPlaneStressAlteredStiffnessTensor` classes)
- Fix a small typo in the `MonoCrystal_DD_CC_Irradiation` when
  defining the convergence criterion value (`@Epsilon 1.e-0; ->
  @Epsilon 1.e-10;`)
- Fix of [Ticket #21](https://sourceforge.net/p/tfel/tickets/21/): the
  `t2tot2::trpd` method used to call the `TensorProductLeftDerivative`
  class rather than the `TensorProductRightDerivative` class.
- Fix of [Ticket #22](https://sourceforge.net/p/tfel/tickets/22/): a
  typo leading to incorrect results was fixed.
- Fix of [Ticket #25](https://sourceforge.net/p/tfel/tickets/25/): the
  initial value of the driving variable were not written when
  generating a `MTest` file in case of integration failure (see the
  `UMATGenerateMTestFileOnFailure` and
  `AsterGenerateMTestFileOnFailure` keywords for details).
- Fix of [Ticket #27](https://sourceforge.net/p/tfel/tickets/27/): in
  `tfel-config`, add missing dependencies of `TFELMaterial` to
  `TFELUtilities`.
- Fix of [Ticket #30](https://sourceforge.net/p/tfel/tickets/30/): the
  `CxxTokenizer::stripComments` method was corrected to correctly
  handle the case where the first token of a file is a comment.
- Fix of [Ticket #32](https://sourceforge.net/p/tfel/tickets/32/):
  some methods of the `MTest` class were not wrapped in `python`.

## Version 2.0.5

- [release notes of version 2.0.5](release-notes-2.0.5.html)

# Versions 2.1.x

The 2.1.x versions will be the last versions based build on C++98.

# Versions 3.x

## Versions 3.0.x

- [release notes of version 3.0.0](release-notes-3.0.html)
- [release notes of version 3.0.1](release-notes-3.0.1.html)
- [release notes of version 3.0.2](release-notes-3.0.2.html)

## Versions 3.1.x

- [release notes of version 3.1.0](release-notes-3.1.html)

# References

<!-- Local IspellDict: english -->
