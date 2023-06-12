#ifndef CATEGORY_H
#define CATEGORY_H

#include <string>
#include <vector>
#include "Product.h"

class Category {
private:
    std::string name;
    std::string description;
    int id;
    std::vector<Product> products;

public:
    Category(const std::string& name, const std::string& description, int id);
    void addProduct(const Product& product);
    void removeProduct(int productId);
    std::vector<Product> getProducts() const;
};

#endif  
