# Ice Programming Language

[![](https://img.shields.io/badge/request-new%20features-blue.svg)](https://github.com/ice-lang/ice/issues/new)
[![PRs Welcome](https://img.shields.io/badge/PRs-welcome-brightgreen.svg?style=flat-square)](https://github.com/ice-lang/ice/compare)
[![Gitter](https://badges.gitter.im/JoinChat.svg)](https://gitter.im/ice-lang)
[![license](https://img.shields.io/github/license/MU001999/ice.svg)](https://github.com/ice-lang/ice)

## Quick Usage

### Install

```bash
~> git clone https://github.com/ice-lang/ice-lang.git
~> cd ice-lang
~/ice-lang> make install
```

### Usage

```bash
~> ice
```

You can see some examples in `example/` or the `test/sample-tester.cpp`, this is the yin-yang puzzle for fun

```
(@(yang): @(yin): yin(yang))
    ((@(cc) { print("*"); return cc; })
        (call_with_current_continuation(@(cont): cont)))
    ((@(cc) { print("@"); return cc; })
        (call_with_current_continuation(@(cont): cont)));

```

### Extension in VSCode

Search `Ice-Lang`

## To Do

Ice-lang is segrageted to about three parts, the front end, codegen and the virtual machine

+ [ ] Prompt undefined symbol with the symbol's name
+ [ ] Useful error info when meeting runtime error
+ [ ] Complete use statement and try to design a package system
+ [ ] Support type convertion
+ [ ] Support threads and processes
+ [ ] Complete all codegens
+ [ ] Read from file and complie IR to file

## Change Log

+ [X] Allow call/cc copy the current stack
+ [X] Reduce redundant instruction LambdaDecl
+ [X] Fix bug in REPL mode
+ [X] Remove the spending of nested stack at before versions.
+ [X] Fix bug in eval function
+ [X] Fix bug in updating pc when meeting error in REPL
+ [X] Refactor the run-way to support complete call/cc
+ [X] Add built-in function call_with_current_continuation easily with some limitations such as caller must exist;
+ [X] Prompt undefined symbol but without its name loaded by
+ [X] Support new operator `is`
+ [X] Add new built-in function id
+ [X] Fix bug in tail call optimization when meeting `return cond ? true_expr, false_expr`
+ [X] Change the type of the internal value of the integer from long to int64_t
+ [X] Support tail call optimization
+ [X] Support the built-in class and method
+ [X] Allow multi code objects when using eval
+ [X] Support eval function
+ [X] Reduce the switch-time
+ [X] Support call-by-need
+ [X] Better implementation of REPL without try-throw
+ [X] Simple REPL with try-throw
+ [X] Complete basic things in vm such as function, control flow and some operators
+ [X] Report errors with line info in detail in front
+ [X] Implementation of the front end but without reporting errors with line info in detail
