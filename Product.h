#ifndef PRODUCT_H
#define PRODUCT_H

#include <string>

class Category;

class Product {
private:
    std::string name;
    double price;
    bool availability;
    std::string description;
    int id;
    Category* category;

public:
    Product(const std::string& name, double price, bool availability, const std::string& description, int id);
    void setCategory(Category* category);
    void removeFromCategory();
    Category* getCategory() const;
    int getId() const;
};

#endif  
