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

```
char* error_category_name = "some_error";
int64_t some_error_id = 0;

int main(){
  some_error_id = register_new_error(error_category_name);
  
  int64_t rc = call_some_function_which_can_return_error();
  
  size_t len = 0;
  char* str = 0L;
  
  if(get_error_string(&len, &str)){
    exit(-1);
  }
  
  printf("%s %s", error_category_name, err_str);
  return 0;
}
```
