# Minor benchmark tests  

This benchmark generally is a bit shakey.  
On my system clang++ doesn't optimize the simple map access. g++ does.  

```
Amount of elements: 112210
Time in ns
                     Regular Map                       Index Map        Index Map with ID access
                       262562920                       262572644                           93491
                            100%                        100.004%                      0.0356071%
```
