// A simple C++ console app to recommend recipes based on available ingredients.
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

// Ingredient class to store name, quantity, and unit
class Ingredient {
public:
    string name;
    double qty;
    string unit;

    Ingredient(string n = "", double q = 0.0, string u = "") : name(n), qty(q), unit(u) {}
};

// Converting strings to lowercase for consistent comparison
string toLower(string str) {
    transform(str.begin(), str.end(), str.begin(), ::tolower);
    return str;
}

// Recipe class holds details like name, cuisine, ingredients, preparation time, and meal type
class Recipe {
protected:
    string name;
    string cuisine;
    vector<Ingredient> ingredients;
    int time_reqd;
    string mealType;

public:
    Recipe(string n = "", string c = "", vector<Ingredient> ing = {}, int p = 0, string m = "")
        : name(toLower(n)), cuisine(toLower(c)), ingredients(ing), time_reqd(p), mealType(toLower(m)) {}

    string getName() const { return name; }
    friend class Cookbook; // Allow Cookbook to access private/protected members
};

// Cookbook class manages a list of recipes and finds matches based on user input
class Cookbook {
private:
    vector<Recipe> recipes;

public:
    // Add a new recipe to the collection
    void addRecipe(const Recipe& recipe) {
        recipes.push_back(recipe);
    }
    // To find and display recipes that match available ingredients, time, and meal type
    void findBestRecipes(vector<Ingredient>& ing_available, int timeAvailable, string type) {
        type = toLower(type);
        bool recipeFound = false;
        for (const auto& recipe : recipes) {
            // Skip if time or meal type doesn't match
            if (recipe.time_reqd > timeAvailable || recipe.mealType != type)
                continue;

            bool canPrepare = true;
            for (const auto& req_ing : recipe.ingredients) {
                bool found = false;
                for (const auto& avail_ing : ing_available) {
                    // Checking to see if required ingredient is available in sufficient quantity
                    if (toLower(req_ing.name) == toLower(avail_ing.name) && req_ing.qty <= avail_ing.qty) {
                        found = true;
                        break;
                    }
                }
                if (!found) {
                    canPrepare = false;
                    break;
                }
            }
            // Displaying matching recipe
            if (canPrepare) {
                cout << "Recipe: " << recipe.getName() << " | Cuisine: " << recipe.cuisine << " | Time: " << recipe.time_reqd << " mins" << endl;
                recipeFound = true;
            }
        }
        // If no recipes matched
        if (!recipeFound)
            cout << "No exact match found. Try adjusting your preferences!" << endl;
    }
};

int main() {
    Cookbook myCookbook;
    
    // Preloading some recipes
    myCookbook.addRecipe(Recipe("Pasta", "Italian", {{"Pasta", 200, "g"}, {"TomatoSauce", 100, "ml"}}, 30, "Guest"));
    myCookbook.addRecipe(Recipe("Cheese Sandwich", "American", {{"Bread", 4, "slices"}, {"Cheese", 2, "slices"}}, 10, "Basic"));
    myCookbook.addRecipe(Recipe("Green Salad", "International", {{"Lettuce", 100, "g"}, {"Tomato", 2, "pieces"}, {"Cucumber", 1, "pieces"}}, 15, "Basic"));
    myCookbook.addRecipe(Recipe("Paneer Butter Masala", "Indian", {{"Paneer", 250, "g"}, {"Butter", 50, "g"}, {"Tomato", 2, "pieces"}}, 40, "Guest"));
    myCookbook.addRecipe(Recipe("Omelette", "French", {{"Eggs", 2, "pieces"}, {"Salt", 1, "tsp"}, {"Butter", 10, "g"}}, 5, "Basic"));
    myCookbook.addRecipe(Recipe("Chicken Curry", "Indian", {{"Chicken", 500, "g"}, {"Onion", 2, "pieces"}, {"Tomato", 2, "pieces"}, {"Spices", 10, "g"}}, 45, "Guest"));

    int choice;
    do {
        // Menu options
        cout << "\n1. Add a Recipe\n2. Find Recipes\n3. Exit\nChoose an option: ";
        cin >> choice;
        cin.ignore();

        if (choice == 1) {
            // Adding a new recipe
            string name, cuisine, mealType;
            int time_reqd, ing_count;
            vector<Ingredient> ingredients;
            cout << "Enter Recipe Name: ";
            getline(cin, name);
            cout << "Enter Cuisine: ";
            getline(cin, cuisine);
            cout << "Enter Meal Type (Basic/Guest): ";
            getline(cin, mealType);
            cout << "Enter Time Required (mins): ";
            cin >> time_reqd;
            cout << "Enter Number of Ingredients: ";
            cin >> ing_count;
            cin.ignore();

            // Input ingredients
            for (int i = 0; i < ing_count; i++) {
                string ing_name, unit;
                double qty;
                cout << "Ingredient " << i + 1 << " (name qty unit): ";
                cin >> ing_name >> qty >> unit;
                ingredients.push_back(Ingredient(toLower(ing_name), qty, unit));
            }
            myCookbook.addRecipe(Recipe(name, cuisine, ingredients, time_reqd, mealType));
        }
        else if (choice == 2) {
            // Finding matching recipes
            int availableCount, timeAvailable;
            vector<Ingredient> ing_available;
            cout << "Enter the number of available ingredients: ";
            cin >> availableCount;
            cin.ignore();

            for (int i = 0; i < availableCount; ++i) {
                string ing_name, unit;
                double qty;
                cout << "Enter ingredient " << i + 1 << " (name qty unit): ";
                cin >> ing_name >> qty >> unit;
                ing_available.push_back(Ingredient(toLower(ing_name), qty, unit));
            }
            cout << "Enter the time available (in mins): ";
            cin >> timeAvailable;
            cin.ignore();

            string type;
            cout << "Enter meal type preference (Basic/Guest): ";
            getline(cin, type);

            myCookbook.findBestRecipes(ing_available, timeAvailable, type);
        }
    } while (choice != 3); // Exit on choice 3

    return 0;
}
