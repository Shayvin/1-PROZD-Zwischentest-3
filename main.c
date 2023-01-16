#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// STRUCT, ENUMS

enum node_type
{
    number_type = 1, operator_type, parenthesis_type
};

struct node
{
    char operator;
    int number;
    char parenthesis;
    enum node_type type;
    struct node* next;
};

// CREATE NODE SECTION

struct node* createOperatorNode(char operator)
{
    struct node* newNode = (struct node*)malloc(sizeof(struct node));

    newNode->type = operator_type;
    newNode->operator = operator;
    newNode->next = NULL;

    return newNode;
}

struct node* createNumberNode(int number)
{
    struct node* newNode = (struct node*)malloc(sizeof(struct node));

    newNode->type = number_type;
    newNode->number = number;
    newNode->next = NULL;

    return newNode;
}

struct node* createParenthesisNode(char parenthesis)
{
    struct node* newNode = (struct node*)malloc(sizeof(struct node));

    newNode->type = parenthesis_type;
    newNode->parenthesis = parenthesis;
    newNode->next = NULL;

    return newNode;
}

// FIND STUFF

struct node* findFirstPointOperator(struct node *startNode)
{
    for (struct node* current = startNode; current != NULL; current = current->next)
    {
        if(current->type == operator_type)
        {
            if (current->operator == '*' || current->operator == '/')
            {
                return current;
            }
        }
    }
    return NULL;
}

struct node* findFirstDashOperator(struct node *startNode)
{
    for(struct node* current = startNode; current != NULL; current = current->next)
    {
        if(current->type == operator_type)
        {
            if (current->operator == '+' || current->operator == '-')
            {
                return current;
            }
        }
    }
    return NULL;
}

struct node* findFirstParenthesis(struct node *startNode)
{
    for(struct node* current = startNode; current != NULL; current = current->next)
    {
        if(current->type == parenthesis_type)
        {
            if(current->parenthesis == '(')
            {
                return current;
            }
        }
    }
    return NULL;
}

struct node* findPreviousNode(struct node* head, struct node* node)
{
    while(head != NULL && head->next != node)
    {
        head = head->next;
    }
    return head;
}

// Eingabe umwandeln (vorgegeben)

struct node *inputToNode(const char *input)
{
    int number = atoi(input);
    if (number != 0) {
        return createNumberNode(number);
    }
    if (input[0] == '0') {
        return createNumberNode(0);
    }
    if (strcmp(input, "(") == 0 ||
        strcmp(input, ")") == 0) {
        return createParenthesisNode(input[0]);
    }

    if (strcmp(input, "+") == 0 ||
        strcmp(input, "-") == 0 ||
        strcmp(input, "*") == 0 ||
        strcmp(input, "/") == 0) {
        return createOperatorNode(input[0]);
    }
    return NULL;
}

struct node* addLast(struct node* head, struct node* newNode)
{
    if(head == NULL)
    {
        head = newNode;
    }
    else
    {
        struct node* currentNode = head;
        while(currentNode->next != NULL)
        {
            currentNode = currentNode->next;
        }
        currentNode->next = newNode;
    }
    return head;
}

void printList(struct node* head)
{
    while(head != NULL)
    {
        if(head->type == operator_type)
        {
            printf("%c", head->operator);
        }
        else if(head->type == number_type)
        {
            printf("%d", head->number);
        }
        else if(head->type == parenthesis_type)
        {
            printf("%c", head->parenthesis);
        }
        head = head->next;
    }
}

// Remove Node Funktion

struct node* removeNode(struct node* head, struct node* node)
{
    if(head != NULL && node != NULL)
    {
        if(head == node)
        {
            head = head->next;
        }
        else
        {
            findPreviousNode(head, node)->next = node->next;
        }
        free(node);
    }
    return head;
}

void calculate(struct node* head)
{
    struct node *pointNode = findFirstPointOperator(head); // node pointer auf ersten point operator

    while (pointNode != NULL) // Solange nicht leer
    {
        struct node *number1 = findPreviousNode(head, pointNode); // Finde Nummer vor dem Operator
        struct node *number2 = pointNode->next; // Finde Nummer nach dem Operator

        if (pointNode->operator == '*')
        {
            number1->number *= number2->number; // Rechne aus
            removeNode(head, pointNode); // Remove Operator
            removeNode(head, number2); // Remove 2. Zahl
        }
        else if (pointNode->operator == '/')
        {
            if (number2->number == 0) // Divisionen durch 0 nicht erlaubt
            {
                number2->number = 1; // Setze wenn Division durch 0 auf 1
            }
            number1->number /= number2->number;
            removeNode(head, pointNode);
            removeNode(head, number2);
        }
        printf("\nResulting term: ");
        printList(head);
        pointNode = findFirstPointOperator(head); // Nächsten Operator suchen
    }

    struct node *dashNode = findFirstDashOperator(head);

    while (dashNode != NULL)
    {
        struct node *number1 = findPreviousNode(head, dashNode);
        struct node *number2 = dashNode->next;

        if(dashNode->operator == '+')
        {
            number1->number += number2->number;
            removeNode(head, dashNode);
            removeNode(head, number2);
        }
        else if(dashNode->operator == '-')
        {
            number1->number -= number2->number;
            removeNode(head, dashNode);
            removeNode(head, number2);
        }

        printf("\nResulting term: ");
        printList(head);

        dashNode = findFirstDashOperator(head);
    }
}

int main()
{
    char input[20]; // String
    struct node* result = NULL;

    while(1)
    {
        printf("\nInput: ");
        scanf("%s", input);
        if(input[0] == '=') // Wenn eingabe '=' abbrechen
        {
            calculate(result);
            break;
        }
        result = addLast(result, inputToNode(input));
        printf("\nTerm: ");
        printList(result);
    }
    return 0;
}