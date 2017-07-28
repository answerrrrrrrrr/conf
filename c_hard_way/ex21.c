Learn C The Hard Way

Online Video Course Plus PDFs $29

Python | Ruby | C | SQL | Regex

# Exercise 21: Advanced Data Types And Flow Control

This exercise will be a complete compendium of the available C data types and flow control structures you can use. It will work as a reference to complete your knowledge, and won't have any code for you to enter. I'll have you memorize some of the information by creating flash cards so you can get the important concepts solid in your mind.

For this exercise to be useful, you should spend at least a week hammering the content and filling out all the element I have missing here. You'll be writing out what each one means, and then writing a program to confirm what you've researched.

# Available Data Types

int
    Stores a regular integer, defaulting to 32 bits in size.
double
    Holds a large floating point number.
float
    Holds a smaller floating point number.
char
    Holds a single 1 byte character.
void
    Indicates "no type" and used to say a function returns nothing, or a pointer has no type as in void *thing.
enum
    Enumerated types, work as integers, convert to integers, but give you symbolic names for sets. Some compilers will warn you when you don't cover all elements of an enum in switch-statements.

## Type Modifiers

unsigned
    Changes the type so that it does not have negative numbers, giving you a larger upper bound but nothing lower than 0.
signed
    Gives you negative and positive numbers, but halves your upper bound in exchange for the same lower bound negative.
long
    Uses a larger storage for the type so that it can hold bigger numbers, usually doubling the current size.
short
    Uses smaller storage for the type so it stores less, but takes half the space.

## Type Qualifiers

const
    Indicates the variable won't change after being initialized.
volatile
    Indicates that all bets are off, and the compiler should leave this alone and try not to do any fancy optimizations to it. You usually only need this if you're doing really weird stuff to your variables.
register
    Forces the compiler to keep this variable in a register, and the compiler can just ignore you. These days compilers are better at figuring out where to put variables, so only use this if you actually can measure it improving the speed.

## Type Conversion

C uses a sort of "stepped type promotion" mechanism, where it looks at two operands on either side of an expression, and promotes the smaller side to match the larger side before doing the operation. If one side of an expression is on this list, then the other side is converted to that type before the operation is done, and this goes in this order:

   long double
   double
   float
   int (but only char and short int);
   long

If you find yourself trying to figure out how your conversions are working in an expression, then don't leave it to the compiler. Use explicit casting operations to make it exactly what you want. For example, if you have:

long + char - int * double

Rather than trying to figure out if it will be converted to double correctly, just use casts:

(double)long - (double)char - (double)int * double

Putting the type you want in parenthesis before the variable name is how you force it into the type you really need. The important thing though is always promote up, not down. Don't cast long into char unless you know what you're doing.

## Type Sizes

The stdint.h defines both a set of typdefs for exact sized integer types, as well as a set of macros for the sizes of all the types. This is easier to work with than the older limits.h since it is consistent. The types defined are:

int8_t
    8 bit signed integer.
uint8_t
    8 bit unsigned integer.
int16_t
    16 bit signed integer.
uint16_t
    16 bit unsigned integer.
int32_t
    32 bit signed integer.
uint32_t
    32 bit unsigned integer.
int64_t
    64 bit signed integer.
uint64_t
    64 bit unsigned integer.

The pattern here is of the form (u)int(BITS)_t where a u is put in front to indicate "unsigned", then BITS is a number for the number of bits. This pattern is then repeated for macros that return the maximum values of these types:

INT(N)_MAX
    Maximum positive number of the signed integer of bits (N), such as INT16_MAX.
INT(N)_MIN
    Minimum negative number of signed integer of bits (N).
UINT(N)_MAX
    Maximum positive number of unsigned integer of bits (N). Since it's unsigned the minimum is 0 and can't have a negative value.

Warning

Pay attention! Do not go looking for a literal INT(N)_MAX definition in any header file. I'm using the (N) as a placeholder for any number of bits your platform currently supports. This (N) could be any number, 8, 16, 32, 64, even maybe 128. I use this notation in this exercise so that I don't have to literally write out every possible combination.

There are also macros in stdint.h for sizes of the size_t type, integers large enough to hold pointers, and other handy size defining macros. Compilers have to at least have these, and then they can allow other larger types.

Here is a full list should be in stdint.h:

int_least(N)_t
    holds at least (N) bits.
uint_least(N)_t
    holds at least (N) bits unsigned.
INT_LEAST(N)_MAX
    max value of the matching least (N) type.
INT_LEAST(N)_MIN
    min value of the matching least (N) type.
UINT_LEAST(N)_MAX
    unsigned maximum of the matching (N) type.
int_fast(N)_t
    similar to int_least*N*_t but asking for the "fastest" with at least that precision.
uint_fast(N)_t
    unsigned fastest least integer.
INT_FAST(N)_MAX
    max value of the matching fastest (N) type.
INT_FAST(N)_MIN
    min value of the matching fastest (N) type.
UINT_FAST(N)_MAX
    unsigned max value of the matching fastest (N) type.
intptr_t
    a signed integer large enough to hold a pointer.
uintptr_t
    an unsigned integer large enough to hold a pointer.
INTPTR_MAX
    max value of a intptr_t.
INTPTR_MIN
    min value of a intptr_t.
UINTPTR_MAX
    unsigned max value of a uintptr_t.
intmax_t
    biggest number possible on that system.
uintmax_t
    biggest unsigned number possible.
INTMAX_MAX
    largest value for the biggest signed number.
INTMAX_MIN
    smallest value for the biggest signed number.
UINTMAX_MAX
    largest value for the biggest unsigned number.
PTRDIFF_MIN
    minimum value of ptrdiff_t.
PTRDIFF_MAX
    maximum value of ptrdiff_t.
SIZE_MAX
    maximum of a size_t.

# Available Operators

This is a comprehensive list of all the operators you have in the C language. In this list, I'm indicating the following:

(binary)
    The operator has a left and right: X + Y.
(unary)
    The operator is on its own: -X.
(prefix)
    The operator comes before the variable: ++X.
(postfix)
    

Usually the same as the (prefix) version, but placing it

after gives it a different meaning: X++.

(ternary)
    

There's only one of these, so it's actually called the

ternary but it means "three operands": X ? Y : Z.

## Math Operators

These are your basic math operations, plus I put () in with these since it calls a function and is close to a "math" operation.

()
    Function call.
* (binary)
    multiply.
/
    divide.
+ (binary)
    addition.
+ (unary)
    positive number.
++ (postfix)
    read, then increment.
++ (prefix)
    increment, then read.
-- (postfix)
    read, then decrement.
-- (prefix)
    decrement, then read.
- (binary)
    subtract.
- (unary)
    negative number.

## Data Operators

These are used to access data in different ways and forms.

->
    struct pointer access.
.
    struct value access.
[]
    Array index.
sizeof
    size of a type or variable.
& (unary)
    Address of.
* (unary)
    Value of.

## Logic Operators

These handle testing equality and inequality of variables.

!=
    does not equal.
<
    less than.
<=
    less than or equal.
==
    equal (not assignment).
>
    greater than.
>=
    greater than or equal.

## Bit Operators

These are more advanced and for shifting and modifying the raw bits in integers.

& (binary)
    Bitwise and.
<<
    Shift left.
>>
    Shift right.
^
    bitwise xor (exclusive or).
|
    bitwise or.
~
    compliment (flips all the bits).

## Boolean Operators

Used in truth testing. Study the ternary operator carefully, it is very handy.

!
    not.
&&
    and.
||
    or.
?:
    Ternary truth test, read X ? Y : Z as "if X then Y else Z".

## Assignment Operators

Compound assignment operators that assign a value, and/or perform an operation at the same time. Most of the above operations can also be combined into a compound assignment operator.

=
    assign.
%=
    modulus assign.
&=
    bitwise and assign.
*=
    multiply assign.
+=
    plus assign.
-=
    minus assign.
/=
    divide assign.
<<=
    shift left, assign.
>>=
    shift right, assign.
^=
    bitwise xor, assign.
|=
    bitwise or, assign.

# Available Control Structures

There's a few control structures you haven't encountered yet:

do-while
    

do { ... } while(X); First does the code in the block, then

tests the X expression before exiting.

break
    Put this in a loop, and it breaks out ending it early.
continue
    Stops the body of a loop and jumps to the test so it can continue.
goto
    

Jumps to a spot in the code where you've placed a label:, and

you've been using this in the dbg.h macros to go to the error: label.

## Extra Credit

   Read stdint.h or a description of it and write out all the possible available size identifiers.
   Go through each item here and write out what it does in code. Research it so you know you got it right by looking it up online.
   Get this information solid as well by making flash cards and spending 15 minutes a day memorizing it.
   Create a program that prints out examples of each type and confirm that your research is right.

Donate

Copyright (C) 2010 Zed. A. Shaw

Credits

 

















