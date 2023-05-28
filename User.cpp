//
// Created by Eva Poluliakhov on 24/05/2023.
//

#include "User.h"

// _________________________________________________CONSTRUCTOR______________________________________________________________

// TODO: Consumer must check if ID already exists
User::User(int userId,bool isVip) : userId(userId), isVip(isVip) , groupId(NONE) {}
User::User(int userId): userId(userId), isVip(false), groupId(0){}

/// @brief Add views to certain genre
/// @param type
/// @param nunOfViews
void User::add_views_in_genre(Genre type){
    viewsPerGenre[(int)type]++;
}

/// @param type
/// @return how many movies the current user watched, of genre type.
int User::get_views_per_genre(Genre type) const {
    return viewsPerGenre[(int)type];
}

// _________________________________________________SETTERS______________________________________________________________

void User::set_group_id(int id) {
    User::groupId = id;
}

void User::leave_group() {
    groupId = NONE;
}

// _________________________________________________GETTERS______________________________________________________________

int User::get_user_id() const {
    return userId;
}

bool User::is_vip() const {
    return isVip;
}

int User::get_group_id() const {
    return groupId;
}



// ___________________________________________Operator Overloading__________________________________________

bool operator>(const User &user1, const User &user2)
{
    return (user1.get_user_id() > user2.get_user_id());
}

bool operator<(const User& user1, const User& user2)
{
    return (user2 > user1);
}

bool operator==(const User& user1, const User& user2)
{
    return (user1.get_user_id() == user2.get_user_id());
}