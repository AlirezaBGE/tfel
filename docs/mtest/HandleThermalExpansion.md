The `@HandleThermalExpansion` keyword allow the desactivation of the
computation of thermal strain, which is done by default if a material
property named `ThermalExpansion` is defined (for isotropic
materials).

This keyword is followed by a boolean. If `true`, the computation of
the thermal strain is done.

## Example

~~~~ {.cpp}
@HandleThermalExpansion false;
~~~~~~~~~~
