# Mathify Grammar
the space, tab and new line characters are all ignored by the compiler and therefore play no effect in the compiler

## Expressions
An expression is a constant value that can be used by mathify at compile time.
All expressions are prefixed with a type, followed by a base which is finished out with a collection of 0-9 and a-z/A-Z characters <br>
A full Expression would look like
```
id 1000 ## signed integer
ud 1000 ## unsigned integer
fd 1000 ## float
```

#### Type
###### Signed Integers
A value stored by the compiler as the register size (i.e the largest single signed value)
In most cases this results to a 64 bit value.
<br>
In the future there could be development for a better integeral system which could result in values above 64 bit.
This is however a more complex task then what is currently needed here.
<br>
To create a signed integer value then (i/I) can be used as a prefix then followed by the base character.

###### Unsigend Integers
Just read the above
<br>
To create an unsigned integer value then (u/U) can be used as a prefix then followed by the base character.

###### Floating
In some cases systems can support a 10 byte or 16 byte float value but this shouldn't be relied on.
This should for the most part be assumed a 8 byte float value unless special knowledge of your system is reliable.
The rest follows the rules of the above with some operands maybe not supporting this type (i.e modulus)
(aparrently c doesn't support this i am not entirely sure why but maybe it will be emulated by us in the future)
<br>
To create an unsigned integer value then (f/F) can be used as a prefix then followed by the base character.

#### Base
The base prefix is the value immediatly after the type correlating to a value between 1 and 36

###### Special
The special prefixes are as follows <br>
<br>
b a shorthand for binary / base 2 <br>
o a shorthand for octal / base 8 <br>
d a shorthand for decimal / base 10 <br>
x a shorthand for hexadecimal / base 16 <br>

###### Default
The other prefixes are based on the max value supported by the base
i.e 1 is base 2 meaning the values 0 and 1 can be used.
this follows for A,B,C,D,etc... where F would be equivalant to hexadecimal and 9 equivalent to decimal

#### Value
The value is then as stated above a value consisting of characters 0-9, a-z and A-Z.

## Variables
A string of characters for some value

#### Mathify
In Progress...

#### Runtime
In Progress...

## Functions

#### Mathify
In Progress...

#### Runtime
In Progress...
