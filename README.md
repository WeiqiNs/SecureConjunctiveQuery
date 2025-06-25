# Practical Secure Conjunctive Query Revisited

Xinle Cao, Weiqi Feng, Quanqing Xu, Chuanhui Yang, Rui Zhang, Jinfei Liu, and Jian Liu.

(**Abstract**) Encrypted databases (EDBs) have emerged as a promising research direction within the database community over the past two decades. Designed to ensure both data confidentiality and functionality in cloud databases, EDBs introduce new challenges in secure database design. One critical challenge is how to support conjunctive queries over multiple attributes securely and practically. In this paper, we propose a novel encrypted database system, FlexDB, tailored for secure conjunctive query processing. Built upon a set of newly devised encryption schemes, FlexDB supports a wide range of query functionalities, including single/multi-value filter, equi-join, equality test, and even range query. More importantly, it achieves strong security guarantees by ensuring that intermediate results on individual attributes remain hidden throughout query execution. At the same time, FlexDB offers improved theoretical complexity and practical performance in both storage overhead and computational efficiency. These advances in functionality, security, and efficiency make FlexDB a highly practical solution for secure conjunctive query processing in EDBs. We validate these improvements through extensive experiments on multiple real-world datasets.

#### The full version of the paper is provided [here](Full_Version_Practical_Secure_Conjunctive_Query_Revisited.pdf).

## Project Structure
- The [experiment](experiment) folder consists of files to reproduce experimental results that we report in the paper.
- The [include](include) folder consists of header files of our implementations.
    - The files with the `ipe` prefix are for the constructions introduced by Shafieinejad et al. in this [paper](https://arxiv.org/pdf/2103.05792).
    - The files with the `flexdb` prefix are for the $\textsf{FlexDB}$ constructions.
- The [src](src) folder consists of source files of our implementations.
- The [test](test) folder consists of test cases for validating correctness of our implementations.

## How to run this code
Our code base depends on the [RELIC library](https://github.com/relic-toolkit/relic) and the [LibRBP library](https://github.com/WeiqiNs/LibRBP); note that our experiment is done on the BLS12-381 curve. One could refer to the [Dockerfile](Dockerfile) to see how to create the dev environment (or use it to build a container directly).


## Testing

[![SCQ CI](https://github.com/WeiqiNs/SecureConjunctiveQuery/actions/workflows/ci.yml/badge.svg)](https://github.com/WeiqiNs/SecureConjunctiveQuery/actions/workflows/ci.yml)

To support continuous development in this research area, we maintain this codebase with a suite of unit tests integrated into our GitHub CI workflow. One could also refer to the [`ci.yml`](.github/workflows/ci.yml) file for details on setting up the dev environment.
