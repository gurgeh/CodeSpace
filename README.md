This project implements a tiny stack based language (thus far interpreted, but one day soon compiled to x86), where most instructions are 4 or 5 bits long.

All bit strings represent valid programs that can never throw exceptions. They are guaranteed to terminate, since the number of allowed jumps are fixed.

Using this language, I can iterate through all possible programs up to a certain length and examine the output.

One reason why this is interesting is that if you have a sequence of integers (or symbols or bits or..) that you know
nothing about, let us call this your history, X. Then, the best estimate for the probability for an integer Z to occur next
(i.e the best prediction) is given by the set of all programs that output your history X followed by Z.

The more programs that output X : Z the more likely Z is to occur next in the sequence. The space of all programs is dominated by the
short programs, since permutations and modifications of those will take up a sizeable proportion of the space. The best guess for
the next symbol is often given by the shortest possible program that outputs X.

For more info, see my blog post here:
http://fendrich.se/blog/2010/12/28/compression-prediction-and-artificial/

The main purpose of this project is to run all programs up to a certain length and store the first N (10 - 20) integers that
are output, and keep a record of how many different programs output each sequence (and how long the shortest program is for each
sequence).

Using this table of sequences, you can do fun stuff like make predictions for what the next integer in the sequence 1, 2, 5 should
be. Or try to find sequences from OEIS, The On-Line Encyclopedia of Integer Sequences - http://oeis.org/. Or try to find the shortest program that outputs only primes, although I have yet to find such a program, searching through all programs
up to 40 (I think.. or was it 42?) bits. I did find one interesting program that output 10 large primes in order.

The most interesting sequences are those that don't occur too often (they are too simple, like 5 5 5 5 ... or 1 2 3 ..., etc)
and not too seldom, because they look like random garbage.

I don't know if there can be any practical purpose of this approach - I made it just as an odd exploration. As you can see by the
latest commit date, I have not worked on this for a while. I would love to see what happens when you search deeper and run even longer
programs. Can you finally find the shortest prime generator?

The general approach to this would be:
1) run the programs faster than in my un-optimized interpreter. Probably by generating machine code directly.
2) find other people who are interested in this, and run the program distributed across many machines
3) maybe adapt the code to GPU, which could be much faster.

After that, to get even deeper, I think you would have to start to automatically analyze code, to for example prove that certain classes of programs always give the same output.




