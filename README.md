# DFA-CFG-PDA

### grammar_2_pda.py
> A program written in **python** to convert a **Context Free Grammar** to a **Push Down Automata**.
#### Input Pattern ( from some 'file.txt' in 'cfg_sample_files' folder )
```
S:aSa|bA
A:aS|bB
B:bB|b
```
> S - Start State <br>
> A, B - Non-Terminals <br>
> a, b - Terminals <br>
#### Steps to Solve
> Read line by line from the input file and do the following steps for each line - 
* Separate the left and right-hand side.
* Isolate all the productions on the right-hand side.
* Now, say the l.h.s is X and r.h.s. productions are y1, y2,..., yn, then, the corresponding transition function will be - 
```
δ(q0, ε, X) -> {(q0, y1), (q0, y2), ..., (q0, yn)}
```
* In each production, there may be **Terminal** variables, detecting them will not be difficult, because they are written in **lower-case alphabets**, where the Non-terminals are not.
* After all the conversions are over, one more transition function will be created for each Terminal **a**, in the form - 
```
δ(q0, a, a) -> ε
```
#### Output
> In the above example, a and b are terminals. So, the converted **pda** will look like - 
```
δ(q,ε,S) -> {(q,aAA)}
δ(q,ε,A) -> {(q,aS), (q,bS), (q,a)}
δ(q,a,a) -> {(q,ε)}
δ(q,b,b) -> {(q,ε)}
```
> <hr>
### pda_2_grammar.py
