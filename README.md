
# Reduce Leakages in Query Processing on Encrypted Multi-dimensional Data with Practicality

Xinle Cao, Weiqi Feng, Quanqing Xu, Chuanhui Yang, Jian Liu and Kui Ren.

(**Abstract**) Encrypted databases (EDBs) have been a prominent focus in the database community with the rise of cloud computing. However, many challenging open problems in EDBs make most existing solutions undeployable in real-world applications. One significant issue is how to avoid dangerous unexpected leakages when querying encrypted multi-dimensional data. For example, given one query SELECT $user\_id$ FROM $T$ WHERE $\mathsf{Age}=30$ AND $\mathsf{Salary}=1000$, the minimal leakage should be the records that satisfy both conditions. However, most existing EDBs reveal separate frequency leakages: 1) records that satisfy $\mathsf{Age}=30$, and 2) records that satisfy $\mathsf{Salary} = 1000$. As numerous works have shown, frequency leakage is very dangerous for EDBs, making it crucial to reduce additional frequency leakage when querying multi-dimensional data. In this paper, we build upon a line of works, including the SOTA work in ICDE 2022, to address the additional leakage. Compared to prior works, our proposed approach elegantly eliminates leakage in a more efficient and flexible manner. The experimental results demonstrate that our work outperforms the SOTA work in a series of common queries over multi-dimensional data, such as equality tests on a single table and equi-joins over two tables where each table consists of multiple columns to be filtered.

The full version of the paper is listed in this repo [here](Full_version_Towards_Practical_Oblivious_Map.pdf). There are two methods implemented here, which are applied to different operations with different minor modifications.

## FixSch (ICDE 2022)

- [FixSch](https://www.computer.org/csdl/proceedings-article/icde/2022/088300b635/1FwFv0bDBba), which is
  implemented [here](include/ipfe_join.hpp) and [here](include/ipfe_filter.hpp).

## Our method FleSch

- The filter method is implemented [here](include/filter.hpp) and the join method is implemented [here](include/join.hpp).

## Project Structure

- The [experiments](experiments) folder consists of files to run experiments that we report in the paper.
- The [include](include) folder consists of header files of our implementations.
- The [src](src) folder consists of source files of our implementations. 
- The [tests](tests) folder consists of test cases for validating correctness of our implementations.

## How to run this code

You need to first install the RELIC library; note that our experiment is done on the BLS12-381 curve. One could refer to the [Dockerfile](Dockerfile) to see how to setup the dev environment (or use it to build a container directly).
