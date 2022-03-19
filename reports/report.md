# Lisperers Project Update

A Lisp-Based Language in C developed by [Conan McGannon](https://github.com/hyrtzhyro) and [Daniel Park](https://github.com/DanPark13).

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

Add more potential resources later.

## 4) What were you able to get done?  Include in the report whatever evidence is appropriate to demonstrate the outcome of the project.  Consider including snippets of code with explanation; do not paste in large chunks of unexplained code.  Consider including links to relevant files.  And do include output from the program(s) you wrote.

Full program that acts as a calculator where you can save numbers to variables and complex operations into functions.

TODO: Link snippets of code with explanations of our process.

## 5) Explain at least one design decision you made.  Were there several ways to do something?  Which did you choose and why?

Although the Lisp Documentation was vast to be able to create a full fledged programming language that can build projects, we decided that we wanted to keep things simple by restricting what we wanted in our language. We wanted to build a command line tool that was able to do simple computations with addition, subtraction, multiplication, division and being able to return them, assign values to variables within the local environment, and the user able to define functions in the prompt.

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