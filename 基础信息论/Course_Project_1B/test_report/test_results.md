# Test Results

## Shannon codes

### Chinese texts

`war_and_peace_cn.txt`

- Information source entropy: 7.9203 bits
- Average codeword length: 8.3680 bits/symbol
- Encoding efficiency: 94.65%
- Decoding time (seconds)
  - results of 5 tests

    |1|2|3|4|5|
    |-|-|-|-|-|
    |0.004598|0.005057|0.005708|0.004568|0.005826|

  - average: 0.005151

`zhaoxia_cn.txt`

- Information source entropy: 8.0812 bits
- Average codeword length: 8.4022 bits/symbol
- Encoding efficiency: 96.18%
- Decoding time (seconds)
  - results of 5 tests

    |1|2|3|4|5|
    |-|-|-|-|-|
    |0.001554|0.001999|0.002099|0.001871|0.001630|

  - average: 0.001831

`quantum_cn.txt`

- Information source entropy: 8.1755 bits
- Average codeword length: 8.7180 bits/symbol
- Encoding efficiency: 93.78%
- Decoding time (seconds)
  - results of 5 tests

    |1|2|3|4|5|
    |-|-|-|-|-|
    |0.003631|0.004001|0.003520|0.002996|0.003756|

  - average: 0.003581

### English texts

`proust_en.txt`

- Information source entropy: 4.2701 bits
- Average codeword length: 4.8120 bits/symbol
- Encoding efficiency: 88.74%
- Decoding time (seconds)
  - results of 5 tests

    |1|2|3|4|5|
    |-|-|-|-|-|
    |0.003204|0.003088|0.003502|0.003554|0.003509|

  - average: 0.003371

`alphabetia_en.txt`

- Information source entropy: 4.6123 bits
- Average codeword length: 5.0929 bits/symbol
- Encoding efficiency: 90.56%
- Decoding time (seconds)
  - results of 5 tests

    |1|2|3|4|5|
    |-|-|-|-|-|
    |0.001811|0.002379|0.002000|0.002666|0.001502|

  - average: 0.002072

`Steve_Jobs_Speech_en.txt`

- Information source entropy: 4.3870 bits
- Average codeword length: 4.7736 bits/symbol
- Encoding efficiency: 91.90%
- Decoding time (seconds)
  - results of 5 tests

    |1|2|3|4|5|
    |-|-|-|-|-|
    |0.021007|0.014702|0.013281|0.014235|0.014085|

  - average: 0.015462

## Huffman codes

### Binary

#### Chinese texts

`war_and_peace_cn.txt`

- Information source entropy: 7.9203 bits
- Average codeword length: 7.9582 bits/symbol
- Encoding efficiency: 99.52%
- Decoding time: 0.005460s

`zhaoxia_cn.txt`

- Information source entropy: 8.0812 bits
- Average codeword length: 8.1043 bits/symbol
- Encoding efficiency: 99.71%
- Decoding time: 0.002007s

`quantum_cn.txt`

- Information source entropy: 8.1755 bits
- Average codeword length: 8.2099 bits/symbol
- Encoding efficiency: 99.58%
- Decoding time: 0.002342s

#### English texts

`proust_en.txt`

- Information source entropy: 4.2701 bits
- Average codeword length: 4.3133 bits/symbol
- Encoding efficiency: 99.00%
- Decoding time: 0.003362s

`alphabetia_en.txt`

- Information source entropy: 4.6123 bits
- Average codeword length: 4.6480 bits/symbol
- Encoding efficiency: 99.23%
- Decoding time: 0.001315s

`Steve_Jobs_Speech_en.txt`

- Information source entropy: 4.3870 bits
- Average codeword length: 4.4281 bits/symbol
- Encoding efficiency: 99.07%
- Decoding time: 0.013029s

### Ternary

#### Chinese texts

`war_and_peace_cn.txt`

- Information source entropy: 4.9972 bits
- Average codeword length: 5.0384 bits/symbol
- Encoding efficiency: 99.18%
- Decoding time: 0.003550s

`zhaoxia_cn.txt`

- Information source entropy: 5.0987 bits
- Average codeword length: 5.1511 bits/symbol
- Encoding efficiency: 98.98%
- Decoding time: 0.000762s

`quantum_cn.txt`

- Information source entropy: 5.1582 bits
- Average codeword length: 5.1963 bits/symbol
- Encoding efficiency: 99.27%
- Decoding time: 0.001408s

#### English texts

`proust_en.txt`

- Information source entropy: 2.6941 bits
- Average codeword length: 2.7681 bits/symbol
- Encoding efficiency: 97.33%
- Decoding time: 0.002276s

`alphabetia_en.txt`

- Information source entropy: 2.9100 bits
- Average codeword length: 2.9626 bits/symbol
- Encoding efficiency: 98.23%
- Decoding time: 0.001619s

`Steve_Jobs_Speech_en.txt`

- Information source entropy: 2.7679 bits
- Average codeword length: 2.8320 bits/symbol
- Encoding efficiency: 97.74%
- Decoding time: 0.009434s

### Quaternary

#### Chinese texts

`war_and_peace_cn.txt`

- Information source entropy: 3.9602 bits
- Average codeword length: 4.0226 bits/symbol
- Encoding efficiency: 98.45%
- Decoding time: 0.002999s

`zhaoxia_cn.txt`

- Information source entropy: 4.0406 bits
- Average codeword length: 4.0870 bits/symbol
- Encoding efficiency: 98.87%
- Decoding time: 0.005518s

`quantum_cn.txt`

- Information source entropy: 4.0878 bits
- Average codeword length: 4.1429 bits/symbol
- Encoding efficiency: 98.67%
- Decoding time: 0.001597s

#### English texts

`proust_en.txt`

- Information source entropy: 2.1351 bits
- Average codeword length: 2.1795 bits/symbol
- Encoding efficiency: 97.96%
- Decoding time: 0.002959s

`alphabetia_en.txt`

- Information source entropy: 2.3062 bits
- Average codeword length: 2.3745 bits/symbol
- Encoding efficiency: 97.12%
- Decoding time: 0.001698s

`Steve_Jobs_Speech_en.txt`

- Information source entropy: 2.1935 bits
- Average codeword length: 2.2443 bits/symbol
- Encoding efficiency: 97.74%
- Decoding time: 0.007618s
