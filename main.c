#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define FLUSH stdin=freopen(NULL,"r",stdin)

typedef struct{
  int SIN;
  char firstName[20];
  char lastName[20];
  double Income;
} Employee;

typedef struct node{
  Employee emp;
  struct node* next; //Pointer to the nextnode
  struct node* prev;//Pointer to the previousnode
} Node;

Node* head = NULL;
Node* tail = NULL;

Node* createNode(Employee node) {
  Node* nd = (Node*)malloc(sizeof(Node));
  nd->emp = node; // Put the value in the node  
  nd->next = NULL; // Next pointer is null for now  
  nd->prev = NULL;
  return nd; // Return a pointer to the new node
}

void List(Node* temp) {

  printf("=========== PRINTING Employee INFORMATIONS ===========\n");
  // Loop as long as there are more nodes  
  while (temp != NULL) {
    printf("%d \t %s \t %s \t %lf\n", temp->emp.SIN, temp->emp.firstName,temp->emp.lastName, temp->emp.Income);
    temp = temp->next; // Go on to the next node
  }
}

void readFile(int *countStudent){
  FILE *in;
  if ((in = fopen("records.csv", "r")) == NULL) {
      printf("File does not exist");
  } else {
    printf("Loading data from records.csv file ...");
    Node* nd = NULL;
    Employee emp;
    char buf[4098];
    int row_count = 0;
    int field_count = 0;
    while (fgets(buf, 4098, in)) {
        field_count = 0;
        row_count++;
        char *field = strtok(buf, ",");
        while (field) {
            if (field_count == 0) {
                emp.SIN = strtol(field,NULL,10);
            }
            if (field_count == 1) {
                strcpy(emp.firstName,field);
            }
            if (field_count == 2) {
                strcpy(emp.lastName,field);
            }
            if (field_count == 3) {
                emp.Income = strtof(field,NULL);
            }
            field = strtok(NULL, ",");
            field_count++;
        }
        nd = createNode(emp);
        if (head == NULL){
          head = nd;
          tail = nd;
        } else {
          tail->next = nd;
          nd->prev = tail;
          tail = nd;
        }
    }
    printf("%d records loaded.\n",row_count);
  }

  fclose(in);

}

void saveFile(Node *temp, char file[]) {
   
  FILE *out = fopen(file, "w");
  while (temp != NULL) {
    fprintf(out, "%d, %s, %s, %lf\n", temp->emp.SIN,temp->emp.firstName, temp->emp.lastName, temp->emp.Income);
    temp = temp->next; // Go on to the next node
  }
  printf("Data saved in the file %s\n",file);
  fclose(out);
}

void promtSaveOperation(Node* temp){
  FLUSH;
  char sid[1024];
  printf("Save displayed records in the file (Enter to skip):");
  scanf("%[^\n]%*c",sid);
  if(strcmp(sid,"") == 0){
    printf("Data was not saved.\n");
    return;
  }else if(strcmp(sid,"record.csv") == 0){
    char c;
    printf("File records.csv already exist. Do you want to override it? (Y/N):");
    scanf("%c",&c);
    if(c == 'Y'){
      saveFile(temp,sid);
    }else{
      printf("Data was not saved.\n");
      return;
    }
  }else{
    saveFile(temp,sid);
  }

}

Node* duplicateDataForSort(){
    Node* src;
    Node* rear;
    Node* temp;
    Node* nn;
    temp = head;
    while (temp != NULL) {
      nn = createNode(temp->emp);
      if(src == NULL){
        src = rear = nn;
      }else{
        rear->next = nn;
        nn->prev = rear;
        rear = nn;
      }

      temp = temp->next; // Go on to the next node
    }
    return src;
}

void deleteEmployeeLName(char *name){
  Node* temp = head;
  if (temp == NULL){
    printf("List is empty!");
    return;
  }
  Node* prev = NULL;
  if (strcmp(temp->emp.lastName,name) == 0){
      head = temp->next;
      free(temp);
      return;
  } else {
      while ((temp != NULL) && (strcmp(temp->emp.lastName,name) != 0)){
        prev = temp;
        temp = temp->next;
      }
      prev->next=temp->next;
      free(temp);
  }
}

void deleteEmployeeFName(char *name){
  printf("Delete using first name.\n");
  Node* temp = head;
  if (temp == NULL){
    printf("List is empty!");
    return;
  }
  Node* prev = NULL;
  if (strcmp(temp->emp.firstName,name) == 0){
      head = temp->next;
      free(temp);
      return;
  } else {
      while ((temp != NULL) && (strcmp(temp->emp.firstName,name) != 0)){
        prev = temp;
        temp = temp->next;
      }
      prev->next=temp->next;
      free(temp);
  }
}


void deleteEmployeeSID(int id){
  Node* temp = head;
  if (temp == NULL){
    printf("List is empty!");
    return;
  }
  Node* prev = NULL;
  if (temp->emp.SIN == id){
      head = temp->next;
      free(temp);
      return;
  } else {
      while ((temp != NULL) && (temp->emp.SIN != id)){
        prev = temp;
        temp = temp->next;
      }
      prev->next=temp->next;
      free(temp);
  }
}

void deleteEmployeeIncome(float id){
  Node* temp = head;
  if (temp == NULL){
    printf("List is empty!");
    return;
  }
  Node* prev = NULL;
  if (temp->emp.Income == id){
      head = temp->next;
      free(temp);
      return;
  } else {
      while ((temp != NULL) && (temp->emp.Income != id)){
        prev = temp;
        temp = temp->next;
      }
      prev->next=temp->next;
      free(temp);
  }
}

void deleteEmployee(int attribute){
  printf("%d\n",attribute );
  int sid;
  float income;
  char fname[1025],lname[1024];
  FLUSH;
  switch(attribute){
    case 1:
          printf("Please provide the SID you want to delete.");
          scanf("%d",&sid);
          deleteEmployeeSID(sid);
          break;
    case 2:
          printf("Please provide the first name you want to delete.");
          scanf("%[^\n]%*c",fname);
          deleteEmployeeFName(fname);
          break;
    case 3:
          printf("Please provide the last name you want to delete.");
          scanf("%[^\n]%*c",fname);
          deleteEmployeeLName(fname);
          break;

    case 4:
          printf("Please provide the income record you want to delete.");
          scanf("%f",&income);
          deleteEmployeeIncome(income);
          break;
  }

}


Node* addEmployee(Node* list, Node *rear){

  printf("Adding new record:\n");
  Employee employee;
  Node* nd = NULL;
  printf("Enter SIN NO:");
  scanf("%d", &employee.SIN);
  printf("Enter Employee First Name:");
  scanf("%s", employee.firstName);
  printf("Enter Employee Last Name:");
  scanf("%s", employee.lastName);
  printf("Enter Employee Income:");
  scanf("%lf", &employee.Income);
  nd = createNode(employee);
  if (list == NULL){
    list = nd;
    rear = nd;
  } else {
    rear->next = nd;
    nd->prev = rear;
    rear = nd;
  }

  return list;
}

Node* searchList(int value) {

  bool found = false;
  Node* temp = head; // Start at given node
  Node* src;
  Node* rear;
  Node* newNode;
  src = rear = NULL;

  printf("Displaying record(s) with SID %d :\n", value);
  while (temp != NULL) {
    if (temp->emp.SIN == value) { // Found a match?  
      found = true;
      newNode = createNode(temp->emp);
      if(src == NULL){
        src = newNode;
        rear = newNode;
      }else{
        rear->next = newNode;
        newNode->prev = rear;
        rear = newNode;
      }
      printf("SIN: %d,\tFirstName: %s,\tLastName: %s,\tIncome: %lf\n", temp->emp.SIN,temp->emp.firstName, temp->emp.lastName, temp->emp.Income);
    }
    temp = temp->next; // Go on to the next node
  }
  if (found == false){
    printf("No records found!\n");
  }

  return src;
}

Node* searchListIncome(float value) {  
  bool found = false;
  Node* temp = head; // Start at given node
  Node* src;
  Node* rear;
  Node* newNode;
  src = rear = NULL;

  printf("Displaying record(s) with Income %f :\n", value);
  while (temp != NULL) {
    if (temp->emp.Income == value) { // Found a match?  
      found = true;
      newNode = createNode(temp->emp);
      if(src == NULL){
        src = newNode;
        rear = newNode;
      }else{
        rear->next = newNode;
        newNode->prev = rear;
        rear = newNode;
      }
      printf("SIN: %d,\tFirstName: %s,\tLastName: %s,\tIncome: %lf\n", temp->emp.SIN,temp->emp.firstName, temp->emp.lastName, temp->emp.Income);
    }
    temp = temp->next; // Go on to the next node
  }
  if (found == false){
    printf("No records found!\n");
  }

  return src;
}

Node* searchListFirstName(char* value) {  
  bool found = false;
  Node* temp = head; // Start at given node
  Node* src;
  Node* rear;
  Node* newNode;
  src = rear = NULL;
  printf("Displaying record(s) with first name %s:\n",value);
  while (temp != NULL) {
    if (strcmp(temp->emp.firstName,value) == 0) { // Found a match?  
      found = true;
      newNode = createNode(temp->emp);
      if(src == NULL){
        src = newNode;
        rear = newNode;
      }else{
        rear->next = newNode;
        newNode->prev = rear;
        rear = newNode;
      }
      printf("SIN: %d,\tFirstName: %s,\tLastName: %s,\tIncome: %lf\n", temp->emp.SIN,temp->emp.firstName, temp->emp.lastName, temp->emp.Income);
    }
    temp = temp->next; // Go on to the next node
  }
  if (found == false){
    printf("No records found!\n");
  }
  return src;
}

Node* searchListLastName(char* value) {  
  bool found = false;
  Node* temp = head; // Start at given node
  Node* src;
  Node* rear;
  Node* newNode;
  src = rear = NULL;

  printf("Displaying record(s) with last name %s:\n",value);
  while (temp != NULL) {
    if (strcmp(temp->emp.lastName,value) == 0) { // Found a match?  
      found = true;
      newNode = createNode(temp->emp);
      if(src == NULL){
        src = newNode;
        rear = newNode;
      }else{
        rear->next = newNode;
        newNode->prev = rear;
        rear = newNode;
      }
      printf("SIN: %d,\tFirstName: %s,\tLastName: %s,\tIncome: %lf\n", temp->emp.SIN,temp->emp.firstName, temp->emp.lastName, temp->emp.Income);
    }
    temp = temp->next; // Go on to the next node
  }
  if (found == false){
    printf("No records found!\n");
  }
  FLUSH;
  return src;
}

void searchByAttribute(int attribute){
  FLUSH;
  char  sid[1024];
  int id;
  float income;
  Node* result;
  if(attribute == 1){
    printf("Please provide the student ID you want to find:");
    scanf("%d",&id);
    result = searchList(id);
    if(result != NULL){
      promtSaveOperation(result);
    }
    while(1){
      FLUSH;
      strcpy(sid,"");
      printf("Please provide the student ID you want to find (press Enter to skip):");
      scanf("%[^\n]%*c",sid);
      if(strcmp(sid,"") == 0) break;
      id = strtol(sid,NULL,10);
      result = searchList(id);
      if(result != NULL){
        promtSaveOperation(result);
      }
    }
  }

  if(attribute == 2 || attribute == 3){
    char *placeholder;
    char name[1024];
    if(attribute == 2){
      placeholder = "first name";
    } else {
      placeholder = "last name";
    } 

    printf("Please provide the %s you want to find:",placeholder);
    scanf("%[^\n]%*c",name);
    if(attribute == 2)
      result = searchListFirstName(name);
    else  
      result = searchListLastName(name);

    if(result != NULL){
      promtSaveOperation(result);
    }

    while(1){
      FLUSH;
      printf("Please provide the %s you want to find: (press Enter to skip):",placeholder);
      strcpy(name,"");
      scanf("%[^\n]%*c",name);
      if(strcmp(name,"") == 0) break;
      printf("---%s---\n",name);
      if(attribute == 2)
        result = searchListFirstName(name);
      else  
        result = searchListLastName(name);
      if(result != NULL){
        promtSaveOperation(result);
      }
    }

  }

  if(attribute == 4){
    printf("Please provide the Income you want to find:");
    scanf("%f",&income);
    result = searchListIncome(income);
    if(result != NULL){
      promtSaveOperation(result);
    }

    while(1){
      FLUSH;
      printf("Please provide the Income you want to find (press Enter to skip):");
      strcpy(sid,"");
      scanf("%[^\n]%*c",sid);
      if(strcmp(sid,"") == 0) break;
      income = strtof(sid,NULL);
      result = searchListIncome(income);
      if(result != NULL){
        promtSaveOperation(result);
      }
    }
  }

}

/* function to swap data of two nodes a and b*/
void swapNode(Node* a, Node* b)
{
  Employee emp;
  emp = a->emp;
  a->emp = b->emp;
  b->emp = emp;
}

/* Bubble sort the given linked list */
Node* bubbleSort(int attribute, int order)
{
    int change;
    Node *temp;
    temp = duplicateDataForSort();
    if(head == NULL){
      printf("List is empty.\n");
      return temp;
    }

    Node *ptr1;
    Node *lptr = NULL;
    change = 1;
    while(change){
        change = 0;
        ptr1 = temp;
   
        while (ptr1->next != lptr)
        {
            switch(attribute){
              case 1:
                    if(order == 1){
                        if(ptr1->emp.SIN > ptr1->next->emp.SIN){
                          swapNode(ptr1,ptr1->next);
                          change = 1;
                        }
                    } else {
                      if(ptr1->emp.SIN < ptr1->next->emp.SIN){
                          swapNode(ptr1,ptr1->next);
                          change = 1;
                      }
                    }
                    break;
              case 2: 
                    if(order == 1){
                        if(strcmp(ptr1->emp.firstName,ptr1->next->emp.firstName) > 0){
                          swapNode(ptr1,ptr1->next);
                          change = 1;
                        }
                    } else {
                      if(strcmp(ptr1->emp.firstName,ptr1->next->emp.firstName) < 0){
                          swapNode(ptr1,ptr1->next);
                          change = 1;
                      }
                    }
                    break;
              case 3:
                    if(order == 1){
                        if(strcmp(ptr1->emp.lastName,ptr1->next->emp.lastName) > 0){
                          swapNode(ptr1,ptr1->next);
                          change = 1;
                        }
                    } else {
                      if(strcmp(ptr1->emp.lastName,ptr1->next->emp.lastName) < 0){
                          swapNode(ptr1,ptr1->next);
                          change = 1;
                      }
                    }
                    break;
              case 4:
                    if(order == 1){
                        if(ptr1->emp.Income > ptr1->next->emp.Income){
                          swapNode(ptr1,ptr1->next);
                          change = 1;
                        }
                    } else {
                      if(ptr1->emp.Income < ptr1->next->emp.Income){
                          swapNode(ptr1,ptr1->next);
                          change = 1;
                      }
                    }
                    break;
            } 
            ptr1 = ptr1->next;
        }
        lptr = ptr1;
    }

    return temp;
}
 

int main(){
  int countEmployee = 0;
  readFile(&countEmployee);
 
  int choice = 0;
  int searchSin = 0;
  int attribute,order;
  Node* result;
  Node* list;
  Node* rear;
  list = rear = NULL;
  char ch;
  for(;;){
    printf("\n1 - List records\n2 - Sort records\n3 - Find records \n4 - Add records \n5 - Delete records \n0 - Exit\n");
    printf("Select one option from the menu:");
    scanf("%d", &choice);
    switch(choice){
        case 0:
              saveFile(head,"records.csv");
              return 0;
        case 1:
              List(head);
              break;
        case 2:
              
              while(1){
                printf("Please select sort by attribute (1=SIN, 2=First Name, 3=Last Name, 4=Annual Income):");
                scanf("%d",&attribute);
                if(attribute >= 1 && attribute <= 4) break;
                printf("Incorret Value! Please try again\n");
              }

              while(1){
                printf("Please select order (1=Ascending, 2=Descending):");
                scanf("%d",&order);
                if(order == 1 || order == 2) break;
                printf("Incorret Value! Please try again\n");
              }

              result = bubbleSort(attribute,order);
              List(result);
              promtSaveOperation(result);
              break;
        case 3:
              readFile(&countEmployee);
              while(1){
                printf("Please select the attribute (1=SIN, 2=First Name, 3=Last Name, 4=Annual Income):");
                scanf("%d",&attribute);
                if(attribute >= 1 && attribute <= 4) break;
                printf("Incorret Value! Please try again\n");
              }
              searchByAttribute(attribute);
              break;
              
        case 4:
              addEmployee(list,rear);
              ch = 'Y';
              while(ch == 'Y'){
                FLUSH;
                printf("Add another record (Y/N) ?");
                scanf("%c",&ch);
                if(ch == 'Y'){
                  addEmployee(list,rear);
                }
              }
              promtSaveOperation(list);

              break;
              
        case 5:
              readFile(&countEmployee);
              while(1){
                printf("Please select the attribute (1=SIN, 2=First Name, 3=Last Name, 4=Annual Income):");
                scanf("%d",&attribute);
                if(attribute >= 1 && attribute <= 4) break;
                printf("Incorret Value! Please try again\n");
              }
              deleteEmployee(attribute);
              List(head);
              break;

        default:
              printf("Please select from the options!");
              break;
    }
  }

  return 0;
}