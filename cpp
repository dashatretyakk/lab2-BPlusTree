#include <iostream>
#include <vector>

// Абстракційний клас або інтерфейс для роботи з продуктами
class IProductManager {
public:
    virtual void addProduct() = 0;
    virtual void removeProduct() = 0;
    virtual void displayProducts() const = 0;
};

// Конкретна реалізація абстракційного класу IProductManager
class ProductManager : public IProductManager {
private:
    std::vector<std::string> products;

public:
    void addProduct() override {
        std::string productName;
        std::cout << "Enter the name of the product: ";
        std::cin >> productName;
        products.push_back(productName);
    }

    void removeProduct() override {
        if (!products.empty()) {
            std::cout << "Removing product: " << products.back() << std::endl;
            products.pop_back();
        }
    }

    void displayProducts() const override {
        std::cout << "Product List:" << std::endl;
        for (const auto& product : products) {
            std::cout << "- " << product << std::endl;
        }
    }
};

// Клас категорії, який використовує об'єкт ProductManager
class Category {
private:
    std::string name;
    IProductManager* productManager;

public:
    Category(const std::string& categoryName, IProductManager* manager)
        : name(categoryName), productManager(manager) {}

    void addProductToCategory() {
        std::cout << "Adding a product to the category: " << name << std::endl;
        productManager->addProduct();
    }

    void removeProductFromCategory() {
        std::cout << "Removing a product from the category: " << name << std::endl;
        productManager->removeProduct();
    }

    void displayProductsInCategory() const {
        std::cout << "Products in the category: " << name << std::endl;
        productManager->displayProducts();
    }
};

int main() {
    // Створення об'єкту ProductManager
    ProductManager productManager;

    // Створення об'єкту Category з використанням ProductManager
    Category category("Electronics", &productManager);

    // Взаємодія з категорією і продуктами
    category.addProductToCategory();
    category.addProductToCategory();
    category.displayProductsInCategory();
    category.removeProductFromCategory();
    category.displayProductsInCategory();

    return 0;
}
