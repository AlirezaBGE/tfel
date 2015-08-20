The `@Behaviour` keyword declares the behaviour used for the
test. This keyword must be followed by an option specifying the
interface used by the behaviour. The `umat`, `cyrano` and `aster`
interfaces are supported.

Two strings are then expected:

- the library in which the behaviour is implemented;
- the name of the function.

## Example

~~~~ {.cpp}
@Behaviour<umat> 'libMFrontCastemBehaviours.so' 'umatnorton';
~~~~~~~~

