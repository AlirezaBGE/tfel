The global algorithm uses two criteria to check if a satisfying
equilibrium has been found: one on opening displacements, the other on
cohesive forces.

This criterium on cohesive forces checks that the residual of the
Newton algorithm is low enought. By default, this value is equal to
1.e-3, which is a convenient value if cohesive force are expressed in
Newton.

The `@CohesiveEpsilon` let the user specifiy the criterium value used
for the criterium on cohesive force.

It is followed by the criterium value.

## Example

~~~~~ {.cpp}
@CohesiveForce 1.e2;
~~~~~~~~~~
