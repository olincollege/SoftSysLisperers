# LISPERERS

A Lisp-Based Language in C developed by [Conan McGannon](https://github.com/hyrtzhyro) and [Daniel Park](https://github.com/DanPark13).

## Introduction

**Lisp** is a family of programming languages characterised by its source code comprised of lists with a history dated backed to John McCarthy developing Lisp in 1958, making it the second-oldest high-level programming language (still in use) after Fortran. The language was originally created as a practical mathematical notation for computer programs, which later derived intself to become the favored language for computer science research, particularly artificial intelligence, due to its ability to process symbolic information effectively.

All code within a Lisp language is written as a type of list known as an S-Expression, and a function call within a Lisp is also structured as a list, of the form (`function` argument1 argument2 argument3). 

## Project and Learning Goals

The goal of our project is to gain fluency in C and its lower-level capabilities within memory management and language constructs. We want to attain our goal of getting more familiar with C by working with Lisps by learning more about its history and uses over time while developing our own basic implementation based on *Common Lisp*, a general-purpose Lisp language specification that supports multiple paradigms. And by working with Lisps, we want to understand how a core programming language is structured and seek new ways to view computation. Outside of computing, we want to collaborate effectively on the project through goal allocations and proper source code management to build a final product we can be proud of.

With the Lisp's roots within mathematical notation for computers, as a lower bound, we wanted to implement a language that acts as a simple calculator, which can take in user input of integers and operations (addition, subtraction, multiplication, and division), and output answers. And at a higher bound, we want to implement a full-fledged language that can take in different inputs from integers, booleans, floats, and strings, be able to save them to variables and implement functions. This full implementation would essentially be a programming language in of itself and can be used to make programs.

## Resources

Before going straight into building "Lisperers", we did research on Lisps learning more about its history, flavor implementations, and use cases. Starting with the [Wikipedia Page](https://en.wikipedia.org/wiki/Lisp_(programming_language)) on Lisps, we went on a explorer's tangent, such as learning more about the very origins through [John McCarthy's Article](http://jmc.stanford.edu/articles/recursive/recursive.pdf) and Lisp' history through [History Computer's Guide](https://history-computer.com/lisp-programming-language-guide/). We also explored more about the [Common Lisp](https://en.wikipedia.org/wiki/Common_Lisp). We also got involved with several Lisp communities on Reddit, General Forums, and even 4chan to stay up to date with what is going on in the world with Lisps (answer: not too much in the past decade).

For implementation, we mainly consulted Daniel Holden's [Build Your Own Lisp Guide](https://buildyourownlisp.com/), following the tutorial and gaining insight to key points on what goes into a Lisp language. We also used [TutorialPoint's Lisp Tutorial](https://www.tutorialspoint.com/lisp/index.htm) to learn more about Lisp Syntax as well as Peter Siebel's [Practical Common Lisp](https://gigamonkeys.com/book/) to get familiar with the Common Lisp and its structure specifically.

The full list of resources we consulted through our research can be found [here](https://github.com/olincollege/SoftSysLisperers/blob/main/resources/all_links.md).

## Implementation

 Most of our time was spent on wrapping our heads around studying these concepts, implementing them while also devling into Lisps and its history, we were at a point where we had our lower-bound implementation of computing numerical operations by the time break came around. We knew that we couldn't implement our higher-bound projected goal of a full-fledged programming language during break, so we decided to pivot into a "complex calculator" that can do integer computations while also saving numbers into variables and expressions into functions with parameters. We also wanted to implement our own "flavor" of Lisp with its own syntax, but after learning more about its history and common syntax across different Lisps, we decided to keep its general structure of `fun var1 var2 var3` throughout our program.

And with that, we have our final Lisperers program that acts as an integer calculator where you can save numbers to variables and complex operations into functions. The output starts by printing three introductory lines, and reading the user's input:

    Lisperers Version 1.0
    Press Ctrl+c to Exit

    Lisperers> 

From here the program can complete operations between integers:

    Lisperers> + 1 2 3 4 5 6 7 8 9
    45
    Lisperers> * 3 5 7 9
    945
    Lisperers> / 314 100
    3
    Lisperers> / 386 100
    3

The user can also define different variables in the following way:

    Lisperers> def {var} 187
    ()
    Lisperers> var
    187

We can also save order of operations equations into functions. This function in particular takes in two parameters, x and y, and first multiples them to a single number, in this case 2 * 3 = 6. Then the program calculates the sum between x, y, and the product so 2 + 3 + 6 = 11.

    Lisperers> def {addition-multiplication} (\ {x y} {+ x y (* x y)})
    ()
    Lisperers> addition-multiplication 2 3
    11

And we can combine these two concepts by placing variables into functions. `var`, which we previously defined as 187, replaces the parameter in the function. So 187 * 3 = 561. And with the sum of 187, 3, and 561 we get 751.

    Lisperers> addition-multiplication var 3
    751

And with our implementation we have our Lisp-based calculator that can save variables and equations in the environment.

To run the program yourself, navigate to the `src` folder and run `./main` on your terminal. Or you can recompile the program using the command `gcc main.c mpc.c -ledit -lm -o main.`

## Reflection

Overall, we are satisfied with our work completing the Lisp-based calculator. While we were not able to reach our goals of building a full-fledged Lisp language that can have programs be built off of it, we are satisfied more in the progress we made learning about the Lisps, its uses, history, and implementation. We learned a lot when it comes to special ways in parsing through Symbolic Expressions and Quoted Expressions within a Lisp environment. In our next steps, we hope to continue this project further to include string and conditional implementations as well as implementing a standard library of functions so a future user may be able to implement a project using the language.

But with where we are at right now, through all that learning and progress we made in this month, we were able to have an implementation that can take in input, parse it for specific commands, compute mathematical operations, and save operations and integers into functions and variables respectively.