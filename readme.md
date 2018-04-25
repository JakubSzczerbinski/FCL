
# FCL

FCL stands for function compostition library and also for function composition language. I will refere form now on to FCL(library) as FCLlib, and to FCL(language) as FCLlang. FCLlib is a library for C++ that allows execute function composition of C++ functions at runtime. FCLlang is language that allows to define graph of function composition.

# FCLlang
In FCLlang all we is define a DAG that represents function composition. For example if I wanted to evaluate this expression written in C++:
~~~
int_mult(int_add(1, 2), int_add(1, 2))
~~~

We would write in FCL:
~~~
int one = '1'
int two = '2'
int_add_node = int_add(one.0, two.0)
int_mult_node = int_mult(int_add_node.0, int_add_node.0)
return mult_node.0
~~~

And the graph looks like this:
~~~
|-------|
| one   |    
|-------|    |------------|    |-------------|
|   int*|\   |int_add_node|    |int_mult_node|
|-------| \  |------------|    |-------------|
           \-|*int        |  /-|*int         |
|-------|  /-|*int        | /--|*int         |
| two   | /  |        int*|/   |         int*|
|-------|/   |------------|    |-------------|
|   int*|
|-------|
~~~

So lets take a look at the FCL code.
~~~
int one = '1'
int two = '2'
~~~

In first line we do two things, we create a value function (constant function that take 0 arguments) that returns int which is equal to 1 and then we create node using this function. We do almost in the second the same except the returned value is 2.

~~~
int_add_node = int_add(one.0, two.0)
int_mult_node = int_mult(int_add_node.0, int_add_node.0)
~~~

Now we can use our values (value functions) to calculate some 'real' functions. So we create node 'int_add_node' defined by int_add function and two inputs 'one.0' and 'two.0'. Inputs describe which value should node's function use as arguments during evalution, in this case its 0-th value in result of evaluating node 'one' and 0-th value in result of evaluating node 'two'. This means that arguments for 'int_add' during evaluation will be 1 and 2 nd the result of evaluating 'int_add_node' will be (3). We do the same 'int_mult_node' function we use is 'int_mult' and the both inputs are 0-th value in result of evaluating 'int_add_node'.