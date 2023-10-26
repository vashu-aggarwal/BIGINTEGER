#include "BigInteger.h"
// Function to create a new node with the given data
struct node *createNode(int data)
{
    struct node *newNode = (struct node *)malloc(sizeof(struct node));
    if (newNode == NULL)
    {
        printf("Memory allocation failed\n");
    }
    newNode->data = data;
    newNode->next = NULL;
    return newNode;
}
// Function to add a digit to the BigInteger
void addDigit(struct BigInteger *bigInt, int digit)
{
    struct node *newNode = createNode(digit);
    newNode->next = bigInt->head;
    bigInt->head = newNode;
    bigInt->length++;
}

// Function to initialize a BigInteger from a string
struct BigInteger initialize(const char *s)
{
    struct BigInteger bigInt;
    bigInt.head = NULL;
    bigInt.length = 0;

    int strLen = strlen(s);
    int startIndex = 0;

    if (s[0] == '-')
    {
        bigInt.sign = -1;
        startIndex = 1;
    }
    else
    {
        bigInt.sign = 1;
    }

    for (int i = startIndex; i < strLen; i++)
    {
        if (s[i] >= '0' && s[i] <= '9')
        {
            addDigit(&bigInt, s[i] - '0');
        }
        else
        {
            printf("Invalid character in input string: %c\n", s[i]);
        }
    }

    return bigInt;
}

struct BigInteger sub(struct BigInteger a, struct BigInteger b)
{
    struct node *first = a.head;
    struct node *second = b.head;

    struct BigInteger result;
    result.head = NULL;
    result.length = 0;

    // Handle the signs of a and b
    if (a.sign == -1 && b.sign == 1)
    {
        a.sign = -1;
        result = add(a, b);

        result.sign = -1;
    }
    else if (a.sign == 1 && b.sign == -1)
    {
        b.sign = 1;
        result = add(a, b);
    }
    else
    {

        struct node *currentA = a.head;
        struct node *currentB = b.head;
        struct node *prev = NULL;
        int borrow = 0;

        while (currentA != NULL || currentB != NULL)
        {

            int diff = borrow + (currentA ? currentA->data : 0) - (currentB ? currentB->data : 0);

            if (currentA)
            {
                currentA = currentA->next;
            }
            if (currentB)
            {
                currentB = currentB->next;
            }

            if (diff < 0)
            {
                diff += 10;
                borrow = -1;
            }
            else
            {
                borrow = 0;
            }

            struct node *newNode = (struct node *)malloc(sizeof(struct node));
            newNode->data = diff;
            newNode->next = NULL;

            if (prev == NULL)
            {
                result.head = newNode;
            }
            else
            {
                prev->next = newNode;
            }

            prev = newNode;
            result.length++;
        }

        // Remove leading zeros
        reverse(&result);
        while (result.head->next != NULL && result.head->data == 0)
        {
            struct node *temp = result.head;
            result.head = result.head->next;
            free(temp);
            result.length--;
        }
    }
    reverse(&result);
    return result;
}

struct BigInteger add(struct BigInteger a, struct BigInteger b)
{
    struct BigInteger result;
    result.head = NULL;
    result.length = 0;

    // Handle the signs of a and b
    if (a.sign == -1 && b.sign == 1)
    {
        a.sign = 1;
        result = sub(b, a);
    }
    else if (a.sign == 1 && b.sign == -1)
    {
        b.sign = 1;
        result = sub(a, b);
    }
    else
    {
        result.sign = a.sign;
        struct node *currentA = a.head;
        struct node *currentB = b.head;
        struct node *prev = NULL;
        int carry = 0;

        while (currentA != NULL || currentB != NULL || carry)
        {
            int sum = carry + (currentA ? currentA->data : 0) + (currentB ? currentB->data : 0);

            if (currentA)
            {
                currentA = currentA->next;
            }
            if (currentB)
            {
                currentB = currentB->next;
            }

            struct node *newNode = (struct node *)malloc(sizeof(struct node));
            newNode->data = sum % 10;
            newNode->next = NULL;

            if (prev == NULL)
            {
                result.head = newNode;
            }
            else
            {
                prev->next = newNode;
            }

            prev = newNode;
            carry = sum / 10;
            result.length++;
        }
    }

    return result;
}

void insert(Node **head, int val)
{
    Node *newNode = createNode(val);
    if (*head == NULL)
    {
        *head = newNode;
        return;
    }
    Node *temp = *head;
    while (temp->next != NULL)
    {
        temp = temp->next;
    }
    temp->next = newNode;
}

// Function to reverse the linked list and return its length
int rev(struct node **head)
{
    Node *prev = NULL;
    Node *curr = *head;
    Node *forward;
    int len = 0;
    while (curr != NULL)
    {
        len++;
        forward = curr->next;
        curr->next = prev;
        prev = curr;
        curr = forward;
    }
    *head = prev;
    return len;
}
// Function to make an empty linked list of given size
Node *empty_list(int size)
{
    Node *head = NULL;
    while (size--)
    {
        insert(&head, 0);
    }
    return head;
}

Node *multiply(Node *first, Node *second, int n, int m)
{

    Node *result = empty_list(m + n + 1);
    Node *ptr2 = second, *resptr1 = result, *resptr2, *ptr1;
    while (ptr2)
    {

        int carry = 0;
        resptr2 = resptr1;
        ptr1 = first;
        while (ptr1)
        {
            int mul = ptr1->data * ptr2->data + carry;
            resptr2->data += mul % 10;
            carry = mul / 10 + resptr2->data / 10;
            resptr2->data = resptr2->data % 10;

            ptr1 = ptr1->next;
            resptr2 = resptr2->next;
        }
        if (carry > 0)
        {
            resptr2->data += carry;
        }
        resptr1 = resptr1->next;
        ptr2 = ptr2->next;
    }
    rev(&result);

    while (result->data == 0)
    {
        struct node *temp = result;
        result = result->next;
        free(temp);
    }
    rev(&result);
    return result;
}

struct BigInteger mul(struct BigInteger a, struct BigInteger b)
{
    struct node *first = a.head;
    struct node *second = b.head;

    struct BigInteger result;
    result.head = multiply(first, second, a.length, b.length);
    return result;
}

void reverse(struct BigInteger *bigInt)
{
    struct node *prev = NULL;
    struct node *current = bigInt->head;
    struct node *next = NULL;

    while (current != NULL)
    {
        next = current->next;
        current->next = prev;
        prev = current;
        current = next;
    }

    bigInt->head = prev;
}

int compare(struct BigInteger a, struct BigInteger b)
{
    if (a.length > b.length)
    {
        return 1;
    }
    else if (a.length < b.length)
    {
        return -1;
    }
    else
    {
        struct node *nodeA = a.head;
        struct node *nodeB = b.head;

        while (nodeA != NULL)
        {
            if (nodeA->data > nodeB->data)
            {
                return 1;
            }
            else if (nodeA->data < nodeB->data)
            {
                return -1;
            }
            nodeA = nodeA->next;
            nodeB = nodeB->next;
        }
    }

    return 0;
}

void cleanUp(struct BigInteger *num)
{
    struct node *current = num->head;
    while (current != NULL)
    {
        struct node *temp = current;
        current = current->next;
        free(temp);
    }
    num->head = NULL;
    num->length = 0;
}


struct BigInteger div1(struct BigInteger a, struct BigInteger b)
{
    // Check for division by zero
    if (b.length == 0 || (b.length == 1 && b.head->data == 0))
    {
        printf("Division by zero is not allowed.\n");
        exit(1);
    }

    // Initialize the result BigInteger
    struct BigInteger result;
    result.head = NULL;
    result.length = 0;
    result.sign = a.sign * b.sign;

    // Check if the dividend (a) is zero
    if (a.length == 0 || (a.length == 1 && a.head->data == 0))
    {
        // Result is zero
        return result;
    }

    // Initialize a temporary dividend for the division process
    struct BigInteger tempDividend;
    tempDividend.head = NULL;
    tempDividend.length = 0;
    tempDividend.sign = 1; // Always positive for the intermediate steps

    struct node *currentDigit = a.head;

    // Perform long division
    while (currentDigit != NULL)
    {
        addDigit(&tempDividend, currentDigit->data);
        int quotient = 0;

        // Perform subtraction until the dividend is less than the divisor
        while (compare(tempDividend, b) >= 0)
        {
            tempDividend = sub(tempDividend, b);
            quotient++;
        }

        // Append the calculated quotient digit to the result
        addDigit(&result, quotient);

        currentDigit = currentDigit->next;
    }

    // Clean up the temporary dividend
    cleanUp(&tempDividend);

    return result;
}


struct BigInteger mod(struct BigInteger a, struct BigInteger b)
{
    // Check for division by zero
    if (b.length == 0 || (b.length == 1 && b.head->data == 0))
    {
        printf("Division by zero is not allowed.\n");
        exit(1);
    }

    // Check if the dividend (a) is zero
    if (a.length == 0 || (a.length == 1 && a.head->data == 0))
    {
        // Result is zero
        return a;
    }

    // Initialize a temporary dividend for the division process
    struct BigInteger tempDividend;
    tempDividend.head = NULL;
    tempDividend.length = 0;
    tempDividend.sign = 1; // Always positive for the intermediate steps

    struct node *currentDigit = a.head;

    // Perform long division
    while (currentDigit != NULL)
    {
        addDigit(&tempDividend, currentDigit->data);
        int quotient = 0;

        // Perform subtraction until the dividend is less than the divisor
        while (compare(tempDividend, b) >= 0)
        {
            tempDividend = sub(tempDividend, b);
            quotient++;
        }

        currentDigit = currentDigit->next;
    }

    // Clean up the temporary dividend
    cleanUp(&tempDividend);

    return tempDividend;
}


// Function to print the BigInteger
void display(struct BigInteger num)
{
    if (num.sign == -1)
    {
        printf("-");
    }

    reverse(&num);

    struct node *current = num.head;
    while (current != NULL)
    {
        printf("%d", current->data);
        current = current->next;
    }
    printf("\n");
}
