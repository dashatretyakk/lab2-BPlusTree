#include "Product.h"
#include "Category.h"

Product::Product(const std::string& name, double price, bool availability, const std::string& description, int id)
    : name(name), price(price), availability(availability), description(description), id(id), category(nullptr) {}

void Product::setCategory(Category* category) {
    this->category = category;
    category->addProduct(*this);
}

void Product::removeFromCategory() {
    if (category != nullptr) {
        category->removeProduct(id);
        category = nullptr;
    }
}

Category* Product::getCategory() const {
    return category;
}

int Product::getId() const {
    return id;
}
