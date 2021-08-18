# BST On Hard Disk

Writing a BST (Binary Search Tree) on the hard disk. Done as part of the course "Fine Techniques" (UE18CS257D) at PES University.

This code is used to store a BST into a file i.e. the hard disk itself, instead of the heap of the program. Offsets are used to get the position of subtrees.

## Functions:

### Open File:
```c
init_file("<file_name>.dat")
```
### Insert:

```c
insert_key(<key>, <file_pointer>)
```

### Delete:

```c
delete_key(<key>, <file_pointer>)
```

### Display Inorder BST:

```c
display_inorder(<file_pointer>)
```

### Display Preorder BST:

```c
display_preorder(<file_pointer>)
```

Client code should contain the functions above if you want to test this code. For examples, look at [tests](https://github.com/KulkarniKaustubh/BST-HardDisk/tree/master/tests)

To compile and execute:

``` sh
gcc implementation.c <client>.c -o <output_file>.out
./<output_file>.out
```

This will run the client code against the file which will implement the BST in the file. You can use the three client files already provided if you want to.
