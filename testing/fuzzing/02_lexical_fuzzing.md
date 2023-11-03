# Lexical Fuzzing

## Basic Fuzzing

### Fuzzer at the very beginning

1990, Miller, finding 1/3 linux utilities did not appropriately handle input.

buffer overflow, missing error checks, rogue numbers (input).

enable dynamic checking while development! CI!


### Architecture

fuzzer: random tests generator

runner: function unit invoker

```
fuzzer.run(runner)
```

### Exercises: Troff

At the time of Miller et al., troff would fail if its input included:

1. The input sequence `\D` (backslash + D) followed by a non-printable character 
2. A character in the ASCII range 128–255 (i.e., the 8th bit is set) 3. followed by a newline character
3. A single dot (.) followed by a newline character.

```
1. .find("\\D") && c not in string.printable
2. ord(char) <= 127 or followed "\n"
3. .find(".\n")
```

## Code Coverage

Guide fuzzer towards specific features. 

For example, by maximize *code coverage*.
- lines, branches, functions, statements, etc.


> Example: cgi-decode. Change url-string into original string.

Python's Tracing function: `sys.settrace()`. frame, event, arg. This (a powerful runtime) makes python easy to implement a fuzzer.

Black-Box testing: derive tests from the specification. Cover all features. Can define tests before implementation.

But programs behavior is more subtle than the specification. Need white-box testing to at least dive into programs implementation in some ways. And there'll be some coverage criteria (aiming to fully coverage all possible dynamic behaviors), like statement coverage, branch coverage, path (sequences of branches), loop iterations, data flow, ... `\cite{https://ix.cs.uoregon.edu/~michal/book/}`

> behaviors; specified behaviors; coverage

* Black-Box testing is important to specify targeting functionality of the software. 
* White-Box testing is important to test unspecified behaviors of the software does not ruin usability (is consistent).