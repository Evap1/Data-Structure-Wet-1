//
// Created by Eva Poluliakhov on 24/05/2023.
//

#include "User.h"

// Constructor
// Consumer must check if ID already exists
User::User(int userId,bool isVip) : userId(userId), isVip(isVip) {}

// Add views to certain genre
void User::add_views_in_genre(Genre type, int nunOfViews){
    viewsPerGenre[(int)type]+=nunOfViews;
}

int User::get_views_per_genre(Genre type) const {
    return viewsPerGenre[(int)type];
}

// SETTERS
void User::setGroupId(int id) {
    User::groupId = id;
}

// GETTERS
int User::getUserId() const {
    return userId;
}

bool User::is_vip() const {
    return isVip;
}



int User::getGroupId() const {
    return groupId;
}

// Operator overloading
bool operator>(const User &user1, const User &user2)
{
    return (user1.getUserId() > user2.getUserId());
}

bool operator<(const User& user1, const User& user2)
{
    return (user2 > user1);
}