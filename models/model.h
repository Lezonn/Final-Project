struct Ingredient {
    char name[1024];
    int qty;
    Ingredient* next, * prev;
} *headIngredient, * tailIngredient, * currIngredient;

struct Recipe {
    char name[255];
    char description[1024];
    char instruction[1024];
    Ingredient* headIngred, * tailIngred;
    Recipe* prev, * next;
} *headRecipe[26], * tailRecipe[26], * currRecipe;

int userId=1;
struct User {
    char name[1024];
    char email[1024];
    char foodType[255];
    Recipe* headSavedRecipe[26], * tailSavedRecipe[26];
    Ingredient* headPantry, * tailPantry, * currPantry;
    User* prev, * next;
} *headUser[26], * tailUser[26], * currUser;
