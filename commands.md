# Example Doc

## Calculations

Polish Notation (Common in Lisps)

Addition: + 2 5
Subtraction: - 2 5
Multiplication: * 2 5
Division: / 2 5
Orders of Operation: + 1 (* 7 5) 3

## Objects

### Lists

list 1 2 3 4
Head of List: {head (list 1 2 3 4)}

### Variables: 

def {x} 4
+ x 8 -> 12

def arglist 1 2 3 4


### Functions

def {addition} +
addition 1 2 3 -> 6

def {add-mul} (\ {x y} {+ x (* x y)})
add-mul 10 20 -> 210

## Conditionals

> 10 5 -> 1

+ 1 {5 6 7}