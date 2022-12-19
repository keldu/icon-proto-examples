# Error handling  

C interface not yet pushed, but basically you save an `int64_t` on your side after registering an error with e.g

```
  int64_t register_new_error(char* error_name);
```
