#define _CRT_SECURE_NO_WARNINGS

#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

typedef struct Book
{
    char* title;
    char** authors;
    int author_count;
    int year;
    float rating;
    float price;
    char* genre;

    int height;
    struct Book* left, * right;
} Book;

char* readstring(FILE* stream)
{
    size_t length = 0;
    char* temp = (char*)calloc(1, sizeof(char));
    char inputchar;

    while ((inputchar = fgetc(stream)) != '\n' && !feof(stream))
    {
        temp[length++] = inputchar;
        temp = (char*)realloc(temp, length + 1);
    }

    temp[length] = '\0';

    return temp;
}

char* copyString(char* source)
{
    char* dest = malloc(strlen(source) + 1);
    strcpy(dest, source);
    return dest;
}

void removeSpaces(char* str)
{
    int i = 0, j = 0;
    int space_flag = 0;

    while (str[i] != '\0')
    {
        if (str[i] == ' ' || str[i] == '\t')
        {
            if (space_flag == 0)
            {
                str[j++] = ' ';
                space_flag = 1;
            }
        }
        else
        {
            str[j++] = str[i];
            space_flag = 0;
        }

        i++;
    }

    str[j] = '\0';

    if (j > 0 && str[j - 1] == ' ')
    {
        str[j - 1] = '\0';
    }
}

char** splitAuthors(char* authorsString, int* author_count)
{
    char* token;
    char* str = copyString(authorsString);
    char** authors = NULL;
    *author_count = 0;

    token = strtok(str, ",");

    while (token != NULL)
    {
        token += (*token == ' ' || *token == '\t');
        authors = realloc(authors, (*author_count + 1) * sizeof(char*));
        authors[*author_count] = copyString(token);
        (*author_count)++;
        token = strtok(NULL, ",");
    }

    free(str);
    return authors;
}

//

int height(Book* N)
{
    if (N == NULL)
    {
        return 0;
    }

    return N->height;
}

int maximum(int a, int b)
{
    return (a > b) ? a : b;
}

// 

Book* createBook(char* title, char* authorsString, int year, float rating, float price, char* genre)
{
    Book* newBook = (Book*)malloc(sizeof(Book));

    newBook->title = (char*)malloc((strlen(title) + 1) * sizeof(char));
    strcpy(newBook->title, title);

    newBook->authors = splitAuthors(authorsString, &newBook->author_count);

    newBook->genre = (char*)malloc((strlen(genre) + 1) * sizeof(char));
    strcpy(newBook->genre, genre);

    newBook->year = year;
    newBook->rating = rating;
    newBook->price = price;

    newBook->left = newBook->right = NULL;
    newBook->height = 1;

    return newBook;
}

void freeTree(Book* root)
{
    if (root == NULL)
    {
        return;
    }

    freeTree(root->left);
    freeTree(root->right);

    for (int i = 0; i < root->author_count; i++)
    {
        free(root->authors[i]);
    }

    free(root->title);
    free(root->authors);
    free(root->genre);

    free(root);
}

// 

Book* rightRotate(Book* y)
{
    Book* x = y->left;
    Book* T2 = x->right;

    x->right = y;
    y->left = T2;

    y->height = maximum(height(y->left), height(y->right)) + 1;
    x->height = maximum(height(x->left), height(x->right)) + 1;

    return x;
}

Book* leftRotate(Book* x)
{
    Book* y = x->right;
    Book* T2 = y->left;

    y->left = x;
    x->right = T2;

    x->height = maximum(height(x->left), height(x->right)) + 1;
    y->height = maximum(height(y->left), height(y->right)) + 1;

    return y;
}

int getBalance(Book* N)
{
    if (N == NULL)
    {
        return 0;
    }

    return height(N->left) - height(N->right);
}

Book* insertBook(Book* node, char* title, char* authors, int year, float rating, float price, char* genre)
{
    if (node == NULL)
    {
        printf("ADD_DONE\n");
        return createBook(title, authors, year, rating, price, genre);
    }

    int cmp = strcmp(title, node->title);

    if (cmp < 0)
    {
        node->left = insertBook(node->left, title, authors, year, rating, price, genre);
    }
    else if (cmp > 0)
    {
        node->right = insertBook(node->right, title, authors, year, rating, price, genre);
    }
    else
    {
        printf("ADD_ALREADY\n");
        return node;
    }

    node->height = 1 + maximum(height(node->left), height(node->right));

    int balance = getBalance(node);

    if (balance > 1 && strcmp(title, node->left->title) < 0)
    {
        return rightRotate(node);
    }

    if (balance < -1 && strcmp(title, node->right->title) > 0)
    {
        return leftRotate(node);
    }

    if (balance > 1 && strcmp(title, node->left->title) > 0)
    {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }

    if (balance < -1 && strcmp(title, node->right->title) < 0)
    {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    return node;
}

//

Book* minValueNode(Book* node)
{
    Book* current = node;

    while (current->left != NULL)
    {
        current = current->left;
    }

    return current;
}

// 

Book* deleteBook(Book* root, char* title)
{
    if (root == NULL)
    {
        printf("DELETE_CANNOT\n");
        return root;
    }

    int cmp = strcmp(title, root->title);

    if (cmp < 0)
    {
        root->left = deleteBook(root->left, title);
    }
    else if (cmp > 0)
    {
        root->right = deleteBook(root->right, title);
    }
    else
    {
        if (root->left == NULL || root->right == NULL)
        {
            Book* temp = root->left ? root->left : root->right;

            if (temp == NULL)
            {
                temp = root;
                root = NULL;
            }
            else
            {
                *root = *temp;
            }

            free(temp);
            printf("DELETE_DONE\n");
        }
        else
        {
            Book* temp = minValueNode(root->right);

            free(root->title);
            free(root->genre);

            for (int i = 0; i < root->author_count; i++)
            {
                free(root->authors[i]);
            }

            free(root->authors);

            root->title = temp->title;
            root->authors = temp->authors;
            root->genre = temp->genre;

            root->year = temp->year;
            root->rating = temp->rating;
            root->price = temp->price;
            root->author_count = temp->author_count;

            root->right = deleteBook(root->right, temp->title);
        }
    }

    if (root == NULL)
    {
        return root;
    }

    root->height = 1 + maximum(height(root->left), height(root->right));
    int balance = getBalance(root);

    if (balance > 1 && getBalance(root->left) >= 0)
    {
        return rightRotate(root);
    }

    if (balance > 1 && getBalance(root->left) < 0)
    {
        root->left = leftRotate(root->left);
        return rightRotate(root);
    }

    if (balance < -1 && getBalance(root->right) <= 0)
    {
        return leftRotate(root);
    }

    if (balance < -1 && getBalance(root->right) > 0)
    {
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }

    return root;
}

// 

Book* searchBook(Book* root, char* title)
{
    if (root == NULL || strcmp(root->title, title) == 0)
    {
        return root;
    }

    if (strcmp(title, root->title) < 0)
    {
        return searchBook(root->left, title);
    }
    else
    {
        return searchBook(root->right, title);
    }
}

//

void editBook(Book* root, char* title, char* newTitle, char* authors, int year, float rating, float price, char* genre)
{
    Book* book = searchBook(root, title);

    if (book != NULL)
    {
        free(book->title);
        free(book->genre);

        for (int i = 0; i < root->author_count; i++)
        {
            free(book->authors[i]);
        }

        free(book->authors);

        book->title = (char*)malloc((strlen(newTitle) + 1) * sizeof(char));
        book->genre = (char*)malloc((strlen(genre) + 1) * sizeof(char));

        strcpy(book->title, newTitle);
        strcpy(book->genre, genre);

        book->authors = splitAuthors(authors, &book->author_count);

        book->year = year;
        book->rating = rating;
        book->price = price;

        printf("EDIT_DONE\n");
    }
    else
    {
        printf("EDIT_CANNOT\n");
    }
}

//

int searchField(Book* root, char* item, int type)
{
    if (root == NULL)
    {
        return 0;
    }

    if (type == 1)
    {
        if (strstr(root->title, item) || strstr(root->genre, item))
        {
            return 1;
        }

        for (int i = 0; i < root->author_count; i++)
        {
            if (strstr(root->authors[i], item))
            {
                return 1;
            }
        }
    }
    else
    {
        float numValue = atof(item);

        if ((int)numValue == root->year || numValue == root->rating || numValue == root->price)
        {
            return 1;
        }
    }

    return searchField(root->left, item, type) || searchField(root->right, item, type);
}

//

void printAuthors(char** authors, int count)
{
    for (int i = 0; i < count; i++)
    {
        printf("%s", authors[i]);
        if (i < count - 1)
        {
            printf(", ");
        }
    }
}

void inOrder(Book* root)
{
    if (root != NULL)
    {
        inOrder(root->left);
        printf("\"%s\" \"", root->title);
        printAuthors(root->authors, root->author_count);
        printf("\" %d %.2f %.2f \"%s\"\n", root->year, root->rating, root->price, root->genre);
        inOrder(root->right);
    }
}

void preOrder(Book* root)
{
    if (root != NULL)
    {
        printf("\"%s\" \"", root->title);
        printAuthors(root->authors, root->author_count);
        printf("\" %d %.2f %.2f \"%s\"\n", root->year, root->rating, root->price, root->genre);
        preOrder(root->left);
        preOrder(root->right);
    }
}

void postOrder(Book* root)
{
    if (root != NULL)
    {
        postOrder(root->left);
        postOrder(root->right);
        printf("\"%s\" \"", root->title);
        printAuthors(root->authors, root->author_count);
        printf("\" %d %.2f %.2f \"%s\"\n", root->year, root->rating, root->price, root->genre);
    }
}

//

int checkArguments(char* str, int arguments, int colon)
{
    int current = 0;
    int flag = 0;
    int colonflag = !colon;

    while (*str != ' ' && *str != '\t' && *str != '\0')
    {
        str++;
    }

    while (*str != '\0')
    {
        while (*str == ' ' || *str == '\t')
        {
            str++;
        }

        if (*str != '\0')
        {
            if (*str != ':')
            {
                while (((*str != ' ' && *str != '\t') || flag) && *str != '\0')
                {
                    if (*str == '\"')
                    {
                        flag = !flag;
                    }

                    if (*str == ':' && !flag)
                    {
                        if (current != 0 && colon)
                        {
                            return 0;
                        }
                        else
                        {
                            colonflag = colon;
                        }
                    }

                    str++;
                }
            }
            else if (current != 1 && colon)
            {
                return 0;
            }
            else
            {
                colonflag = colon;
            }

            current += (!flag && *str != ':');
            str += (*str == ':');
        }
    }

    return (current == arguments) && colonflag;
}

int checkCommand(char* command, Book** root)
{
    while (*command == ' ' || *command == '\t')
    {
        command++;
    }

    removeSpaces(command);

    if (strncmp(command, "ADD", 3) == 0 && checkArguments(command, 6, 0))
    {
        char* titleStart = strchr(command + 4, '\"');
        if (titleStart == NULL) return 0;
        char* titleEnd = strchr(titleStart + 1, '\"');
        if (titleEnd == NULL) return 0;

        char* authorStart = strchr(titleEnd + 1, '\"');
        if (authorStart == NULL) return 0;
        char* authorEnd = strchr(authorStart + 1, '\"');
        if (authorEnd == NULL) return 0;

        int year;
        float rating, price;

        int numFields = sscanf(authorEnd + 1, "%d %f %f", &year, &rating, &price);

        if (numFields != 3 || year <= 0 || rating < 0 || price < 0)
        {
            return 0;
        }

        char* genreStart = strchr(authorEnd + 1, '\"');
        if (genreStart == NULL) return 0;
        char* genreEnd = strchr(genreStart + 1, '\"');
        if (genreEnd == NULL) return 0;

        char* title, * authors, * genre;

        title = copyString(titleStart + 1);
        title[titleEnd - titleStart - 1] = '\0';

        authors = copyString(authorStart + 1);
        authors[authorEnd - authorStart - 1] = '\0';

        genre = copyString(genreStart + 1);
        genre[genreEnd - genreStart - 1] = '\0';

        *root = insertBook(*root, title, authors, year, rating, price, genre);
    }
    else if (strncmp(command, "DELETE", 6) == 0 && checkArguments(command, 1, 0))
    {
        char* titleStart = strchr(command + 7, '\"');
        if (titleStart == NULL) return 0;
        char* titleEnd = strchr(titleStart + 1, '\"');
        if (titleEnd == NULL) return 0;

        char* title;

        title = copyString(titleStart + 1);
        title[titleEnd - titleStart - 1] = '\0';

        *root = deleteBook(*root, title);
    }
    else if (strncmp(command, "SEARCH", 6) == 0 && checkArguments(command, 1, 0))
    {
        if (strchr(command + 7, '\"') != NULL)
        {
            char* itemStart = strchr(command + 7, '\"');
            if (itemStart == NULL) return 0;
            char* itemEnd = strchr(itemStart + 1, '\"');
            if (itemEnd == NULL) return 0;

            char* item;

            item = copyString(itemStart + 1);
            item[itemEnd - itemStart - 1] = '\0';

            printf("%s\n", searchField(*root, item, 1) ? "YES" : "NO");
        }
        else
        {
            printf("%s\n", searchField(*root, command + 7, 2) ? "YES" : "NO");
        }
    }
    else if (strncmp(command, "EDIT", 4) == 0 && checkArguments(command, 7, 1))
    {
        char* titleStart = strchr(command + 5, '\"');
        if (titleStart == NULL) return 0;
        char* titleEnd = strchr(titleStart + 1, '\"');
        if (titleEnd == NULL) return 0;

        char* newTitleStart = strchr(titleEnd + 1, '\"');
        if (newTitleStart == NULL) return 0;
        char* newTitleEnd = strchr(newTitleStart + 1, '\"');
        if (newTitleEnd == NULL) return 0;

        char* authorStart = strchr(newTitleEnd + 1, '\"');
        if (authorStart == NULL) return 0;
        char* authorEnd = strchr(authorStart + 1, '\"');
        if (authorEnd == NULL) return 0;

        int year;
        float rating, price;

        int numFields = sscanf(authorEnd + 1, "%d %f %f", &year, &rating, &price);

        if (numFields != 3 || year <= 0 || rating < 0 || price < 0)
        {
            return 0;
        }

        char* genreStart = strchr(authorEnd + 1, '\"');
        if (genreStart == NULL) return 0;
        char* genreEnd = strchr(genreStart + 1, '\"');
        if (genreEnd == NULL) return 0;

        char* title, * newTitle, * authors, * genre;

        title = copyString(titleStart + 1);
        title[titleEnd - titleStart - 1] = '\0';

        newTitle = copyString(newTitleStart + 1);
        newTitle[newTitleEnd - newTitleStart - 1] = '\0';

        authors = copyString(authorStart + 1);
        authors[authorEnd - authorStart - 1] = '\0';

        genre = copyString(genreStart + 1);
        genre[genreEnd - genreStart - 1] = '\0';

        editBook(*root, title, newTitle, authors, year, rating, price, genre);
    }
    else if (strncmp(command, "LNRPRINTTREE", 12) == 0 && checkArguments(command, 0, 0))
    {
        if (*root != NULL)
        {
            inOrder(*root);
        }
        else
        {
            printf("Tree is empty\n");
        }
    }
    else if (strncmp(command, "NLRPRINTTREE", 12) == 0 && checkArguments(command, 0, 0))
    {
        if (*root != NULL)
        {
            preOrder(*root);
        }
        else
        {
            printf("Tree is empty\n");
        }
    }
    else if (strncmp(command, "LRNPRINTTREE", 12) == 0 && checkArguments(command, 0, 0))
    {
        if (*root != NULL)
        {
            postOrder(*root);
        }
        else
        {
            printf("Tree is empty\n");
        }
    }
    else if (strncmp(command, "EXIT", 4) == 0 && checkArguments(command, 0, 0))
    {
        freeTree(*root);
        exit(0);
    }
    else
    {
        return 0;
    }

    return 1;
}

int main()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    Book* root = NULL;
    char* command;

    while (1)
    {
        command = readstring(stdin);

        if (!checkCommand(command, &root))
        {
            printf("Error parse\n");
        }

        free(command);
    }

    return 0;
}
