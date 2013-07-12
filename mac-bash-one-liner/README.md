# Mac Bash One Liner

Why bother to note mac OS X down as a special example?
Former linux users all suffer from the differences in commands. 
By looking those one liners, one can quickly pick up different conventions.

## Remove all `.DS_Store` under current dir

	find . -name ".DS_Store" -print0 | xargs -0 -I{} sh -c 'rm -f "{}"'

`-print0` and `xargs -0` come handy together. 
This is documented in the manual (but in a obsecure location).
