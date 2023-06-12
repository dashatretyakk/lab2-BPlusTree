#include "Category.h"

Category::Category(const std::string& name, const std::string& description, int id)
    : name(name), description(description), id(id) {}

void Category::addProduct(const Product& product) {
    products.push_back(product);
}

void Category::removeProduct(int productId) {
    for (auto it = products.begin(); it != products.end(); ++it) {
        if (it->getId() == productId) {
            products.erase(it);
            break;
        }
    }
}

std::vector<Product> Category::getProducts() const {
    return products;
}

