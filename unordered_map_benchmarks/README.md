# Comparison of an indexed hash map and a regular hash map  

This benchmark generally is a bit shakey.  
On my system clang++ doesn't optimize the simple map access. g++ does.  

Only concerning single thread operations.  

## Benchmarks  

```
Amount of elements: 112210
Time in ns
                Regular Hash Map                  Index Hash Map   Index Hash Map with ID access
                       262562920                       262572644                           93491
                            100%                        100.004%                      0.0356071%
```
```
Amount of elements: 112210
Time in ns
                Regular Hash Map                  Index Hash Map   Index Hash Map with ID access
                       257532845                       255340067                           74469
                            100%                        99.1485%                      0.0289163%
```
```
Amount of elements: 1030300
Time in ns
                Regular Hash Map                  Index Hash Map   Index Hash Map with ID access
                     65765740218                     66591373408                          742819
                            100%                        101.255%                     0.00112949%
```
```
Amount of elements: 1030300
Time in ns
                Regular Hash Map                  Index Hash Map   Index Hash Map with ID access
                     65742156922                     67014876284                          663718
                            100%                        101.936%                     0.00100958%
```
