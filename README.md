
# Reduce Leakages in Query Processing on Encrypted Multi-dimensional Data with Practicality

Xinle Cao, Weiqi Feng, Quanqing Xu, Chuanhui Yang, Jian Liu and Kui Ren.

(**Abstract**) Encrypted databases (EDBs) have been a prominent focus in the database community with the rise of cloud computing. However, many challenging open problems in EDBs make most existing solutions undeployable in real-world applications. One significant issue is how to avoid dangerous unexpected leakages when querying encrypted multi-dimensional data. For example, given one query SELECT $user\_id$ FROM $T$ WHERE $\mathsf{Age}=30$ AND $\mathsf{Salary}=1000$, the minimal leakage should be the records that satisfy both conditions. However, most existing EDBs reveal separate frequency leakages: 1) records that satisfy $\mathsf{Age}=30$, and 2) records that satisfy $\mathsf{Salary} = 1000$. As numerous works have shown, frequency leakage is very dangerous for EDBs, making it crucial to reduce additional frequency leakage when querying multi-dimensional data. In this paper, we build upon a line of works, including the SOTA work in ICDE 2022, to address the additional leakage. Compared to prior works, our proposed approach elegantly eliminates leakage in a more efficient and flexible manner. The experimental results demonstrate that our work outperforms the SOTA work in a series of common queries over multi-dimensional data, such as equality tests on a single table and equi-joins over two tables where each table consists of multiple columns to be filtered.

The full version of the paper is listed in this repo [here](Full_version_Towards_Practical_Oblivious_Map.pdf). There are two methods implemented here, which are applied to different operations with different minor modifications.

## FixSch (ICDE 2022)

- [Insecure Freecursive (ASPLOS 2015)](https://people.csail.mit.edu/devadas/pubs/freecursive.pdf), which is
  implemented [here](orams/freecursive_oram.py). The `reset_method` needs to be set to `"hard"`.
- [Secure Freecursive with probabilistic resets (TCC 2017)](https://eprint.iacr.org/2016/1084), which is
  implemented [here](orams/freecursive_oram.py). The `reset_method` needs to be set to `"prob"`.
- The proposed DAORAM with deterministic resets, which is implemented [here](orams/da_oram.py).

## Our method FleSch

- [OMAP based on AVL (CCS 2014)](https://dl.acm.org/doi/10.1145/2660267.2660314), which is
  implemented [here](omaps/avl_ods_omap.py). The `distinguishable_search` needs to be set to `False`.
- [OMAP based on optimized AVL (VLDB 2024)](https://www.vldb.org/pvldb/vol16/p4324-chamani.pdf), which is
  implemented [here](omaps/avl_ods_omap.py). The `distinguishable_search` needs to be set to `True`.
- [OMAP based on the B+ tree (VLDB 2020)](https://people.eecs.berkeley.edu/~matei/papers/2020/vldb_oblidb.pdf), which is
  implemented [here](omaps/bplus_ods_omap.py).
- The proposed OMAP framework is implemented [here](omaps/oram_ods_omap.py). It can be instantiated with any ORAM class
  contained in this repo combined with any OMAP class in this repo.

The open-sourced repositories we used as reference for the OMAP based on AVL tree and OMAP based on B+ tree are
here: [AVL](https://github.com/obliviousram/oblivious-avl-tree) [B+ tree](https://github.com/SabaEskandarian/ObliDB).

## Project Structure

- The [demo](demo) folder consists of demonstrations of how to use socket to set up ORAM/OMAP server and client.
- The [dependency](dependency) folder consists of some dependencies used including the socket, cryptography, etc.
- The [omaps](omaps) folder consists of all the OMAP constructions considered.
- The [orams](orams) folder consists of all the ORAM constructions considered.
- The [tests](tests) folder consists of test cases for validating correctness of our implementations.

## How to run this code

You need to first install the package listed in [`requirements.txt`](requirements.txt). If you want to run the schemes
with "local server", sample usages can be found in [`tests/test_orams.py`](tests/test_orams.py)
or [`tests/test_omaps.py`](tests/test_omaps.py). If you wish to set up a remote server, you should first
run [`demo/server.py`](demo/server.py) on the server and then run [`demo/oram_client.py`](demo/oram_client.py)
or [`demo/oram_client.py`](demo/omap_client.py) on your client device.
