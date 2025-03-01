# Practical Secure Conjunctive Query Revisited

Xinle Cao, Weiqi Feng, Rui Zhang, Quanqing Xu, Chuanhui Yang, and Jian Liu.

(**Abstract**) Encrypted database (EDB) has been a prominent focus in the database community with the rise of cloud computing. However, many challenging open problems in EDBs make most existing solutions undeployable in real-world applications. One significant and well-known issue is how to avoid dangerous unexpected leakages when processing conjunctive queries. While the minimal leakage should be only the result of the conjunctive query, all existing EDBs cannot achieve such a strict (but necessary) security guarantee in a practical way: they usually require unacceptable communication or calculation overhead. In this work, we aim to design new *stateless* and *non-interactive* encryption schemes to support the strict security guarantee with practicality. Our schemes significantly improve both the theoretical complexity and actual performance on a series of functionalities, including single/multi-value filter, equality test, and even equi-join. Besides, they enable multiple essential properties for applicability, e.g., &#9312; they allow the database to be *dynamic* in attributes (i.e., adding attributes and deleting attributes); &#9313; they can be combined arbitrarily to support *composable* and *flexible* functionalities in conjunctive queries for extending their applications in practice. We evaluate our schemes by comparing them with two SOTA works, the results show that our schemes indeed outperform prior works in both efficiency and practicality for deployment.

## Project Structure

- The [experiment](experiment) folder consists of files to reproduce experimental results that we report in the paper.
- The [include](include) folder consists of header files of our implementations.
    - The files with the `ipe` prefix are for the $\textsf{FE}$ methods.
    - The files with the `sse` prefix are for the $\textsf{SSE}$ methods.
    - The files without prefix are for our proposed methods.
- The [src](src) folder consists of source files of our implementations.
- The [test](test) folder consists of test cases for validating correctness of our implementations.

## How to run this code
Our code base depends on the [RELIC library](https://github.com/relic-toolkit/relic) and the [LibRBP library](https://github.com/WeiqiNs/LibRBP); note that our experiment is done on the BLS12-381 curve. One could refer to the [Dockerfile](Dockerfile) to see how to create the dev environment (or use it to build a container directly).
