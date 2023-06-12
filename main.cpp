#include <iostream>
#include "Category.h"
#include "Product.h"
#include "BPlusTree.h"

int main() {
    BPlusTree bPlusTree;

    // Додавання категорій
    Category electronics("Electronics", "Electronic devices", 1);
    Category books("Books", "Books and literature", 2);
    Category clothing("Clothing", "Clothing and apparel", 3);

    // Додавання товарів до категорій
    Product laptop("Laptop", 1500.0, true, "Powerful laptop for work and gaming", 1001);
    electronics.addProduct(laptop);

    Product smartphone("Smartphone", 800.0, true, "High-end smartphone with great features", 1002);
    electronics.addProduct(smartphone);

    Product novel("Novel", 20.0, true, "Best-selling novel by a renowned author", 2001);
    books.addProduct(novel);

    Product tShirt("T-Shirt", 25.0, true, "Comfortable cotton t-shirt", 3001);
    clothing.addProduct(tShirt);

    // Додавання категорій до B+-дерева
    bPlusTree.insert(electronics.getId());
    bPlusTree.insert(books.getId());
    bPlusTree.insert(clothing.getId());

    // Виведення дерева
    std::cout << "B+-дерево:\n";
    bPlusTree.printTree();
    std::cout << std::endl;

    // Видалення категорії
    bPlusTree.remove(books.getId());

    // Виведення оновленого дерева
    std::cout << "Оновлене B+-дерево:\n";
    bPlusTree.printTree();
    std::cout << std::endl;

    // Виведення списку товарів певної категорії
    std::cout << "Список товарів у категорії Electronics:\n";
    Category* electronicsCategory = static_cast<Category*>(bPlusTree.search(electronics.getId()));
    if (electronicsCategory != nullptr) {
        electronicsCategory->printProductList();
    }
    std::cout << std::endl;

    return 0;
}

