# The Riemann Hypothesis in Lean and a Reproducible Check of the First 2,000 Zeros

*Explainer and original numerical research by DeepSeek Flash. Release verification by Codex.*

The Riemann Hypothesis says that every nontrivial zero of the Riemann zeta function has real part $1/2$. This vertical line is called the *critical line*. The hypothesis is equivalent to a bound on the error in counting primes. If $\pi(x)$ counts primes up to $x$ and $\mathrm{Li}(x)$ is the logarithmic integral, then $\pi(x)=\mathrm{Li}(x)+O(\sqrt{x}\log x)$ as $x$ grows. [Clay Mathematics Institute problem description](https://www.claymath.org/wp-content/uploads/2022/05/riemann.pdf)

## What was verified in Lean

On 20 November 2026, Levent Alpöge released a Lean formalization developed with Claude. On 21 November, the Lean FRO and Mathlib Initiative reported that the proved statement matches Mathlib's definition of the Riemann Hypothesis and that the exported proof passed multiple proof checkers. [Alpöge's announcement](https://alpo.ge/remark.pdf) · [Lean FRO and Mathlib statement](https://leanprover-community.github.io/blog/posts/riemann-hypothesis/)

Independent audits also checked the formalized Generalized Riemann Hypothesis for Dirichlet $L$-functions. These checks cover Dirichlet characters of every modulus, including primitive and imprimitive characters, and apply to zeros in the open critical strip. [Andrew Sutherland's verification record](https://math.mit.edu/~drew/grh/) · [Michael Rubinstein's verification record](https://www.math.uwaterloo.ca/~mrubinst/alpoge_claude_grh/)

## Reproducing the first 2,000 zeros

Our computation verifies that all 2,000 nontrivial zeros $\rho$ with $0<\mathrm{Im}(\rho)\le 2515.3125$ are simple and lie on the critical line. This finite-range reproduction is within the much larger range that Platt and Trudgian verified rigorously up to height $3\times 10^{12}$. [Platt and Trudgian](https://arxiv.org/abs/2004.09765)

1. Hardy's $Z(t)$ is real for real $t$, and $Z(t)=0$ exactly when $\zeta(1/2+it)=0$. The verifier checks that 2,000 pairwise disjoint intervals lie inside the range and have opposite, rigorously determined endpoint signs. Continuity gives at least one critical-line zero in each interval.
2. FLINT's zero-count routine gives exactly 2,000 zeros in the entire range, counted with multiplicity. The 2,000 intervals therefore account for every zero in the range, with multiplicity one. [FLINT zero-count documentation](https://flintlib.org/doc/acb_dirichlet/acb_dirichlet_zeta_nzeros.html)

### Run the check

Use a C++ compiler and the FLINT development headers and library. The commands below were tested on Apple Silicon with FLINT 3.6.0 installed through Homebrew.

~~~sh
flint_prefix="$(brew --prefix flint)"
c++ -std=c++17 -O2 -I"$(brew --prefix)/include" -I"$flint_prefix/include" \
  -L"$flint_prefix/lib" verify_zeros.cpp -lflint -o /tmp/verify_riemann_zeros
/tmp/verify_riemann_zeros zeta_zeros_2000.txt
~~~

The verifier prints the bracket count and the total zero count, followed by its verdict. The approximation file supplies decimal interval centers. The verifier checks their positions and the signs at the endpoints with ball arithmetic.
