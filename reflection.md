# Stack Explorer Reflection

## Stack Mechanics

The factorial trace showed calls entering with decreasing inputs until the base case. For factorial(5), five factorial frames were active at the deepest point. The return messages appeared in reverse order, from factorial(1) through factorial(5), showing the stack unwinding. The results were 120 for input 5 and 3,628,800 for input 10. Fibonacci printed both entry and return depths, making its branching calls and repeated calculations visible. Inputs 10, 20, and 30 reached maximum depths of 10, 20, and 30 when counting the initial frame as depth one.

The intentional infinite recursion test terminated with SIGSEGV, signal 11, the segmentation fault associated with exhausting stack space in this run. The test process used a smaller stack limit to reach the failure quickly. Safe recursion stopped at 10 and returned normally. A typical stack frame contains storage for local variables, saved registers, and information needed to return to the caller. Parameters and return addresses may be held in registers or on the stack depending on the architecture, calling convention, and compiler optimization.

## Recursion Costs

At input 10, factorial and Fibonacci both reached ten active recursive frames. Fibonacci did not have exponentially greater maximum depth: its total calls grew rapidly instead. Fibonacci required 177 calls at 10, 21,891 at 20, and 2,692,537 at 30. Factorial follows one chain, while Fibonacci repeatedly evaluates two branches. Those branches execute sequentially, so their frames are not all present simultaneously. Both algorithms have linear maximum stack depth, although Fibonacci frames can require more storage per call. Python limits recursion to protect against uncontrolled recursion exhausting available stack resources. Without that protection, excessive nesting could cause a process crash similar to the C demonstration. An iterative implementation avoids accumulating recursive frames.

## Function Pointers and Callbacks

A function pointer identifies a function with a compatible signature. Passing it into process_array lets the processor call the selected operation for each element without hardcoding that operation. The same loop produced doubled, squared, and negated outputs by receiving different callbacks. The event system similarly stored callback pointers and invoked registered handlers when an event fired. Separate registries represented score, level, and health events, and the score event demonstrated multiple registered callbacks.

GUI button handlers are a practical example: a framework detects a click and calls application code registered for that event. Systems programmers use callbacks to separate reusable infrastructure from application behavior, support extensions, and avoid duplicating processing logic. Explicit signatures and descriptive function names help make indirect calls understandable despite their extra complexity.
