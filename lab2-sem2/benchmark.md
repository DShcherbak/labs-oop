## Benchmark results
Used [Google Benchmark](https://github.com/google/benchmark "Sourcecode repository")

`Run on (8 X 1800 MHz CPU s)`

Every time measurement includes vector of keys' iteration

| Size of vector | Time      |
| -------------- | --------- |
| 1              | 27.6 ns   |
| 2              | 33.7 ns   |
| 4              | 48.7 ns   |
| 8              | 80.2 ns   |
| 16             | 146 ns    |
| 32             | 267 ns    |
| 64             | 0.001 ms  |
| 128            | 0.001 ms  |
| 256            | 0.002 ms  |
| 512            | 0.004 ms  |
| 1024           | 0.008 ms  |
| 2048           | 0.015 ms  |
| 4096           | 0.031 ms  |
| 8192           | 0.061 ms  |
| 16384          | 0.125 ms  |

---

### Insertion
Time taken to insert keys to empty tree

| Number of keys | Time (Red-Black tree) | Time (B-tree) |
| -------------- | --------------------- |---------------|
| 1              | 295 ns                | 5190 ns       |
| 2              | 586 ns                | 10562 ns      |
| 4              | 1145 ns               | 20882 ns      |
| 8              | 2356 ns               | 41746 ns      |
| 16             | 4878 ns               | 81313 ns      |
| 32             | 10252 ns              | 164840 ns     |
| 64             | 0.021 ms              | 0.327 ms      |
| 128            | 0.042 ms              | 0.652 ms      |
| 256            | 0.086 ms              | 1.34 ms       |
| 512            | 0.177 ms              | 2.65 ms       |
| 1024           | 0.357 ms              | 5.10 ms       |
| 2048           | 0.773 ms              | 10.6 ms       |
| 4096           | 1.64 ms               | 21.2 ms       |
| 8192           | 3.90 ms               | 41.2 ms       |
| 16384          | 11.4 ms               | 83.6 ms       |

---

### Removal
Time taken to remove all keys from tree (key is chosen randomly)

| Number of keys | Time (Red-Black tree) | Time (B-tree) |
| -------------- | --------------------- |---------------|
| 1              | 59.9 ns               | 173 ns        |
| 2              | 103 ns                | 348 ns        |
| 4              | 204 ns                | 664 ns        |
| 8              | 376 ns                | 1289 ns       |
| 16             | 724 ns                | 2544 ns       |
| 32             | 1413 ns               | 5049 ns       |
| 64             | 0.003 ms              | 0.010 ms      |
| 128            | 0.006 ms              | 0.020 ms      |
| 256            | 0.011 ms              | 0.040 ms      |
| 512            | 0.022 ms              | 0.080 ms      |
| 1024           | 0.044 ms              | 0.161 ms      |
| 2048           | 0.089 ms              | 0.323 ms      |
| 4096           | 0.187 ms              | 0.657 ms      |
| 8192           | 0.355 ms              | 1.35 ms       |
| 16384          | 0.711 ms              | 2.89 ms       |

---

### Search

Time taken to find all keys one by one in tree (key is chosen randomly)

| Number of keys | Time (Red-Black tree) | Time (B-tree) |
| -------------- | --------------------- |---------------|
| 1              | 60.7 ns               | 178 ns        |
| 2              | 111 ns                | 358 ns        |
| 4              | 215 ns                | 1357 ns       |
| 8              | 437 ns                | 2822 ns       |
| 16             | 929 ns                | 8836 ns       |
| 32             | 1980 ns               | 24362 ns      |
| 64             | 0.004 ms              | 0.061 ms      |
| 128            | 0.010 ms              | 0.127 ms      |
| 256            | 0.022 ms              | 0.307 ms      |
| 512            | 0.054 ms              | 0.732 ms      |
| 1024           | 0.125 ms              | 1.70 ms       |
| 2048           | 0.285 ms              | 3.79 ms       |
| 4096           | 0.631 ms              | 7.76 ms       |
| 8192           | 1.42 ms               | 17.2 ms       |
| 16384          | 3.06 ms               | 37.4 ms       |
