
## DATA

Seems like std::unique_ptr seems gucci here.

- type encapsulates data
- gives void* to users of data.
- ensures that data is allocated once by system
- ensures deletion of data

## FUNCTION

- recieves pointers to input and output args
- calls underlying function (which is pure)
- return result in output args

## NODE

- encapsulates function
- ensures that function is called with proper args
- gives out NodeLinks which allow to get data from parent functions

## NODELINK

- allow to acess output data of node
- keeps track of all accessors of node's data