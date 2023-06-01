//
// Created by Eva Poluliakhov on 24/05/2023.
//

#include "User.h"

// _________________________________________________CONSTRUCTOR______________________________________________________________

// TODO: Consumer must check if ID already exists.
User::User(int userId, bool isVip) : userId(userId), isVip(isVip) , groupId(NONE), group(NULL) {}
User::User(int userId): userId(userId), isVip(false), groupId(0), group(NULL) {}


// _________________________________________________SETTERS______________________________________________________________

void User::set_group_id(int id) {
    groupId = id;
}

void User::set_group_ptr(Group* other){
    group = other;
}

void User::set_group_ptr_null(std::nullptr_t){
    group = nullptr;
}
void User::leave_group() {
    groupId = NONE;
}

/// @brief Add views to certain genre
/// @param type
/// @param nunOfViews
void User::add_views_in_genre(Genre type){
    viewsPerGenre[(int)type]++;
}

/// @brief add the num of views the group have when the member joins the team.
/// @param type
void User::set_views_as_group_join(Genre type, int amount) {
    groupWatchWhenJoin[(int)type]+= amount;
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

Group* User::get_group_ptr() const{
    return group;
}

/// @param type
/// @return how many movies the current user watched, of genre type.
int User::get_views_per_genre(Genre type) const {
    return viewsPerGenre[(int)type];
}

/// @param type
/// @return how many movies the group watched, without the member of genre type.
int User::get_views_when_leave(Genre type) const {
    return groupWatchWhenJoin[(int)type];
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