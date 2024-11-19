1 Adding a book.
 2 Deleting a book.
 3 Editing the book.
 4 Search for any of the fields in the entire database.
 5 Output information about books.
Requirements:
1 Books can be described in both Russian and English.
 2 Using the most optimal tree for the implementation of the program.
 3 The program starts and waits for a set of commands to be entered. 
 The 4 commands that come to the input have the following format:
1 ADD "Book title" "Author/Authors" (separated by commas) "Year of release" "Reader rating" "Book price" "Genre". If the specified book is not yet in the tree, insert and print the word "ADD_DONE", if it already exists, leave the tree as it was and print the word "ADD_ALREADY".
 2 DELETE the "Book Title". If the specified book is in the tree, delete and print the word "DELETE_DONE", if not, leave the tree as it was and print the word "DELETE_CANNOT".
 3 SEARCH item, where item is one of the fields of the book. The answer is "YES/NO".
 4 EDIT "Book Title": "Book Title" "Author/Authors" (separated by commas) "
