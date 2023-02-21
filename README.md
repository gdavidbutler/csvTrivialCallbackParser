## csvTrivialCallbackParser
Trivial [CSV](https://tools.ietf.org/rfc/rfc4180.txt) callback parser in standard C.

No dependencies. No dynamic memory allocation. No recursion. No memory copying. Small and Fast.

This parser has a few differences from RFC 4180:

* It allows double quote delimited text anywhere in a value, not only the first and last characters.
* It allows any number of CR or NL to separate rows.
* It requires at least one CR or NL to terminate the last row.

To *check* that a CSV string can be parsed, use a NULL callback and verify the return offset is the same as the source length.

Some examples of using this:

* test/coders.c: example of using the coders.
* test/main.c: example of checking (./main 0 test/test.csv) and parsing CSV (./main 1 test/test.csv).
