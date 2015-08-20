The `@AccelerationAlgorithm` keyword is used to give the name of the
acceeleration algorithm used to accelerate the convergence toward
equilibrium.

The follwing algorithm are available:

- `Cast3M`
- `Secant`
- `Steffensen`
- `Irons-Tuck`

## Example:

~~~~~{.cpp}
@AccelerationAlgorithm 'Secant';
~~~~~
