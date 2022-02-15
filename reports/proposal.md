# Lisperers

A Lisp-Based Language in C developed by [Conan McGannon](https://github.com/hyrtzhyro) and [Daniel Park](https://github.com/DanPark13)

## 1) What is the goal of your project; for example, what do you plan to make, and what should it do?  Identify a lower bound you are confident you can achieve and a stretch goal that is more ambitious.

Lisp is a family of programming languages characterised by the fact that all their computation is represented by lists. In this project, we will be developing a minimalistic Lisp-based language with a few core structures and builtins such as variables, functions, and conditional handling. The purpose of this is for us to allow us to understand what goes into making a core language while also having it easy to debug and trim away any waste away if it develops into something bigger than can be used for a large project.

By the end of the project, we should be able to have a functional programming language that can successfully compile and compute simple tasks such as printing, conditional checking, and mathematical equations.

## 2) What are your learning goals; that is, what do you intend to achieve by working on this project?

From this project, we fundamentally want to get a better understanding of the C Programming Language, building a base on top of what we have learned so far in SoftSys. We also want to make decisions based on understanding how a programming language is structured and seek new ways to view computation. Outside of computing, we want to build something that we will be proud of and build our communication skills with each other and build our software development collaboration skills.

## 3) What do you need to get started?  Have you found the resources you need, do you have a plan to find them, or do you need help?

We will be using this [guide](https://buildyourownlisp.com/) to help us build a lisp language by walking through all the important aspects of building the lisp. Although we will be using this guide to build the programming language, we will build our own “flavour” of Lisp to make sure we fully grasp the concepts of making a Lisp language while also developing something unique for ourselves.

We will also be referring to additional resources such as the GigaMonkeys *Practical Common Lisp* [Book](https://gigamonkeys.com/book/) to introduce us more extensively to the world of Lisp and the [Common Lisp Developers Community](https://common-lisp.net/) to connect with other Lisp developers about getting started and additional help we need along the way.

While we are also learning more about developing a Lisp language, we will also delve into the history of Lisp, being the second-oldest language only behind Fortran, and how it evolved into the base of many programming languages and tools we use today. Resources such as the John McCarthy's [History of Lisp](http://jmc.stanford.edu/articles/lisp/lisp.pdf) and History Computer's [Guide](https://history-computer.com/lisp-programming-language-guide/) will help us learn more about the Lisp language, and hopefully, help us gain a better appreciation for the beauty of computing.

We will also welcome any additional resources that may be useful for us on our journey with this project developing our own Lisp language.

## 4) What are your first steps?  Describe at least three concrete tasks that you can do immediately, and identify which member of the team will do them.  For each one, what is the "definition of done"; that is, what will you produce to demonstrate that the task is done?

- Finish reading the resources: The “definition of done” is if we have a basic language that is consistent with the non-bonus-material expectations of our [main resource](https://buildyourownlisp.com/) and the other resources we list above. Both of us will need to do this in order for us to continue forwards to fundamentally understand in depth what we need to do.
- Create an Evaluation Function: The “definition of done” for this is if it can evaluate and manipulate S-expressions and Q-expressions (e.g. operate on them) in a stable way. We will both do this step because this function will be dependent on the S-expressions and the Q-expressions working (which will be divided between Conan and Daniel)
- Create an S-Expression (Symbolic Expression): The “definition of done” would be if, as a list, its first item acts as an operator that acts on the other items in it as operands, and if it could be manipulated by the evaluation function. Daniel will take care of this before convening together to create the evaluation function.
- Create a Q-Expression (Quoted Expression): The “definition of done” would be if, as a list, it is able to store values, and not be operated upon by the Evaluation function under normal Lisp mechanics. Conan will be responsible for putting this together.
