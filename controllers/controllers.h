#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../models/model.h"

void mainScreen();

/* PUSH POP CREATE */
Recipe *createRecipe(char *name, char *desc, char *ins) {
	Recipe *temp = (Recipe*)malloc(sizeof(Recipe));
	strcpy(temp->name, name);
    strcpy(temp->description, desc);
	strcpy(temp->instruction, ins);
	temp->next = temp->prev = NULL;
	temp->headIngred = temp->tailIngred = NULL;
	return temp;
}

Ingredient* createNewIngredient(char* name, int quantity) {
    Ingredient* temp = (Ingredient*)malloc(sizeof(Ingredient));
    strcpy(temp->name, name);
    temp->qty = quantity;
    temp->next = temp->prev = NULL;
    return temp;
}

User *createNewUser(char* name, char* email, char* foodType) {
    User *newUser = (User*)malloc(sizeof(User));
    strcpy(newUser->name, name);
    strcpy(newUser->email, email);
    strcpy(newUser->foodType, foodType);
    for(int i = 0; i < 26; i++) {
        newUser->headSavedRecipe[i] = newUser->tailSavedRecipe[i] = NULL;
    }
    newUser->headPantry = newUser->tailPantry = NULL;
    return newUser;
}

void pushIngredient(char* name, int quantity) {
    Ingredient* newIngred = createNewIngredient(name, quantity);
    if (!currRecipe->headIngred) {
        currRecipe->headIngred = currRecipe->tailIngred = newIngred;
    } else {
        currRecipe->tailIngred->next = newIngred;
        newIngred->prev = currRecipe->tailIngred;
        currRecipe->tailIngred = newIngred;
    }
}

void popHeadIngredient() {
    if (!currUser->headPantry) {
        return;
    } else if (currUser->headPantry == currUser->tailPantry) {
        currUser->headPantry = currUser->tailPantry = NULL;
        free(currUser->headPantry);
    } else {
        Ingredient *newHead = currUser->headPantry->next;
        newHead->prev = NULL;
        currUser->headPantry->next = NULL;
        free(currUser->headPantry);
        currUser->headPantry = newHead;
    }
}

void popTailIngredient() {
    if (!currUser->headPantry) {
        return;
    }
    else if (currUser->headPantry == currUser->tailPantry) {
        currUser->headPantry = currUser->tailPantry = NULL;
        free(currUser->headPantry);
    } 
    else {
        Ingredient *newTail = currUser->tailPantry->prev;
        newTail->next = NULL;
        currUser->tailPantry->prev = NULL;
        free(currUser->tailPantry);
        currUser->tailPantry = newTail;
    }
}

void popMidIngredient(char *name) {
    if (!currUser->headPantry) {
        return;
    } else if (strcmp(currUser->headPantry->name, name) == 0) {
        popHeadIngredient();
    } else if (strcmp(currUser->tailPantry->name, name) == 0) {
        popTailIngredient();
    } else {
        Ingredient *temp = currUser->headPantry;
        while (temp->next != NULL && strcmp(temp->next->name, name) != 0) {
            temp = temp->next;
        }

        if (temp->next != NULL) {
            Ingredient *temp2 = temp->next->next;
            temp->next->next = NULL;
            temp->next->prev = NULL;
            temp2->prev = NULL;
            temp->next = NULL;
            free(temp->next);
            temp->next = temp2;
        }
    }
}

void pushUserIngredient(char *name, int qty) {
    Ingredient *newIngred = createNewIngredient(name, qty);
    if (!currUser->headPantry) {
        currUser->headPantry = currUser->tailPantry = newIngred;
    } else {
        Ingredient *temp = currUser->headPantry;
        while (temp != NULL && strcmp(temp->name, name) != 0) {
            temp = temp->next;
        }

        if (!temp) {
            currUser->tailPantry->next = newIngred;
            newIngred->prev = currUser->tailPantry;
            currUser->tailPantry = newIngred;
        } else {
            temp->qty += qty;
        }
    }
}

void insertRecipe(char* name, char* desc, char* ins) {
    int idx = DJB2(name);
    Recipe* newRecipe = createRecipe(name, desc, ins);
    if (!headRecipe[idx]) {
        headRecipe[idx] = tailRecipe[idx] = newRecipe;
    }
    else {
        tailRecipe[idx]->next = newRecipe;
        newRecipe->prev = tailRecipe[idx];
        tailRecipe[idx] = newRecipe;
    }
    currRecipe = newRecipe;
}

void readRecipe() {
    FILE *fp = fopen("recipe.txt", "r");
    while(!feof(fp)) {
        char name[1024];
        char desc[1024];
        char ins[1024];
        char type;
        fscanf(fp, "%[^#]#%[^#]#%[^#]#", name, desc, ins);
        insertRecipe(name, desc, ins);
        char temp=' ';
        while(temp != '\n' && !feof(fp)) {
            char ingredient[1024];
            int qty;
            fscanf(fp, "%[^x]x%d%c", ingredient, &qty, &temp);
            ingredient[strlen(ingredient)-1] = '\0';
            pushIngredient(ingredient, qty);
        }
    }
    fclose(fp);
}

Recipe *searchRecipe(char *name) {
    int idx = DJB2(name);
    if (!headRecipe[idx]) {
        return NULL;
    } else if (strcmp(headRecipe[idx]->name, name) == 0) {
        return headRecipe[idx];
    } else if (strcmp(tailRecipe[idx]->name, name) == 0) {
        return tailRecipe[idx];
    } else {
        Recipe *temp = headRecipe[idx];
        while (temp != NULL && strcmp(temp->name, name) != 0) {
            temp = temp->next;
        }

        if (!temp) {
            return NULL;
        } else {
            return temp;
        }
    }
}

void viewRecipe() {
    int ctr=1;
    for(int i = 0; i < 26; i++) {
        if(headRecipe[i]) {
            Recipe *tempCurrRecipe = headRecipe[i];
            Ingredient *tempCurrIngredient = tempCurrRecipe->headIngred;
            while(tempCurrRecipe->next) {
                tempCurrIngredient = tempCurrRecipe->headIngred;
                printf("%d. ", ctr++);
                printf("%s\n%s\n%s\n", tempCurrRecipe->name, tempCurrRecipe->description, tempCurrRecipe->instruction);
                while(tempCurrIngredient->next) {
                    printf("%s x%d\n", tempCurrIngredient->name, tempCurrIngredient->qty);
                    tempCurrIngredient = tempCurrIngredient->next;
                }
                printf("%s x%d\n", tempCurrIngredient->name, tempCurrIngredient->qty);
                tempCurrRecipe = tempCurrRecipe->next;
            }
            printf("%d. ", ctr++);
            printf("%s\n%s\n%s\n", tempCurrRecipe->name, tempCurrRecipe->description, tempCurrRecipe->instruction);
            while(tempCurrIngredient->next) {
                printf("%s x%d\n", tempCurrIngredient->name, tempCurrIngredient->qty);
                tempCurrIngredient = tempCurrIngredient->next;
            }
            printf("%s x%d\n", tempCurrIngredient->name, tempCurrIngredient->qty);
        }
    }
}

void viewRecipeName() {
    int ctr=1;
    for(int i = 0; i < 26; i++) {
        if(headRecipe[i]) { 
            Recipe *tempCurrRecipe = headRecipe[i]; 
            while(tempCurrRecipe->next) {
                printf("%d. %s\n", ctr++, tempCurrRecipe->name);
                tempCurrRecipe = tempCurrRecipe->next;
            }
            printf("%d %s\n", ctr++, tempCurrRecipe->name);
        }
    }
}

void pushSavedRecipe() {
    if (!currRecipe) {
        printf("That recipe doesn't exist!\n");
        return;
    } else {
        int idx = DJB2(currRecipe->name);
        if (!currUser->headSavedRecipe[idx]) {
            currUser->headSavedRecipe[idx] = currUser->tailSavedRecipe[idx] = currRecipe;
        } else {
            currUser->tailSavedRecipe[idx]->next = currRecipe;
            currRecipe->prev = currUser->tailSavedRecipe[idx];
            currUser->tailSavedRecipe[idx] = currRecipe;
        }
    }
}

/* BAGIAN HOMEPAGE */
void viewSuggestedRecipe() {
    int ctr=1;
    if(strcmpi(currUser->foodType, "manis")) {
        for(int i = 0; i < 26; i++) {
            if(headRecipe[i] && i%2) {
                Recipe *curr = headRecipe[i];
                while(curr->next) {
                    printf("%d. %s\n", ctr++, curr->name);
                    curr = curr->next;
                }
                printf("%d. %s\n", ctr++, curr->name);
            }
        }
    } else {
        for(int i = 0; i < 26; i++) {
            if(headRecipe[i] && !(i%2)) {
                Recipe *curr = headRecipe[i];
                while(curr->next) {
                    printf("%d. %s\n", ctr++, curr->name);
                    curr = curr->next;
                }
                printf("%d. %s\n", ctr++,curr->name);
            }
        }
    }
}

void homePage() {
    int choose;
    do {
        viewRecipeName();
        char name[255];
        puts("1. Suggested recipe");
        puts("2. Search");
        puts("3. Back");
        do {
            printf(">> ");
            scanf("%d", &choose); getchar();
        } while(choose < 1 || choose > 3);
        if(choose == 1) {
            viewSuggestedRecipe();
        } else if(choose == 2) {
            printf("Recipe name : "); 
            scanf("%[^\n]", name); getchar();
            Recipe *temp = searchRecipe(name);
            currRecipe = temp;
            if(currRecipe) {
                printf("%s\n%s\n%s\n", temp->name, temp->description, temp->instruction);
                Ingredient* curr = temp->headIngred;
                int ctr=1;
                while(curr->next) {
                    printf("%d. %s %d\n", ctr++, curr->name, curr->qty);
                    curr = curr->next;
                }
                printf("%d. %s %d\n", ctr++,curr->name, curr->qty);
                char str[32];
                do {
                    printf("Do you want to save this recipe ? [y/n]");
                    scanf("%s", str); getchar();
                } while(str[0] != 'y' && str[0] != 'n');
                if(str[0] == 'y') {
                    pushSavedRecipe();
                }
            } else {
                puts("Recipe not found");
                pressEnter();
            }
        }
    } while(choose != 3);   
}

/* BAGIAN KITCHEN */
// zon aku jadinya bikin cookbook dulu deh wkkwkw

/* BAGIAN COOKBOOK */
void viewSavedRecipes() {
    int j = 1;
    for (int i = 0; i < 26; i++) {
        if (currUser->headSavedRecipe[i]) {
            Recipe *temp = currUser->headSavedRecipe[i];
            while (temp != NULL) {
                printf("%d.\n", j++);
                printf("%s\n", temp->name);
                printf("%s\n", temp->description);
                temp = temp->next;
            }
        } // lah hmmmm
    } // masi ada 3, atau salah di print ya??
}
    
Recipe *searchSavedRecipes(char *name) {
    int idx = DJB2(name);
    if (!currUser->headSavedRecipe[idx]) {
        return NULL;
    } else {
        Recipe *temp = currUser->headSavedRecipe[idx];
        while (temp != NULL && strcmp(temp->name, name) == 0) {
            temp = temp->next;
        }
        return temp;
    }
}

void viewCookbook() {
    int exist = 0;
    for (int i = 0; i < 26; i++) {
        if (currUser->headSavedRecipe[i]) {
            exist = 1;
            break;
        }
    }

    if (!exist) {
        printf("You don't have any saved recipes!\n");
        pressEnter();
        return;
    }

    viewSavedRecipes();

    char name[255];
    Recipe *find;
    int exit = 0;
    do {
        printf("Which recipe do you want to see? [input name | press X to exit]: ");
        scanf("%[^\n]", name);
        getchar();

        if (strcmp(name, "X") == 0) {
            exit = 1;
            break;
        }

        find = searchSavedRecipes(name);
    } while (!find);

    if (exit) {
        return;
    }

    printf("%s\n", find->name);
    printf("%s\n", find->description);
    printf("%s\n", find->instruction);
    pressEnter();
    return;
}

/* BAGIAN PANTRY */
void printPantry() {
    Ingredient* temp = currUser->headPantry;
    int i = 1;
    printf("%-3s %-35s %-3s\n", "No", "Ingredient Name", "Quantity");
    printf("===========================================\n");
    while (temp != NULL) {
        printf("%-3d. %-35s %-3d\n", i, temp->name, temp->qty);
        temp = temp->next;
    }
    printf("===========================================\n");
}

void searchUserIngredient(char* search) {
    currUser->currPantry = currUser->headPantry;
    while (currUser->currPantry != NULL && strcmp(currUser->currPantry->name, search) != 0) {
        currUser->currPantry = currUser->currPantry->next;
    }
}

void searchPantry() {
    if (!currUser->headPantry) {
        printf("There are no ingredients yet in your pantry!\n");
        pressEnter();
        return;
    }
    char search[255];
    printf("Input ingredient name: ");
    scanf("%[^\n]", search);
    getchar();

    searchUserIngredient(search);
    Ingredient *found = currUser->currPantry;
    if (found)
        printf("You have %d %s(s)\n", found->qty, found->name);
    else
        printf("You don't have that ingredient!\n");
    pressEnter();
}

void addPantry() {
    char name[255];
    int qty;
    do {
        printf("Input ingredient name [10 - 35 characters]: ");
        scanf("%[^\n]", name);
        getchar();
    } while (strlen(name) < 10 || strlen(name) > 35);

    do {
        printf("Input ingredient quantity [1 - 999]: ");
        scanf("%d", &qty);
        getchar();
    } while (qty < 1 || qty > 999);
    
    pushUserIngredient(name, qty);
    printf("Ingredient added!\n");
    pressEnter();
}

void removePantry() {
    if (!currUser->headPantry) {
        printf("There are no ingredients yet in your pantry!\n");
        pressEnter();
        return;
    }
    char name[255];
    do {
        printf("Which to delete? [input name]: ");
        scanf("%[^\n]", name);
        getchar();

        searchUserIngredient(name);
    } while (!currUser->currPantry);
    
    popMidIngredient(currUser->currPantry->name);
    printf("Ingredient deleted!\n");
    pressEnter();
}

void viewPantry() {
    int pantryMenu;
    do {
        if (!currUser->headPantry) {
            printf("There are no ingredients yet in your pantry!\n");
        } else {
            printPantry();
        }
        printf("1. Search pantry\n");
        printf("2. Add ingredient to pantry\n");
        printf("3. Delete ingredient from pantry\n");
        printf("4. Exit\n");

        do {
            printf(">> ");
            scanf("%d", &pantryMenu);
            getchar();
        } while (pantryMenu < 1 || pantryMenu > 4);

        clrscr();
        switch (pantryMenu) {
            case 1:
                searchPantry();
                break;
            case 2:
                addPantry();
                break;
            case 3:
                removePantry();
                break;
        }
        clrscr();
    } while (pantryMenu != 4);
}

/* BAGIAN PROFILE */

void insertUser(char* name, char* email, char* foodType) {
    User *newUser = createNewUser(name, email, foodType);
    int idx = DJB2(name);
    if(!headUser[idx]) {
        headUser[idx] = tailUser[idx] = newUser;
    } else {
        tailUser[idx]->next = newUser;
        newUser->prev = tailUser[idx];
        tailUser[idx] = newUser;
    }
    currUser = newUser;
}

bool foodTypeValdation(char* str) {
    if(!strcmpi(str, "manis") || !strcmpi(str, "asin")) {
        return false;
    }
    return true;
}

void readUser() {
    FILE *fp = fopen("user.txt", "r");
    while(!feof(fp)) {
        char name[255], email[255], foodType[255];
        fscanf(fp, "%[^#]#%[^#]#%[^\n]\n", name, email, foodType);
        insertUser(name, email, foodType);
    }
    fclose(fp);
}

bool searchUserName(char *name) {
    int idx = DJB2(name);
    if(!strcmpi(name, "back")) {
        return true;
    } else if(headUser[idx]) {
        currUser = headUser[idx];
        while(currUser) {
            if(!strcmp(name, currUser->name)) {
                return false; // nemu nama usernamenya
            }
            currUser = currUser->next;
        }
    }
    return true;
}

void showUser() {
    int ctr=1;
    for(int i = 0; i < 26; i++) {
        if(headUser[i]) {
            User *currTempUser = headUser[i];
            while(currTempUser->next) {
                printf("%d. %s\n", ctr++,currTempUser->name);
                currTempUser = currTempUser->next;
            }
            printf("%d. %s\n", ctr++, currTempUser->name);
        }
    }
    char name[255];
    puts("Type back to return");
    do {
        printf("Enter username : ");
        scanf("%[^\n]", name); getchar();
    } while(searchUserName(name));
    if(!strcmpi(name, "back")) return;
    mainScreen();
}

void addUser(char* name, char* email, char* foodType) {
    FILE* fp = fopen("user.txt", "a");
    fprintf(fp, "%s#%s#%s\n", name, email, foodType);
    insertUser(name, email, foodType);
    fclose(fp);
}

void profileScreen() {
    readUser();
    int choose;
    do {
        puts("1. Pick user");
        puts("2. Add new user");
        puts("3. Exit");
        do {
            printf(">> ");
            scanf("%d", &choose); getchar();
        } while(choose < 1 || choose > 3);
        if(choose == 1) {
            clrscr();
            showUser();
            clrscr();
        } else if(choose == 2) {
            clrscr();
            char name[255];
            char email[255];
            char foodType[255];
            printf("Enter username : ");
            scanf("%[^\n]", name); getchar();
            printf("Enter email : ");
            scanf("%[^\n]", email); getchar();
            do {
                printf("Enter food type [Manis || Asin][case insensitive] : ");
                scanf("%[^\n]", foodType); getchar();
            } while(foodTypeValdation(foodType));
            addUser(name, email, foodType);
            clrscr();
        }
    } while(choose != 3);
    

}

/* MAIN SCREEN */

void mainScreen() {
    readRecipe();
    int choose;
    do {
        puts("1. Home Page");
        puts("2. Cookbook");
        puts("3. Kitchen");
        puts("4. Pantry");
        puts("5. Profile");
        puts("6. Back");
        do {
            printf(">> ");
            scanf("%d", &choose); getchar();
        } while(choose > 6 || choose < 1);
        switch(choose) {
            case 1:
                clrscr();
                homePage();
                clrscr();
                break;
            case 2:
                clrscr();
                viewCookbook();
                clrscr();
                break;
            case 3:
                clrscr();
                // viewKitchen();
                clrscr();
                break;
            case 4:
                clrscr();
                viewPantry();
                clrscr();
                break;
            case 5:
                clrscr();
                // viewProfile();
                clrscr();
                break;
        }
    } while(choose != 6);
}