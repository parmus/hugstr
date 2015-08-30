

Hugstr – the original hug T-shirt
=================================


Introduction to Arduino programming
-----------------------------------

### What is a program?

A computer program is a list of instructions, which tells a computer exactly what to do step by step. You can think of a computer program kinda list a baking or cooking recipe. Let's take a look at a simple recipe for boiling an egg:

1. Put the eggs in a saucepan
2. Add enough cold water to cover them by about 1 inch.
3. Set the pan over medium-high heat
4. As soon as the water reaches a brisk simmer, start timing.
5. As the eggs cook, adjust the heat as needed to maintain a brisk simmer.
6. Let the egg cook for 4 min.
7. When the eggs are cooked, carefully pour out most of the hot water, leaving the eggs in the pan.
8. Set the pan in the sink under cool running water for a few minutes until the eggs are barely warm.

Let's examine the implicit structure of this recipe a bit. This will help us understand the more explicit structure of a computer program in a bit.

* The first 3 lines simply instructs us to put the eggs in a saucepan with some water and turn on the heat. These are simple step-by-step instructions.
* Line 4 asks us to wait until the water reaches a brisk simmer, and when it does then continue with the recipe. This instruction is worded as a passive wait, while what actually happens is that you actively check if the water has reached a brisk simmer, and repeat until it does.
* Line 5 asks us to repeatedly adjust the heat to maintain a brisk simmer. Unlike line 4 here the repeated behavior
* is worded more explicitly. However also notice that the line doesn't contain any information about when to stop this repeated adjustment of temperatures. You have to read line 6 to know that you shall do this for 4 min. Also notice that the recipe implicitly talks about the timing you started in line 4.
* Line 7 instructs us to pour most of the water out. Notice that both the start and the end of the line is redundant information. "When the eggs are cooked" is redundant, because line 6 already told us to cook for 4 min., so by the time we reach line 7, the eggs are cooked by definition. "leaving the eggs in the pan." is also redundant, because we only put water and eggs in the saucepan, and so since we're pouring out water, the eggs must be what's left.
* Finally line 7 is another implicit repeat, where we wait for the eggs to cool down. Notice that the recipe never instructs to close the water tap again and what to do with the cold water and dirty saucepan afterwards.
  
The above recipe is written in a human readable language, and as we can see it contains some implicit structure and hidden assumption. Similarly a computer program is written in a computer language, which the computer understands, but
unlike human language as computer language is very explicit. Let's try to rewrite the above recipe to make is explicit and remove hidden assumptions.

1. Put the eggs in a saucepan
2. Add enough cold water to cover them by about 1 inch.
3. Set the pan over medium-high heat
4. *If* the water is not briskly simmering, *then* *repeat from* step 4
5. Start timing.
6. *If* the water is not briskly simmering, *then* turn the heat slightly up.
7. *If* the water is boiling, *then* turn the heat slight down.
8. *If* less than 4 min. have passed, *then* *repeat from* step 6.
9. Carefully pour out most of the hot water
10. Turn off the heat.
11. Turn on the cold water tap.
12. Set the pan in the sink under cool running water
13. *If* the eggs are still hot, *then* *repeat from* step 12.
14. Turn off the cold tap.

Let's take a look at this new recipe and notice a few things:

* First of all, notice that the new recipe sounds much more boring and mechanical. This is because we've rewritten the three steps, where the recipe contains waiting or repeated behavior into an explicit *If* this *then* do that form. In the original recipe these three steps were worded in slightly different way, which sounds more natural to human, because we like variations. However, strictly speaking this variation is completely unnecessary as we can express the same with just *If* this *then* do that.
* Secondly, notice that we're now actively and explicitly jumping back in the recipe with *repeat from*. This creates what we call a *loop*. Notice that everything in the recipe is now an action, where you actively do something, e.g. you don't "wait until the water has reached a brisk simmer"; instead you actively and repeatedly checks if the water is briskly simmmering. You can think of this similarly to kids in a car going "Are we there yet? Are we there yet? Are we there yet?".
* Thirdly, notice that all three loop can be described with the same basic structure: they contain a *condition*, which is being checked and zero or more lines of instructions, which are being repeated until we leave the loop.
* Finally, notice that not all *If* this *then* do that patterns are loops. Line 6 and 7 doesn't contain *repeat from* and thus aren't loops.
  
Let's do a final rewrite, where we make the loops explicit using a new pattern *while* this *do* do that. And let's use in indention rather than *repeat from* with line numbers. The recipe will now look like this:

1. Put the eggs in a saucepan
2. Add enough cold water to cover them by about 1 inch.
3. Set the pan over medium-high heat
4. *While* the water is not briskly simmering *do*
  1. Nothing.
5. Start timing.
6. *While* less than 4 min. have passed, *do* 
  1. *If* the water is not briskly simmering, *then* turn the heat slightly up.
  2. *If* the water is boiling, *then* turn the heat slight down.
7. Carefully pour out most of the hot water
8. Turn off the heat.
9. Turn on the cold water tap.
10. Set the pan in the sink under cool running water
11. *While* the eggs are still hot, *do*
  1. Nothing
12. Turn off the cold tap.

This recipe is still boring and mechanic, but it's a lot more readable than the previous version, and the implicit structure of the original recipe is now a lot more explicit.
Now, let's leave the egg boiling recipe for now and look at some real code. We will return to this example a little later, but first we need some very basic building block.

### The basics of Arduino programming

The programming language used to program the Arduino microcomputer is based on a very old and well-known computer language called **C++**, however the Arduino language is much smaller and easier to use. We will return to the relationship between C++ and the Arduino laungage in the end of this chapter, but for now you don't have to worry about C++ at all.

The entire Arduino language is documented in a nice reference on the Arduino website: https://www.arduino.cc/en/Reference/HomePage

However in the following sections we will look at just the small subset you need to work with the Hugstr project. Let's get started with an examples:

```C++
2 + 2;
42 / 10;
```

An Arduino program is composed from a number of *expressions* separated ```;```. The program above is a rather boring program, which will calculate the 2 mathematical expressions, but do absolutely nothing with the results; they will simply just be thrown away.

#### Comments

An Arduino program can also contain *comments*, which are blocks of texts, which the computer will ignore.

```C++
// This is a comment
2 + 2; // This is also a comment
42 / 10;
/* This is a multiline comment
   spanning multiple lines */
```

The program above contains 3 comments, but as they are *completely* ignored by the computer, this program will do exactly the same as the previous one.

Comments are very useful for leaving bits of documentation in the your program, e.g. for explaining a particular difficult part or for leaving information about who wrote the program.
The Arduino language supports two types of comments.

* Single-line comments begins with ```//``` and spans to the end of the line.
* Multi-line comments begins with ```/*``` and ends with ```*/```. Everything in between will be ignored by the computer.

#### Spaces, new lines and code style

Notice that Arduino expressions are separated with ```;```, but the above programs also contains new lines, so each expression is on it's own line. This is only done to make the program more readable to humans, and is completely unnecessary for the computer. So are the spaces around the mathematical operators. We could also have written the above program completely without these extra newlines and spaces like so:

```C++
2+2;40/10;
```
contains 3 mathematical expressions. Notice that the line spaces and newlines doesn't matter for the computer. The extra empty line between the first expression and the rest will simply be ignored, just as the lack of newline between the second and the third expression. 



### Setting up your Arduino programming environment

### The structure of an Arduino program

```C++
void setup() {
  // put your setup code here, to run once:
}

void loop() {
  // put your main code here, to run repeatedly:
}
```

### Our first Arduino program

### The basics

### Variables and datatypes

'''
int a = 2;
int b = 10;
float c = 4.5;

void setup() {
  // put your setup code here, to run once:
}

void loop() {
  // put your main code here, to run repeatedly:
}
'''


### Simple math

'''
void setup() {
  // put your setup code here, to run once:
}

void loop() {
  // put your main code here, to run repeatedly:
}
'''

### Control structures




### Functions




### Putting it all together

### Constants and defines

### Including code from files and libraries

The hug sensor
--------------
### A quick introduction to eletricity and resistance
