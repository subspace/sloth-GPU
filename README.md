# sloth-GPU
GPU implementation of sloth (CUDA and OpenCL)


## CUDA

### uint_t Libraries
---
1. uint128_t
2. uint256_t
3. uint512_t

### Misc Functions
---
1. Legendre
2. Montgomery


## OpenCL


## Research

*Required Operations for the sqrt-permuatation (high-level only):*
1. Jacobi (legendre can be a better fit for our needs)
2. Modular Exponentiation (choose the best Montgomery for this)
3. IsOdd (to be implemented in uint_t libraries directly)
4. IsEven (to be implemented in uint_t libraries directly)
5. Subtract (to be implemented in uint_t libraries directly)

**Note:** All the other details for the implementation of the sqrt-permuatation can be found in *Research* folder (i.e. requirements for Montgomery, all the other operations needed for sqrt-permutation in low-level, etc...).