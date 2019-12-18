If the `@CompareToNumericalTangentOperator` is followed by `true`, a
comparison of the tangent operator given by the behaviour to a
numerical approximation computed by a centered finite difference is
performed.

The value of the perturbation used to compute this numerical
approximation can be adjusted by the
`@NumericalTangentOperatorPerturbationValue`.

The value used for to compare the tangent operator given by the behaviour to a
numerical approximation can be adjusted by the
`@TangentOperatorComparisonCriterium`.

## Example

~~~~ {.cpp}
@CompareToNumericalTangentOperator true;
~~~~~~~~
