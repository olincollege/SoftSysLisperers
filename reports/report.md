# LISPERERS

A Lisp-Based Language in C developed by [Conan McGannon](https://github.com/hyrtzhyro) and [Daniel Park](https://github.com/DanPark13).

## Introduction

**Lisp** is a family of programming languages characterised by its source code comprised of lists with a history dated backed to John McCarthy developing Lisp in 1958, making it the second-oldest high-level programming language (still in use) after Fortran. The language was originally created as a practical mathematical notation for computer programs, which later derived intself to become the favored language for computer science research, particularly artificial intelligence, due to its ability to process symbolic information effectively.

## Project and Learning Goals

The goal of our project is to gain fluency in C and its lower-level capabilities within memory management and language constructs. We want to attain our goal of getting more familiar with C by working with Lisps by learning more about its history and uses over time while developing our own basic implementation based on *Common Lisp*, a general-purpose Lisp language specification that supports multiple paradigms. And by working with Lisps, we want to understand how a core programming language is structured and seek new ways to view computation. Outside of computing, we want to collaborate effectively on the project through goal allocations and proper source code management to build a final product we can be proud of.

With the Lisp's roots within mathematical notation for computers, as a lower bound, we wanted to implement a language that acts as a simple calculator, which can take in user input of integers and operations (addition, subtraction, multiplication, and division), and output answers. And at a higher bound, we want to implement a full-fledged language that can take in different inputs from integers, booleans, floats, and strings, be able to save them to variables and implement functions. This full implementation would essentially be a programming language in of itself and can be used to make programs.

## Resources

Before going straight into building "Lisperers", we did research on Lisps learning more about its history, flavor implementations, and use cases. Starting with the [Wikipedia Page](https://en.wikipedia.org/wiki/Lisp_(programming_language)) on Lisps, we went on a explorer's tangent, such as learning more about the very origins through [John McCarthy's Article](http://jmc.stanford.edu/articles/recursive/recursive.pdf) and Lisp' history through [History Computer's Guide](https://history-computer.com/lisp-programming-language-guide/). We also explored more about the [Common Lisp](https://en.wikipedia.org/wiki/Common_Lisp). We also got involved with several Lisp communities on Reddit, General Forums, and even 4chan to stay up to date with what is going on in the world with Lisps (answer: not too much in the past decade).

For implementation, we mainly consulted Daniel Holden's [Build Your Own Lisp Guide](https://buildyourownlisp.com/), following the tutorial and gaining insight to key points on what goes into a Lisp language. We also used [TutorialPoint's Lisp Tutorial](https://www.tutorialspoint.com/lisp/index.htm) to learn more about Lisp Syntax as well as Peter Siebel's [Practical Common Lisp](https://gigamonkeys.com/book/) to get familiar with the Common Lisp and its structure specifically.

The full list of resources we consulted through our research can be found [here](https://github.com/olincollege/SoftSysLisperers/blob/main/resources/all_links.md).

## Implementation

Since most of our time was spent on wrapping our heads around learning more about Lisp and its history and studying concepts such as S-Expressions to format and parse data, we were at a point where we had our lower-bound implementation of computing numerical operations by the time break came around. We knew that we couldn't implement our higher-bound projected goal of a full-fledged programming language during break, so we decided to pivot into a "complex calculator" that can do integer computations while also saving numbers into variables and expressions into functions with parameters. We also wanted to implement our own "flavor" of Lisp with its own syntax, but after learning more about its history and common syntax across different Lisps, we decided to keep its general structure of `fun var1 var2 var3`.

After completing that, we have our final Lisperers program that acts as an integer calculator where you can save numbers to variables and complex operations into functions. The output starts by printing three introductory lines, and reading the user's input:

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

## Outline:

Your project report should answer the following questions (note that some are the same as in the proposal and the update):

## 1) What is the goal of your project; for example, what do you plan to make, and what should it do?

Lisp is a family of programming languages characterised by the fact that all their computation is represented by lists. In this project, we will be developing a minimalistic Lisp-based language with a few core structures and builtins such as variables, functions, and conditional handling. The purpose of this is for us to allow us to understand what goes into making a core language while also having it easy to debug and trim away any waste away if it develops into something bigger than can be used for a large project.

By the end of the project, we should be able to have a functional programming language that can successfully compile and compute simple tasks.

These simple tasks include
- Printing
- Computing Mathematical Equations
- Assigning Values to Variables

## 2) What are your learning goals; that is, what do you intend to achieve by working on this project?

From this project, we fundamentally want to get a better understanding of the C Programming Language, building a base on top of what we have learned so far in SoftSys. We want to get a better feeling for core C concepts such as memory allocation and pointers. We also want to make decisions based on understanding how a programming language is structured and seek new ways to view computation. Outside of computing, we want to build something that we will be proud of and build our communication skills with each other and build our software development collaboration skills through GitHub and ToDoIst.

## 3) What resources did you find that were useful to you.  If you found any resources you think I should add to the list on the class web page, please email them to me.

We will be using this [guide](https://buildyourownlisp.com/) to help us build a lisp language by walking through all the important aspects of building the lisp. Although we will be using this guide to build the programming language, we will build our own “flavour” of Lisp to make sure we fully grasp the concepts of making a Lisp language while also developing something unique for ourselves.

We will also be referring to additional resources such as the GigaMonkeys *Practical Common Lisp* [Book](https://gigamonkeys.com/book/) to introduce us more extensively to the world of Lisp and the [Common Lisp Developers Community](https://common-lisp.net/) to connect with other Lisp developers about getting started and additional help we need along the way.

While we are also learning more about developing a Lisp language, we will also delve into the history of Lisp, being the second-oldest language only behind Fortran, and how it evolved into the base of many programming languages and tools we use today. Resources such as the John McCarthy's [History of Lisp](http://jmc.stanford.edu/articles/lisp/lisp.pdf) and History Computer's [Guide](https://history-computer.com/lisp-programming-language-guide/) will help us learn more about the Lisp language, and hopefully, help us gain a better appreciation for the beauty of computing.

## 4) What were you able to get done?  Include in the report whatever evidence is appropriate to demonstrate the outcome of the project.  Consider including snippets of code with explanation; do not paste in large chunks of unexplained code.  Consider including links to relevant files.  And do include output from the program(s) you wrote.

We were able to complete a full program that acts as an integer calculator where you can save numbers to variables and complex operations into functions. The output starts by printing three introductory lines, and reading the user's input:

    Lisperers Version 1.0
    Press Ctrl+c to Exit

    Lisperers> 

Since most of our time was spent on wrapping our heads around learning more about Lisp and its history and studying concepts such as S-Expressions to format and parse data, we were at a point where we had our lower-bound implementation of computing numerical operations by the time break came around. We knew that we couldn't implement our higher-bound projected goal of a full-fledged programming language during break, so we decided to pivot into a "complex calculator" that can do integer computations while also saving numbers into variables and expressions into functions with parameters.

We implemented variables and functions as Lisp values that can be saved on the Lisp environment to have our final Lisperers program.

As mentioned before, the program can compute operations between integers:

    Lisperers> + 1 2 3 4 5 6 7 8 9
    45
    Lisperers> * 3 5 7 9
    945
    Lisperers> / 314 100
    3
    Lisperers> / 386 100
    3

Users can define different variables in the following way:

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

And with our implementation we have our Lisp-based calculator.

To run the program yourself, navigate to the `src` folder and run `./main` on your terminal.

TODO: Link snippets of code with explanations of our process.

## 5) Explain at least one design decision you made.  Were there several ways to do something?  Which did you choose and why?

Although the Lisp Documentation was vast to be able to create a full-fledged programming language that can build projects, we decided that we wanted to keep things simple by restricting what we wanted in our language. We wanted to build a command line tool that was able to do simple computations with addition, subtraction, multiplication, division and being able to return them, assign values to variables within the local environment, and the user able to define functions in the prompt.

## 6) You will probably want to present a few code snippets that present the most important parts of your implementation.  You should not paste in large chunks of code or put them in the Appendix.  You can provide a link to a code file, but the report should stand alone; I should not have to read your code files to understand what you did.

Note: Useful for Number 4.

## 7) Reflect on the outcome of the project in terms of your learning goals.  Between the lower and upper bounds you identified in the proposal, where did your project end up?  Did you achieve your learning goals?

Achieved learning goals of
- Understanding innerworks of C better and apply what we learned in class
- Make a basic language we a basic structure
- Learn more about lisps, functionalities, purpose, and history.

Unfortunate we weren't able to extend this lisp into a full functioning language, just a base language that can handle basic computations and store variables and functions into the environment. Next iteration we would want to expand into being able to handle other operations as well as going into strings and conditionals to flesh out a proper lisp language.

Audience: Target an external audience that wants to know what you did and why.  More specifically, think about students in future versions of SoftSys who might want to work on a related project.  Also think about people who might look at your online portfolio to see what you know, what you can do, and how well you can communicate.

# You don't have to answer the questions above in exactly that order, but the logical flow of your report should make sense.  Do not paste the questions into your final report.