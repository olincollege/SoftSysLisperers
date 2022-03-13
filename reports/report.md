# Lisperers Project Update

A Lisp-Based Language in C developed by [Conan McGannon](https://github.com/hyrtzhyro) and [Daniel Park](https://github.com/DanPark13).


Your project report should answer the following questions (note that some are the same as in the proposal and the update):

1) What is the goal of your project; for example, what do you plan to make, and what should it do?

2) What are your learning goals; that is, what do you intend to achieve by working on this project?

3) What resources did you find that were useful to you.  If you found any resources you think I should add to the list on the class web page, please email them to me.

4) What were you able to get done?  Include in the report whatever evidence is appropriate to demonstrate the outcome of the project.  Consider including snippets of code with explanation; do not paste in large chunks of unexplained code.  Consider including links to relevant files.  And do include output from the program(s) you wrote.

5) Explain at least one design decision you made.  Were there several ways to do something?  Which did you choose and why?

6) You will probably want to present a few code snippets that present the most important parts of your implementation.  You should not paste in large chunks of code or put them in the Appendix.  You can provide a link to a code file, but the report should stand alone; I should not have to read your code files to understand what you did.

7) Reflect on the outcome of the project in terms of your learning goals.  Between the lower and upper bounds you identified in the proposal, where did your project end up?  Did you achieve your learning goals?

Audience: Target an external audience that wants to know what you did and why.  More specifically, think about students in future versions of SoftSys who might want to work on a related project.  Also think about people who might look at your online portfolio to see what you know, what you can do, and how well you can communicate.

 You don't have to answer the questions above in exactly that order, but the logical flow of your report should make sense.  Do not paste the questions into your final report.

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