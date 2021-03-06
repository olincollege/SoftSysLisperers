# Lisperers Project Update

A Lisp-Based Language in C developed by [Conan McGannon](https://github.com/hyrtzhyro) and [Daniel Park](https://github.com/DanPark13).

## 1)

Lisp is a family of programming languages characterised by the fact that all their computation is represented by lists. In this project, we will be developing a minimalistic Lisp-based language with a few core structures and builtins such as variables, functions, and conditional handling. The purpose of this is for us to allow us to understand what goes into making a core language while also having it easy to debug and trim away any waste away if it develops into something bigger than can be used for a large project.

By the end of the project, we should be able to have a functional programming language that can successfully compile and compute simple tasks such as printing, conditional checking, and mathematical equations.

## 2)

From this project, we fundamentally want to get a better understanding of the C Programming Language, building a base on top of what we have learned so far in SoftSys. We also want to make decisions based on understanding how a programming language is structured and seek new ways to view computation. Outside of computing, we want to build something that we will be proud of and build our communication skills with each other and build our software development collaboration skills.

## 3)

We started by creating a c script, prompt.c, which was later changed to parsing.c. This script was made initially to test user input, and later to test parsing between numbers, operators, and expressions. With the guidance of the “Build Your Own Lisp” Tutorials, we implemented the Polish Notation in mathematical operations using Regex, and returned the regex and keys associated with the input. No computations have been completed yet.

## 4)

- Finish the Evaluation Function: The “definition of done” for this is if it can evaluate and manipulate S-expressions and Q-expressions (e.g. operate on them) in a stable way. We will both do this step because this function will be dependent on the S-expressions and the Q-expressions working (which will be divided between Conan and Daniel)
- Catch Error Handling: The “definition of done” is to be able to catch if any crashes happen and will raise an error and print an associated error message.
- Create an S-Expression (Symbolic Expression): The “definition of done” would be if, as a list, its first item acts as an operator that acts on the other items in it as operands, and if it could be manipulated by the evaluation function. Daniel will take care of this aspect of the project.
- Create a Q-Expression (Quoted Expression): The “definition of done” would be if, as a list, it is able to store values, and not be operated upon by the Evaluation function under normal Lisp mechanics. Conan will be responsible for putting this together.
 
These are very similar to our stated initial tasks, but we underestimated the prep work needed to pursue them. With a greater knowledge of the topics involved, we now feel that we are very close to being able to enact these tasks to complete over Spring Break.

## Links

[Todoist Board](https://todoist.com/app/project/2285405220)

[GitHub Repository](https://github.com/olincollege/SoftSysLisperers)