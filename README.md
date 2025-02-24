# Reduce Leakages in Query Processing on Encrypted Multi-dimensional Data with Practicality

Xinle Cao, Weiqi Feng, Rui Zhang, Quanqing Xu, Chuanhui Yang, and Jian Liu.

(**Abstract**) Encrypted databases (EDBs) have been a prominent focus in the database community with the rise of cloud computing. However, many challenging open problems in EDBs make most existing solutions undeployable in real-world applications. One significant issue is how to avoid dangerous unexpected leakages when querying encrypted multi-dimensional data. For example, given one query SELECT $user\_id$ FROM $T$ WHERE $\mathsf{Garde}=\mathsf{A}$ AND $\mathsf{Depart}= \mathsf{CS}$, the minimal leakage should be the records that satisfy both the two conditions. However, most existing EDBs reveal separate frequency leakages: 1) records that satisfy $\mathsf{Garde}=\mathsf{A}$, and 2) records that satisfy $\mathsf{Depart} = \mathsf{CS}$. This incurs additional leakage of frequency information which has been widely recognized as very sensitive and dangerous information in EDBs.

To this end, this work aims to reduce leakages as much as possible when querying multi-dimensional data. Specifically, we propose several new novel constructions to guarantee only minimal leakages revealed in a series of common queries over multi-dimensional data such as filtering single/multiple values within multiple columns, equi-join, and general equality test. We evaluate our schemes by comparing them with a naive (insecure) baseline and two SOTA works, the results show that our schemes indeed outperform prior works in both efficiency and practicality in deployment.

## Project Structure

- The [experiment](experiment) folder consists of files to run experiments that we report in the paper.
- The [include](include) folder consists of header files of our implementations.
- The [src](src) folder consists of source files of our implementations. 
- The [test](test) folder consists of test cases for validating correctness of our implementations.

## How to run this code
You need to first install the RELIC library and the LibRBP library; note that our experiment is done on the BLS12-381 curve. One could refer to the [Dockerfile](Dockerfile) to see how to create the dev environment (or use it to build a container directly).
