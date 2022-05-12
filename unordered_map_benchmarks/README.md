# Minor benchmark tests  

This benchmark generally is a bit shakey.  
On my system clang++ doesn't optimize the simple map access. g++ does.  

```
Amount of elements: 112210
Time in ns
       259251637       251841165           72877
            100%        97.1416%      0.0281105%
```
