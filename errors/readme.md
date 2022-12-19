# Error handling  

C interface not yet pushed, but basically you save an `int64_t` on your side after registering an error with e.g

```
  /**
   * register a new error_type
   */
  int64_t register_new_error(char* error_name, bool is_critical);
  
  /**
   * register a new error_type
   */
  char* get_error_name(int64_t error_id);
  
  /**
   * get error_string
   */
  int get_error_string(size_t* length, char** str);
```
